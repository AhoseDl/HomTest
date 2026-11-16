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
//#include<petsc.h>
#include<matrix.h>
#include<MeshPbc.h>

namespace scaletag {
namespace models {

PetscErrorCode objAndGradFunc(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx);
PetscErrorCode hessianFunc(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx);
PetscErrorCode constraintsFunc(Tao tao, Vec X, Vec ce, void* ctx);
PetscErrorCode constraintsGradFunc(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx);

class DataForPetsc{

public:
    DataForPetsc(){}

    DataForPetsc( fem::matrix<double>* _KG, fem::matrix<double>* _constrMat, std::valarray<double>* _constrVec,
        PetscErrorCode (*_objAndGradFunc)(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx),
        PetscErrorCode (*_hessianFunc)(Tao tao, Vec X, Mat H, Mat Hpre, void*),
        PetscErrorCode (*_constraintsFunc)(Tao tao, Vec X, Vec ce, void* ctx),
        PetscErrorCode (*_constraintsGradFunc)(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx)
        );

   /*static*/ PetscErrorCode valarrayToVec(const std::valarray<double>&val, Vec* res);
   /*static*/ PetscErrorCode VecToValarray(const Vec& vec, std::valarray<double>& res);
   /*static*/ PetscErrorCode matrixToPetscMat(const fem::matrix<double>& mat, Mat* res);

     // PetscErrorCode init(/*Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec*/);
     static  PetscErrorCode init(Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec);

      Mat* getRigidityMat(){return &petscKG;}
      Mat* getConstraintsMat(){return &petscConstrMat;}
      Vec* getConstraintsVec(){return &petscConstrVec;}

      fem::matrix<double>* getStdConstraintsMat(){return constrMat;}


     PetscErrorCode petscSolve(int* argc, char*** argv,const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
                             const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
                                Mat* _petscKG, Mat* _petscConstrMat, Vec* _petscConstrVec, std::valarray<double>& sol);
//     static PetscErrorCode petscSolve2(const MeshPbc<double,3>& Mh, size_t ndof, const fem::matrix<double>& _KG,
//     const fem::matrix<double>& constrMat, const std::valarray<double>& constrVec,
//        Mat* petscKG, Mat* petscConstrMat, Vec* petscConstrVec, std::valarray<double>& sol);


 private:

    PetscErrorCode (*objAndGradFunc)(Tao tao, Vec X, PetscReal* f, Vec g, void* ctx);
    PetscErrorCode (*hessianFunc)(Tao tao, Vec X, Mat H, Mat Hpre, void* ctx);
    PetscErrorCode (*constraintsFunc)(Tao tao, Vec X, Vec ce, void* ctx);
    PetscErrorCode (*constraintsGradFunc)(Tao tao, Vec X, Mat J,  Mat Jpre,  void* ctx);

    fem::matrix<double>* KG;
    fem::matrix<double>* constrMat;
    std::valarray<double>* constrVec;

    Mat petscKG;
    Mat petscConstrMat;
    Vec petscConstrVec;
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
