#include<petscutilities.h>

namespace scaletag {
namespace models {

//Custom functions for the PBC Case
//Function for computing the objective function and the gradient
PetscErrorCode objAndGradFunc(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx){
        const PetscReal* Xr;
        PetscReal* gr;
        PetscMPIInt rank;
        Mat* KG;
        Mat* mat;
        Vec* vec; // No more a pointer
        Vec Xd; //will contain the production KG*X

        PetscFunctionBeginUser;
        //KG= (reinterpret_cast<DataForPetsc*>(ctx))->getRigidityMat();
//        mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
//        vec = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsVec();
        //MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
        //if(rank==0){
            VecDuplicate(X, &Xd);
            //MatMult(*KG, X, Xd);
            MatMult(*((reinterpret_cast<DataForPetsc*>(ctx))->getRigidityMat()), X, Xd);
            VecDot(X, Xd, f);
            *f=0.5*(*f);
            //In computing of g, we suppose that KG is symmetric
//            MatMult(*KG, X, g);
            MatMult(*((reinterpret_cast<DataForPetsc*>(ctx))->getRigidityMat()), X, g);
        //}
            VecDestroy(&Xd);
        PetscFunctionReturn(PETSC_SUCCESS);

}

//Function for computing the Hessian
PetscErrorCode hessianFunc(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx){

        // Mat* mat;

        PetscFunctionBeginUser;

        // mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
        //MatDuplicate(*mat, MAT_COPY_VALUES, &J);
        // MatCopy(*mat, J, /*SAME_NONZERO_PATTERN*/DIFFERENT_NONZERO_PATTERN);
        MatCopy( *((reinterpret_cast<DataForPetsc*>(ctx))->getRigidityMat()) , H, DIFFERENT_NONZERO_PATTERN);
        // MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY); MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);

        /* DataForPetsc::matrixToPetscMat(*((reinterpret_cast<DataForPetsc*>(ctx))->getStdConstraintsMat()), mat);
        MatCopy(*mat, J, DIFFERENT_NONZERO_PATTERN);
        MatDestroy(mat);*/

        PetscFunctionReturn(PETSC_SUCCESS);
}


//Function for computing constraints
PetscErrorCode constraintsFunc(Tao tao, Vec X, Vec ce, void* ctx){
        Vec matVec;
        PetscInt dimRCe, dimCCe;
        Mat* mat;
        Vec* vec;

        PetscFunctionBegin;

        //mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
        //vec = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsVec();
//        MatGetSize(*mat, &dimRCe, &dimCCe);
        MatGetSize(*((reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat()), &dimRCe, &dimCCe);
        VecCreate(PETSC_COMM_WORLD, &matVec);
        VecSetSizes(matVec, PETSC_DECIDE, dimRCe );
        VecSetUp(matVec);

//        MatMult(*mat, X, matVec);
        MatMult(*((reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat()), X, matVec);
//        VecWAXPY(ce, -1, *vec, matVec);
        VecWAXPY(ce, -1, *((reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsVec()), matVec);

       // MatMult(mat, vec, matVec); //vec no more a pointer
       // VecCopy(matVec, ce);

        VecDestroy(&matVec);
        PetscFunctionReturn(PETSC_SUCCESS);

}

////Function for computing constraints gradient
//PetscErrorCode constraintsGradFunc(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx){

//        Mat* mat;
//        PetscMPIInt rank;

//        PetscInt nnz; PetscInt N, M;
//        const PetscInt* cols; const PetscScalar* vals;

//        PetscFunctionBeginUser;

//        MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
//        //if(rank==0){


//            { /* mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();*/
//        //MatCopy(mat, J, SAME_NONZERO_PATTERN);
//        //MatCopy changed to MatConvert because of the crash of MatCopy() as J
//        //seems to need to be explicitly created.
//       /* MatConvert(mat, MATSAME, MAT_INITIAL_MATRIX, &J);*/
//        //If problem with reference on J, consider using MatGetRow(...) coupled with
//        // MatPreallocatorPreallocate(...) instead of "MatConvert(...)"
//            }

//        mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
//        MatGetSize(*mat, &N, &M);

//        for(PetscInt i=0; i<N; i++){
//        MatGetRow(*mat, N, &nnz, &cols, &vals); //nnz=number of nonzeros in line N
//        if(nnz!=0){
//                MatSetValues(J, 1, &N, nnz, cols, vals, INSERT_VALUES);
//        }
//        if(N%100 ==0){
//            MatAssemblyBegin(J, MAT_FLUSH_ASSEMBLY);
//            MatAssemblyEnd(J, MAT_FLUSH_ASSEMBLY);
//        }

//        MatRestoreRow(*mat, N, &nnz, &cols, &vals);
//        }
//        MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY);
//        MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);
//        //}//rank end

//        PetscFunctionReturn(PETSC_SUCCESS);
//}

//Function for computing constraints gradient
PetscErrorCode constraintsGradFunc(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx){

       // Mat* mat;

        PetscFunctionBeginUser;

       // mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
        //MatDuplicate(*mat, MAT_COPY_VALUES, &J);
       // MatCopy(*mat, J, /*SAME_NONZERO_PATTERN*/DIFFERENT_NONZERO_PATTERN);
        MatCopy( *((reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat()) , J, DIFFERENT_NONZERO_PATTERN);
       // MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY); MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);

       /* DataForPetsc::matrixToPetscMat(*((reinterpret_cast<DataForPetsc*>(ctx))->getStdConstraintsMat()), mat);
        MatCopy(*mat, J, DIFFERENT_NONZERO_PATTERN);
        MatDestroy(mat);*/

        PetscFunctionReturn(PETSC_SUCCESS);


}



//Definitions of the class DataForPetsc members

DataForPetsc::DataForPetsc( fem::matrix<double>* _KG, fem::matrix<double>* _constrMat, std::valarray<double>* _constrVec,
    PetscErrorCode (*_objAndGradFunc)(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx),
    PetscErrorCode (*_hessianFunc)(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx),
    PetscErrorCode (*_constraintsFunc)(Tao tao, Vec X, Vec ce, void* ctx),
    PetscErrorCode (*_constraintsGradFunc)(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx)
                           ):KG(_KG), constrMat(_constrMat), constrVec(_constrVec), objAndGradFunc(_objAndGradFunc), hessianFunc(_hessianFunc),constraintsFunc(_constraintsFunc), constraintsGradFunc(_constraintsGradFunc){}

 PetscErrorCode DataForPetsc::valarrayToVec(const std::valarray<double>&val, Vec* res){

    /*The conversion is parallelized on PETSC_COMM_WORLD*/
    //PetscMPIInt rank;

    PetscInt N=val.size();
    PetscInt rstart, rend;

    PetscFunctionBeginUser;
    //MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    //if(rank==0){
    VecCreate(PETSC_COMM_WORLD, res);
    //PetscObjectSetName((PetscObject)res, &res);
    VecSetSizes(*res, PETSC_DECIDE, N);
    VecSetFromOptions(*res); //VecSetType(res, VECSEQ);
    VecSetUp(*res);
    VecGetOwnershipRange(*res, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++) VecSetValue(*res, i, val[i], INSERT_VALUES);
    VecAssemblyBegin(*res);
    VecAssemblyEnd(*res);
    //}
    PetscFunctionReturn(PETSC_SUCCESS);

}

// PetscErrorCode DataForPetsc::VecToValarray(const Vec& vec, std::valarray<double>& res){

//    /* vec is supposed distributed on PETSC_COMM_WORLD*/

//    PetscInt N; const PetscReal* vecRead;
//    PetscInt rstart, rend;

//    PetscFunctionBeginUser;
//    VecGetSize(vec, &N);
//    res.resize(N, 0.0);
//    VecGetOwnershipRange(vec, &rstart, &rend);
//    VecGetArrayRead(vec, &vecRead);
//    for(PetscInt i=rstart; i<rend; i++) res[i]=vecRead[i-rstart];
//    VecRestoreArrayRead(vec, &vecRead);

//    PetscFunctionReturn(PETSC_SUCCESS);
//}

PetscErrorCode DataForPetsc::VecToValarray(const Vec& vec, std::valarray<double>& res){

    /* vec is supposed distributed on PETSC_COMM_WORLD*/

    PetscInt N; const PetscReal* vecRead;
    PetscInt rstart, rend;

    Vec vout;
    VecScatter *ctx;

    PetscFunctionBeginUser;
    VecGetSize(vec, &N);
    res.resize(N, 0.0);
  //  VecGetOwnershipRange(vec, &rstart, &rend);
   // VecScatterCreate(vec, NULL, vout, NULL, ctx);
    VecScatterCreateToAll(vec,ctx, &vout);
    VecScatterBegin(*ctx, vec, vout, INSERT_VALUES, SCATTER_FORWARD);
    VecScatterEnd(*ctx, vec, vout, INSERT_VALUES, SCATTER_FORWARD);
    VecGetArrayRead(vout, &vecRead);
    //for(PetscInt i=rstart; i<rend; i++) res[i]=vecRead[i-rstart];
    for(PetscInt i=0; i<N; i++) res[i]=vecRead[i];
    VecRestoreArrayRead(vec, &vecRead);

    VecScatterDestroy(ctx);
 //   VecDestroy(vecRead);
    PetscFunctionReturn(PETSC_SUCCESS);
}

 PetscErrorCode DataForPetsc::matrixToPetscMat(const fem::matrix<double>& mat, Mat* res){

    PetscInt N, M;
    std::valarray<size_t> matPtr, matIdx;
    std::valarray<double> matVal;;
    PetscMPIInt rank;
    PetscInt rstart, rend;
        Mat allocator;

    PetscFunctionBeginUser;
    //As it can be sometimes difficult to compute the required preallocation information
    //efficiently, we adopt the solution below :

    N=mat.nrow(); M=mat.ncol(); matPtr = mat.getPtr(); matIdx= mat.getColumnsIndex(); mat.getVal();
    MatCreate(PETSC_COMM_WORLD, &allocator);
    MatSetType(allocator, MATPREALLOCATOR);
    MatSetSizes(allocator, PETSC_DECIDE, PETSC_DECIDE, N, M);
    MatSetUp(allocator);

        MatGetOwnershipRange(allocator, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++)
        for(PetscInt j=(PetscInt) matPtr[i]; j<(PetscInt)matPtr[i+1]; j++){
            MatSetValue(allocator,i, matIdx[j] , matVal[j], INSERT_VALUES);
        }

    MatAssemblyBegin(allocator, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(allocator, MAT_FINAL_ASSEMBLY);
    PetscBarrier( (PetscObject)allocator );

    //Filling Mat res
    MatCreate(PETSC_COMM_WORLD, res);
    MatSetSizes(*res, PETSC_DECIDE, PETSC_DECIDE, N, M);
    MatSetFromOptions(*res);
        MatPreallocatorPreallocate(allocator, PETSC_TRUE, *res);
    MatSetUp(*res);
    MatGetOwnershipRange(*res, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++){
          for(PetscInt j=  (PetscInt)matPtr[i] ; j<(PetscInt)matPtr[i+1] ; j++){
            MatSetValue(*res,i, (PetscInt)matIdx[j] , matVal[j], INSERT_VALUES);
        }
    }
    MatAssemblyBegin(*res, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(*res, MAT_FINAL_ASSEMBLY);
    MatDestroy(&allocator);
    PetscFunctionReturn(PETSC_SUCCESS);
}

 PetscErrorCode DataForPetsc::init(/*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec*/){
        /* This function creates parameters passed in arguments and keep their addresses in DataForPetsc*/

//        PetscMPIInt rank=0;
        PetscFunctionBeginUser;

//        MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
//        if(rank==0){
            DataForPetsc::matrixToPetscMat(*constrMat, /**_petscConstrMat*/&petscConstrMat);
            DataForPetsc::matrixToPetscMat(*KG, /**_petscKG*/&petscKG);
            DataForPetsc::valarrayToVec(*constrVec, /**_petscConstrVec*/&petscConstrVec);

//            petscKG=_petscKG;
//            petscConstrMat=_petscConstrMat;
//            petscConstrVec=_petscConstrVec;
       // }

        PetscFunctionReturn(PETSC_SUCCESS);
}

// PetscErrorCode DataForPetsc::init2(Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec){

//        PetscFunctionBeginUser;
//        DataForPetsc::matrixToPetscMat(*constrMat, _petscConstrMat/*&petscConstrMat*/);
//        DataForPetsc::matrixToPetscMat(*KG, _petscKG/*&petscKG*/);
//        DataForPetsc::valarrayToVec(*constrVec, _petscConstrVec/*&petscConstrVec*/);

//        PetscFunctionReturn(PETSC_SUCCESS);
// }



 PetscErrorCode DataForPetsc::petscSolve(int* argc, char*** argv, const MeshPbc<double,3>& Mh, size_t ndof,
const fem::matrix<double>& _KG, const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec, std::valarray<double>& sol){

    Tao tao;     TaoConvergedReason reason;
    Vec U, ce; //Solution vector and constraint vector;
    Mat J; //d(ce)/dU -->The jacobian of the objective function
    Mat H; //To contain the obective function Hessian
    PetscInt numberOfConstraints, solSize;

    Mat petscKG;
    Mat petscConstrMat;
    Vec petscConstrVec;
    Vec gradf; //to hold gradient of the objective function

//    PetscMPIInt rank=0;
//    char* testPurpose="testPurpose";
    int petscArgc =0/*1*/;
    char* testPurpose = "petscSolve";
    char** petscArgv = &testPurpose;

    PetscFunctionBeginUser;
    //PetscInitialize(&petscArgc, &petscArgv, NULL, NULL);
   // PetscInitialize(argc, argv, NULL, NULL);

//    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
//    if(rank==0){
     init(/*&petscKG, &petscConstrMat, &petscConstrVec*/);//These objects are created and their addresses kept in DataForPetsc
    MPI_Barrier(MPI_COMM_WORLD);

    numberOfConstraints = constrMat.nrow();
    solSize = constrMat.ncol();
   // MatGetSize(pbcConstraintsPetscMat, &numberOfConstraints, &solSize);

    VecCreate(PETSC_COMM_WORLD, &U);
    VecSetSizes(U, PETSC_DECIDE, solSize);
    VecSetFromOptions(U);
    VecSetUp(U);
    VecSet(U, /*0.01*//*0.08,*/ /*0.005*/0.0); //Solution vector must be initialized
//    VecAssemblyBegin(U);
//    VecAssemblyEnd(U);
    PetscScalar ff=0; Vec U1; PetscScalar gg=0; Vec U2;
    MatCreateVecs(*(this->getRigidityMat()), &U1, NULL);
    MatMult(*(getRigidityMat()), U, U1);
    VecDot(U, U1,&ff);
    MatCreateVecs(*(this->getConstraintsMat()), NULL, &U2);
    MatMult(*(getConstraintsMat()), U, U2);
    VecDot(U2, U2, &gg);

/* Setting up the obective function gradient */
    VecCreate(PETSC_COMM_WORLD, &gradf);
    VecSetSizes(gradf, PETSC_DECIDE, solSize);
    VecSetFromOptions(gradf);
    VecSetUp(gradf);
//    VecSet(gradf, 0.00); //gradient of the objective function - not necessary to initialize it
//    VecAssemblyBegin(gradf);
//    VecAssemblyEnd(gradf);


/* Setting up the Hessian */
    MatCreate(PETSC_COMM_WORLD, &H);
    MatSetSizes(H, PETSC_DECIDE, PETSC_DECIDE, solSize, solSize);
    MatSetFromOptions(H);
    MatAssemblyBegin(H, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(H, MAT_FINAL_ASSEMBLY);

/* Setting up the constraints vector */
    VecCreate(PETSC_COMM_WORLD, &ce);
    VecSetSizes(ce, PETSC_DECIDE, numberOfConstraints);
    VecSetFromOptions(ce);
    VecSetUp(ce);
//    VecAssemblyBegin(ce);
//    VecAssemblyEnd(ce);

    MatCreate(PETSC_COMM_WORLD, &J);
    MatSetSizes(J, PETSC_DECIDE, PETSC_DECIDE, numberOfConstraints, solSize);
    MatSetFromOptions(J);
    MatSetUp(J);
    MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);


    TaoCreate(PETSC_COMM_WORLD, &tao);
    TaoSetType(tao, TAOALMM);//TAOIPM //TAOALMM //TAOPDIPM //TAOCG //TAOADMM
    TaoSetSolution(tao, U);
    TaoSetFromOptions(tao);


//    TaoSetObjectiveAndGradient(tao, NULL, objAndGradFunc, (void*)this);
    TaoSetObjectiveAndGradient(tao, gradf, objAndGradFunc, (void*)this);
    TaoSetHessian(tao, H, H, hessianFunc, (void*)this);
    TaoSetEqualityConstraintsRoutine(tao, ce, constraintsFunc, (void*)this);
    TaoSetJacobianEqualityRoutine(tao, J, J, constraintsGradFunc, (void*)this);

        TaoSetTolerances(tao, 1.e-5, 0.0, 0.0);
        TaoSetConstraintTolerances(tao, 1.e-5, 0.0);
        //TaoSetMaximumFunctionEvaluations(tao, 1e6);
        TaoSetMaximumFunctionEvaluations(tao, PETSC_UNLIMITED);

    Vec Ul, Uu;
    VecDuplicate(U, &Ul);
    VecDuplicate(U, &Uu);
    VecSet(Ul, -0.5/*-1*/);
    VecSet(Uu, 0.5/*1*/);
    TaoSetVariableBounds(tao, Ul, Uu);
    TaoSetUp(tao);


    PetscBarrier( (PetscObject)tao);
    TaoSolve(tao);

    //reason = TAO_CONTINUE_ITERATING;
    TaoGetConvergedReason(tao, &reason);
    if(reason>0) std::cout<<"Converged with reason : "<<reason<<std::endl;
    else std::cout<<"Diverged with reason : "<<reason<<std::endl;

    PetscScalar yy[3]; PetscInt ix[3]={0, 100, 112};
    VecGetValues(U, 3, ix, yy);

    DataForPetsc::VecToValarray(U, sol);

    PetscScalar ku, kup; Vec Uc;
    MatCreateVecs(*(getRigidityMat()), NULL, &Uc);
    MatMult(*(getRigidityMat()), U, Uc);
    VecDot(U, Uc, &ku);// tbdl
   // MatDestroy(&petscKG);
   // MatDestroy(&petscConstrMat);
    //VecDestroy(&petscConstrVec);

    PetscBarrier( PetscObject(tao) );
    VecDestroy(&U); VecDestroy(&ce); VecDestroy(&Ul); VecDestroy(&Uu);
    MatDestroy(&J); MatDestroy(&H);
    TaoDestroy(&tao);
    //}//End MPI_Comm_Rank
    //PetscFinalize();
    PetscFunctionReturn(PETSC_SUCCESS);
}


} // namespace models
} // namespace scaletag
