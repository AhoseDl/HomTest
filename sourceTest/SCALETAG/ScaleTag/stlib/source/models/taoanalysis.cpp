#include<taoanalysis.h>
#include<fstream>

//#include"/home/ahose/Desktop/ThinkCell/kuu.h"

namespace scaletag {
namespace models {


PetscErrorCode DataForPetsc::jacobiCond(Mat& _petscKG, Mat& _petscConstrMat, Vec& _petscConstrVec, Vec* _DInv, PetscScalar& lmax ){

    //EPS eps; //EPSWhich which();
    Vec DInv;
    PetscInt nr;

    PetscInt lr, hr; PetscScalar* val;

    PetscFunctionBeginUser;

// computing _Dinv (D⁻1/2)
//    MatGetSize(_petscKG, &nr, NULL);
//    VecCreate(PETSC_COMM_WORLD, _DInv);
//    VecSetSizes(*_DInv, PETSC_DECIDE, nr);
//    VecSetFromOptions(*_DInv);
//    VecSetUp(*_DInv);
    MatCreateVecs(_petscKG, NULL, _DInv);

    MatGetDiagonal(_petscKG, *_DInv);
    VecGetOwnershipRange(*_DInv, &lr, &hr);
    VecGetArray(*_DInv, &val);
    for(PetscInt i=0; i<hr-lr; i++)
        val[i] = PetscSqrtReal(1./(val[i]));
    VecRestoreArray(*_DInv, &val);
    VecAssemblyBegin(*_DInv);
    VecAssemblyEnd(*_DInv);

//Computing the conditioned petscKG; petscKGCond = DInv*petscKG*DInv;
    MatDiagonalScale(_petscKG, *_DInv, *_DInv);

    VecGetOwnershipRange(*_DInv, &lr, &hr); const PetscScalar* va;
    if(lr==0) {VecGetArrayRead(*_DInv, &va); std::cout<<"DInv(0, 0) "<<va[0]<<std::endl; VecRestoreArrayRead(*_DInv, &va);}

//    PetscInt kr, kc;
//    MatGetOwnershipRange(_petscKG, &kr, &kc); PetscScalar va;
//    for(PetscInt i=kr; i<kc; i++) {MatGetValue(_petscKG, i, i, &va); std::cout<<"("<<i<<", "<<i<< ")"<<va<<std::endl;}

//Computing the maximal eigenvalue
    lmax=solveEigen(_petscKG, EPS_LARGEST_REAL); // Here petscKG has already been conditioned
    std::cout<<"lmax in jacobiCond : "<<lmax<<std::endl;
    lmax=1.0; //3.28967
 //Computing petscConstrMat*DInv (GD-1/2)
    MatDiagonalScale(_petscConstrMat, NULL, *_DInv);

    //Computing InvEta = InvEta(GD-1/2)



 // Computing the conditioned petscConstrMat; petscConstrMatCond = lmax*InvEta*petscConstrMat*DInv
    PetscInt nG; //, ri, rf;
    PetscInt nnz;
    const PetscScalar* vals;
    PetscScalar valr=0.;
    Vec diagVec;

    PetscScalar* diagVecPtr;

    MatGetSize(_petscConstrMat, &nG, NULL);
    //VecCreate(PETSC_COMM_WORLD, &diagVec);
    MatCreateVecs(_petscConstrMat,  NULL, &diagVec);
    //VecSetSizes(diagVec, PETSC_DECIDE, nG);
    //VecSetFromOptions(diagVec);
    //VecSetUp(diagVec);

    MatGetOwnershipRange(_petscConstrMat, &lr, &hr);
    VecGetArray(diagVec, &diagVecPtr);
    for(PetscInt r=lr; r<hr; r++){
        MatGetRow(_petscConstrMat, r, &nnz, NULL, &vals);
        valr = 0;
        for(PetscInt i=0; i<nnz; i++) valr += vals[i]*vals[i];
        if(valr==0.) std::cout<<"Having a problem with the matrix G"<<std::endl;
        valr = PetscSqrtReal(lmax)/PetscSqrtReal(valr);
        MatRestoreRow(_petscConstrMat, r, &nnz, NULL, &vals);
        //VecSetindicesValue(diagVec, r, valr, INSERT_VALUES);
        diagVecPtr[r-lr]=valr;
        //std::cout<<"constraints diagVec for r="<<r<<"is : "<<valr<<std::endl;
    }
    VecRestoreArray(diagVec, &diagVecPtr);
    VecAssemblyBegin(diagVec);
    VecAssemblyEnd(diagVec);

    MatDiagonalScale(_petscConstrMat, diagVec, NULL);
    VecPointwiseMult(_petscConstrVec, diagVec, _petscConstrVec);

       // _DInv=&DInv;

PetscFunctionReturn(PETSC_SUCCESS);
}



//************************* End jacobiCond *****************************
PetscScalar solveEigen(Mat A, EPSWhich which) {
EPS eps;
EPSCreate(PETSC_COMM_WORLD, &eps);
EPSSetOperators(eps, A, NULL);         // Standard eigenproblem: A*x = λ*x
EPSSetProblemType(eps, EPS_HEP); //EPS_NLEIG EPS_HEP EPS_GHEP      // Hermitian (symmetric) problem

if(which==EPS_SMALLEST_REAL){
        ST st;
        EPSGetST(eps, &st);                        // Get the Spectral Transform object
        STSetType(st, STSINVERT);                  // Shift-and-invert: (A - σI)⁻¹

        //PetscScalar shift = 0.0+1.;                   // σ = 0: targets eigenvalues near 0
        //STSetShift(st, shift);                     // Apply the shift

        // Configure the linear solver used inside the ST (KSP + PC)
        KSP ksp;
        STGetKSP(st, &ksp);
        KSPSetType(ksp, KSPPREONLY);               // Direct solve (LU), not iterative
        PC pc;
        KSPGetPC(ksp, &pc);
        PCSetType(pc, PCLU); //PCCHOLESKY                      // LU factorization for exact solvesls
        //The LU factorization is possible if petsc is compiled with MUMPS

        //PCSetUp(pc);
        //EPSSetFromOptions(eps);
        //PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);        // -------------------------------------------------------
        //STSetUp(st);
        //        KSPSetType(ksp, KSPGMRES);
        //        PC pc;
        //        KSPGetPC(ksp, &pc);
        //        PCSetType(pc, PCBJACOBI);                // Block Jacobi with local ILU



        //KSPSetTolerances(ksp, 1e-14, PETSC_DEFAULT, PETSC_DEFAULT, 200);
        // Target the largest transformed eigenvalue (= smallest original)
        //EPSSetTarget(eps, 0.0);

        PetscCall(STSetShift(st, -1.0));   // or any negative value outside the spectrum
        PetscCall(EPSSetTarget(eps, /*-1.0*/0.));
        EPSSetWhichEigenpairs(eps, EPS_TARGET_REAL);
}
else{
        EPSSetWhichEigenpairs(eps, which);     // EPS_LARGEST_REAL or EPS_SMALLEST_REAL
}

EPSSetDimensions(eps, 1, /*PETSC_DEFAULT*/ PETSC_DECIDE, /*PETSC_DEFAULT*/PETSC_DECIDE); // Only 1 eigenpair
PetscCall(EPSSetTolerances(eps, 1e-14, 1000));
EPSSetFromOptions(eps);                // Allow command-line overrides

EPSSolve(eps);

PetscInt    nconv;
EPSConvergedReason epsReason;
EPSGetConvergedReason(eps, &epsReason);
if(epsReason<0) std::cout<<" Eigen problem unconverged with reason "<<epsReason<<" "<<std::endl;
else std::cout<<" Eigen problem converged with reason "<<epsReason<<" "<<std::endl;
EPSGetConverged(eps, &nconv);
if (nconv < 1)
        throw std::runtime_error("No eigenvalue converged!");

PetscScalar eigval;
EPSGetEigenvalue(eps, 0, &eigval, NULL);

//EPSSetDimensions(eps, 6, PETSC_DECIDE, PETSC_DECIDE);  // ask for 6
//EPSSolve(eps);
//EPSGetConverged(eps, &nconv);
//for (PetscInt i = 0; i < nconv; i++) {
//        PetscScalar eigr, eigi;
//        EPSGetEigenvalue(eps, i, &eigr, &eigi);
//        PetscPrintf(PETSC_COMM_WORLD, "λ[%d] = %+e\n", i, PetscRealPart(eigr));
//}

EPSDestroy(&eps);
return eigval;
}


PetscScalar solveEigen2(Mat A, EPSWhich which) {
EPS eps;
EPSCreate(PETSC_COMM_WORLD, &eps);
EPSSetOperators(eps, A, NULL);         // Standard eigenproblem: A*x = λ*x
EPSSetProblemType(eps, EPS_HEP); //EPS_NLEIG EPS_HEP EPS_GHEP      // Hermitian (symmetric) problem

EPSSetWhichEigenpairs(eps, which);     // EPS_LARGEST_REAL or EPS_SMALLEST_REAL

EPSSetDimensions(eps, 1, /*PETSC_DEFAULT*/ PETSC_DECIDE, /*PETSC_DEFAULT*/PETSC_DECIDE); // Only 1 eigenpair
PetscCall(EPSSetTolerances(eps, 1e-14, 1000));
EPSSetFromOptions(eps);                // Allow command-line overrides

EPSSolve(eps);

PetscInt    nconv;
EPSConvergedReason epsReason;
EPSGetConvergedReason(eps, &epsReason);
if(epsReason<0) std::cout<<" Eigen problem unconverged with reason "<<epsReason<<" "<<std::endl;
else std::cout<<" Eigen problem converged with reason "<<epsReason<<" "<<std::endl;
EPSGetConverged(eps, &nconv);
if (nconv < 1)
        throw std::runtime_error("No eigenvalue converged!");

PetscScalar eigval;
EPSGetEigenvalue(eps, 0, &eigval, NULL);
EPSDestroy(&eps);
return eigval;
}
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
        PetscScalar f0;

//        PetscScalar initNorm;
//        initNorm = (reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getInitialGuessNorm();

        PetscFunctionBeginUser;

