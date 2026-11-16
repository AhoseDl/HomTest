#include "../includes/SolverAssembledSym.h"

void MumpsAssembledSymSolver(int n, int64_t nnz, int* irn, int* jcn, double* a, double* rhs){
/*
n - order of the matrix
nnz - number of nonzeros in A
irn - contains row indices
jrn - contains colum indices
a - values in the matix
rhs - right hand side vector
sol - contains the solution
*/

// one may know that the solution is computed in rhs bys mumps. We choose to copy the solution in sol.

DMUMPS_STRUC_C id;
  //id.comm_fortran=USE_COMM_WORLD;
  id.par=1; id.sym=2;
  id.job=JOB_INIT;
  dmumps_c(&id);
  
/* Define the problem on the host */

//sol=new double[n];
 // for( size_t i=0; i<n; i++) sol[i]=rhs[i];  // Mumps modifies the rhs by puting the solution in it. To avoid this, the rhs is copied in sol and passed to the mumps solver.
 // if (myid == 0) {
    id.n = n; id.nnz =nnz; id.irn=irn; id.jcn=jcn;
    id.a = a; id.rhs = rhs;
 // }
  
   #define ICNTL(I) icntl[(I)-1] /* macro s.t. indices match documentation */
  /* No outputs */
  id.ICNTL(1)=-1; id.ICNTL(2)=-1; id.ICNTL(3)=-1; id.ICNTL(4)=0;
  
  
  /* Call the MUMPS package (analyse, factorization and solve). */
  id.job=6;
  dmumps_c(&id);
  /* Terminate instance. */
  id.job=JOB_END;
  dmumps_c(&id);
 }
