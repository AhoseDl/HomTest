#ifndef PETSCUTILITIES_H
#define PETSCUTILITIES_H

#include <QObject>

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

//#include<stlib_global.h>
//#include<matrix.h>

#include<petsctao.h>
#include<slepceps.h>

//#include<petsc.h>
#include<matrix.h>
#include<MeshPbc.h>
#include<bcs.h>



namespace scaletag {
namespace models {

 PetscErrorCode objAndGradFunc(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx);
 PetscErrorCode hessianFunc(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx);
 PetscErrorCode constraintsFunc(Tao tao, Vec X, Vec ce, void* ctx);
 PetscErrorCode constraintsGradFunc(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx);


 PetscScalar solveEigen(Mat A, EPSWhich which);

 class A  {
     int x; double y;

 };

 class Data {
public:
    Data(Mat*_petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec, Mat* _condGMat, Vec* firstGuess
         ):petscKG(_petscKG), petscConstrMat(_petscConstrMat),petscConstrVec(_petscConstrVec), condGMat(_condGMat), initValue(firstGuess){


        Vec Xd0; //will contain  the product KG*initialGuess


//        PetscInt nG, mG, ri, rf;
//        PetscInt nnz;
//        const PetscScalar cols[] ; const PetscScalar vals[];
//        PetscScalar valr=0.; // To contain the norm of a line

//        Vec diagVec; Mat diagMat;

//        MatGetSize(*petscConstrMat, &nG, NULL);

//        VecCreate(PETSC_COMM_WORLD, &diagVec);
//        VecSetSizes(diagVec, PETSC_DECIDE, nG);
//        VecSetFromOptions(diagVec);

//        MatGetOwnershipRange(*petscConstrMat, &ri, &rf);
//        for(PetscInt r=ri; r<rf; r++){
//            MatGetRow(*petscConstrMat, r, &nnz, NULL, &vals);
//            for(PetscInt i=0; i<nnz; i++) valr += vals[i]*vals[i];
//            valr = std::sqrt(valr);
//            VecSetValue(diagVec, r, valr, INSERT_VALUES);
//            MatRestoreRow(*petscConstrMat, r, &nnz, NULL, &vals);
//            valr = 0;
//        }
//        VecAssemblyBegin(diagVec);
//        VecAssemblyEnd(diagVec);

//        MatCreateDiagonal(diagVec, &diagMat); //condGMat


        VecDuplicate(*initValue, &Xd0);
        MatMult(*petscKG,
                *initValue, Xd0);
        VecDot(*initValue, Xd0, &objectInit);
        //objectInit=std::fabs(0.5*objectInit);
        objectInit=PetscAbsReal(0.5*objectInit);
        VecDestroy(&Xd0);
          }




 Data(Mat*_petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec, Vec* firstGuess
               ):petscKG(_petscKG), petscConstrMat(_petscConstrMat),petscConstrVec(_petscConstrVec),initValue(firstGuess){


        Vec Xd0; //will contain  the product KG*initialGuess

        VecDuplicate(*initValue, &Xd0);
        MatMult(*petscKG,
                *initValue, Xd0);
        VecDot(*initValue, Xd0, &objectInit);
        //objectInit=std::fabs(0.5*objectInit);
        objectInit=PetscAbsReal(0.5*objectInit);
        VecDestroy(&Xd0);
          }

    Mat* getRigidityMat(){return petscKG;}
    Mat* getConstraintsMat(){return petscConstrMat;}
    Vec* getConstraintsVec(){return petscConstrVec;}

    Mat* getCondMat(){return condGMat;}

    PetscScalar getInitialGuessNorm(){PetscScalar inorm=0.; if(initValue  != nullptr) { VecNorm(*initValue,NORM_2 ,&inorm);} return inorm; }
    PetscScalar getObjectInit(){return objectInit;}
    Vec* getInitialGuess(){return initValue;}
private:
    Mat* petscKG;
    Mat* petscConstrMat;
    Mat* condGMat;
    Vec* petscConstrVec;
    Vec* initValue;
    PetscScalar objectInit;
};

class DataForPetsc{

public:
    DataForPetsc(){}

