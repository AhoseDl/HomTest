//#include <QObject>

/*#include<MeshPbc.h>
#include<bitset>
#include<node.h>
#include<cmath>
#include<cassert>
#include<QDebug>
#include<bcs.h>
#include<valarray>
#include<vector>
#include<list>
#include<matrix.h>
#include<pbcequation.h>*/

//#include<petsctao.h>
//#include<petsc.h>

#include<petscutilities.h>

namespace scaletag {
namespace models {


PetscErrorCode valarrayToVec(const std::valarray<double>&val, Vec& res){
    //PetscSizeT N=val.size();
    PetscMPIInt rank;
    PetscInt N=val.size();

    PetscFunctionBeginUser;
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    if(rank==0){
    VecCreate(PETSC_COMM_WORLD/*PETSC_COMM_WORLD*/, /*&*/&res);
    //PetscObjectSetName((PetscObject)res, &res);
    VecSetSizes(res, PETSC_DECIDE, N);
    VecSetFromOptions(res); //VecSetType(res, VECSEQ);
    VecSetUp(res);

    for(/*PetscSizeT*/PetscInt i=0; i<N; i++) VecSetValue(res, i, val[i], INSERT_VALUES);
    VecAssemblyBegin(res);
    VecAssemblyEnd(res);}
    PetscFunctionReturn(PETSC_SUCCESS);

}

PetscErrorCode VecToValarray(const Vec& vec, std::valarray<double>& res){
    PetscMPIInt rank;
    PetscInt N; const PetscReal* vecRead;

    PetscFunctionBeginUser;
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    if(rank==0){
    VecGetSize(vec, &N);
    res.resize(N, 0.0);
    VecGetArrayRead(vec, &vecRead);
    for(PetscInt i=0; i<N; i++) res[i]=vecRead[i];
    VecRestoreArrayRead(vec, &vecRead);
    }
    PetscFunctionReturn(PETSC_SUCCESS);
}

PetscErrorCode matrixToPetscMat(const fem::matrix<double>& mat, Mat& res){
    PetscInt N=mat.nrow();
    PetscInt M=mat.ncol();
    std::valarray<size_t> matPtr = mat.getPtr();
    std::valarray<size_t> matIdx= mat.getColumnsIndex();
    std::valarray<double> matVal = mat.getVal();
    PetscMPIInt rank;

    PetscFunctionBeginUser;

    //As it can be sometimes difficult to compute the required preallocation information
    //efficiently, we adopt the solution below :
    Mat allocator;

    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);if(rank==0){
    MatCreate(PETSC_COMM_WORLD/*PETSC_COMM_SELF*/, &allocator);
    MatSetType(allocator, MATPREALLOCATOR);
    MatSetSizes(allocator, PETSC_DECIDE, PETSC_DECIDE, N, M);
    MatSetUp(allocator);


    for(PetscInt i=0; i<N; i++)
        for(PetscInt j=matPtr[i]; j<matPtr[i+1]; j++){
            MatSetValues(allocator, 1, &i, 1, &j , &(matVal[matIdx[j]]), INSERT_VALUES);
            if( (i%100)==0 ){MatAssemblyBegin(allocator, MAT_FLUSH_ASSEMBLY); MatAssemblyEnd(allocator, MAT_FLUSH_ASSEMBLY);}
        }
    MatAssemblyBegin(allocator, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(allocator, MAT_FINAL_ASSEMBLY);

    //Filling Mat res
    MatCreate(PETSC_COMM_WORLD/*PETSC_COMM_SELF*/, &res);
    MatPreallocatorPreallocate(allocator, PETSC_TRUE, res);

    MatSetFromOptions(res); // MatSetType(...)
    MatSetSizes(res, PETSC_DECIDE, PETSC_DECIDE, N, N);
    MatSetUp(res);
    //MPI_Comm_rank(PETSC_COMM_WORLD, &rank);if(rank==0){
    for(PetscInt i=0; i<N; i++){
        for(PetscInt j=matPtr[i]; j<matPtr[i+1]; j++){
//            MatSetValue(*res,i, j , matVal[matIdx[j]], INSERT_VALUES);
            MatSetValues(res, 1, &i, 1, &j , &(matVal[matIdx[j]]), INSERT_VALUES);
        }
    if( (i%100)==0 ){MatAssemblyBegin(res, MAT_FLUSH_ASSEMBLY); MatAssemblyEnd(res, MAT_FLUSH_ASSEMBLY);}
    }
    //}
    MatAssemblyBegin(res, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(res, MAT_FINAL_ASSEMBLY);
    MatDestroy(&allocator);}
    PetscFunctionReturn(PETSC_SUCCESS);
}

//Function for computing the objective function and the gradient
PetscErrorCode objAndGradFunc(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx){
    const PetscReal* Xr;
    PetscReal* gr;
    PetscMPIInt rank;
    Mat KG;
    Mat mat;
    Vec vec; // No more a pointer
    Vec Xd; //will contain the production KG*X

    PetscFunctionBeginUser;
    KG= (reinterpret_cast<DataForPetsc*>(ctx))->getRigidityMat();
    mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
    vec = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsVec();
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    if(rank==0){
        VecDuplicate(X, &Xd);
        MatMult(KG, X, Xd);
        VecDot(X, Xd, f);
        *f=0.5*(*f);
        //In computing of g, we suppose that KG is symmetric
        MatMult(KG, X, g);
    }
    PetscFunctionReturn(PETSC_SUCCESS);

}

//Function for computing constraints
PetscErrorCode constraintsFunc(Tao tao, Vec X, Vec ce, void* ctx){
    Vec matVec;
    PetscInt dimRCe, dimCCe;
    Mat mat;
    Vec vec;

    PetscFunctionBegin;

    mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
    vec = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsVec();
    MatGetSize(mat, &dimRCe, &dimCCe);
    VecCreate(PETSC_COMM_WORLD, &matVec);
    VecSetSizes(matVec, PETSC_DECIDE, dimRCe );
    VecSetUp(matVec);

    MatMult(mat, X, matVec);
    VecWAXPY(ce, -1, vec, matVec);

   // MatMult(mat, vec, matVec); //vec no more a pointer
   // VecCopy(matVec, ce);

    VecDestroy(&matVec);
    PetscFunctionReturn(PETSC_SUCCESS);

}

//Function for computing constraints gradient
PetscErrorCode constraintsGradFunc(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx){

    Mat mat;
    PetscMPIInt rank;

    PetscInt nnz; PetscInt N, M;
    const PetscInt* cols; const PetscScalar* vals;

    PetscFunctionBeginUser;

    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    if(rank==0){


        { /* mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();*/
    //MatCopy(mat, J, SAME_NONZERO_PATTERN);
    //MatCopy changed to MatConvert because of the crash of MatCopy() as J
    //seems to need to be explicitly created.
   /* MatConvert(mat, MATSAME, MAT_INITIAL_MATRIX, &J);*/
    //If problem with reference on J, consider using MatGetRow(...) coupled with
    // MatPreallocatorPreallocate(...) instead of "MatConvert(...)"
        }

    //Modified today
    mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
    MatGetSize(mat, &N, &M);

    for(PetscInt i=0; i<N; i++){
    MatGetRow(mat, N, &nnz, &cols, &vals); //nnz=number of nonzeros in line N
    if(nnz!=0){
            MatSetValues(J, 1, &N, nnz, cols, vals, INSERT_VALUES);
    }
    if(N%100 ==0){
        MatAssemblyBegin(J, MAT_FLUSH_ASSEMBLY);
        MatAssemblyEnd(J, MAT_FLUSH_ASSEMBLY);
    }
//MatRestoreRow(Mat mat, PetscInt row, PetscInt *ncols, const PetscInt *cols[], const PetscScalar *vals[])
    MatRestoreRow(mat, N, &nnz, &cols, &vals);
    }
    MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);
    }

    PetscFunctionReturn(PETSC_SUCCESS);
}




DataForPetsc::DataForPetsc(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                           const fem::matrix<double>& pbcConstraintsMat, const std::valarray<double>& pbcConstraintsVec):
    Mh(&Mh), ndof(ndof){
    PetscFunctionBeginUser;
    //PetscInitialize(NULL, NULL,    NULL, NULL);
    valarrayToVec(pbcConstraintsVec, pbcConstraintsPetscVec); //Removing pointer attributes on pbcConstraintsPetscVec
    matrixToPetscMat(_KG, KG);
    //matrixToPetscMat(_KG, *KG); //To be deleted
    matrixToPetscMat(pbcConstraintsMat, pbcConstraintsPetscMat);
    //PetscFinalize();
    PetscFunctionReturnVoid();
    //PetscFunctionReturn(PETSC_SUCCESS);
}

//DataForPetsc::~DataForPetsc(){
//    //delete KG;
//    //delete pbcConstraintsPetscMat;
//    //delete pbcConstraintsPetscVec;//no more a pointer
//}

Mat DataForPetsc::getRigidityMat(){return KG;}
Mat DataForPetsc::getConstraintsMat(){return pbcConstraintsPetscMat;}
Vec DataForPetsc::getConstraintsVec(){return pbcConstraintsPetscVec;} //No more a pointer

PetscErrorCode DataForPetsc::petscSolve(std::valarray<double>& sol){

    Tao tao;
    Vec U, ce; //Solution vector and constraint vector;
    Mat J; //d(ce)/dU
    PetscInt numberOfConstraints, solSize;

    PetscMPIInt rank;

    //solSize=ndof*Mh->getNumberOfNodes();

    PetscFunctionBeginUser;
    //PetscInitialize(NULL, NULL,     NULL, NULL);

    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    if(rank==0){
    MatGetSize(pbcConstraintsPetscMat, &numberOfConstraints, &solSize);

    VecCreate(PETSC_COMM_WORLD, &U);
    VecSetSizes(U, PETSC_DECIDE, solSize);
    VecSetFromOptions(U);
    VecSetUp(U);
    VecSet(U, 0.02); //Solution vector must be initialized

    VecCreate(PETSC_COMM_WORLD, &ce);
    VecSetSizes(ce, PETSC_DECIDE, numberOfConstraints);
    VecSetFromOptions(ce);
    VecSetUp(ce);

    MatCreate(PETSC_COMM_WORLD, &J);
    MatSetSizes(J, PETSC_DECIDE, PETSC_DECIDE, numberOfConstraints, solSize);
    MatSetFromOptions(J);
    MatSetUp(J);

    TaoCreate(PETSC_COMM_WORLD, &tao);
    TaoSetType(tao, TAOALMM);
    TaoSetSolution(tao,U);
    TaoSetFromOptions(tao);
    TaoSetObjectiveAndGradient(tao, NULL, objAndGradFunc, (void*)this);
    TaoSetEqualityConstraintsRoutine(tao, ce, constraintsFunc, (void*)this);
    TaoSetJacobianEqualityRoutine(tao, J, J, constraintsGradFunc, (void*)this);

        TaoSetTolerances(tao, 1.e-5, 0.0, 0.0);
        TaoSetConstraintTolerances(tao, 1.e-5, 0.0);
        TaoSetMaximumFunctionEvaluations(tao, 1e6);

    Vec Ul, Uu;
    VecDuplicate(U, &Ul);
    VecDuplicate(U, &Uu);
    VecSet(Ul, -0.5);
    VecSet(Uu, 0.5);
    TaoSetVariableBounds(tao, Ul, Uu);
    TaoSetUp(tao); TaoSolve(tao);

    VecToValarray(U, sol);
    VecDestroy(&U); VecDestroy(&ce); VecDestroy(&Ul); VecDestroy(&Uu);
    MatDestroy(&J);
    TaoDestroy(&tao);
    }//End MPI_Comm_Rank
    PetscFunctionReturn(PETSC_SUCCESS);
}

} // namespace models
} // namespace scaletag
