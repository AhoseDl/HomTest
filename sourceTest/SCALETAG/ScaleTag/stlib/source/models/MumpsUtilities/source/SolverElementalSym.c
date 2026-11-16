#include "../includes/SolverElementalSym.h"

void MumpsElementalSymSolver(int n, int nelt, int lengthEltVar, int NA_Elt, int *eltptr, int* eltvar, double* a_elt, double* rhs){
/*
n - order of the matrix
nelt - number of elemental matrices
lengthEltVar - number of variables in elemental matrices
NA_Elt - number of values in elemental matrices
eltptr[]- contains indices of elemental matrices
eltvar[] - contains elemental variables indices
a_elt[] - contains elemental matrices values
rhs[] - right hand side vector
sol[] - contains the solution
*/

// one may know that the solution is computed in rhs bys mumps. We choose to copy the solution in sol.

DMUMPS_STRUC_C id;

   /* Initialize a MUMPS instance. Use MPI_COMM_WORLD */
  //id.comm_fortran=USE_COMM_WORLD;
  id.par=1; id.sym=2;
  id.job=JOB_INIT;
  dmumps_c(&id);
      /* Define the problem on the host */
//sol=new double[n];
//  for( size_t i=0; i<n; i++) sol[i]=rhs[i];  // Mumps modifies the rhs by puting the solution in it. To avoid this, the rhs is copied in sol and passed to the mumps solver.
    id.n = n; id.nelt =nelt; id.eltptr=eltptr; id.eltvar=eltvar;
    id.a_elt = a_elt; id.rhs = rhs;
  
  
   #define ICNTL(I) icntl[(I)-1] /* macro s.t. indices match documentation */
  /* No outputs */
  id.ICNTL(1)=-1; id.ICNTL(2)=-1; id.ICNTL(3)=-1; id.ICNTL(4)=0;
  
  /*specify element entry*/
  id.ICNTL(5)=1;
  
  /* Call the MUMPS package (analyse, factorization and solve). */
  id.job=6;
  dmumps_c(&id);


  /* Terminate instance. */
  id.job=JOB_END;
  dmumps_c(&id);
 }