    DataForPetsc( fem::matrix<double>* _KG, fem::matrix<double>* _constrMat, std::valarray<double>* _constrVec,
        PetscErrorCode (*_objAndGradFunc)(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx),
        PetscErrorCode (*_hessianFunc)(Tao tao, Vec X, Mat H, Mat Hpre, void*),
        PetscErrorCode (*_constraintsFunc)(Tao tao, Vec X, Vec ce, void* ctx),
        PetscErrorCode (*_constraintsGradFunc)(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx)
        );

    PetscErrorCode valarrayToVec(const std::valarray<double>&val, Vec** res);
    PetscErrorCode valarrayToVec(const std::valarray<double>&val, Vec& res);
    PetscErrorCode VecToValarray(const Vec& vec, std::valarray<double>& res);
    PetscErrorCode matrixToPetscMat(const fem::matrix<double>& mat, Mat** res);
    PetscErrorCode matrixToPetscMat(const fem::matrix<double>& mat, Mat& res);

    PetscErrorCode valarrayToVecG(const std::valarray<double>&val, const Mat& G, Vec& res);
    PetscErrorCode matrixToPetscMatG(const fem::matrix<double>& mat, const Mat& K, Mat** res);

    PetscErrorCode init(const fem::matrix<double>& KG,  const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                              Mat* _petscKG, Mat* _petscConstrMat, Vec& _petscConstrVec);

    PetscErrorCode createCondMat(const Mat& petscConstrMat, Mat* condMat){
        PetscInt nG, ri, rf;
        PetscInt nnz;
        /*const *///PetscScalar cols* ;
        const PetscScalar* vals;
        PetscScalar valr=0.; // To contain the norm of a line

        Vec diagVec;
        //Mat diagMat;
       // MPI_Comm matCom;

        PetscFunctionBegin;

        PetscBarrier((PetscObject)petscConstrMat);
        MatGetSize(/***/petscConstrMat, &nG, NULL);

        VecCreate(PETSC_COMM_WORLD, &diagVec);
        VecSetSizes(diagVec, PETSC_DECIDE, nG);
        VecSetFromOptions(diagVec);
        VecSetUp(diagVec);

        MatGetOwnershipRange(/***/petscConstrMat, &ri, &rf);
        for(PetscInt r=ri; r<rf; r++){
            MatGetRow(/***/petscConstrMat, r, &nnz, NULL, &vals);
                        valr = 0;
            for(PetscInt i=0; i<nnz; i++) valr += vals[i]*vals[i];
            MatRestoreRow(/***/petscConstrMat, r, &nnz, NULL, &vals);
            if(valr==0.) std::cout<<"Having a problem with the matrix G"<<std::endl;
            //valr =1./std::sqrt(valr);
            valr =1./PetscSqrtReal(valr);
            VecSetValue(diagVec, r, valr, INSERT_VALUES);
        }
        VecAssemblyBegin(diagVec);
        VecAssemblyEnd(diagVec);

        //MatCreateDiagonal(diagVec, &diagMat); //condGMat
        MatCreateDiagonal(diagVec, condMat); //condGMat
        MatConvert(*condMat, MATMPIAIJ, MAT_INPLACE_MATRIX, condMat); //Enabling further multiplication with matrix of type MATMPIAIJ

        MatCreate(PETSC_COMM_WORLD, condMat);
        MatSetSizes(*condMat, PETSC_DECIDE, PETSC_DECIDE, nG, nG);
        MatSetFromOptions(*condMat);
        MatSetUp(*condMat);
      //MatISSetPreallocation(*condMat, nnz, )
        const PetscScalar* vecVals; VecGetArrayRead(diagVec, &vecVals);
        for(PetscInt i=0; i<nG; i++) MatSetValue(*condMat, i, i, *(vecVals+i), INSERT_VALUES);
        VecRestoreArrayRead(diagVec, &vecVals);

        MatAssemblyBegin(*condMat, MAT_FINAL_ASSEMBLY);
        MatAssemblyEnd(*condMat, MAT_FINAL_ASSEMBLY);

        VecDestroy(&diagVec);
        PetscBarrier( (PetscObject)petscConstrMat ); //likely to be removed
        PetscFunctionReturn(PETSC_SUCCESS);
    }

