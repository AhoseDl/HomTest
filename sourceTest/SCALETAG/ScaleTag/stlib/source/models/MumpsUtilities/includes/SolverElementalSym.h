#ifndef SOLVERELEMENTALSYM_H
#define SOLVERELEMENTALSYM_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>
#include "dmumps_c.h"
#define JOB_INIT -1
#define JOB_END -2

//void MumpsElementalSymSolver(int n, int nelt, int lengthEltVar, int NA_Elt, int* eltptr, int* eltvar, double* a_elt, double* rhs, double* sol);
void MumpsElementalSymSolver(int n, int nelt, int lengthEltVar, int NA_Elt, int* eltptr, int* eltvar, double* a_elt, double* rhs);

#ifdef __cplusplus
}
#endif

#endif
