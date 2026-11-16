#include<stdio.h>
#include<string.h>
#include<iostream>

#include<dmumps_c.h>
#include"../includes/FEMumps.h"
using namespace std;

int main(int argc, char**argv){

//************* Assembled Unsymmetric problem ************************  
/*int n=2;
int64_t nnz=2;
int irn[]= {1,2};
int jcn[] = {1,2};
double a[2];
double rhs[2];

  rhs[0]=1.0;rhs[1]=4.0;
  a[0]=1.0;a[1]=2.0;
     
double sol[n];
MumpsAssembledNonSymSolver(n, nnz,irn, jcn, a, rhs, sol);
printf("Solution is : (%8.2f  %8.2f)\n", sol[0], sol[1]);
printf("The rhs is : (%8.2f  %8.2f)\n", rhs[0], rhs[1]);*/


//************* Elemental Unsymmetric problem ************************ 

int En=5;
int nelt=2;
int lengthEltVar=6;
int NA_Elt=18;
int eltptr[]={1,4,7};
int eltvar[]={1,2,3,3,4,5};
double a_elt[]={-1.0, 2.0, 1.0, 2.0, 1.0, 1.0, 3.0, 1.0, 1.0, 2.0, 1.0, 3.0, -1.0, 2.0, 2.0, 3.0, -1.0, 1.0};
double Erhs[]={12,7,23,6,22};

//double Esol[En];
MumpsElementalNonSymSolver(En, nelt, lengthEltVar, NA_Elt, eltptr, eltvar, a_elt, Erhs/*, Esol*/);
//printf("Solution is : (%8.2f  %8.2f  %8.2f  %8.2f  %8.2f)\n", Esol[0], Esol[1], Esol[2], Esol[3], Esol[4]);
printf("The rhs is : (%8.2f  %8.2f  %8.2f  %8.2f  %8.2f)\n", Erhs[0], Erhs[1], Erhs[2], Erhs[3], Erhs[4]);

//************* Assembled symmetric problem ************************  
int n=3;
int64_t nnz=6;
int irn[]= {1,2,3,2,3,3};
int jcn[] = {1,1,1,2,2,3};
double a[6];
double rhs[3];

  rhs[0]=1.0;rhs[1]=4.0;rhs[2]=9.0;
  a[0]=1.0;a[1]=1.0;a[2]=1.0;a[3]=1.0; a[4]=1.0;a[5]=1.0;
     
//double sol[n];
MumpsAssembledNonSymSolver(n, nnz,irn, jcn, a, rhs/*, sol*/);
//printf("Solution is : (%8.2f  %8.2f  %8.2f)\n", sol[0], sol[1], sol[2]);
printf("The rhs is : (%8.2f  %8.2f  %8.2f)\n", rhs[0], rhs[1], rhs[2]);

//************* Elemental symmetric problem ************************ 

//the sol is : 0.2 0.5 0.1
	int tn=3; size_t tnnz=9; int* tirn; int* tjcn; double* ta; double* trhs; trhs=new double[tn];
    tirn=new int[tnnz]; tjcn=new int[tnnz]; ta=new double[tnnz];
    tirn[0]=1; tirn[1]=1; tirn[2]=1; tirn[3]=2; tirn[4]=2; tirn[5]=2; tirn[6]=3; tirn[7]=3; tirn[8]=3;
    tjcn[0]=1; tjcn[1]=2; tjcn[2]=3; tjcn[3]=1; tjcn[4]=2; tjcn[5]=3; tjcn[6]=1; tjcn[7]=2; tjcn[8]=3;
    ta[0]=1.0; ta[1]=2.0; ta[2]=3.0; ta[3]=4.0; ta[4]=5.0; ta[5]=6.0; ta[6]=7.0; ta[7]=8.0; ta[8]=10.0;
    trhs[0]=1.5; trhs[1]=3.9; trhs[2]=6.4;

    cout<<"rhs : "<<trhs[0]<<" "<<trhs[1]<<" "<<trhs[2]<<std::endl;
    MumpsAssembledNonSymSolver(3,9,tirn, tjcn, ta, trhs);
    cout<<"Test solution : "<<trhs[0]<<" "<<trhs[1]<<" "<<trhs[2]<<std::endl;


  return 0;
}