  PetscErrorCode jacobiCond(Mat& _petscKG, Mat& _petscConstrMat, Vec& _petscConstrVec, Vec* _DInv, PetscScalar& lmax );

   PetscErrorCode petscSolve(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
        const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
        /*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec,*/ std::valarray<double>& sol);

    PetscErrorCode petscSolve2(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                              const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                              /*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec,*/ std::valarray<double>& sol,   const std::valarray<size_t>& numPbc, const fem::BCs& bc);

   PetscErrorCode petscSolveLinear(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                              const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                              /*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec,*/ std::valarray<double>& sol,   const std::valarray<size_t>& numPbc, const fem::BCs& bc);

    PetscErrorCode petscSolveLinearRed(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                    const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                    /*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec,*/ std::valarray<double>& sol,   const std::valarray<size_t>& numPbc, const fem::BCs& bc);


    PetscErrorCode petscSolveLinearRedMumps(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                       const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                       /*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec,*/ std::valarray<double>& sol,   const std::valarray<size_t>& numPbc, const fem::BCs& bc);

    PetscErrorCode petscSolveLinearRedMumpsCond(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
        const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec, const std::valarray<size_t>& numPbc, const fem::BCs& bc,
            std::valarray<double>& sol, std::valarray<double>& lambda);

    PetscErrorCode petscSolveLinearRedMumpsCond2(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                                const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec, const std::valarray<size_t>& numPbc, const fem::BCs& bc,
                                                std::valarray<double>& sol, std::valarray<double>& lambda);

    PetscErrorCode petscSolveLinear2(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                                    const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                    /*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec,*/ std::valarray<double>& sol,   const std::valarray<size_t>& numPbc, const fem::BCs& bc);

