#ifndef GENERALSOLVER_H
#define GENERALSOLVER_H

//#include<stlib_global.h>
#include<matrix.h>
#include<valarray>
#include<MumpsUtilities/includes/FEMumps.h>
#include<algorithm>
#include<cmath>

//#include "/home/ahose/Desktop/SCALETAG/ScaleTag/stlib/source/models/MumpsUtilities/includes/SolverAssembledNonSym.h"
//#include "/home/ahose/Desktop/SCALETAG/ScaleTag/stlib/source/models/MumpsUtilities/includes/SolverElementalNonSym.h"
//#include "/home/ahose/Desktop/SCALETAG/ScaleTag/stlib/source/models/MumpsUtilities/includes/SolverAssembledSym.h"
//#include "/home/ahose/Desktop/SCALETAG/ScaleTag/stlib/source/models/MumpsUtilities/includes/SolverElementalSym.h"


namespace scaletag{
namespace fem{
//STLIB_EXPORT
template<typename T>
class GeneralSolver{

private:
std::valarray<T> sol;
//sol contains dirichlet dofs

public:
template<typename U> void solveMumpsAssembledNonSymMasq(const matrix<U>& mat, const std::valarray<U>& rhs, const std::valarray<bool> masqbar){


    //masq->valarray<bool> true for not dirichlet and false otherwise
    std::valarray<bool> masq(masqbar.size(),0);
    masq=(masqbar==0); //invert boolean values in masqbar; masqbar=1 if dic and 0 if not

    sol.resize(rhs.size(),0.);
    int n= mat.nrow();
    size_t nnz=mat.nnz();

   int* irn = new int[nnz]; int* jcn=new int[nnz]; U* a=new U[nnz];
    U* lrhs=new U[rhs[masq].size()]; //U* lsol=new U[rhs[masq].size()];

    std::valarray<U> val=mat.getVal();
    std::copy(std::begin(val),  std::end(val), a);

    std::valarray<size_t> idc=mat.getColumnsIndex();
        //idx.apply([](size_t n){return n+1;} );
    idc=idc+1;
    std::copy(std::begin(idc), std::end(idc), jcn);

    std::valarray<size_t> idr=mat.getLinesIndex();
        //idr.apply([](size_t n){return n+1;} );
    idr=idr+1;
    std::copy(std::begin(idr), std::end(idr), irn);

    //for(auto i=0; (size_t)i<rhs[masq].size(); i++) lrhs[i]=rhs[masq][i];
    std::valarray<U> rhsmasq(rhs[masq].size());
    rhsmasq=rhs[masq];
    lrhs=std::begin(rhsmasq);

    MumpsAssembledNonSymSolver(n, nnz, irn, jcn, a, lrhs/*, lsol*/); //lrhs is modified


    //for(size_t i=0; i<rhs[masq].size(); i++) sol[masq[i]]=lrhs[i];
    std::valarray<U> lrshmasq(rhs[masq].size());
   // std::begin(lrshmasq)=lrhs;
    std::copy(lrhs, lrhs+rhs[masq].size(),std::begin(lrshmasq));
    //sol[masq]=*lrhs;
    sol[masq]=lrshmasq;  //lrshmasq for non dirichlet
    sol[masqbar]=rhs[masqbar];  //for dirichlet
   // delete[]irn; delete[] jcn;
    //delete[] a; delete[] lrhs;
    //delete[] lsol;
    compareval(rhs,sol);

    }



    template<typename U> void solveMumpsAssembledNonSym(const matrix<U>& mat, const std::valarray<U>& rhs){


    sol.resize(rhs.size(),0.);
    int n= mat.nrow();
    size_t nnz=mat.nnz();

    int* irn = new int[nnz]; int* jcn=new int[nnz]; U* a=new U[nnz];
    //U* lrhs=new U[rhs[masq].size()];
    std::valarray<U> rhsTemp=rhs;

    std::valarray<U> val=mat.getVal();
    std::copy(std::begin(val),  std::end(val), a);

    std::valarray<size_t> idc=mat.getColumnsIndex();
    idc=idc+1;
    std::copy(std::begin(idc), std::end(idc), jcn);

    std::valarray<size_t> idr=mat.getLinesIndex();
    idr=idr+1;
    std::copy(std::begin(idr), std::end(idr), irn);

    MumpsAssembledNonSymSolver(n, nnz, irn, jcn, a, std::begin(rhsTemp)); //rhsTemp is modified

    sol=rhsTemp;
     delete[]irn; delete[] jcn;
    delete[] a;

    //compareval(rhs,sol);

    }

    const std::valarray<T>& getSolution()const{return sol;}

    void testSolver(){
    //sol  is : 0.2 0.5 0.1
    //rhs is : 1.5 3.9 6.4
    //irn is : 1 1 1 2 2 2 3 3 3
    //jcn is : 1 2 3 1 2 3 1 2 3
    //a is : 1 2 3 4 5 6 7 8 10
    int n=3; size_t nnz=9; int* irn; int* jcn; double* a; double* rhs; rhs=new double[n];
    irn=new int[nnz]; jcn=new int[nnz]; a=new double[nnz];
    irn[0]=1; irn[1]=1; irn[2]=1; irn[3]=2; irn[4]=2; irn[5]=2; irn[6]=3; irn[7]=3; irn[8]=3;
    jcn[0]=1; jcn[1]=2; jcn[2]=3; jcn[3]=1; jcn[4]=2; jcn[5]=3; jcn[6]=1; jcn[7]=2; jcn[8]=3;
    a[0]=1.0; a[1]=2.0; a[2]=3.0; a[3]=4.0; a[4]=5.0; a[5]=6.0; a[6]=7.0; a[7]=8.0; a[8]=10.0;
    rhs[0]=1.5; rhs[1]=3.9; rhs[2]=6.4;
    cout<<"rhs : "<<rhs[0]<<" "<<rhs[1]<<" "<<rhs[2]<<std::endl;
    MumpsAssembledNonSymSolver(n,nnz,irn, jcn, a, rhs);
    cout<<"Test solution : "<<rhs[0]<<" "<<rhs[1]<<" "<<rhs[2]<<std::endl;



    }


    bool compareval(valarray<double> val1, valarray<double> val2){
    cout<<"compare :"<<endl;
    for(size_t i=0; i<val1.size(); i++){
        if(fabs(val1[i]-val2[i])<1e-15)
        {if(i<val1.size()-1)continue;
            cout<<"true i : "<<i<<" "<<fabs(val1[i]-val2[i])<<endl;
            return true;
        }
        else
        { cout<<"false i : "<<i<<" "<<fabs(val1[i]-val2[i])<<endl;
            return false;
        }

    }
    return true;
    }


};


}}
#endif
