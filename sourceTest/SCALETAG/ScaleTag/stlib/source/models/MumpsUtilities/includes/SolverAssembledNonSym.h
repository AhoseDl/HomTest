#ifndef SOLVERASSEMBLEDNONSYM_H
#define SOLVERASSEMBLEDNONSYM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include "dmumps_c.h"
#define JOB_INIT -1
#define JOB_END -2
//#define USE_COMM_WORLD -987654
//void MumpsAssembledNonSymSolver(int n, int64_t nnz, int *irn, int* jcn, double* a, double* rhs, double* sol);
void MumpsAssembledNonSymSolver(int n, int64_t nnz, int* irn, int* jcn, double* a, double* rhs);

#ifdef __cplusplus
}
#endif


#endif