    PetscErrorCode kuuTest(const MeshPbc<double,3>& Mh, size_t ndof, /*const fem::matrix<double>& Kuu*/const Mat& petscKuu,
                                         const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                         std::valarray<double>& sol, const std::valarray<size_t>& numPbc, const fem::BCs& bc){


        // PetscInt lt=solveEigen(KGTest, EPS_LARGEST_REAL);
        //Mat petscKuu;

        PetscFunctionBeginUser;

        PetscInt hh=solveEigen(petscKuu, EPS_LARGEST_REAL);
        PetscBarrier(PetscObject(petscKuu));
        PetscScalar ll=solveEigen(petscKuu, EPS_SMALLEST_REAL);

        std::cout<< "petscKuu lambda_max : "<< hh<<std::endl
                  <<"petscKuu lambda_min : "<<ll <<std::endl;
        std::cout<<"condition number : "<<hh/ll<<std::endl;
        std::cout<<"condtion number : "<<hh/ll<<std::endl;

        PetscBarrier(PetscObject(petscKuu));
        //Testing the conditionning of petscKuu
        Vec x, b, xp; PetscScalar val;
        KSP ksp; PC pc;

        MatCreateVecs(petscKuu, &x, &b);
        VecSet(x, 1.0); VecDuplicate(x, &xp); VecSet(xp, 1.0);
        MatMult(petscKuu, x, b);
        VecSet(x, 0.0);
        KSPCreate(PETSC_COMM_WORLD, &ksp);
        KSPSetOperators(ksp, petscKuu, petscKuu);
        KSPSetType(ksp, KSPPREONLY);
        KSPGetPC(ksp, &pc);
        PCSetType(pc, PCLU);
        KSPSolve(ksp, b, x);
        VecAXPY(xp, -1, x);
        VecNorm(xp, NORM_2, &val);
        std::cout<<"pestcKuu test error on x=1 : "<<val<<std::endl;
        KSPDestroy(&ksp);

        std::ofstream ofs("/home/ahose/Desktop/testpetsckuu.dat", std::ios::out|std::ios::app);
        PetscInt rl, rh; const PetscScalar* valx;
        VecGetOwnershipRange(x, &rl, &rh);
        VecGetArrayRead(x, &valx);
        for(PetscInt i=0; i<(rh-rl); i++) ofs<<double(valx[i-rl])<<"\n";
        ofs.close();
        VecRestoreArrayRead(x, &valx);
        PetscFunctionReturn(PETSC_SUCCESS);
    }

//   Mat* getRigidityMat(){return petscKG;}
//   Mat* getConstraintsMat(){return petscConstrMat;}
//   Vec* getConstraintsVec(){return petscConstrVec;}
 private:
    //PetscErrorCode (*objAndGradFunc)(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx);
   // PetscErrorCode (*hessianFunc)(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx);
   // PetscErrorCode (*constraintsFunc)(Tao tao, Vec X, Vec ce, void* ctx);
   // PetscErrorCode (*constraintsGradFunc)(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx);


//   fem::matrix<double>* KG;
//   fem::matrix<double>* constrMat;
//   std::valarray<double>* constrVec;

//   Mat* petscKG;
//   Mat* petscConstrMat;
//   Vec* petscConstrVec;

};

//typedef struct{
//    PetscReal V0=10.;} AppCtx;

////Function for computing the objective function and the gradient
//PetscErrorCode objAndGradFunc(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx){
//    const PetscReal* Xr;
//    PetscReal* gr;
//    PetscReal Pi =3.1415;
//    PetscMPIInt rank;

//    PetscFunctionBeginUser;


//    VecGetArrayRead(X, &Xr);
//    VecGetArray(g, &gr);

//    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

//    if(rank==0){

//        *f = 2*PETSC_PI*Xr[0]*(Xr[0]+Xr[1]);

//        //VecSetSizes(g, PETSC_DECIDE, 2);
//        //VecSetUp(g);
//        gr[0] = 2*PETSC_PI*(2*Xr[0]+Xr[1]);
//        gr[1] = 2*PETSC_PI*Xr[0];}

//    VecRestoreArrayRead(X, &Xr);
//    VecRestoreArray(g, &gr);
//    PetscFunctionReturn(PETSC_SUCCESS);
//}

////Function for computing constraints

//PetscErrorCode constraintsFunc(Tao tao, Vec X, Vec ce, void* ctx){

//    const PetscReal *Xr;
//    PetscReal* cer;
//    PetscReal V0;
//    AppCtx* user;
//    PetscMPIInt rank;

//    PetscFunctionBeginUser;

//    VecGetArrayRead(X, &Xr);
//    VecGetArray(ce, &cer);
//    user =(AppCtx*)ctx;
//    V0=user->V0;

//    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
//    if(rank==0){
//        //cer[0]=PETSC_PI*(Xr[0]*Xr[0])*Xr[1] -V0;
//        *cer=PETSC_PI*(Xr[0]*Xr[0])*Xr[1] -V0;
//    }

//    VecRestoreArrayRead(X, &Xr);
//    VecRestoreArray(ce, &cer);
//    PetscFunctionReturn(PETSC_SUCCESS);
//}


////Function for computing constraints gradient
//PetscErrorCode constraintsGradFunc(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx){

//    PetscInt cols[2]; PetscReal vals[2];
//    PetscInt Jrows;

//    const PetscReal *Xr;
//    AppCtx* user;
//    PetscMPIInt rank;

//    PetscFunctionBeginUser;

//    VecGetArrayRead(X, &Xr);
//    user =(AppCtx*)ctx;

//    Jrows=0;

//    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
//    if(rank==0){
//        cols[0]=0; cols[1]=1;
//        vals[0]=2*PETSC_PI*Xr[0]*Xr[1];
//        vals[1]=PETSC_PI*Xr[0]*Xr[0];
//        MatSetValues(J, 1, &Jrows, 2, cols, vals, INSERT_VALUES);
//    }

//    VecRestoreArrayRead(X, &Xr);

//    MatAssemblyBegin(J, MAT_FINAL_ASSEMBLY);
//    MatAssemblyEnd(J, MAT_FINAL_ASSEMBLY);

//    PetscFunctionReturn(PETSC_SUCCESS);
//}

//int testMain(int argc, char** argv){
//    Mat A;
//    Vec b, x;
//    Vec X;
//    Tao tao;
//    AppCtx user;

//    PetscInitialize(&argc,  &argv, NULL, NULL);

//    MatCreate(PETSC_COMM_WORLD, &A);
//    MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, 1, 2);
//    MatSetFromOptions(A);
//    MatSetUp(A);

//    Vec EqC;
//    VecCreate(PETSC_COMM_WORLD, &EqC);
//    VecSetSizes(EqC, PETSC_DECIDE, 1);
//    VecSetFromOptions(EqC);
//    VecSetUp(EqC);

//    VecCreate(PETSC_COMM_WORLD, &X);
//    VecSetSizes(X, PETSC_DECIDE, 2);
//    VecSetFromOptions(X);
//    VecSetUp(X);
//    VecSet(X, 2500.); //The initial value is important

//    PetscReal* Xr;
//    VecGetArray(X, &Xr);
//    cout<<"X :"<<Xr[0]<<" "<<Xr[1]<<std::endl;
//    VecRestoreArray(X, &Xr);
//    cout<<"PETSC_PI : "<<PETSC_PI<<std::endl;

//    TaoCreate(PETSC_COMM_WORLD, &tao);
//    TaoSetType(tao, /*TAOLMVM*/TAOALMM);


//    TaoSetSolution(tao,X);
//    //TaoSetInitialVector(tao,X);
//    //taoSetUp(tao);
//    TaoSetFromOptions(tao);
//    TaoSetObjectiveAndGradient(tao, NULL, objAndGradFunc, (void*)&user);
//    TaoSetEqualityConstraintsRoutine(tao, EqC, constraintsFunc , (void*)&user);
//    TaoSetJacobianEqualityRoutine(tao, A, A, constraintsGradFunc, (void*)&user);
//    TaoSetTolerances(tao, 1.e-5, 0.0, 0.0);
//    TaoSetConstraintTolerances(tao, 1.e-5, 0.0);
//    TaoSetMaximumFunctionEvaluations(tao, 1e6);
//    Vec Xl, Xu;
//    VecDuplicate(X, &Xl);
//    VecDuplicate(X, &Xu);
//    VecSet(Xl, 0.0001);
//    VecSet(Xu, 100000.);
//    TaoSetVariableBounds(tao, Xl, Xu);

//    TaoSetUp(tao);
//    TaoSolve(tao);
//    //TaoGetSolution(tao, &X);
//    VecGetArray(X, &Xr);
//    cout<<"Sol : "<<Xr[0]<<" "<<Xr[1]<<std::endl;
//    cout<<"Surf : "<<2*PETSC_PI*Xr[0]*(Xr[0]+Xr[1])<<std::endl;
//    cout<<"Vol : "<<PETSC_PI*Xr[0]*Xr[0]*Xr[1]<<std::endl;
//    VecRestoreArray(X, &Xr);

//    MatDestroy(&A);
//    return PetscFinalize();
//}

} // namespace models
} // namespace scaletag

#endif // SCALETAG_MODELS_PETSCUTILITES_H