            VecDuplicate(X, &Xd);
            //MatMult(*KG, X, Xd);
            MatMult(*((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getRigidityMat()), X, Xd);
            VecDot(X, Xd, f);
            *f=0.5*(*f);

            f0=(reinterpret_cast<Data*>(ctx))->getObjectInit();
            *f=*f/f0; // two previous lines commented on 07/05

            //std::cout<<"f0 :"<<f0<<std::endl;
            //In computing of g, we suppose that KG is symmetric
//            MatMult(*KG, X, g);
            MatMult(*((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getRigidityMat()), X, g);
        //}
            VecScale(g, 1./f0); // two previous lines commented on 07/05
            VecDestroy(&Xd);
        PetscFunctionReturn(PETSC_SUCCESS);

}

//Function for computing the Hessian
PetscErrorCode hessianFunc(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx){

        // Mat* mat;
        PetscScalar f0;
        PetscFunctionBeginUser;

        // mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
        //MatDuplicate(*mat, MAT_COPY_VALUES, &J);
        // MatCopy(*mat, J, /*SAME_NONZERO_PATTERN*/DIFFERENT_NONZERO_PATTERN);
        MatCopy( *((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getRigidityMat()) , H, DIFFERENT_NONZERO_PATTERN);
        // MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY); MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);

        /* DataForPetsc::matrixToPetscMat(*((reinterpret_cast<DataForPetsc*>(ctx))->getStdConstraintsMat()), mat);
        MatCopy(*mat, J, DIFFERENT_NONZERO_PATTERN);
        MatDestroy(mat);*/
        MatAssemblyBegin(H, MAT_FINAL_ASSEMBLY);
        MatAssemblyEnd(H, MAT_FINAL_ASSEMBLY);

        f0=(reinterpret_cast<Data*>(ctx))->getObjectInit();
        MatScale(H, 1./f0); // two previous lines commented on 07/05

        PetscFunctionReturn(PETSC_SUCCESS);
}


//Function for computing constraints
PetscErrorCode constraintsFunc(Tao tao, Vec X, Vec ce, void* ctx){
        Vec matVec;
        PetscInt dimRCe, dimCCe;
    //    Mat* mat;
      //  Vec* vec;

        Vec tempCe;

        PetscFunctionBegin;

        //mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
        //vec = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsVec();
//        MatGetSize(*mat, &dimRCe, &dimCCe);
        MatGetSize(*((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getConstraintsMat()), &dimRCe, &dimCCe);
        VecCreate(PETSC_COMM_WORLD, &matVec);
        VecSetSizes(matVec, PETSC_DECIDE, dimRCe );
        VecSetUp(matVec);

//        MatMult(*mat, X, matVec);
        MatMult(*((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getConstraintsMat()), X, matVec);
//        VecWAXPY(ce, -1, *vec, matVec);

        //VecWAXPY(ce, -1, *((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getConstraintsVec()), matVec);

        //VecDuplicate(ce, &tempCe);
        VecWAXPY(/*tempCe*/ce, -1, *((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getConstraintsVec()), matVec);
        //MatMult(*((reinterpret_cast<Data*>(ctx))->getCondMat()), tempCe, ce);


       // MatMult(mat, vec, matVec); //vec no more a pointer
       // VecCopy(matVec, ce);
        PetscScalar f0;
        f0=(reinterpret_cast<Data*>(ctx))->getObjectInit();
        VecScale(ce, 1./f0);
        VecDestroy(&matVec);
        PetscFunctionReturn(PETSC_SUCCESS);

}

//Function for computing constraints gradient
PetscErrorCode constraintsGradFunc(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx){

        Mat tempJ; //PetscInt nr, nc;

        PetscFunctionBeginUser;

        //MatGetSize(
        // mat = (reinterpret_cast<DataForPetsc*>(ctx))->getConstraintsMat();
        //MatDuplicate(*mat, MAT_COPY_VALUES, &J);
        // MatCopy(*mat, J, /*SAME_NONZERO_PATTERN*/DIFFERENT_NONZERO_PATTERN);
        //MatMatMult(Mat A, Mat B, MatReuse scall, PetscReal fill, Mat *C) //MAT_INITIAL_MATRIX.

        /* Instead of MatConvert(), one can use MatDiagonalScale() for more efficiency */
        //     MatConvert(*((reinterpret_cast<Data*>(ctx))->getCondMat()), MATMPIAIJ, MAT_INPLACE_MATRIX, (reinterpret_cast<Data*>(ctx))->getCondMat());
        //MatMatMult(*((reinterpret_cast<Data*>(ctx))->getCondMat()),
        //    *((reinterpret_cast<Data*>(ctx))->getConstraintsMat()),MAT_INITIAL_MATRIX,/*PETSC_DETERMINE*/PETSC_DEFAULT, &tempJ);//multiplication forbidden between MATDIAGONALE and MPIAIJ
        //MatCopy( tempJ , J, SAME_NONZERO_PATTERN);
        //MatCopy( tempJ , J, DIFFERENT_NONZERO_PATTERN);
        //MatCopy( *((reinterpret_cast</*DataForPetsc**/Data*>(ctx))->getConstraintsMat()) , J, DIFFERENT_NONZERO_PATTERN);
        // MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY); MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);

        /* DataForPetsc::matrixToPetscMat(*((reinterpret_cast<DataForPetsc*>(ctx))->getStdConstraintsMat()), mat);
        MatCopy(*mat, J, DIFFERENT_NONZERO_PATTERN);
        MatDestroy(mat);*/

        //MatAssemblyBegin(J, MAT_FLUSH_ASSEMBLY);
        //MatAssemblyEnd(J, MAT_FLUSH_ASSEMBLY);
        // MatDestroy(&tempJ);


        MatCopy( *((reinterpret_cast<Data*>(ctx))->getConstraintsMat()) , J, DIFFERENT_NONZERO_PATTERN);

                PetscScalar f0;
        f0=(reinterpret_cast<Data*>(ctx))->getObjectInit();
        MatScale(J, 1./f0);
        PetscFunctionReturn(PETSC_SUCCESS);


}

void ToRemove(){
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
}

//Definitions of the class DataForPetsc members

DataForPetsc::DataForPetsc( fem::matrix<double>* _KG, fem::matrix<double>* _constrMat, std::valarray<double>* _constrVec,
    PetscErrorCode (*_objAndGradFunc)(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx),
    PetscErrorCode (*_hessianFunc)(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx),
    PetscErrorCode (*_constraintsFunc)(Tao tao, Vec X, Vec ce, void* ctx),
    PetscErrorCode (*_constraintsGradFunc)(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx)
                           ){}//:KG(_KG), constrMat(_constrMat), constrVec(_constrVec), objAndGradFunc(_objAndGradFunc), hessianFunc(_hessianFunc),constraintsFunc(_constraintsFunc), constraintsGradFunc(_constraintsGradFunc){}

 PetscErrorCode DataForPetsc::valarrayToVec(const std::valarray<double>&val, Vec** res){

    /*The conversion is parallelized on PETSC_COMM_WORLD*/
    //PetscMPIInt rank;

    PetscInt N=val.size();
    PetscInt rstart, rend;

    //PetscReal nrm;

    PetscFunctionBeginUser;
    //MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    //if(rank==0){
    VecCreate(PETSC_COMM_WORLD, *res);
    //PetscObjectSetName((PetscObject)res, &res);
    VecSetSizes(**res, PETSC_DECIDE, N);
    VecSetFromOptions(**res); //VecSetType(res, VECSEQ);
    VecSetUp(**res);
    VecGetOwnershipRange(**res, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++) VecSetValue(**res, i, (PetscScalar)val[i], INSERT_VALUES);
    VecAssemblyBegin(**res);
    VecAssemblyEnd(**res);
    //}

//    VecGetSize(**res, &N);
//    VecNorm(**res, NORM_2 , &nrm);
//    std::cout<<N<<" : "<<nrm<<std::endl;
    PetscFunctionReturn(PETSC_SUCCESS);

}


PetscErrorCode DataForPetsc::valarrayToVec(const std::valarray<double>&val, Vec& res){

    /*The conversion is parallelized on PETSC_COMM_WORLD*/
    //PetscMPIInt rank;

    PetscInt N=val.size();
    PetscInt rstart, rend;

    //PetscReal nrm;

    PetscFunctionBeginUser;
    //MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    //if(rank==0){
    VecCreate(PETSC_COMM_WORLD, &res);
    //PetscObjectSetName((PetscObject)res, &res);
    VecSetSizes(res, PETSC_DECIDE, N);
    VecSetFromOptions(res); //VecSetType(res, VECSEQ);
    VecSetUp(res);
    PetscScalar* resV;
    VecGetOwnershipRange(res, &rstart, &rend);
    VecGetArray(res, &resV);
    for(PetscInt i=rstart; i<rend; i++) resV[i-rstart]=(PetscScalar)val[i];
        //VecSetValue(res, i, (PetscScalar)val[i], INSERT_VALUES);
    VecRestoreArray(res, &resV);
    VecAssemblyBegin(res);
    VecAssemblyEnd(res);
    //}

    //    VecGetSize(**res, &N);
    //    VecNorm(**res, NORM_2 , &nrm);
    //    std::cout<<N<<" : "<<nrm<<std::endl;
    PetscFunctionReturn(PETSC_SUCCESS);

}

PetscErrorCode DataForPetsc::valarrayToVecG(const std::valarray<double>&val, const Mat& G, Vec& res){

    /*The conversion is parallelized on PETSC_COMM_WORLD*/
    //PetscMPIInt rank;

    PetscInt N=val.size();
    PetscInt rstart, rend;

    //PetscReal nrm;

    PetscLayout rLay;
    PetscFunctionBeginUser;
    //MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    //if(rank==0){
    VecCreate(PETSC_COMM_WORLD, &res);
    //PetscObjectSetName((PetscObject)res, &res);
    MatGetLayouts(G, &rLay, NULL);
    //VecSetSizes(res, PETSC_DECIDE, N);
    VecSetLayout(res, rLay);
    VecSetFromOptions(res); //VecSetType(res, VECSEQ);
    VecSetUp(res);
    VecGetOwnershipRange(res, &rstart, &rend);
    PetscScalar *resRead;
    VecGetArray(res, &resRead);
    for(PetscInt i=rstart; i<rend; i++) resRead[i-rstart] = (PetscScalar)val[i];
        //VecSetValue(res, i, (PetscScalar)val[i], INSERT_VALUES);
    VecRestoreArray(res, &resRead);
    VecAssemblyBegin(res);
    VecAssemblyEnd(res);
    //}

    //    VecGetSize(**res, &N);
    //    VecNorm(**res, NORM_2 , &nrm);
    //    std::cout<<N<<" : "<<nrm<<std::endl;
    PetscFunctionReturn(PETSC_SUCCESS);

}

PetscErrorCode DataForPetsc::VecToValarray(const Vec& vec, std::valarray<double>& res){

    /* vec is supposed distributed on PETSC_COMM_WORLD*/

    PetscInt N; const PetscScalar* vecRead;
    PetscInt rstart, rend;

    Vec vout;
    VecScatter ctx;

    PetscFunctionBeginUser;
    VecGetSize(vec, &N);
    res.resize(N, 0.0);
  //  VecGetOwnershipRange(vec, &rstart, &rend);
   // VecScatterCreate(vec, NULL, vout, NULL, ctx);
    VecScatterCreateToAll(vec,&ctx, &vout);
    VecScatterBegin(ctx, vec, vout, INSERT_VALUES, SCATTER_FORWARD);
    VecScatterEnd(ctx, vec, vout, INSERT_VALUES, SCATTER_FORWARD);
    VecGetArrayRead(vout, &vecRead);
    //for(PetscInt i=rstart; i<rend; i++) res[i]=vecRead[i-rstart];
    for(PetscInt i=0; i<N; i++) res[i]=(double)vecRead[i];
    VecRestoreArrayRead(vout, &vecRead);

    VecScatterDestroy(&ctx);
    VecDestroy(&vout);

    PetscFunctionReturn(PETSC_SUCCESS);}

PetscErrorCode DataForPetsc::matrixToPetscMat(const fem::matrix<double>& mat, Mat** res){

    PetscInt N, M;
    std::valarray<size_t> matPtr, matIdx;
    std::valarray<double> matVal;
    PetscInt rstart, rend;
    Mat allocator;
    //MPI_Comm matComm;

    PetscFunctionBeginUser;
    //As it can be sometimes difficult to compute the required preallocation information
    //efficiently, we adopt the solution below :
    N=mat.nrow(); M=mat.ncol();
    matPtr = mat.getPtr(); matIdx= mat.getColumnsIndex(); matVal=mat.getVal();
    MatCreate(PETSC_COMM_WORLD, &allocator);
    MatSetType(allocator, MATPREALLOCATOR);
    MatSetSizes(allocator, PETSC_DECIDE, PETSC_DECIDE, N, M);
    MatSetFromOptions(allocator);
    MatSetUp(allocator);

    MatGetOwnershipRange(allocator, &rstart, &rend);

    for(PetscInt i=rstart; i<rend; i++)
        for(PetscInt j=(PetscInt) matPtr[i]; j<(PetscInt)matPtr[i+1]; j++){
            // cout<<"indexes : "<<i<<" , "<< matIdx[j]<<std::endl;
            //MatSetValue(allocator,i, matIdx[j] , matVal[j], INSERT_VALUES);
            PetscInt ii=i, jj=matIdx[j];
            MatSetValues(allocator,1,&ii, 1,&jj , NULL, INSERT_VALUES);
        }
    PetscBarrier( (PetscObject)allocator );
    MatAssemblyBegin(allocator, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(allocator, MAT_FINAL_ASSEMBLY);
    PetscBarrier( (PetscObject)allocator );

    //Filling Mat res
    MatCreate(PETSC_COMM_WORLD, *res);
    MatSetSizes(**res, PETSC_DECIDE, PETSC_DECIDE, N, M);
    MatSetType(**res, MATMPIAIJ);
    MatSetFromOptions(**res);
    MatPreallocatorPreallocate(allocator, PETSC_TRUE, **res);
   // MatSetUp(*res);
    PetscBarrier( (PetscObject) allocator );
    //    MatDestroy(&allocator);
    MatGetOwnershipRange(**res, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++){
          for(PetscInt j=  (PetscInt)matPtr[i] ; j<(PetscInt)matPtr[i+1] ; j++){
            //MatSetValue(*res,i, (PetscInt)matIdx[j] , matVal[j], INSERT_VALUES);
            PetscInt ii=i, jj=matIdx[j]; PetscScalar val=matVal[j];
            MatSetValues(**res, 1,&ii, 1,&jj , &val, INSERT_VALUES);
        }
    }
    MatAssemblyBegin(**res, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(**res, MAT_FINAL_ASSEMBLY);
    PetscBarrier( (PetscObject)allocator );
    MatDestroy(&allocator);
    PetscFunctionReturn(PETSC_SUCCESS);
}

PetscErrorCode DataForPetsc::matrixToPetscMat(const fem::matrix<double>& mat, Mat& res){
    PetscInt N, M;
    std::valarray<size_t> matPtr, matIdx;
    std::valarray<double> matVal;
    PetscInt rstart, rend;
    Mat allocator;

    //std::cout<<"global size for the matrix creation (transmitted) is : "<< mat.nrow()<< " x "<<mat.ncol()<<std::endl;
    PetscFunctionBeginUser;
    //As it can be sometimes difficult to compute the required preallocation information
    //efficiently, we adopt the solution below :

    N=mat.nrow(); M=mat.ncol();
    matPtr = mat.getPtr(); matIdx= mat.getColumnsIndex(); matVal=mat.getVal();
    //    MatCreate(PETSC_COMM_WORLD, &allocator);
    //    MatSetType(allocator, MATPREALLOCATOR);
    //    MatSetSizes(allocator, PETSC_DECIDE, PETSC_DECIDE, N, M);
    //    MatSetFromOptions(allocator);
    //    MatSetUp(allocator);

    //    MatGetOwnershipRange(allocator, &rstart, &rend);
    //    for(PetscInt i=rstart; i<rend; i++)
    //        for(PetscInt j=(PetscInt) matPtr[i]; j<(PetscInt)matPtr[i+1]; j++){
    //            // cout<<"indexes : "<<i<<" , "<< matIdx[j]<<std::endl;
    //            //MatSetValue(allocator,i, matIdx[j] , matVal[j], INSERT_VALUES);
    //            PetscInt ii=i, jj=matIdx[j];
    //            MatSetValues(allocator,1,&ii, 1,&jj , NULL, INSERT_VALUES);

    //        }
    //    PetscBarrier( (PetscObject)allocator );
    //    MatAssemblyBegin(allocator, MAT_FINAL_ASSEMBLY);
    //    MatAssemblyEnd(allocator, MAT_FINAL_ASSEMBLY);
    //    PetscBarrier( (PetscObject)allocator );

    //    //Filling Mat res
    //    MatCreate(PETSC_COMM_WORLD, &res);
    //    MatSetSizes(res, PETSC_DECIDE, PETSC_DECIDE, N, M);
    //    MatSetFromOptions(res);
    //    MatSetType(res, MATMPIAIJ/*MATAIJ*/);

    //    //MatSetUp(res);
    //    MatPreallocatorPreallocate(allocator, PETSC_TRUE, res);
    //    // MatSetUp(*res);

    //    PetscBarrier( (PetscObject) allocator );
    //    //    MatDestroy(&allocator);
    std::cout<<"global size for the rigidity matrix creation is : "<< N<< " x "<<M<<std::endl;
    MatCreate(PETSC_COMM_WORLD, &res);
    MatSetType(res, /*MATMPIAIJ*/MATAIJ);
    MatSetSizes(res, PETSC_DECIDE, PETSC_DECIDE, N, M);
    MatSetFromOptions(res);
    MatSetUp(res);
    MatGetOwnershipRange(res, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++){
        for(PetscInt j=  (PetscInt)matPtr[i] ; j<(PetscInt)matPtr[i+1] ; j++){
            //            MatSetValue(*res,i, (PetscInt)matIdx[j] , matVal[j], INSERT_VALUES);
            PetscInt ii=i, jj=matIdx[j]; PetscScalar val=matVal[j];
            if(jj>=(PetscInt)mat.ncol()){cout<<"i : "<<i<<" j : "<<jj<< " issue "<<endl; continue;}
            MatSetValues(res, 1,&ii, 1,&jj , &val, INSERT_VALUES);
        }
    }

    MatAssemblyBegin(res, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(res, MAT_FINAL_ASSEMBLY);
   // PetscBarrier( (PetscObject)allocator );
    //MatDestroy(&allocator);
    PetscBarrier(PetscObject(res));

    //    MatGetSize(**res, &N, &M);
    //    MatNorm(**res, NORM_FROBENIUS, &nrm);
    //    std::cout<<N<<"  "<<M<<" : "<<nrm<<std::endl;
    PetscFunctionReturn(PETSC_SUCCESS);
}

    PetscErrorCode DataForPetsc::matrixToPetscMatG(const fem::matrix<double>& mat, const Mat& K, Mat** res){
    PetscInt N, M;
    std::valarray<size_t> matPtr, matIdx;
    std::valarray<double> matVal;
    PetscInt rstart, rend;
    Mat allocator;

    PetscLayout clay; PetscInt n;

    PetscFunctionBeginUser;

    //As it can be sometimes difficult to compute the required preallocation information
    //efficiently, we adopt the solution below :

    MatGetLayouts(K, NULL, &clay);
    PetscLayoutGetLocalSize(clay, &n);
    N=mat.nrow(); M=mat.ncol();
    matPtr = mat.getPtr(); matIdx= mat.getColumnsIndex(); matVal=mat.getVal();
    MatCreate(PETSC_COMM_WORLD, &allocator);
    MatSetType(allocator, MATPREALLOCATOR);
    MatSetSizes(allocator, PETSC_DECIDE, /*PETSC_DECIDE*/n, N, M);
    MatSetFromOptions(allocator);
    MatSetUp(allocator);

    MatGetOwnershipRange(allocator, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++)
        for(PetscInt j=(PetscInt) matPtr[i]; j<(PetscInt)matPtr[i+1]; j++){
            // cout<<"indexes : "<<i<<" , "<< matIdx[j]<<std::endl;
            //MatSetValue(allocator,i, matIdx[j] , matVal[j], INSERT_VALUES);
            PetscInt ii=i, jj=matIdx[j];
            MatSetValues(allocator,1,&ii, 1,&jj , NULL, INSERT_VALUES);
        }

    PetscBarrier( (PetscObject)allocator );
    MatAssemblyBegin(allocator, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(allocator, MAT_FINAL_ASSEMBLY);
    PetscBarrier( (PetscObject)allocator );

    //Filling Mat res
    MatCreate(PETSC_COMM_WORLD, *res);
    MatSetSizes(**res, PETSC_DECIDE, PETSC_DECIDE, N, M);
    MatSetFromOptions(**res);
    MatSetType(**res, MATMPIAIJ);
    MatPreallocatorPreallocate(allocator, PETSC_TRUE, **res);
    //MatSetUp(**res);
    PetscBarrier( (PetscObject) allocator );
    //    MatDestroy(&allocator);

    MatGetOwnershipRange(**res, &rstart, &rend);
    for(PetscInt i=rstart; i<rend; i++){
        for(PetscInt j=  (PetscInt)matPtr[i] ; j<(PetscInt)matPtr[i+1] ; j++){
            //MatSetValue(*res,i, (PetscInt)matIdx[j] , matVal[j], INSERT_VALUES);
            PetscInt ii=i, jj=matIdx[j]; PetscScalar val=matVal[j];
            MatSetValues(**res, 1,&ii, 1,&jj , &val, INSERT_VALUES);
        }
    }

    MatAssemblyBegin(**res, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(**res, MAT_FINAL_ASSEMBLY);
    PetscBarrier( (PetscObject)allocator );
    MatDestroy(&allocator);

    //    MatGetSize(**res, &N, &M);
    //    MatNorm(**res, NORM_FROBENIUS, &nrm);
    //    std::cout<<N<<"  "<<M<<" : "<<nrm<<std::endl;
    PetscFunctionReturn(PETSC_SUCCESS);
}

PetscErrorCode DataForPetsc::init(const fem::matrix<double>&KG,
                                   const fem::matrix<double> &constrMat, const std::valarray<double>& constrVec,
                                  Mat* _petscKG, Mat* _petscConstrMat, Vec& _petscConstrVec){

    //PetscScalar a=1.e7;
    //PetscScalar a=1.0;
    PetscScalar a = std::max( std::fabs(KG.min()), std::fabs(KG.max()) );
   // a =a*1e4;
    a*=1.e0;
    a=1./a;
    a=1.;
        PetscFunctionBeginUser;
    //PetscBarrier((PetscObject)

        MPI_Barrier(MPI_COMM_WORLD);
        DataForPetsc::matrixToPetscMat(KG, &_petscKG/*&petscKG*/);
        DataForPetsc::matrixToPetscMatG(constrMat, *_petscKG, &_petscConstrMat/*&petscConstrMat*/);
        DataForPetsc::valarrayToVecG(constrVec, *_petscConstrMat, _petscConstrVec/*&petscConstrVec*/);

        //For convergence purpose
        MatScale(*_petscConstrMat, a);
        VecScale(_petscConstrVec, a);

        std::cout<<"a in init() : "<<a<<std::endl;

        MPI_Barrier(MPI_COMM_WORLD);
        PetscFunctionReturn(PETSC_SUCCESS);
 }


 PetscErrorCode DataForPetsc::petscSolve(const MeshPbc<double,3>& Mh, size_t ndof,
const fem::matrix<double>& _KG, const fem::matrix<double>& constrMat,const std::valarray<double>& constrVec,
        /*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec,*/ std::valarray<double>& sol){

        Tao tao;     TaoConvergedReason reason; Tao subsolver/*(NULL)*/;
    Vec U, ce; //Solution vector and constraint vector;
    Mat J; //d(ce)/dU -->The jacobian of the objective function
    Mat H; //To contain the obective function Hessian
    PetscInt numberOfConstraints, solSize;

    Mat petscKG;
    Mat petscConstrMat;
    Vec petscConstrVec;
    Vec gradf; //to hold gradient of the objective function

    Mat GGt; //for condition number purpose
    PetscScalar mu, condNum;
    Mat condGMat;

    PetscFunctionBeginUser;
    DataForPetsc dataForPetsc;


    {
//    Mat            A;           /* problem matrix */
//    EPS            eps;         /* eigenproblem solver context */
//    EPSType        type;
//    PetscReal      error,tol,re,im;
//    PetscScalar    kkr,kki;
//    Vec            xxr,xxi;
//    PetscInt       nn=30,i,IIstart,IIend,nev,maxit,its,nconv;

//    PetscFunctionBeginUser;
//    //PetscCall(SlepcInitialize(&argc,&argv,NULL,help));

//    PetscCall(PetscOptionsGetInt(NULL,NULL,"-n",&nn,NULL));
//    PetscCall(PetscPrintf(PETSC_COMM_WORLD,"\n1-D Laplacian Eigenproblem, n=%" PetscInt_FMT "\n\n",nn));

//    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     Compute the operator matrix that defines the eigensystem, Ax=kx
//     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//    PetscCall(MatCreate(PETSC_COMM_WORLD,&A));
//    PetscCall(MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,nn,nn));
//    PetscCall(MatSetFromOptions(A));

//    PetscCall(MatGetOwnershipRange(A,&IIstart,&IIend));
//    for (i=IIstart;i<IIend;i++) {
//        if (i>0) PetscCall(MatSetValue(A,i,i-1,-1.0,INSERT_VALUES));
//        if (i<nn-1) PetscCall(MatSetValue(A,i,i+1,-1.0,INSERT_VALUES));
//        PetscCall(MatSetValue(A,i,i,2.0,INSERT_VALUES));
//    }
//    PetscCall(MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY));
//    PetscCall(MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY));

//    PetscCall(MatCreateVecs(A,NULL,&xxr));
//    PetscCall(MatCreateVecs(A,NULL,&xxi));

//    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//                Create the eigensolver and set various options
//     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
//    /*
//     Create eigensolver context
//  */
//    PetscCall(EPSCreate(PETSC_COMM_WORLD,&eps));

//    /*
//     Set operators. In this case, it is a standard eigenvalue problem
//  */
//    PetscCall(EPSSetOperators(eps,A,NULL));
//    PetscCall(EPSSetProblemType(eps,EPS_HEP));

//    /*
//     Set solver parameters at runtime
//  */
//    PetscCall(EPSSetFromOptions(eps));

//    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//                      Solve the eigensystem
//     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//    PetscCall(EPSSolve(eps));
//    /*
//     Optional: Get some information from the solver and display it
//  */
//    PetscCall(EPSGetIterationNumber(eps,&its));
//    PetscCall(PetscPrintf(PETSC_COMM_WORLD," Number of iterations of the method: %" PetscInt_FMT "\n",its));
//    PetscCall(EPSGetType(eps,&type));
//    PetscCall(PetscPrintf(PETSC_COMM_WORLD," Solution method: %s\n\n",type));
//    PetscCall(EPSGetDimensions(eps,&nev,NULL,NULL));
//    PetscCall(PetscPrintf(PETSC_COMM_WORLD," Number of requested eigenvalues: %" PetscInt_FMT "\n",nev));
//    PetscCall(EPSGetTolerances(eps,&tol,&maxit));
//    PetscCall(PetscPrintf(PETSC_COMM_WORLD," Stopping condition: tol=%.4g, maxit=%" PetscInt_FMT "\n",(double)tol,maxit));

//    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//                    Display solution and clean up
//     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
//    /*
//     Get number of converged approximate eigenpairs
//  */
//    PetscCall(EPSGetConverged(eps,&nconv));
//    PetscCall(PetscPrintf(PETSC_COMM_WORLD," Number of converged eigenpairs: %" PetscInt_FMT "\n\n",nconv));

//    if (nconv>0) {
//        /*
//       Display eigenvalues and relative errors
//    */
//        PetscCall(PetscPrintf(PETSC_COMM_WORLD,
//                              "           k          ||Ax-kx||/||kx||\n"
//                              "   ----------------- ------------------\n"));

//        for (i=0;i<nconv;i++) {
//            /*
//        Get converged eigenpairs: i-th eigenvalue is stored in kr (real part) and
//        ki (imaginary part)
//      */
//            PetscCall(EPSGetEigenpair(eps,i,&kkr,&kki,xxr,xxi));
//            /*
//         Compute the relative error associated to each eigenpair
//      */
//            PetscCall(EPSComputeError(eps,i,EPS_ERROR_RELATIVE,&error));

//#if defined(PETSC_USE_COMPLEX)
//            re = PetscRealPart(kkr);
//            im = PetscImaginaryPart(kkr);
//#else
//            re = kkr;
//            im = kki;
//#endif
//            if (im!=0.0) PetscCall(PetscPrintf(PETSC_COMM_WORLD," %9f%+9fi %12g\n",(double)re,(double)im,(double)error));
//            else PetscCall(PetscPrintf(PETSC_COMM_WORLD,"   %12f       %12g\n",(double)re,(double)error));
//        }
//        PetscCall(PetscPrintf(PETSC_COMM_WORLD,"\n"));
//    }

//    /*
//     Free work space
//  */
//    PetscCall(EPSDestroy(&eps));
//    PetscCall(MatDestroy(&A));
//    PetscCall(VecDestroy(&xxr));
//    PetscCall(VecDestroy(&xxi));
 }



   //  init(_petscKG, _petscConstrMat, _petscConstrVec);

    int ws; int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &ws);
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    PetscCall(dataForPetsc.init(_KG, constrMat, constrVec, &petscKG, &petscConstrMat, petscConstrVec));//These objects are created and their addresses kept in DataForPetsc
    MPI_Barrier(PETSC_COMM_WORLD);


    PetscCall(dataForPetsc.createCondMat(petscConstrMat, &condGMat)); // This initializes the diagonal condition matrix related to the constraints

{//    Mat* models::DataForPetsc::petscKG=&petscKG;
//    Mat* models::DataForPetsc::petscConstrMat=&petscConstrMat;
//    Vec* models::DataForPetsc::petscConstrVec=&petscConstrVec;

//    fem::matrix<double>* models::DataForPetsc::KG=_KG;
//    fem::matrix<double>* models::DataForPetsc::constrMat=_petscConstrMat;
//    std::valarray<double>* models::DataForPetsc::constrVec=constrVec;
}

PetscInt kr, kc;
MatGetSize(petscKG, &kr, &kc);
std::cout<<"petscKG dim : "<<kr<<" "<<kc<<std::endl;

//std::system("sleep 5");
PetscInt cr, cc;  //To test condMat
MatGetSize(condGMat, &cr, &cc);
std::cout<<"cr : "<<cr<<" For condGMat testing"<<std::endl;
std::cout<<"cc : "<<cc<<" For condGMat testing"<<std::endl;
//std::system("sleep 300");

    numberOfConstraints = constrMat.nrow();
    solSize = constrMat.ncol();
   // MatGetSize(pbcConstraintsPetscMat, &numberOfConstraints, &solSize);

    VecCreate(PETSC_COMM_WORLD, &U);
    VecSetSizes(U, PETSC_DECIDE, solSize);
    VecSetFromOptions(U);
    VecSetUp(U);
   // VecSet(U, 0.005/*0.08,*/ /*0.005*/); //Solution vector must be initialized
{//    VecAssemblyBegin(U);
//    VecAssemblyEnd(U);

//    PetscScalar ff=0; Vec U1; PetscScalar gg=0; Vec U2;
//    MatCreateVecs(*(this->getRigidityMat()), &U1, NULL);
//    MatMult(*(getRigidityMat()), U, U1);
//    VecDot(U, U1,&ff);
//    MatCreateVecs(*(this->getConstraintsMat()), NULL, &U2);
//    MatMult(*(getConstraintsMat()), U, U2);
//    VecDot(U2, U2, &gg);


//    PetscScalar ff=0; Vec U1; PetscScalar gg=0; Vec U2;
//    MatCreateVecs(*(taoData.getRigidityMat()), &U1, NULL);
//    MatMult(*(taoData.getRigidityMat()), U, U1);
//    VecDot(U, U1,&ff);
//    MatCreateVecs(*(taoData.getConstraintsMat()), NULL, &U2);
//    MatMult(*(taoData.getConstraintsMat()), U, U2);
//    VecDot(U2, U2, &gg);
}
PetscRandom   rctx;
PetscRandomCreate(PETSC_COMM_WORLD, &rctx);
PetscRandomSetInterval(rctx, -1.0, 1.0); //Default values are in [0, 1].
PetscRandomSetFromOptions(rctx);

// Use current time as a seed for variability
unsigned long seed = (unsigned long)time(NULL);
PetscRandomSetSeed(rctx, seed);
PetscRandomSeed(rctx); // Required to finalize the new seed

VecSetRandom(U, rctx);
PetscRandomDestroy(&rctx);
//VecSetRandom(U);

Vec initVec; //To memorize the initial guess
VecDuplicate(U, &initVec);
VecCopy(U, initVec);

PetscScalar initNorm; VecNorm(/*initVec*/U, NORM_2, &initNorm);
std::cout<<"initNorm : "<<initNorm<<std::endl;

models::Data taoData(&petscKG, &petscConstrMat, &petscConstrVec, &condGMat, &initVec); //So they can be used in TaoSet{Obj, Hess, ...} functions


/* Testing eigen value computation with A */
PetscScalar lambdaMax = solveEigen(petscKG, EPS_LARGEST_REAL);
std::cout<<"lambdaMax : "<<lambdaMax<<std::endl;

PetscScalar lambdaMin = solveEigen(petscKG, EPS_SMALLEST_REAL);
std::cout<<"lambdaMin : "<<lambdaMin<<std::endl;

PetscScalar f0;
Vec Xd; VecDuplicate(initVec, &Xd); MatMult(*(taoData.getRigidityMat()), U, Xd);VecDot(U, Xd, &f0); f0 /=2.;
PetscBarrier((PetscObject)petscKG);
/*Mat GGt;*/ Mat RedConstrMat;
 // f0=taoData.getObjectInit();
std::cout<<"f0 : "<<f0<<std::endl;
PetscScalar traceK; MatGetTrace(petscKG, &traceK); traceK /= f0;
MatMatMult(condGMat, petscConstrMat,MAT_INITIAL_MATRIX, PETSC_DETERMINE,  &RedConstrMat);
MatMatTransposeMult(RedConstrMat, RedConstrMat, MAT_INITIAL_MATRIX, PETSC_DETERMINE, &GGt);
PetscInt RedNr, RedNc;
MatGetSize(RedConstrMat, &RedNr, &RedNc);
std::cout<<"RedConstrMat size : "<<RedNr<<" "<<RedNc<<std::endl;
PetscScalar traceGGt; MatGetTrace(GGt, &traceGGt);
PetscScalar alpha =10.0;
mu=alpha*traceK/(traceGGt/*+1.e-300*/);; //alpha=100.;
std::cout<<"traceGGt : "<<traceGGt<<std::endl;
PetscScalar muMax=(1.e04)*mu;
PetscScalar fatol=1.e-8;

MPI_Barrier(MPI_COMM_WORLD);

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

//    MatCreate(PETSC_COMM_WORLD, &J);
//    MatSetSizes(J, PETSC_DECIDE, PETSC_DECIDE, numberOfConstraints, solSize);
//    MatSetFromOptions(J);
//    MatSetUp(J);
//    MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY);
//    MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);
    MatDuplicate(petscConstrMat, MAT_COPY_VALUES, &J);//To replace 542-548

    TaoCreate(PETSC_COMM_WORLD, &tao);
    TaoSetType(tao, TAOALMM);//TAOIPM //TAOALMM //TAOPDIPM //TAOCG //TAOADMM
    TaoSetSolution(tao, U);

//    MatMatTransposeMult(petscConstrMat, petscConstrMat, MAT_INITIAL_MATRIX, PETSC_DETERMINE, &GGt);
//    PetscScalar traceK; MatGetTrace(petscKG, &traceK);
//    PetscScalar traceGGt; MatGetTrace(GGt, &traceGGt);
//    PetscScalar alpha =10.0;
//    mu=alpha*traceK/traceGGt; //alpha=100.;
  //  PetscScalar muMax=(1.e04)*mu;
  //  PetscScalar fatol=1.e-8;

    std::ostringstream sstream;
    sstream<<mu;

    PetscOptionsSetValue(NULL, "-tao_almm_mu_init", sstream.str().c_str()); //"100"
    sstream.str("");
    sstream<<muMax;
    PetscOptionsSetValue(NULL, "-tao_almm_mu_max", sstream.str().c_str());
    sstream.str("");
    sstream<<fatol;
     PetscOptionsSetValue(NULL, "-tao_fatol", sstream.str().c_str());
    PetscOptionsSetValue(NULL, "-tao_almm_mu_factor", "50.0");

//    std::cout<<"traceK/traceGGt : "<<mu/alpha<<std::endl;
    //std::system("sleep 10");
    //TaoSetFromOptions(tao);

/*    TAOALMMSetInitialPenalty(1000.);    // Sets μ₀ (initial penalty parameter)
    TAOALMMSetPenaltyMaximum(1.e5.) ;   // Sets μ_max
    TAOALMMSetPenaltyMinimum(1.e-1.);    // Sets μ_min
    TAOALMMSetPenaltyIncrease(10.);   // Sets factor for increasing μ
    //TAOALMMSetPenaltyDecrease()   // Sets factor for decreasing μ
    //TAOALMMSetPenaltyUpdate()   */  // Sets how μ is updated




//    TaoSetObjectiveAndGradient(tao, NULL, objAndGradFunc, (void*)this);
    TaoSetObjectiveAndGradient(tao, gradf, objAndGradFunc, /*(void*)*/&taoData);
    TaoSetHessian(tao, H, H, hessianFunc, /*(void*)this*/&taoData);
    TaoSetEqualityConstraintsRoutine(tao, ce, constraintsFunc, /*(void*)this*/&taoData);
    TaoSetJacobianEqualityRoutine(tao, J, J, constraintsGradFunc, /*(void*)this*/&taoData);

    TaoALMMGetSubsolver(tao, &subsolver);
    TaoSetType(subsolver, TAOBQNKTR);//TAOBQNKTR(default) TAOBNCG TAOBQNLS
    TaoSetTolerances(subsolver, /*1.e-4*/0.038, 0.0, 0.0);
    //TaoSetMaximumFunctionEvaluations(subsolver, PETSC_UNLIMITED);
    KSP subksp;
    TaoGetKSP(subsolver, &subksp);
    KSPSetType(subksp, KSPPREONLY);
    KSPSetTolerances(subksp, 1e-8, PETSC_DEFAULT, PETSC_DEFAULT, 200);
    PC subpc;
    KSPGetPC(subksp, &subpc);
    PCSetType(subpc, PCLU);

    TaoSetMaximumFunctionEvaluations(subsolver, 200);
    TaoSetUp(subsolver);


        //TaoSetTolerances(tao, 1.e-6, 1.e-5, 1.e-5);
    //TaoSetTolerances(tao, PETSC_DETERMINE, PETSC_DETERMINE, PETSC_DETERMINE);
        TaoSetTolerances(tao, /*1.e-8*//*1.e-4*/0.038, 0.0, 0.0); //(tao, 1.e-5, 0.0, 0.0)
        TaoSetConstraintTolerances(tao, 1.e-5, 0.0);
        //TaoSetMaximumFunctionEvaluations(tao, PETSC_UNLIMITED);
        TaoSetMaximumFunctionEvaluations(tao, PETSC_UNLIMITED);
        //-tao_almm_constraint_tolerance 1.e-4
        //-tao_almm_gradient_tolerance 1.e-10

    Vec Ul, Uu;
    VecDuplicate(U, &Ul);
    VecDuplicate(U, &Uu);
    VecSet(Ul, -0.5/*-1*/);
    VecSet(Uu, 0.5/*1*/);
    TaoSetVariableBounds(tao, Ul, Uu);

//Taking in account ill-formed conditions

    PetscBarrier((PetscObject)tao);
            TaoSetFromOptions(tao);
    TaoSetUp(tao);

    PetscBarrier( (PetscObject)tao);
    std::cout<<"Before solving ..."<<std::endl;
    TaoSolve(tao);
    std::cout<<"After solving ..."<<std::endl;

    //reason = TAO_CONTINUE_ITERATING;
    TaoGetConvergedReason(tao, &reason);
    if(reason>0) std::cout<<"Converged with reason : "<<reason<<std::endl;
    else std::cout<<"Diverged with reason : "<<reason<<std::endl;

     DataForPetsc::VecToValarray(U, sol);

    PetscBarrier((PetscObject)tao);

//    PetscScalar ku, kup; Vec Uc;
//    MatCreateVecs(*(taoData.getRigidityMat()), NULL, &Uc);
//    MatMult(*(taoData.getRigidityMat()), U, Uc);
//    VecDot(U, Uc, &ku);// tbdl

    PetscBarrier( PetscObject(tao) );


    Mat* KK=taoData.getRigidityMat();
    Mat* CC=taoData.getConstraintsMat();
    Vec* vv=taoData.getConstraintsVec();
    PetscReal kn, cn, vn;
    MatNorm(*KK, NORM_FROBENIUS, &kn);
    MatNorm(*CC, NORM_FROBENIUS, &cn);
    VecNorm(*vv, NORM_FROBENIUS, &vn);
    std::cout<<"Norms : "<<kn<<" "<<cn<<" "<<vn<<std::endl;
    std::cout<<"Adresses : "<<KK<<" "<<CC<<" "<<vv<<std::endl;


    MatDestroy(&GGt);
    MatDestroy(&petscKG);
    MatDestroy(&petscConstrMat);
    VecDestroy(&petscConstrVec);
    VecDestroy(&initVec);

    VecDestroy(&U); VecDestroy(&ce); VecDestroy(&Ul); VecDestroy(&Uu);
    MatDestroy(&J); MatDestroy(&H);
    MatDestroy(&condGMat);
    TaoDestroy(&tao);

    PetscFunctionReturn(PETSC_SUCCESS);
}




PetscErrorCode DataForPetsc::petscSolve2(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                          const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                         std::valarray<double>& sol, const std::valarray<size_t>& numPbc, const fem::BCs& bc){

    Tao tao;     TaoConvergedReason reason; Tao subsolver;
    Vec U, ce; //Solution vector and constraint vector;
    Mat J; //d(ce)/dU -->The jacobian of the objective function
    Mat H; //To contain the obective function Hessian
    PetscInt numberOfConstraints, solSize;

    Mat petscKG;
    Mat petscConstrMat;
    Vec petscConstrVec;
    Vec gradf; //to hold gradient of the objective function

    Mat GGt; //for condition number purpose
    PetscScalar mu, condNum;
    //Mat condGMat;

    Vec Dinv;
    PetscScalar lmax;

    PetscFunctionBeginUser;
    DataForPetsc dataForPetsc;

    PetscCall(dataForPetsc.init(_KG, constrMat, constrVec, &petscKG, &petscConstrMat, petscConstrVec));//These objects are created and their addresses kept in DataForPetsc
    PetscBarrier((PetscObject)petscKG);

    lmax=solveEigen(petscKG, EPS_LARGEST_REAL);
    std::cout<<"lmax in petscSolve2 before jacobi : "<<lmax<<std::endl;
    PetscCall(dataForPetsc.jacobiCond(petscKG, petscConstrMat, petscConstrVec, &Dinv, lmax));

    std::cout<<"lmax in petscSolve2 after jacobi : "<<lmax<<std::endl;

    PetscInt kr, kc;
    MatGetOwnershipRange(petscKG, &kr, &kc); PetscScalar va;
    for(PetscInt i=kr; i<kc; i++) {MatGetValue(petscKG, i, i, &va); std::cout<<"("<<i<<", "<<i<< ")"<<va<<std::endl;}

    //PetscInt kr, kc;
    MatGetSize(petscKG, &kr, &kc);
    std::cout<<"petscKG dim : "<<kr<<" "<<kc<<std::endl;

    numberOfConstraints = constrMat.nrow();
    solSize = constrMat.ncol();
    // MatGetSize(pbcConstraintsPetscMat, &numberOfConstraints, &solSize);

    //VecCreate(PETSC_COMM_WORLD, &U);
    MatCreateVecs(petscKG, &U, NULL);
    //VecSetSizes(U, PETSC_DECIDE, solSize);
    //VecSetFromOptions(U);
    //VecSetUp(U);

    PetscRandom   rctx;
    PetscRandomCreate(PETSC_COMM_WORLD, &rctx);
    PetscRandomSetInterval(rctx, -0.05, 0.05/*10, 50.0*/); //Default values are in [0, 1].
    PetscRandomSetFromOptions(rctx);

    // Use current time as a seed for variability
    unsigned long seed = (unsigned long)time(NULL);
    PetscRandomSetSeed(rctx, seed);
    PetscRandomSeed(rctx); // Required to finalize the new seed

    VecSetRandom(U, rctx);
    PetscRandomDestroy(&rctx);
    //VecSetRandom(U);

    Vec initVec; //To memorize the initial guess
    VecDuplicate(U, &initVec);
    VecCopy(U, initVec);

    PetscScalar initNorm; VecNorm(/*initVec*/U, NORM_2, &initNorm);
    std::cout<<"initNorm : "<<initNorm<<std::endl;

    //PetscScalar a=1.e-14;
    //MatShift(petscKG, a);//To avoid null space ... (constraint and objective function incompability)
    models::Data taoData(&petscKG, &petscConstrMat, &petscConstrVec, &initVec); //So they can be used in TaoSet{Obj, Hess, ...} functions

//    MatGetOwnershipRange(petscKG, &kr, &kc); PetscScalar va;
//    for(PetscInt i=kr; i<kc; i++) {MatGetValue(petscKG, i, i, &va); std::cout<<"("<<i<<", "<<i<< ")"<<va<<std::endl;}

    /* Setting up the obective function gradient */
   // VecCreate(PETSC_COMM_WORLD, &gradf);
    MatCreateVecs(petscKG, &gradf, NULL);
    //VecSetSizes(gradf, PETSC_DECIDE, solSize);
    //VecSetFromOptions(gradf);
    //VecSetUp(gradf);

    /* Setting up the Hessian */
    //MatCreate(PETSC_COMM_WORLD, &H);
    MatDuplicate(petscKG, MAT_COPY_VALUES, &H);
    //MatSetSizes(H, PETSC_DECIDE, PETSC_DECIDE, solSize, solSize);
    //MatSetFromOptions(H);
    //MatAssemblyBegin(H, MAT_FINAL_ASSEMBLY);
    //MatAssemblyEnd(H, MAT_FINAL_ASSEMBLY);

    /* Setting up the constraints vector */
    //VecCreate(PETSC_COMM_WORLD, &ce);
    MatCreateVecs(petscConstrMat, NULL, &ce);
    //VecSetSizes(ce, PETSC_DECIDE, numberOfConstraints);
    //VecSetFromOptions(ce);
    //VecSetUp(ce);
    MatDuplicate(petscConstrMat, MAT_COPY_VALUES, &J);//To replace 542-548

    TaoCreate(PETSC_COMM_WORLD, &tao);
    TaoSetType(tao, TAOALMM);//TAOIPM //TAOALMM //TAOPDIPM //TAOCG //TAOADMM
    TaoSetSolution(tao, U);


    //PetscScalar
    //mu=10.;
    //PetscScalar muMax=(1.e3)*mu;
    //PetscScalar fatol=1.e-8;

    PetscScalar f0; //Mat GGt;
    Vec Xd; VecDuplicate(initVec, &Xd); MatMult(*(taoData.getRigidityMat()), U, Xd);VecDot(U, Xd, &f0); f0 /=2.;
    PetscBarrier((PetscObject)petscKG);
    std::cout<<"f0 : "<<f0<<std::endl;
    //PetscScalar traceK; MatGetTrace(petscKG, &traceK); traceK /= f0;
    //MatMatMult(condGMat, petscConstrMat,MAT_INITIAL_MATRIX, PETSC_DETERMINE,  &RedConstrMat);
    MatMatTransposeMult(petscConstrMat, petscConstrMat, MAT_INITIAL_MATRIX, PETSC_DETERMINE, &GGt);

    PetscScalar traceK; MatGetTrace(petscKG, &traceK); traceK /= f0;
    PetscInt RedNr, RedNc;
    MatGetSize(petscConstrMat, &RedNr, &RedNc);
    std::cout<<"petscConstrMat size : "<<RedNr<<" "<<RedNc<<std::endl;
    PetscScalar traceGGt; MatGetTrace(GGt, &traceGGt);
    PetscScalar alpha =10.0;
    mu=alpha*traceK/(traceGGt/*+1.e-300*/);; //alpha=100.;
    std::cout<<"traceGGt : "<<traceGGt<<std::endl;
    PetscScalar muMax=(1.e04)*mu;
    PetscScalar fatol=1.e-8;


    std::ostringstream sstream;
    sstream<<mu;

    PetscOptionsSetValue(NULL, "-tao_almm_mu_init", sstream.str().c_str()); //"100"
    sstream.str("");
    sstream<<muMax;
    PetscOptionsSetValue(NULL, "-tao_almm_mu_max", sstream.str().c_str());
    sstream.str("");
    sstream<<fatol;
    //PetscOptionsSetValue(NULL, "-tao_fatol", sstream.str().c_str());
    PetscOptionsSetValue(NULL, "-tao_almm_mu_factor", "100.0");//10
    PetscOptionsSetValue(NULL, "-tao_almm_mu_update_factor", "1.");

    TaoSetObjectiveAndGradient(tao, gradf, objAndGradFunc, &taoData);
    TaoSetHessian(tao, H, H, hessianFunc, &taoData);
    TaoSetEqualityConstraintsRoutine(tao, ce, constraintsFunc, &taoData);
    TaoSetJacobianEqualityRoutine(tao, J, J, constraintsGradFunc, &taoData);

    TaoALMMGetSubsolver(tao, &subsolver);

//    TaoSetType(subsolver, TAOBQNKTR);//TAOBQNKTR(default) TAOBNCG TAOBQNLS
//    //TaoSetConstraintTolerances(subsolver, 1.e-06, 0.0);
//    //TaoSetMaximumFunctionEvaluations(subsolver, PETSC_UNLIMITED);

//    TaoSetMaximumFunctionEvaluations(subsolver, 1000);
//    TaoSetTolerances(subsolver, 1.e-25/*0.038*/, 0.0, 0.0);
//    //TaoSetUp(subsolver);

    TaoSetTolerances(subsolver, 1.e-25/*0.038*/, 0.0, 0.0);
    TaoSetMaximumFunctionEvaluations(subsolver, 1000);
    KSP subksp;
    TaoGetKSP(subsolver, &subksp);
    KSPSetType(subksp, KSPPREONLY);
    KSPSetTolerances(subksp, 1.e-25, PETSC_DEFAULT, PETSC_DEFAULT, 500);
    PC subpc;
    KSPGetPC(subksp, &subpc);
    PCSetType(subpc, PCLU);


    TaoSetTolerances(tao, /*1.e-8*/1.e-05, 0.0, 0.0); //(tao, 1.e-5, 0.0, 0.0)
    TaoSetConstraintTolerances(tao, /*1.e-5*/1.e-06, 0.0);
    //TaoSetMaximumFunctionEvaluations(tao, PETSC_UNLIMITED);
    TaoSetMaximumFunctionEvaluations(tao, PETSC_UNLIMITED);
    TaoSetMaximumIterations(tao, PETSC_UNLIMITED/*10000*/); //raisonablement 500
    //-tao_almm_constraint_tolerance 1.e-4
    //-tao_almm_gradient_tolerance 1.e-10

    Vec Ul, Uu;
    VecDuplicate(U, &Ul);
    VecDuplicate(U, &Uu);
    VecSet(Ul, -0.06/*-1*/);
    VecSet(Uu, 0.06/*1*/);
    PetscScalar eps=(PetscScalar)numeric_limits<double>::epsilon(); //1.e-50;

    std::vector<size_t> dirdofs= bc.getDirichletDofs();
    for(size_t i=0; i<dirdofs.size(); i++){
        VecSetValue(Ul, numPbc[dirdofs[i]], bc.pbcDirichletValue(dirdofs[i], ndof)/*-1.e14*eps*/, INSERT_VALUES);
        VecSetValue(Uu, numPbc[dirdofs[i]], bc.pbcDirichletValue(dirdofs[i], ndof)/*+1.e14*eps*/, INSERT_VALUES);
        std::cout<<"dirdofs values "<<i<<" : "<<bc.pbcDirichletValue(dirdofs[i], ndof)<<std::endl;
    }
    VecAssemblyBegin(Ul);
    VecAssemblyEnd(Ul);
    VecAssemblyBegin(Uu);
    VecAssemblyEnd(Uu);
    VecPointwiseDivide(Ul, Ul, Dinv);
    VecPointwiseDivide(Uu, Uu, Dinv);

    TaoSetVariableBounds(tao, Ul, Uu);

    TaoSetFromOptions(tao);
    TaoSetUp(tao);

    std::cout<<"Before solving ..."<<std::endl;
    TaoSolve(tao);
    std::cout<<"After solving ..."<<std::endl;

    //reason = TAO_CONTINUE_ITERATING;
    TaoGetConvergedReason(tao, &reason);
    if(reason>0) std::cout<<"Converged with reason : "<<reason<<std::endl;
    else std::cout<<"Diverged with reason : "<<reason<<std::endl;

    VecPointwiseMult(U, Dinv, U);
    //DataForPetsc::VecToValarray(U, sol);
    dataForPetsc.VecToValarray(U, sol);

    PetscBarrier((PetscObject)tao);

    MatDestroy(&GGt);
    MatDestroy(&petscKG);
    MatDestroy(&petscConstrMat);
    VecDestroy(&petscConstrVec);
    VecDestroy(&initVec);

    VecDestroy(&U); VecDestroy(&ce); VecDestroy(&Ul); VecDestroy(&Uu);
    MatDestroy(&J); MatDestroy(&H);
    //MatDestroy(&condGMat);
    TaoDestroy(&tao);

    PetscFunctionReturn(PETSC_SUCCESS);

}


PetscErrorCode DataForPetsc::petscSolveLinear(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                         const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                         std::valarray<double>& sol, const std::valarray<size_t>& numPbc, const fem::BCs& bc){

    Vec U; //Solution vector;

    Mat petscKG;
    Mat petscConstrMat;
    Vec petscConstrVec;

    Vec Dinv;
    PetscScalar lmax;

    PetscFunctionBeginUser;
    DataForPetsc dataForPetsc;

    PetscCall(dataForPetsc.init(_KG, constrMat, constrVec, &petscKG, &petscConstrMat, petscConstrVec));//These objects are created and their addresses kept in DataForPetsc
    PetscBarrier((PetscObject)petscKG);

    lmax=solveEigen(petscKG, EPS_LARGEST_REAL);
    std::cout<<"lmax in petscSolve2 before jacobi : "<<lmax<<std::endl;

    PetscReal shift = lmax * 1e-5; //KG is singular
    MatShift(petscKG, shift);

    PetscCall(dataForPetsc.jacobiCond(petscKG, petscConstrMat, petscConstrVec, &Dinv, lmax));

    std::cout<<"lmax in petscSolve2 after jacobi : "<<lmax<<std::endl;

    MatCreateVecs(petscKG, &U, NULL);
//*************************************** Linear Solving using MatNest ***************************************
    PetscErrorCode ierr;
    KSP ksp; PC pc;
    Mat            A;             // Matrice globale MATNEST
    Vec rhs;
    Vec ULambda; //ULambda groups U and Lambda
    PetscInt       nr = 2, nc = 2;     // Nombre de blocs de lignes et de colonnes

    PetscInt nrG, ncG; MatGetSize(petscConstrMat, &nrG, &ncG);
    PetscInt nrLG, ncLG;
    Mat LL; MatCreate(PETSC_COMM_WORLD, &LL);
    MatGetLocalSize(petscConstrMat, &nrLG, NULL);
    MatSetSizes(LL, nrLG, nrLG, PETSC_DETERMINE, PETSC_DETERMINE);
    MatSetType(LL, MATMPIAIJ);

    MatSetFromOptions(LL);
    MatSetUp(LL);

    PetscInt rb, re;
    MatGetOwnershipRange(LL, &rb, &re);
    for(PetscInt i=rb; i<re; i++) MatSetValue(LL, i, i, 0., INSERT_VALUES);
    MatAssemblyBegin(LL, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(LL, MAT_FINAL_ASSEMBLY);
    //MatShift(LL, 0.0);

    Mat petscConstrMatT;
    MatTranspose(petscConstrMat, MAT_INITIAL_MATRIX, &petscConstrMatT);

//******************  Creating the right side *************************************************
    Vec ff;
    MatCreateVecs(petscKG, NULL, &ff); VecZeroEntries(ff);

//******************  Creating the solution vector *************************************************
    Vec Lambda;
    MatCreateVecs(petscConstrMat, NULL, &Lambda/*, NULL*/);

    Mat submats[4]={petscKG, petscConstrMatT, petscConstrMat, /*NULL*/LL};
    Vec subrhs[2]={ff, petscConstrVec};
    Vec subsol[2]=  {U,Lambda};

//******************  Creating the NEST quantities *************************************************
    ierr = MatCreateNest(PETSC_COMM_WORLD, nr, NULL, nc, NULL, submats, &A); CHKERRQ(ierr);
    ierr = VecCreateNest(PETSC_COMM_WORLD, 2, NULL, subrhs, &rhs); CHKERRQ(ierr);
    ierr = VecCreateNest(PETSC_COMM_WORLD, 2, NULL, subsol, &ULambda); CHKERRQ(ierr);

//******************  Verification of quantities created *************************************************
    PetscViewer viewer;
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscKG5.dat", FILE_MODE_WRITE, &viewer);
    MatView(petscKG, viewer);
    PetscViewerDestroy(&viewer);
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscConstrMat5.dat", FILE_MODE_WRITE, &viewer);
    MatView(petscConstrMat, viewer);
    PetscViewerDestroy(&viewer);
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscConstrVec5.dat", FILE_MODE_WRITE, &viewer);
    VecView(petscConstrVec, viewer);

//******************  NEST solver parametrization *************************************************

    ierr = KSPCreate(PETSC_COMM_WORLD, &ksp); CHKERRQ(ierr);
    ierr = KSPSetOperators(ksp, A, A); CHKERRQ(ierr);
    // Récupération du préconditionneur pour appliquer le découpage de champs
    ierr = KSPGetPC(ksp, &pc); CHKERRQ(ierr);
    ierr = PCSetType(pc, /*PCFIELDSPLIT*/PCLU); CHKERRQ(ierr);
    //ierr = PCSetType(pc, PCLU); CHKERRQ(ierr);
    ierr=KSPSetType(ksp, KSPPREONLY); CHKERRQ(ierr);
    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
//    KSPSetTolerances(ksp, 1e-5, PETSC_DEFAULT, PETSC_DEFAULT, 1000);
    KSPSetTolerances(ksp, 0, 1.e-12, PETSC_DEFAULT, 1000);

    IS is_row[2];
    MatNestGetISs(A, is_row, NULL);
    PCFieldSplitSetIS(pc, "champ_0", is_row[0]); //
    PCFieldSplitSetIS(pc, "champ_1", is_row[1]); //

    PetscBarrier((PetscObject)petscKG);
    KSPConvergedReason reason;

    ierr = KSPSolve(ksp, rhs, ULambda); CHKERRQ(ierr);

    // 2. Retrieve the reason after solving
    ierr = KSPGetConvergedReason(ksp, &reason); CHKERRQ(ierr);

    // 3. Evaluate the result
    if (reason > 0) {
        PetscPrintf(PETSC_COMM_WORLD, "Linear solve CONVERGED. Reason code: %d\n", reason);
    } else if (reason < 0) {
        PetscPrintf(PETSC_COMM_WORLD, "Linear solve DIVERGED. Reason code: %d\n", reason);
        // Optional: Handle error or switch to a safer solver here
    }

//    //****************** Recovering the real U ***************************
    VecPointwiseMult(U, Dinv, U);
    //DataForPetsc::VecToValarray(U, sol);
    dataForPetsc.VecToValarray(U, sol);

    {
    VecDestroy(&Dinv);
    //VecDestroy(&ff);
    VecDestroy(&Lambda);
    VecDestroy(&rhs);
    VecDestroy(&ULambda);
    MatDestroy(&LL);
    MatDestroy(&A);
   MatDestroy(&petscKG);
   MatDestroy(&petscConstrMat);
   //VecDestroy(&petscConstrVec);
   VecDestroy(&U);
   MatDestroy(&petscConstrMatT);
   KSPDestroy(&ksp);
   //PCDestroy(&pc);
    }

    PetscFunctionReturn(PETSC_SUCCESS);
}

PetscErrorCode DataForPetsc::petscSolveLinearRed(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                              const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                              std::valarray<double>& sol, const std::valarray<size_t>& numPbc, const fem::BCs& bc){



    fem::matrix<double> Kuu, Kud, Gu, Gd;
    std::valarray<double> fu, fl, ud;
    std::vector<size_t> dirdofs;
    fem::matrix<double> constrMatBar; //To erase the hardcoded dirichlet dofs

    dirdofs=bc.getDirichletDofs();

    size_t nd=dirdofs.size();
    size_t nu=_KG.nrow()-nd ;

    Kuu=_KG; Kuu.nFirstLines(nu); Kuu.nFirstColumns(nu);
//    Kud=_KG; Kud.nLastColumns(nd); Kud.nFirstLines(nu);
    Kud=_KG; Kud.nFirstLines(nu); Kud.nLastColumns(nd);


    constrMatBar=constrMat; constrMatBar.nFirstLines(constrMat.nrow()-nd); //To ignore the hardcoded dirichlet boundary conditions
    //constrMat=constrMat.nFirstColumns(_KG.nrow()); //To ignore the hardcoded dirichlet boundary conditions
    Gu=constrMatBar; Gu.nFirstColumns(nu);
    Gd=constrMatBar; Gd.nLastColumns(nd);

    ud.resize(nd, 0.);
    for(size_t i =0; i<nd; i++) {ud[ numPbc[dirdofs[i]] -nu]=bc.pbcDirichletValue(dirdofs[i], ndof);
   std::cout<<"i : "<<bc.pbcDirichletValue(dirdofs[i], ndof)<<std::endl;}

    MPI_Barrier(MPI_COMM_WORLD);

    fu=-1*(Kud*ud);
    fl=-1*(Gd*ud);
    std::cout<<"fl en i : "<<2678<<" : "<<fl[2678]<<std::endl;

    MPI_Barrier(MPI_COMM_WORLD);

    Vec U; //Solution vector;

    Mat petscKG;
    Mat petscConstrMat;
    Vec petscConstrVec;

    Vec petscForceVec;

    Vec Dinv;
    PetscScalar lmax;

    PetscFunctionBeginUser;
    DataForPetsc dataForPetsc;

    PetscCall(dataForPetsc./*this->*/init(Kuu, Gu, /*constrVec*/fl, &petscKG, &petscConstrMat, petscConstrVec));//These objects are created and their addresses kept in DataForPetsc
    PetscBarrier((PetscObject)petscKG);
    dataForPetsc./*this->*/valarrayToVecG(fu, petscKG, petscForceVec);
    PetscBarrier((PetscObject)petscKG);

    lmax=solveEigen(petscKG, EPS_LARGEST_REAL);
    std::cout<<"lmax in petscSolve2 before jacobi : "<<lmax<<std::endl;

    //PetscReal shift = lmax * 1e-5; //KG is singular
    //MatShift(petscKG, shift);

    PetscBarrier((PetscObject)petscKG);
    PetscCall(dataForPetsc./*this->*/jacobiCond(petscKG, petscConstrMat, petscConstrVec, &Dinv, lmax));
    VecPointwiseMult(petscForceVec, Dinv, petscForceVec);
    PetscBarrier((PetscObject)petscKG);

    std::cout<<"lmax in petscSolve2 after jacobi : "<<lmax<<std::endl;

    MatCreateVecs(petscKG, &U, NULL);
    //*************************************** Linear Solving using MatNest ***************************************
    PetscErrorCode ierr;
    KSP ksp; PC pc;
    Mat            A;             // Matrice globale MATNEST
    Vec rhs;
    Vec ULambda; //ULambda groups U and Lambda
    PetscInt       nr = 2, nc = 2;     // Nombre de blocs de lignes et de colonnes

    PetscInt nrG, ncG; MatGetSize(petscConstrMat, &nrG, &ncG);
    PetscInt nrLG, ncLG;
    Mat LL; MatCreate(PETSC_COMM_WORLD, &LL);
    MatGetLocalSize(petscConstrMat, &nrLG, NULL);
    MatSetSizes(LL, nrLG, nrLG, PETSC_DETERMINE, PETSC_DETERMINE);
    MatSetType(LL, MATMPIAIJ);

    MatSetFromOptions(LL);
    MatSetUp(LL);

    PetscInt rb, re;
    MatGetOwnershipRange(LL, &rb, &re);
    for(PetscInt i=rb; i<re; i++) MatSetValue(LL, i, i, 0., INSERT_VALUES);
    MatAssemblyBegin(LL, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(LL, MAT_FINAL_ASSEMBLY);
    //MatShift(LL, 0.0);

    Mat petscConstrMatT;
    MatTranspose(petscConstrMat, MAT_INITIAL_MATRIX, &petscConstrMatT);

    //******************  Creating the right side *************************************************
    Vec ff;
    MatCreateVecs(petscKG, NULL, &ff); VecZeroEntries(ff);
    VecCopy(petscForceVec, ff);

    //******************  Creating the solution vector *************************************************
    Vec Lambda;
    MatCreateVecs(petscConstrMat, NULL, &Lambda/*, NULL*/);


    Mat submats[4]={petscKG, petscConstrMatT, petscConstrMat, /*NULL*/LL};
    Vec subrhs[2]={ff, petscConstrVec};
    Vec subsol[2]=  {U,Lambda};

    PetscInt           local_size;
    const PetscScalar *array;
    PetscBool          has_nan;

    // =========================================================================
    // 1. VÉRIFICATION DES VECTEURS (ff et petscConstrVec)
    // =========================================================================
    // On vérifie le second membre des forces (ff)
    has_nan = PETSC_FALSE;
    VecGetLocalSize(ff, &local_size);
    VecGetArrayRead(ff, &array);
    for (PetscInt i = 0; i < local_size; i++) {
   if (PetscIsNanReal(PetscRealPart(array[i]))) { has_nan = PETSC_TRUE; break; }
    }
    VecRestoreArrayRead(ff, &array);
    if (has_nan) std::cout << "⚠️ VECTEUR : 'ff' contient des NaN !" << std::endl;

               // On vérifie le second membre des contraintes (petscConstrVec)
    //PetscInt rlow, rhigh;
   // VecGetOwnershipRange(petscConstrVec, &rlow, &rhigh);
               has_nan = PETSC_FALSE;
    VecGetLocalSize(petscConstrVec, &local_size);
    VecGetArrayRead(petscConstrVec, &array);
    for (PetscInt i = 0; i < local_size; i++) {
   if (PetscIsNanReal(PetscRealPart(array[i]))) {
            //std::cout<<" NaN at "<<rlow+i <<" of petscConstrVec"<<std::endl;
            has_nan = PETSC_TRUE; break; }
    }
    VecRestoreArrayRead(petscConstrVec, &array);
    if (has_nan) std::cout << "⚠️ VECTEUR : 'petscConstrVec' contient des NaN !" << std::endl;


    // =========================================================================
    // 2. VÉRIFICATION DES MATRICES (Par multiplication de test)
    // =========================================================================
    auto check_matrix_nan = [](Mat M, const std::string& name) {
        PetscBool l_nan = PETSC_FALSE;
        PetscInt  l_size;
        const PetscScalar *l_array;
        Vec x, y;

        MatCreateVecs(M, &x, &y);
        VecSet(x, 1.0);
        MatMult(M, x, y); // Multiplie par 1.0 pour propager les NaN internes

        VecGetLocalSize(y, &l_size);
        VecGetArrayRead(y, &l_array);
        for (PetscInt i = 0; i < l_size; i++) {
            if (PetscIsNanReal(PetscRealPart(l_array[i]))) { l_nan = PETSC_TRUE; break; }
        }
        VecRestoreArrayRead(y, &l_array);
        VecDestroy(&x);
        VecDestroy(&y);

        if (l_nan) std::cout << "⚠️ MATRICE : '" << name << "' génère ou contient des NaN !" << std::endl;
    };

    // On scanne les 3 matrices physiques de votre bloc
    check_matrix_nan(petscKG, "petscKG");
    check_matrix_nan(petscConstrMat, "petscConstrMat");
    check_matrix_nan(petscConstrMatT, "petscConstrMatT");

    //******************  Creating the NEST quantities *************************************************
    ierr = MatCreateNest(PETSC_COMM_WORLD, nr, NULL, nc, NULL, submats, &A); CHKERRQ(ierr);
    ierr = VecCreateNest(PETSC_COMM_WORLD, 2, NULL, subrhs, &rhs); CHKERRQ(ierr);
    ierr = VecCreateNest(PETSC_COMM_WORLD, 2, NULL, subsol, &ULambda); CHKERRQ(ierr);

    //******************  Verification of quantities created *************************************************
    PetscViewer viewer;
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscKG5.dat", FILE_MODE_WRITE, &viewer);
    MatView(petscKG, viewer);
    PetscViewerDestroy(&viewer);
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscConstrMat5.dat", FILE_MODE_WRITE, &viewer);
    MatView(petscConstrMat, viewer);
    PetscViewerDestroy(&viewer);
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscConstrVec5.dat", FILE_MODE_WRITE, &viewer);
    VecView(petscConstrVec, viewer);
    PetscViewerDestroy(&viewer);

    //******************  NEST solver parametrization *************************************************
    PetscInt nf; KSP* subksp ;
    ierr = KSPCreate(PETSC_COMM_WORLD, &ksp); CHKERRQ(ierr);
    ierr = KSPSetOperators(ksp, A, A); CHKERRQ(ierr);

    // 1. Définition du PC global en FieldSplit
    ierr = KSPGetPC(ksp, &pc); CHKERRQ(ierr);
    ierr = PCSetType(pc, PCFIELDSPLIT); CHKERRQ(ierr);

    // Définition manuelle des deux champs
    IS is_row[2];
    MatNestGetISs(A, is_row, NULL);
    PCFieldSplitSetIS(pc, "champ_0", is_row[0]);
    PCFieldSplitSetIS(pc, "champ_1", is_row[1]);

    // 2. Activation du mode Schur
//    ierr = PCFieldSplitSetType(pc, PC_COMPOSITE_SCHUR); CHKERRQ(ierr);
//    ierr = PCFieldSplitSetSchurFactType(pc, PC_FIELDSPLIT_SCHUR_FACT_FULL); CHKERRQ(ierr);
//    ierr = PCFieldSplitSetSchurPre(pc, PC_FIELDSPLIT_SCHUR_PRE_A11, NULL); CHKERRQ(ierr);

    // =========================================================================
    // INJECTION PROGRAMMATIQUE DES OPTIONS (Évite le besoin de PCFieldSplitGetSubKSP)
    // =========================================================================
    // Récupération de la base d'options locale attachée au PC ou globale
    PetscOptions options;
    PetscObjectGetOptions((PetscObject)ksp, &options);

    // CONFIGURATION DU CHAMP 0 (Rigidité -> MUMPS)
    // Équivalent à la ligne de commande : -fieldsplit_champ_0_ksp_type preonly etc.
    PetscOptionsSetValue(options, "-fieldsplit_champ_0_ksp_type", "preonly");
    PetscOptionsSetValue(options, "-fieldsplit_champ_0_pc_type", "lu");
    PetscOptionsSetValue(options, "-fieldsplit_champ_0_pc_factor_mat_solver_type", "mumps");

    // CONFIGURATION DU CHAMP 1 (Schur / Lagrange -> GMRES + PCNONE)
    // C'est ce "-fieldsplit_champ_1_pc_type none" qui supprime définitivement le NANORINF !
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_ksp_type", "gmres"); //gmres
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_pc_type", "gamg");//none
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_ksp_rtol", "1e-14");
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_ksp_max_it", "1000");

    // 3. Application des options et configuration globale
    ierr = KSPSetType(ksp, KSPGMRES); CHKERRQ(ierr);
    KSPSetTolerances(ksp, 0, 1.e-10, PETSC_DEFAULT, 1000);

    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr); // Applique les options injectées ci-dessus

    // 4. Initialisation sécurisée
    // PETSc va lire les options forcées (mumps et none), le PCSetUp ne crashera pas !
    ierr = PCSetUp(pc); CHKERRQ(ierr);

    // Vos lignes de vérification ou d'affichage peuvent suivre ici si nécessaire
    // ...


    PetscInt rrk, cck, rrG, ccG, rrGt, ccGt, rrLL, ccLL, lff, lSS;
    MatGetSize(petscKG, &rrk, &cck);
    MatGetSize(petscConstrMat, &rrG, &ccG);
    MatGetSize(petscConstrMatT, &rrGt, &ccGt);
    MatGetSize(LL, &rrLL, &ccLL);
    VecGetSize(ff, &lff);
    VecGetSize(petscConstrVec, &lSS);
    std::cout<<"petscKG = "<<rrk<<" x "<<cck<<std::endl;
    std::cout<<"petscConstrMat = "<<rrG<<" x "<<ccG<<std::endl;
    std::cout<<"petscConstrMatT = "<<rrGt<<" x "<<ccGt<<std::endl;
    std::cout<<"LL = "<<rrLL<<" x "<<ccLL<<std::endl;
    std::cout<<"ff = "<<lff<<std::endl;
    std::cout<<"S = "<<lSS<<std::endl;
    std::cout<<"KG size : "<<_KG.nrow()<<std::endl;

//    std::cout<<"Solving ..."<<std::endl;
//    PetscBarrier((PetscObject)petscKG);
//    MatNullSpace nullsp;
//    MatNullSpaceCreate(PETSC_COMM_WORLD,PETSC_TRUE, 0, NULL, &nullsp);
//    MatSetNullSpace(A , nullsp);
//    MatNullSpaceDestroy(&nullsp);
    PetscBarrier((PetscObject)petscKG);
    KSPConvergedReason reason;

//    PetscBool assembled;
//    MatAssembled(petscKG, &assembled);
//    PetscPrintf(PETSC_COMM_WORLD, "petscKG assembled: %d\n", assembled);
//    MatAssembled(petscConstrMat, &assembled);
//    PetscPrintf(PETSC_COMM_WORLD, "petscConstrMat assembled: %d\n", assembled);
//    MatAssembled(petscConstrMatT, &assembled);
//    PetscPrintf(PETSC_COMM_WORLD, "petscConstrMatT assembled: %d\n", assembled);
//    MatAssembled(LL, &assembled);
//    PetscPrintf(PETSC_COMM_WORLD, "LL assembled: %d\n", assembled);

    ierr = KSPSolve(ksp, rhs, ULambda); CHKERRQ(ierr);


        PetscBarrier((PetscObject)petscKG);

    std::cout<<"End solving."<<std::endl;
    // 2. Retrieve the reason after solving
    ierr = KSPGetConvergedReason(ksp, &reason); CHKERRQ(ierr);

    // 3. Evaluate the result
    if (reason > 0) {
   PetscPrintf(PETSC_COMM_WORLD, "Linear solve CONVERGED. Reason code: %d\n", reason);
    } else if (reason < 0) {
   PetscPrintf(PETSC_COMM_WORLD, "Linear solve DIVERGED. Reason code: %d\n", reason);
   // Optional: Handle error or switch to a safer solver here
    }

    //    //****************** Recovering the real U ***************************

PetscBarrier((PetscObject)petscKG);
VecPointwiseMult(U, Dinv, U);
    //DataForPetsc::VecToValarray(U, sol);
PetscBarrier((PetscObject)petscKG);
    std::valarray<double> solBar;
    dataForPetsc.VecToValarray(U, solBar);
    sol.resize(_KG.nrow(), 0.);

    for(size_t i=0; i<nd; i++) sol[numPbc[dirdofs[i]]]=bc.pbcDirichletValue(i, ndof);
  assert(solBar.size() == nu);  sol[std::slice(0, nu, 1)]=solBar;

PetscBarrier((PetscObject)petscKG);
    //dataForPetsc.VecToValarray(U, sol);

    {
   VecDestroy(&Dinv);
   VecDestroy(&ff);
   VecDestroy(&Lambda);
   //VecDestroy(&rhs);
   //VecDestroy(&ULambda);
   MatDestroy(&LL);
   //MatDestroy(&A);
   MatDestroy(&petscKG);
   MatDestroy(&petscConstrMat);
   VecDestroy(&petscConstrVec);
   VecDestroy(&U);
   MatDestroy(&petscConstrMatT);
   KSPDestroy(&ksp);
   //PCDestroy(&pc);
    }

    PetscFunctionReturn(PETSC_SUCCESS);
}

PetscErrorCode DataForPetsc::petscSolveLinear2(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                              const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                              std::valarray<double>& sol, const std::valarray<size_t>& numPbc, const fem::BCs& bc){

    Tao tao;     TaoConvergedReason reason; Tao subsolver;
    Vec U, ce; //Solution vector and constraint vector;
    Mat J; //d(ce)/dU -->The jacobian of the objective function
    Mat H; //To contain the obective function Hessian
    PetscInt numberOfConstraints, solSize;

    Mat petscKG;
    Mat petscConstrMat;
    Vec petscConstrVec;
    Vec gradf; //to hold gradient of the objective function

    Mat GGt; //for condition number purpose
    PetscScalar mu, condNum;
    //Mat condGMat;

    Vec Dinv;
    PetscScalar lmax;

    PetscFunctionBeginUser;
    DataForPetsc dataForPetsc;

    PetscCall(dataForPetsc.init(_KG, constrMat, constrVec, &petscKG, &petscConstrMat, petscConstrVec));//These objects are created and their addresses kept in DataForPetsc
    PetscBarrier((PetscObject)petscKG);

    lmax=solveEigen(petscKG, EPS_LARGEST_REAL);
    std::cout<<"lmax in petscSolve2 before jacobi : "<<lmax<<std::endl;
    PetscCall(dataForPetsc.jacobiCond(petscKG, petscConstrMat, petscConstrVec, &Dinv, lmax));

    std::cout<<"lmax in petscSolve2 after jacobi : "<<lmax<<std::endl;

    //    PetscInt kr, kc;
    //    MatGetOwnershipRange(petscKG, &kr, &kc); PetscScalar va;
    //    for(PetscInt i=kr; i<kc; i++) {MatGetValue(petscKG, i, i, &va); std::cout<<"("<<i<<", "<<i<< ")"<<va<<std::endl;}

    //PetscInt kr, kc;
    //    MatGetSize(petscKG, &kr, &kc);
    //    std::cout<<"petscKG dim : "<<kr<<" "<<kc<<std::endl;

    numberOfConstraints = constrMat.nrow();
    solSize = constrMat.ncol();
    // MatGetSize(pbcConstraintsPetscMat, &numberOfConstraints, &solSize);

    //VecCreate(PETSC_COMM_WORLD, &U);
    MatCreateVecs(petscKG, &U, NULL);
    //VecSetSizes(U, PETSC_DECIDE, solSize);
    //VecSetFromOptions(U);
    //VecSetUp(U);

    PetscRandom   rctx;
    PetscRandomCreate(PETSC_COMM_WORLD, &rctx);
    PetscRandomSetInterval(rctx, -0.05, 0.05/*10, 50.0*/); //Default values are in [0, 1].
    PetscRandomSetFromOptions(rctx);

    // Use current time as a seed for variability
    unsigned long seed = (unsigned long)time(NULL);
    PetscRandomSetSeed(rctx, seed);
    PetscRandomSeed(rctx); // Required to finalize the new seed

    VecSetRandom(U, rctx);
    PetscRandomDestroy(&rctx);
    //VecSetRandom(U);

    Vec initVec; //To memorize the initial guess
    VecDuplicate(U, &initVec);
    VecCopy(U, initVec);

    //    PetscScalar initNorm; VecNorm(/*initVec*/U, NORM_2, &initNorm);
    //    std::cout<<"initNorm : "<<initNorm<<std::endl;
    {
        //    models::Data taoData(&petscKG, &petscConstrMat, &petscConstrVec, &initVec); //So they can be used in TaoSet{Obj, Hess, ...} functions

        //    MatCreateVecs(petscKG, &gradf, NULL);
        //    MatDuplicate(petscKG, MAT_COPY_VALUES, &H);
        //    MatCreateVecs(petscConstrMat, NULL, &ce);
        //    MatDuplicate(petscConstrMat, MAT_COPY_VALUES, &J);//To replace 542-548

        //    TaoCreate(PETSC_COMM_WORLD, &tao);
        //    TaoSetType(tao, TAOALMM);//TAOIPM //TAOALMM //TAOPDIPM //TAOCG //TAOADMM
        //    TaoSetSolution(tao, U);

        //    PetscScalar f0; //Mat GGt;
        //    Vec Xd; VecDuplicate(initVec, &Xd); MatMult(*(taoData.getRigidityMat()), U, Xd);VecDot(U, Xd, &f0); f0 /=2.;
        //    PetscBarrier((PetscObject)petscKG);
        //    std::cout<<"f0 : "<<f0<<std::endl;
        //    MatMatTransposeMult(petscConstrMat, petscConstrMat, MAT_INITIAL_MATRIX, PETSC_DETERMINE, &GGt);

        //    PetscScalar traceK; MatGetTrace(petscKG, &traceK); traceK /= f0;
        //    PetscInt RedNr, RedNc;
        //    MatGetSize(petscConstrMat, &RedNr, &RedNc);
        //    std::cout<<"petscConstrMat size : "<<RedNr<<" "<<RedNc<<std::endl;
        //    PetscScalar traceGGt; MatGetTrace(GGt, &traceGGt);
        //    PetscScalar alpha =10.0;
        //    mu=alpha*traceK/(traceGGt/*+1.e-300*/);; //alpha=100.;
        //    std::cout<<"traceGGt : "<<traceGGt<<std::endl;
        //    PetscScalar muMax=(1.e04)*mu;
        //    PetscScalar fatol=1.e-8;


        //    std::ostringstream sstream;
        //    sstream<<mu;

        //    PetscOptionsSetValue(NULL, "-tao_almm_mu_init", sstream.str().c_str()); //"100"
        //    sstream.str("");
        //    sstream<<muMax;
        //    PetscOptionsSetValue(NULL, "-tao_almm_mu_max", sstream.str().c_str());
        //    sstream.str("");
        //    sstream<<fatol;
        //    //PetscOptionsSetValue(NULL, "-tao_fatol", sstream.str().c_str());
        //    PetscOptionsSetValue(NULL, "-tao_almm_mu_factor", "100.0");//10
        //    PetscOptionsSetValue(NULL, "-tao_almm_mu_update_factor", "1.");

        //    TaoSetObjectiveAndGradient(tao, gradf, objAndGradFunc, &taoData);
        //    TaoSetHessian(tao, H, H, hessianFunc, &taoData);
        //    TaoSetEqualityConstraintsRoutine(tao, ce, constraintsFunc, &taoData);
        //    TaoSetJacobianEqualityRoutine(tao, J, J, constraintsGradFunc, &taoData);

        //    TaoSetTolerances(subsolver, 1.e-25/*0.038*/, 0.0, 0.0);
        //    TaoSetMaximumFunctionEvaluations(subsolver, 1000);
        //    KSP subksp;
        //    TaoGetKSP(subsolver, &subksp);
        //    KSPSetType(subksp, KSPPREONLY);
        //    KSPSetTolerances(subksp, 1.e-25, PETSC_DEFAULT, PETSC_DEFAULT, 500);
        //    PC subpc;
        //    KSPGetPC(subksp, &subpc);
        //    PCSetType(subpc, PCLU);

        //    TaoSetTolerances(tao, /*1.e-8*/1.e-05, 0.0, 0.0); //(tao, 1.e-5, 0.0, 0.0)
        //    TaoSetConstraintTolerances(tao, /*1.e-5*/1.e-06, 0.0);
        //    //TaoSetMaximumFunctionEvaluations(tao, PETSC_UNLIMITED);
        //    TaoSetMaximumFunctionEvaluations(tao, PETSC_UNLIMITED);
        //    TaoSetMaximumIterations(tao, 10000);

        //    Vec Ul, Uu;
        //    VecDuplicate(U, &Ul);
        //    VecDuplicate(U, &Uu);
        //    VecSet(Ul, -0.06/*-1*/);
        //    VecSet(Uu, 0.06/*1*/);
        //    PetscScalar eps=(PetscScalar)numeric_limits<double>::epsilon(); //1.e-50;

        //    std::vector<size_t> dirdofs= bc.getDirichletDofs();
        //    for(size_t i=0; i<dirdofs.size(); i++){
        //        VecSetValue(Ul, numPbc[dirdofs[i]], bc.pbcDirichletValue(dirdofs[i], ndof)/*-1.e14*eps*/, INSERT_VALUES);
        //        VecSetValue(Uu, numPbc[dirdofs[i]], bc.pbcDirichletValue(dirdofs[i], ndof)/*+1.e14*eps*/, INSERT_VALUES);
        //        std::cout<<"dirdofs values "<<i<<" : "<<bc.pbcDirichletValue(dirdofs[i], ndof)<<std::endl;
        //    }
        //    VecAssemblyBegin(Ul);
        //    VecAssemblyEnd(Ul);
        //    VecAssemblyBegin(Uu);
        //    VecAssemblyEnd(Uu);
        //    VecPointwiseDivide(Ul, Ul, Dinv);
        //    VecPointwiseDivide(Uu, Uu, Dinv);

        //    TaoSetVariableBounds(tao, Ul, Uu);

        //    TaoSetFromOptions(tao);
        //    TaoSetUp(tao);

        //    std::cout<<"Before solving ..."<<std::endl;
        //    //TaoSolve(tao);
        //    std::cout<<"After solving ..."<<std::endl;

        //    //reason = TAO_CONTINUE_ITERATING;
        //    TaoGetConvergedReason(tao, &reason);
        //    if(reason>0) std::cout<<"Converged with reason : "<<reason<<std::endl;
        //    else std::cout<<"Diverged with reason : "<<reason<<std::endl;
    }

    //*************************************** Linear Solving using MatNest ***************************************
    PetscErrorCode ierr;
    KSP ksp; PC pc;

    Mat            submats[4];        // Tableau à 1D représentant l'ordre Row-Major
    Mat            A;             // Matrice globale MATNEST
    PetscInt       nr = 2, nc = 2;     // Nombre de blocs de lignes et de colonnes

    Mat petscConstrMatT;

    PetscInt nrG, ncG; MatGetSize(petscConstrMat, &nrG, &ncG);
    PetscInt nrLG, ncLG;
    Mat LL; MatCreate(PETSC_COMM_WORLD, &LL);
    MatGetLocalSize(petscConstrMat, &nrLG, NULL);
    //MatGetLocalSize(Lambda, NULL, &ncLG);
    // MatSetSizes(LL, PETSC_DECIDE, PETSC_DECIDE, nrG, nrG);
    MatSetSizes(LL, nrLG, nrLG, PETSC_DETERMINE, PETSC_DETERMINE);
    MatSetType(LL, MATMPIAIJ);
    // MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE);
    //ierr = MatSeqAIJSetPreallocation(LL, 0, NULL); CHKERRQ(ierr);
    //ierr = MatMPIAIJSetPreallocation(LL, 1, NULL, 0, NULL); CHKERRQ(ierr);

    MatSetFromOptions(LL);
    MatSetUp(LL);
    //MatZeroEntries(LL);
    MatAssemblyBegin(LL, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(LL, MAT_FINAL_ASSEMBLY);
    PetscInt rb, re;
    MatGetOwnershipRange(LL, &rb, &re);
    for(PetscInt i=rb; i<re; i++) MatSetValue(LL, i, i, 0, INSERT_VALUES);
    MatAssemblyBegin(LL, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(LL, MAT_FINAL_ASSEMBLY);
        //MatShift(LL, 0.0);

    MatTranspose(petscConstrMat, MAT_INITIAL_MATRIX, &petscConstrMatT);

    submats[0] = petscKG; // Ligne 0, Colonne 0
    submats[1] = petscConstrMatT; // Ligne 0, Colonne 1
    submats[2] = petscConstrMat; // Ligne 1, Colonne 0
    submats[3] = /*NULL*/LL; // Ligne 1, Colonne 1

    PetscInt tr, tc;
    MatGetSize(petscConstrMat, &tr, &tc);
    std::cout<<"petscConstrMat size : "<<tr<<", "<<tc<<std::endl;
    MatGetSize(petscConstrMatT, &tr, &tc);
    std::cout<<"petscConstrMat transpose : "<<tr<<", "<<tc<<std::endl;
    MatGetSize(LL, &tr, &tc);
    std::cout<<"LL : "<<tr<<", "<<tc<<std::endl;

    ierr = MatCreateNest(PETSC_COMM_WORLD, nr, NULL, nc, NULL, submats, &A); CHKERRQ(ierr);

    //******************  Creating the right side *************************************************
    Vec ff; Vec rhs; Vec  subrhs[2]; Vec subsol[2]; Vec ULambda; //ULambda groups U and Lambda
    Vec Lambda;

    MatCreateVecs(petscKG, NULL, &ff); VecZeroEntries(ff);
    subrhs[0]=ff; subrhs[1]=petscConstrVec;

    //    PetscInt sc;  VecGetSize(petscConstrVec, &sc); std::cout<<"constr vec size sc : "<<sc<<std::endl;
    ierr = VecCreateNest(PETSC_COMM_WORLD, 2, NULL, subrhs, &rhs); CHKERRQ(ierr);

    MatCreateVecs(petscConstrMat, NULL, &Lambda/*, NULL*/);
    subsol[0]=U; subsol[1]=Lambda;
    ierr = VecCreateNest(PETSC_COMM_WORLD, 2, NULL, subsol, &ULambda); CHKERRQ(ierr);


    Mat Ap; Vec rhsp; Vec ULambdap;
    MatConvert(A, MATMPIAIJ, MAT_INITIAL_MATRIX, &Ap);

    MatCreateVecs(Ap, &ULambdap, &rhsp);

    //VecDuplicate(rhs, &rhsp);
    //MatCreateVecs()
    VecSetType(rhs, VECMPI);
    VecCopy(rhs, rhsp);

    ierr = KSPCreate(PETSC_COMM_WORLD, &ksp); CHKERRQ(ierr);
    ierr = KSPSetOperators(ksp, Ap, Ap); CHKERRQ(ierr);
    ierr = KSPGetPC(ksp, &pc); CHKERRQ(ierr);
    ierr = PCSetType(pc, PCLU); CHKERRQ(ierr);
    ierr=KSPSetType(ksp, KSPPREONLY); CHKERRQ(ierr);
    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
    KSPSetTolerances(ksp, 1e-12, PETSC_DEFAULT, PETSC_DEFAULT, 200);
    ierr = KSPSolve(ksp, rhsp, ULambdap); CHKERRQ(ierr);

    std::valarray<double> ULambdapVal; Vec solVec;
    dataForPetsc.VecToValarray(ULambdap, ULambdapVal);
    sol=ULambdapVal[std::slice(0, _KG.nrow(), 1)];
    dataForPetsc.valarrayToVec(sol, solVec);
    VecPointwiseMult(solVec, Dinv,solVec);
    dataForPetsc.VecToValarray(solVec, sol);



//    ierr = KSPCreate(PETSC_COMM_WORLD, &ksp); CHKERRQ(ierr);
//    ierr = KSPSetOperators(ksp, A, A); CHKERRQ(ierr);
//    // Récupération du préconditionneur pour appliquer le découpage de champs
//    ierr = KSPGetPC(ksp, &pc); CHKERRQ(ierr);
//    ierr = PCSetType(pc, PCFIELDSPLIT/*PCLU*/); CHKERRQ(ierr);
//    ierr = PCSetType(pc, PCLU); CHKERRQ(ierr);
//    ierr=KSPSetType(ksp, KSPPREONLY); CHKERRQ(ierr);
//    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
//    KSPSetTolerances(ksp, 1e-12, PETSC_DEFAULT, PETSC_DEFAULT, 200);
//    // KSPSetUp(ksp);

//    IS is_row[2];
//    MatNestGetISs(A, is_row, NULL);
//    PCFieldSplitSetIS(pc, "champ_0", is_row[0]); //
//    PCFieldSplitSetIS(pc, "champ_1", is_row[1]); //


//    ierr = KSPSolve(ksp, rhs, ULambda); CHKERRQ(ierr);

//    //    //****************** Recovering the real U ***************************
//    VecPointwiseMult(U, Dinv, U);
//    //DataForPetsc::VecToValarray(U, sol);
//    dataForPetsc.VecToValarray(U, sol);

    //    PetscBarrier((PetscObject)tao);

    //    MatDestroy(&GGt);
    //    MatDestroy(&petscKG);
    //    MatDestroy(&petscConstrMat);
    //    VecDestroy(&petscConstrVec);
    //    VecDestroy(&initVec);

    //    VecDestroy(&U);
    //    MatDestroy(&petscConstrMat);
    //    MatDestroy(&petscConstrMatT);
    //    KSPDestroy(&ksp);

    //PCDestroy(&pc);

    PetscFunctionReturn(PETSC_SUCCESS);




}


PetscErrorCode DataForPetsc::petscSolveLinearRedMumps(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                                 const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                                 std::valarray<double>& sol, const std::valarray<size_t>& numPbc, const fem::BCs& bc){


    fem::matrix<double> Kuu, Kud, Gu, Gd;
    std::valarray<double> fu, fl, ud;
    std::vector<size_t> dirdofs;
    fem::matrix<double> constrMatBar; //To erase the hardcoded dirichlet dofs

    std::valarray<double> ulambda;              //for ubar&lambda
    std::valarray<double> dispPbc;                //(size_t(nu+nd), 0.); //for u

    dirdofs=bc.getDirichletDofs();
    size_t nd=dirdofs.size();
    size_t nu=_KG.nrow()-nd ;

    Kuu=_KG; Kuu.nFirstLines(nu); Kuu.nFirstColumns(nu);
    Kud=_KG; Kud.nFirstLines(nu); Kud.nLastColumns(nd);


    constrMatBar=constrMat; constrMatBar.nFirstLines(constrMat.nrow()-nd); //To ignore the hardcoded dirichlet boundary conditions
    Gu=constrMatBar; Gu.nFirstColumns(nu);
    Gd=constrMatBar; Gd.nLastColumns(nd);

    ud.resize(nd, 0.);
    for(size_t i =0; i<nd; i++) ud[ numPbc[dirdofs[i]] -nu]=bc.pbcDirichletValue(dirdofs[i], ndof);

    dispPbc.resize(_KG.ncol(), 0.);

    MPI_Barrier(MPI_COMM_WORLD);

    fu=-1*(Kud*ud);
    fl=-1*(Gd*ud);

    MPI_Barrier(MPI_COMM_WORLD);


    Mat Af; Vec sol2, rhs2; PetscInt rb, re; PetscErrorCode ierr;
    KSP ksp; PC pc;

    fem::matrix<double> Ap= Kuu;
    fem::matrix<double> Gup= Gu;
    fem::matrix<double> GupT=Gu.transpose();
    Ap.appendMatrixToRight(GupT);
    //Gup.setIdx(Gu.nrow()+Gu.ncol()); //This to take in account A_cc=0
    Ap.appendMatrixToBottom(Gup);

    std::valarray<double> cstrVec, uForce, rhsplat; std::valarray<double> rhsp;

    uForce=fu;
    cstrVec=fl;
    size_t rhspsize= (uForce.size()) +(cstrVec.size());
    rhsp.resize(/*uForce.size()+cstrVec.size()*/rhspsize, 0.);
    rhsp[std::slice(0, uForce.size(), 1)]=uForce;
    rhsp[std::slice(uForce.size(), cstrVec.size(), 1)] = cstrVec;

    PetscFunctionBeginUser;

    MPI_Barrier(MPI_COMM_WORLD);
    //valarrayToVec(rhsp, rhs2);
    matrixToPetscMat(Ap, Af);
    for(PetscInt i=0; i<(PetscInt)Gu.nrow(); i++) MatSetValue(Af, PetscInt(nu+i), PetscInt(nu+i), 0., INSERT_VALUES);
    MatAssemblyBegin(Af, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(Af, MAT_FINAL_ASSEMBLY);
    PetscScalar hh=solveEigen2(Af, EPS_LARGEST_REAL);
    std::cout<< "Af higher proper value  after conditioning: "<<hh<<std::endl;
    PetscScalar ll=solveEigen2(Af, EPS_SMALLEST_REAL);
    std::cout<< "Af smallest proper value after conditioning: "<<ll<<std::endl;
    PetscViewer viewer;
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscAf.dat", FILE_MODE_WRITE, &viewer);
    MatView(Af, viewer);
    //return 0;

PetscBarrier(PetscObject(Af));
    MatCreateVecs(Af, &sol2, &rhs2);
    PetscScalar* rhsArray;
    VecGetOwnershipRange(rhs2, &rb, &re);
    VecGetArray(rhs2, &rhsArray);
    for(PetscInt i=rb; i<re; i++) rhsArray[i-rb]=rhsp[i];
    VecRestoreArray(rhs2, &rhsArray);

PetscBarrier(PetscObject(Af));

    KSPCreate(PETSC_COMM_WORLD, &ksp);
//MatShift(Af, 1e-6);
    KSPSetOperators(ksp,Af, Af);
    //KSPSetType(ksp, KSPPREONLY);
    //KSPGetPC(ksp,&pc);
    //PCSetType(pc, PCLU);
    //PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);
   // KSPGetPC(ksp, &pc);
    //PCSetType(pc, PCLU);
//    PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);
//    PCSetFromOptions(pc);
//    KSPSetFromOptions(ksp);

    KSPGetPC(ksp, &pc);
    KSPSetType(ksp, KSPFGMRES);
    //KSPSetType(ksp, KSPPREONLY);
    PCSetType(pc, PCFIELDSPLIT);

    PetscInt sizeU=(PetscInt)Kuu.nrow();
    PetscInt sizeL=(PetscInt)Gu.nrow();

    IS is_u, is_L;
    ierr = ISCreateStride(PETSC_COMM_WORLD, sizeU, 0, 1, &is_u); CHKERRQ(ierr);
    ierr = ISCreateStride(PETSC_COMM_WORLD, sizeL, sizeU, 1, &is_L); CHKERRQ(ierr);

    ierr = PCFieldSplitSetIS(pc, "champ_0", is_u); CHKERRQ(ierr);
    ierr = PCFieldSplitSetIS(pc, "champ_1", is_L); CHKERRQ(ierr);

    ierr = PCFieldSplitSetType(pc, PC_COMPOSITE_SCHUR); CHKERRQ(ierr);
    ierr = PCFieldSplitSetSchurFactType(pc, PC_FIELDSPLIT_SCHUR_FACT_FULL); CHKERRQ(ierr);

    ierr = ISDestroy(&is_u); CHKERRQ(ierr);
    ierr = ISDestroy(&is_L); CHKERRQ(ierr);

    // 6. Autoriser le contrôle via les options utilisateur
    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);

    PetscInt Ar, Ac, rhssize, solsize;
    MatGetSize(Af, &Ar, &Ac);
    VecGetSize(rhs2, &rhssize);
    VecGetSize(sol2, &solsize);
    std::cout<<"Af size : "<<Ar<<" x "<<Ac<<std::endl;
    std::cout<<"rsh2 size :  "<<rhssize<<std::endl;
    std::cout<<"sol2 size : "<<solsize<<std::endl;

{//    Mat petscKuu;
//    matrixToPetscMat(Kuu, petscKuu);
//    // MatShift(petscKuu, 1e-6*608.*5.*5e-7);
//    PetscScalar hh=solveEigen(petscKuu, EPS_LARGEST_REAL);
//    PetscScalar ll=solveEigen(petscKuu, EPS_SMALLEST_REAL);
//    std::cout<< "Af higher proper value : "<<hh<<std::endl;
//    std::cout<< "Af smallest proper value : "<<ll<<std::endl;


//    Vec DInv;
//    MatCreateVecs(petscKuu, NULL, &DInv);
//    MatGetDiagonal(petscKuu, DInv);
//    VecPow(DInv, -0.5);
//    MatDiagonalScale(petscKuu, DInv, DInv);
//    hh=solveEigen(petscKuu, EPS_LARGEST_REAL);
//    ll=solveEigen(petscKuu, EPS_SMALLEST_REAL);
//    std::cout<< "Af higher proper value  after conditioning: "<<hh<<std::endl;
//    std::cout<< "Af smallest proper value after conditioning: "<<ll<<std::endl;
//    size_t ne=Mh.getNumberOfElements()*4*3;
//    std::cout<< "Numbers of dofs based on number of elements "<<ne<<std::endl;
//    MPI_Barrier(MPI_COMM_WORLD);
}

    MPI_Barrier(MPI_COMM_WORLD);
    KSPSetTolerances(ksp,0., 1.e-15, PETSC_UNLIMITED, PETSC_UNLIMITED);
    std::cout<<" Solving Af.sol2=rhs2 ... "<<std::endl;

    PetscBarrier(PetscObject(Af));
    ierr = KSPSolve(ksp, rhs2, sol2); CHKERRQ(ierr);
    PetscBarrier(PetscObject(Af));

    std::cout<<"End solving Af.sol2=rhs2 "<<std::endl;
    KSPConvergedReason kspreason;
    KSPGetConvergedReason(ksp, &kspreason);
    if(kspreason<0) std::cout<<"Diverged with reason : "<<kspreason<<std::endl;
    else std::cout<<"Converged with reason : "<<kspreason<<std::endl;

    if (kspreason == KSP_DIVERGED_PC_FAILED) {
    PC pc;
    PCFailedReason pc_reason;
    KSPGetPC(ksp, &pc);
    PCGetFailedReason(pc, &pc_reason);
    // Imprime le code d'erreur spécifique du PC (ex: voir la liste des PCFailedReason)
    PetscPrintf(PETSC_COMM_WORLD, "Le PC a échoué avec le code : %d\n", pc_reason);
}
    //    //****************** Recovering the real U ***************************
    MPI_Barrier(MPI_COMM_WORLD);

    VecToValarray(sol2, ulambda);

    dispPbc[std::slice(0, nu, 1)]=ulambda[std::slice(0, nu, 1)];
    for(size_t i=0; i<(nd); i++) dispPbc[ numPbc[dirdofs[i]] ]=bc.pbcDirichletValue( dirdofs[i], ndof );
    //std::valarray<double> dispPbc(std::begin(ulambda), nu+nd);
    sol=dispPbc;
    //dataForPetsc.VecToValarray(U, sol);


    PetscFunctionReturn(PETSC_SUCCESS);
}


PetscErrorCode DataForPetsc::petscSolveLinearRedMumpsCond(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
        const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec , const std::valarray<size_t>& numPbc , const fem::BCs& bc,
                                                          std::valarray<double>& sol, std::valarray<double>& lambda){

    fem::matrix<double> Kuu, Kud, Gu, Gd;
    std::valarray<double> fu, fl, ud;
    std::vector<size_t> dirdofs;
    fem::matrix<double> constrMatBar; //To erase the hardcoded dirichlet dofs

    std::valarray<double> ulambda;              //for ubar&lambda
    std::valarray<double> dispPbc;                //(size_t(nu+nd), 0.); //for u

    dirdofs=bc.getDirichletDofs();
    size_t nd=dirdofs.size();
    size_t nu=_KG.nrow()-nd ;

    Kuu=_KG; Kuu.nFirstLines(nu); Kuu.nFirstColumns(nu);
    Kud=_KG; Kud.nFirstLines(nu); Kud.nLastColumns(nd);

    constrMatBar=constrMat; constrMatBar.nFirstLines(constrMat.nrow()-nd); //To ignore the hardcoded dirichlet boundary conditions
    Gu=constrMatBar; Gu.nFirstColumns(nu);
    Gd=constrMatBar; Gd.nLastColumns(nd);

    ud.resize(nd, 0.);
    for(size_t i =0; i<nd; i++) ud[ numPbc[dirdofs[i]] -nu]=bc.pbcDirichletValue(dirdofs[i], ndof);

    dispPbc.resize(_KG.ncol(), 0.);

    //Conditionning
    MPI_Barrier(MPI_COMM_WORLD);
    fu=-1*(Kud*ud);
    fl=-1*(Gd*ud);

    std::valarray<double> DPowerMinusHalf, etaPowerMinuxOne;
    //fem::matrix<double> DPowerMinusHalfM, etaPowerMinuxOneM;
    //DPowerMinusHalfM = fem::toDiagMatrix (DPowerMinusHalf);
    //etaPowerMinuxOneM = fem::toDiagMatrix(etaPowerMinuxOne);
    Kuu.getDiagonal(DPowerMinusHalf);
    for(size_t i =0; i<DPowerMinusHalf.size() ; i++) if(std::fabs(DPowerMinusHalf[i])<=1.e-50) DPowerMinusHalf[i]=1. ;
    DPowerMinusHalf =  std::pow(DPowerMinusHalf, -1./2.);

    fem::matrix<double> GuD = Gu.diagRightMult(DPowerMinusHalf);
    fem::matrix<double> GuDT = GuD.transpose();
    fem::matrix<double> etaM = GuD*GuDT;
    std::valarray<double> etaDiag;
    etaM.getDiagonal(etaDiag);
    for(size_t i=0; i<etaDiag.size(); i++) if(std::fabs(etaDiag[i])<=1.e-50) etaDiag[i]=1.;
    etaPowerMinuxOne=std::pow(etaDiag, -1.);

    fem::matrix<double> KuuCond, GuCond;
    std::valarray<double> FuCond, FlCond;

    KuuCond=Kuu.diagRightMult(DPowerMinusHalf);
    KuuCond=KuuCond.diagLeftMult(DPowerMinusHalf);
    GuCond = Gu.diagRightMult(DPowerMinusHalf);
    GuCond = GuCond.diagLeftMult(etaPowerMinuxOne);
    FuCond = DPowerMinusHalf*fu;
    FlCond = etaPowerMinuxOne*fl;

    //End Conditioning

    MPI_Barrier(MPI_COMM_WORLD);


    Mat Af; Vec sol2, rhs2; PetscInt rb, re; PetscErrorCode ierr;
    KSP ksp; PC pc;

    fem::matrix<double> Ap = KuuCond;
    fem::matrix<double> Gup = GuCond;
    fem::matrix<double> GupT = GuCond.transpose();
    Ap.appendMatrixToRight(GupT);
    //Gup.setIdx(Gu.nrow()+Gu.ncol()); //This to take in account A_cc=0
    Ap.appendMatrixToBottom(Gup);

    std::valarray<double> cstrVec, uForce, rhsplat; std::valarray<double> rhsp;

    uForce = FuCond;
    cstrVec = FlCond;
    size_t rhspsize = (uForce.size()) +(cstrVec.size());
    rhsp.resize(/*uForce.size()+cstrVec.size()*/rhspsize, 0.);
    rhsp[std::slice(0, uForce.size(), 1)] = uForce;
    rhsp[std::slice(uForce.size(), cstrVec.size(), 1)] = cstrVec;

    PetscFunctionBeginUser;

    MPI_Barrier(MPI_COMM_WORLD);
    //valarrayToVec(rhsp, rhs2);
    matrixToPetscMat(Ap, Af);
    for(PetscInt i=0; i<(PetscInt)Gu.nrow(); i++) MatSetValue(Af, PetscInt(nu+i), PetscInt(nu+i), 0., INSERT_VALUES);
    MatAssemblyBegin(Af, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(Af, MAT_FINAL_ASSEMBLY);
    //PetscScalar hh=solveEigen2(Af, EPS_LARGEST_REAL);
    //std::cout<< "Af higher proper value  after conditioning: "<<hh<<std::endl;
    //PetscScalar ll=solveEigen2(Af, EPS_SMALLEST_REAL);
    //std::cout<< "Af smallest proper value after conditioning: "<<ll<<std::endl;
    PetscViewer viewer;
    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscAf.dat", FILE_MODE_WRITE, &viewer);
    MatView(Af, viewer);
    //return 0;

PetscBarrier(PetscObject(Af));
    MatCreateVecs(Af, &sol2, &rhs2);
    PetscScalar* rhsArray;
    VecGetOwnershipRange(rhs2, &rb, &re);
    VecGetArray(rhs2, &rhsArray);
    for(PetscInt i=rb; i<re; i++) rhsArray[i-rb]=rhsp[i];
    VecRestoreArray(rhs2, &rhsArray);

PetscBarrier(PetscObject(Af));

    KSPCreate(PETSC_COMM_WORLD, &ksp);
//MatShift(Af, 1e-6);
    KSPSetOperators(ksp,Af, Af);
    //KSPSetType(ksp, KSPPREONLY);
    //KSPGetPC(ksp,&pc);
    //PCSetType(pc, PCLU);
    //PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);
   // KSPGetPC(ksp, &pc);
    //PCSetType(pc, PCLU);
//    PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);
//    PCSetFromOptions(pc);
//    KSPSetFromOptions(ksp);

    KSPGetPC(ksp, &pc);
    KSPSetType(ksp, KSPFGMRES);
    //PCSetType(pc, PCFIELDSPLIT);

//    PetscInt sizeU=(PetscInt)Kuu.nrow();
//    PetscInt sizeL=(PetscInt)Gu.nrow();

//    IS is_u, is_L;
//    ierr = ISCreateStride(PETSC_COMM_WORLD, sizeU, 0, 1, &is_u); CHKERRQ(ierr);
//    ierr = ISCreateStride(PETSC_COMM_WORLD, sizeL, sizeU, 1, &is_L); CHKERRQ(ierr);

//    ierr = PCFieldSplitSetIS(pc, "champ_0", is_u); CHKERRQ(ierr);
//    ierr = PCFieldSplitSetIS(pc, "champ_1", is_L); CHKERRQ(ierr);

//    ierr = PCFieldSplitSetType(pc, PC_COMPOSITE_SCHUR); CHKERRQ(ierr);
//    ierr = PCFieldSplitSetSchurFactType(pc, PC_FIELDSPLIT_SCHUR_FACT_FULL); CHKERRQ(ierr);

//    ierr = ISDestroy(&is_u); CHKERRQ(ierr);
//    ierr = ISDestroy(&is_L); CHKERRQ(ierr);

    // 6. Autoriser le contrôle via les options utilisateur
    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);

    PetscInt Ar, Ac, rhssize, solsize;
    MatGetSize(Af, &Ar, &Ac);
    VecGetSize(rhs2, &rhssize);
    VecGetSize(sol2, &solsize);
    std::cout<<"Af size : "<<Ar<<" x "<<Ac<<std::endl;
    std::cout<<"rsh2 size :  "<<rhssize<<std::endl;
    std::cout<<"sol2 size : "<<solsize<<std::endl;

{//    Mat petscKuu;
//    matrixToPetscMat(Kuu, petscKuu);
//    // MatShift(petscKuu, 1e-6*608.*5.*5e-7);
//    PetscScalar hh=solveEigen(petscKuu, EPS_LARGEST_REAL);
//    PetscScalar ll=solveEigen(petscKuu, EPS_SMALLEST_REAL);
//    std::cout<< "Af higher proper value : "<<hh<<std::endl;
//    std::cout<< "Af smallest proper value : "<<ll<<std::endl;


//    Vec DInv;
//    MatCreateVecs(petscKuu, NULL, &DInv);
//    MatGetDiagonal(petscKuu, DInv);
//    VecPow(DInv, -0.5);
//    MatDiagonalScale(petscKuu, DInv, DInv);
//    hh=solveEigen(petscKuu, EPS_LARGEST_REAL);
//    ll=solveEigen(petscKuu, EPS_SMALLEST_REAL);
//    std::cout<< "Af higher proper value  after conditioning: "<<hh<<std::endl;
//    std::cout<< "Af smallest proper value after conditioning: "<<ll<<std::endl;
//    size_t ne=Mh.getNumberOfElements()*4*3;
//    std::cout<< "Numbers of dofs based on number of elements "<<ne<<std::endl;
//    MPI_Barrier(MPI_COMM_WORLD);
}

    MPI_Barrier(MPI_COMM_WORLD);
    KSPSetTolerances(ksp,0., 1.e-12, PETSC_UNLIMITED, PETSC_UNLIMITED);
    KSPSetFromOptions(ksp);
    std::cout<<" Solving Af.sol2=rhs2 ... "<<std::endl;

    PetscBarrier(PetscObject(Af));
    //VecSet(sol2, 1.0);
    ierr = KSPSolve(ksp, rhs2, sol2); CHKERRQ(ierr);
    KSPSetFromOptions(ksp);
    PetscBarrier(PetscObject(Af));

    std::cout<<"End solving Af.sol2=rhs2 "<<std::endl;
    KSPConvergedReason kspreason;
    KSPGetConvergedReason(ksp, &kspreason);
    if(kspreason<0) std::cout<<"Diverged with reason : "<<kspreason<<std::endl;
    else std::cout<<"Converged with reason : "<<kspreason<<std::endl;

    if (kspreason == KSP_DIVERGED_PC_FAILED) {
    PC pc;
    PCFailedReason pc_reason;
    KSPGetPC(ksp, &pc);
    PCGetFailedReason(pc, &pc_reason);
    // Imprime le code d'erreur spécifique du PC (ex: voir la liste des PCFailedReason)
    PetscPrintf(PETSC_COMM_WORLD, "Le PC a échoué avec le code : %d\n", pc_reason);
}
    //    //****************** Recovering the real U ***************************
    MPI_Barrier(MPI_COMM_WORLD);

    VecToValarray(sol2, ulambda);

    //dispPbc[std::slice(0, nu, 1)] = ulambda[std::slice(0, nu, 1)];
    dispPbc.resize(nu+nd, 0.);

    std::valarray<double> solubar = ulambda[std::slice(0, nu, 1)] ;
    dispPbc[std::slice(0, nu, 1)] = solubar*DPowerMinusHalf;
    lambda.resize(Gu.nrow(), 0.);
    lambda = ulambda[std::slice(solubar.size(), Gu.nrow(), 1)] ;
    lambda = etaPowerMinuxOne*lambda;
    std::ofstream ofs("/home/ahose/Desktop/lambda.dat");
    for(size_t i=0; i<lambda.size(); i++) ofs<<lambda[i]<<std::endl;
    ofs.close();

    //std::valarray<double> fLambda = (constrMat.transpose())*solLambda;
    //dispPbc += fLambda;
    for(size_t i=0; i<(nd); i++) dispPbc[ numPbc[dirdofs[i]] ]=bc.pbcDirichletValue( dirdofs[i], ndof );
    sol=dispPbc;
    //dataForPetsc.VecToValarray(U, sol);

    PetscFunctionReturn(PETSC_SUCCESS);
}

PetscErrorCode DataForPetsc::petscSolveLinearRedMumpsCond2(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
        const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec , const std::valarray<size_t>& numPbc , const fem::BCs& bc,
                                                          std::valarray<double>& sol, std::valarray<double>& lambda){
    fem::matrix<double> KS, Km =_KG , Gm, Gr, O12, O21, O22, O33, O2;
    fem::matrix<double> GS, GST;

    std::valarray<double> FS;
    FS.resize(ndof*Mh.getNumberOfNodes()+ constrMat.nrow());

    size_t meshAndNDir, meshAndDir, NMeshAndDir, NMeshAndNDir;
    bc.getRefCount(meshAndNDir, meshAndDir, NMeshAndDir, NMeshAndNDir);
    Gm=constrMat; Gm.nFirstColumns(meshAndNDir+meshAndDir);
    Gr=constrMat; Gr.nLastColumns(NMeshAndDir+NMeshAndNDir);


    O12.zero(meshAndNDir+meshAndDir, NMeshAndDir+NMeshAndNDir);
    O21.zero(NMeshAndDir+NMeshAndNDir, meshAndNDir+meshAndDir);
    O22.zero(NMeshAndDir+NMeshAndNDir, NMeshAndDir+NMeshAndNDir);
    O33.zero(constrMat.nrow(), constrMat.nrow());

    GS=constrMat; GST=constrMat.transpose();

    KS=_KG; KS.appendMatrixToRight(O12);
    O2=O21; O21.appendMatrixToRight(O22);
    KS.appendMatrixToBottom(O2);
    KS.appendMatrixToRight(GST);
    GS.appendMatrixToRight(O33);
    KS.appendMatrixToBottom(GS);


    FS[std::slice(meshAndNDir+meshAndDir+NMeshAndDir+NMeshAndNDir, constrMat.nrow(),1)] = constrVec;

    std::vector<size_t> dirdofs;
    dirdofs=bc.getDirichletDofs();
    size_t nd=dirdofs.size();

    std::valarray<double> ulambda;              //for ubar&lambda
    std::valarray<double> dispPbc;                //(size_t(nu+nd), 0.); //for u

    MPI_Barrier(MPI_COMM_WORLD);


    Mat Af; Vec solf, rhsf; PetscInt rb, re; PetscErrorCode ierr;
    KSP ksp; PC pc;

  //  std::valarray<double> cstrVec, uForce, rhsplat; //std::valarray<double> rhsp;

    PetscFunctionBeginUser;

    MPI_Barrier(MPI_COMM_WORLD);

    PetscInt nLamb = constrMat.nrow();
    matrixToPetscMat(KS, Af);

    for(PetscInt i=0; i<(NMeshAndDir+NMeshAndNDir); i++)
    MatSetValue(Af, PetscInt(meshAndNDir+meshAndDir+i), PetscInt(meshAndNDir+meshAndDir+i), 0., INSERT_VALUES);

    for(PetscInt i=0; i<(nLamb); i++)
    MatSetValue(Af, PetscInt(meshAndNDir+meshAndDir+NMeshAndDir+NMeshAndNDir+i), PetscInt(meshAndNDir+meshAndDir+NMeshAndDir+NMeshAndNDir+i), 0., INSERT_VALUES);

    MatAssemblyBegin(Af, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(Af, MAT_FINAL_ASSEMBLY);

//    PetscViewer viewer;
//    PetscViewerBinaryOpen(PETSC_COMM_WORLD, "/home/ahose/venus/petscAf.dat", FILE_MODE_WRITE, &viewer);
//    MatView(Af, viewer);

PetscBarrier(PetscObject(Af));
    MatCreateVecs(Af, &solf, &rhsf);
    PetscScalar* rhsArray;
    VecGetOwnershipRange(rhsf, &rb, &re);
    VecGetArray(rhsf, &rhsArray);
    for(PetscInt i=rb; i<re; i++) rhsArray[i-rb]=FS[i];
    VecRestoreArray(rhsf, &rhsArray);

    //MatShift(Af, 1e-20);
PetscBarrier(PetscObject(Af));

    KSPCreate(PETSC_COMM_WORLD, &ksp);
    KSPSetOperators(ksp,Af, Af);
 {    //KSPSetType(ksp, KSPPREONLY);
    //KSPGetPC(ksp,&pc);
    //PCSetType(pc, PCLU);
    //PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);
   // KSPGetPC(ksp, &pc);
    //PCSetType(pc, PCLU);
//    PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);
//    PCSetFromOptions(pc);
//    KSPSetFromOptions(ksp);

    //KSPGetPC(ksp, &pc);
    //KSPSetType(ksp, KSPFGMRES);
    //PCSetType(pc, PCFIELDSPLIT);

//    PetscInt sizeU=(PetscInt)Kuu.nrow();
//    PetscInt sizeL=(PetscInt)Gu.nrow();

//    IS is_u, is_L;
//    ierr = ISCreateStride(PETSC_COMM_WORLD, sizeU, 0, 1, &is_u); CHKERRQ(ierr);
//    ierr = ISCreateStride(PETSC_COMM_WORLD, sizeL, sizeU, 1, &is_L); CHKERRQ(ierr);

//    ierr = PCFieldSplitSetIS(pc, "champ_0", is_u); CHKERRQ(ierr);
//    ierr = PCFieldSplitSetIS(pc, "champ_1", is_L); CHKERRQ(ierr);

//    ierr = PCFieldSplitSetType(pc, PC_COMPOSITE_SCHUR); CHKERRQ(ierr);
//    ierr = PCFieldSplitSetSchurFactType(pc, PC_FIELDSPLIT_SCHUR_FACT_FULL); CHKERRQ(ierr);

//    ierr = ISDestroy(&is_u); CHKERRQ(ierr);
//    ierr = ISDestroy(&is_L); CHKERRQ(ierr);

    // 6. Autoriser le contrôle via les options utilisateur
 }
    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);

    PetscInt Ar, Ac, rhssize, solsize;
    MatGetSize(Af, &Ar, &Ac);
    VecGetSize(rhsf, &rhssize);
    VecGetSize(solf, &solsize);
    std::cout<<"Af size : "<<Ar<<" x "<<Ac<<std::endl;
    std::cout<<"rsh2 size :  "<<rhssize<<std::endl;
    std::cout<<"sol2 size : "<<solsize<<std::endl;

{//    Mat petscKuu;
//    matrixToPetscMat(Kuu, petscKuu);
//    // MatShift(petscKuu, 1e-6*608.*5.*5e-7);
//    PetscScalar hh=solveEigen(petscKuu, EPS_LARGEST_REAL);
//    PetscScalar ll=solveEigen(petscKuu, EPS_SMALLEST_REAL);
//    std::cout<< "Af higher proper value : "<<hh<<std::endl;
//    std::cout<< "Af smallest proper value : "<<ll<<std::endl;


//    Vec DInv;
//    MatCreateVecs(petscKuu, NULL, &DInv);
//    MatGetDiagonal(petscKuu, DInv);
//    VecPow(DInv, -0.5);
//    MatDiagonalScale(petscKuu, DInv, DInv);
//    hh=solveEigen(petscKuu, EPS_LARGEST_REAL);
//    ll=solveEigen(petscKuu, EPS_SMALLEST_REAL);
//    std::cout<< "Af higher proper value  after conditioning: "<<hh<<std::endl;
//    std::cout<< "Af smallest proper value after conditioning: "<<ll<<std::endl;
//    size_t ne=Mh.getNumberOfElements()*4*3;
//    std::cout<< "Numbers of dofs based on number of elements "<<ne<<std::endl;
//    MPI_Barrier(MPI_COMM_WORLD);
}

    MPI_Barrier(MPI_COMM_WORLD);
    KSPSetTolerances(ksp,0., 1.e-12, PETSC_UNLIMITED, PETSC_UNLIMITED);
    KSPSetFromOptions(ksp);
    std::cout<<" Solving Af.solf=rhsf ... "<<std::endl;

    PetscBarrier(PetscObject(Af));
    //VecSet(sol2, 1.0);
    ierr = KSPSolve(ksp, rhsf, solf); CHKERRQ(ierr);
    KSPSetFromOptions(ksp);
    PetscBarrier(PetscObject(Af));

    std::cout<<"End solving Af.solf=rhsf "<<std::endl;
    KSPConvergedReason kspreason;
    KSPGetConvergedReason(ksp, &kspreason);
    if(kspreason<0) std::cout<<"Diverged with reason : "<<kspreason<<std::endl;
    else std::cout<<"Converged with reason : "<<kspreason<<std::endl;

    if (kspreason == KSP_DIVERGED_PC_FAILED) {
    PC pc;
    PCFailedReason pc_reason;
    KSPGetPC(ksp, &pc);
    PCGetFailedReason(pc, &pc_reason);
    // Imprime le code d'erreur spécifique du PC (ex: voir la liste des PCFailedReason)
    PetscPrintf(PETSC_COMM_WORLD, "Le PC a échoué avec le code : %d\n", pc_reason);
}
    //    //****************** Recovering the real U ***************************
    MPI_Barrier(MPI_COMM_WORLD);

    VecToValarray(solf, ulambda);

    //dispPbc[std::slice(0, nu, 1)] = ulambda[std::slice(0, nu, 1)];
    /*dispPbc*/sol.resize(meshAndNDir+meshAndDir+NMeshAndDir+NMeshAndNDir, 0.);
    /*dispPbc*/sol = ulambda[std::slice(0,meshAndNDir+meshAndDir+NMeshAndDir+NMeshAndNDir, 1)];

    lambda.resize(nLamb, 0.);
    lambda = ulambda[std::slice(meshAndNDir+meshAndDir+NMeshAndDir+NMeshAndNDir, nLamb, 1)] ;
    //lambda = etaPowerMinuxOne*lambda;
    std::ofstream ofs("/home/ahose/Desktop/lambda.dat");
    for(size_t i=0; i<lambda.size(); i++) ofs<<lambda[i]<<std::endl;
    ofs.close();

//    for(size_t i=0; i<(nd); i++) dispPbc[ numPbc[dirdofs[i]] ]=bc.pbcDirichletValue( dirdofs[i], ndof );
//    sol=dispPbc;

    //dataForPetsc.VecToValarray(U, sol);
    PetscFunctionReturn(PETSC_SUCCESS);
}


} // namespace models
} // namespace scaletag
