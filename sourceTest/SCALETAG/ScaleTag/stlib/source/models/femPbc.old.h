#ifndef SCALETAGFEMPBC_H
#define SCALETAGFEMPBC_H

#include<matrix.h>
#include<MeshPbc.h>
#include<list>
#include<valarray>
#include<pbcutilities.h>
#include<bcs.h>
#include<fem.h>
#include<solver.h>
#include<iostream>
#include<algorithm>

#include<petscutilities.h>

namespace scaletag{
namespace fem{

//*********************************unique_insert
template <typename T>
void unique_insert (std::list<T>& L, const T& j) ;

//********************************* renumbering *************************************************
template<typename T, size_t D>
void renumbering(const MeshPbc<T,D>& Th, std::valarray<size_t>& num, size_t& _icount, size_t ndof,const BCs& bc);

template<typename T, size_t D>
void renumberingPbc(const MeshPbc<T,D>& Mh, std::valarray<size_t>& numPbc, size_t& _iNotDir, size_t ndof, BCs& bc,
                    std::valarray<bool>& masqDir, std::valarray<bool>& masqTied, std::valarray<bool>& masqRet,
                    std::valarray<bool>& masqNotConstrained){

    /*
     * This function renumbers dofs in the order used in fem computation.
    original numbering ➡(0 to ndof*nnode-1)
    final numbering in numPbc ➡ (it depends ; min=0; max=ndof*nnode-1)*
    It is supposed that "bc.setPbc() - fills param. tiedDofs and retainedDofs in bc ..."
    is previously invoked and Dirichlets boundary conditions set
    ndof is the degree of freedom per node

One may note that :
 a dof can be both retained and dirichlet
a dof can't be both tied  and dirichlet
a dof can't be both tied and retained
The author chooses to order the dofs in numPbc in the order : {ncstr, tied, ret&NotDir, ret&Dir, NotRet&Dir}
*/

    numPbc.resize(ndof*Mh.getNumberOfNodes(), 0);
    masqDir.resize(ndof*Mh.getNumberOfNodes(), 0);
    masqTied.resize(ndof*Mh.getNumberOfNodes(), 0);
    masqRet.resize(ndof*Mh.getNumberOfNodes(), 0);
    masqNotConstrained.resize(ndof*Mh.getNumberOfNodes(),0);

    size_t iDir = 0, iRet = 0, iTied=0, iNotCstr=0; size_t ik=0;
    size_t nnode=Mh.getNumberOfNodes();

{//    for(size_t knode=0; knode<nnode; knode++){
//        for (size_t kndof = 0; kndof < ndof; kndof++){

//            ik=ndof*knode+kndof;
            //if (bc.isDirichlet(ik) /*&& !bc.isRetained(ik) && !bc.isTied(ik)*/){numPbc[ik] = iDir++; masqDir[ik]=1; continue;}
            //else
//            if (bc.isRetained(ik) /*&& !bc.isDirichlet(ik) && !bc.isTied(ik)*/) {numPbc[ik] = iRet++; masqRet[ik]=1; continue;}
            //else
//            if (bc.isTied(ik) /*&& !bc.isDirichlet(ik) && !bc.isRetained(ik)*/){numPbc[ik] = iTied++; masqTied[ik]=1; continue;}
            //if( !bc.isDirichlet(ik) && !bc.isTied(ik) && !bc.isRetained(ik))
//            {numPbc[ik] = iNotCstr++; masqNotConstrained[ik]=1;}
 //       }}
}
    size_t iRetAndDir=0, iRetAndNDir=0, iNRetAndDir=0;
    for(size_t ik=0; ik<ndof*nnode; ik++){

        if(bc.isRetained(ik)||bc.isDirichlet(ik)){
            if( bc.isRetained(ik)&&(!bc.isDirichlet(ik)) ){numPbc[ik] =iRetAndNDir++; iRet++; masqRet[ik]=1; continue;}
            if( bc.isRetained(ik)&&(bc.isDirichlet(ik)) ){numPbc[ik] =iRetAndDir++; iRet++; iDir++; masqRet[ik]=1; masqDir[ik]=1; continue;}
            if( !bc.isRetained(ik)&&(bc.isDirichlet(ik)) ){numPbc[ik] =iNRetAndDir++; iDir++; masqDir[ik]=1; continue;}
           // continue;
        }

        if(bc.isTied(ik)){ numPbc[ik] = iTied++; masqTied[ik]=1; continue; }

        numPbc[ik] = iNotCstr++; masqNotConstrained[ik]=1;
    }
numPbc[masqTied]=std::valarray<size_t>(numPbc[masqTied])+iNotCstr;
numPbc[masqRet&&(masqDir==0)]=std::valarray<size_t>(numPbc[masqRet&&(masqDir==0)])+iNotCstr+iTied;
numPbc[masqRet&&masqDir]=std::valarray<size_t>(numPbc[masqRet&&masqDir])+iNotCstr+iTied+iRetAndNDir;
numPbc[masqDir&&(masqRet==0)]=std::valarray<size_t>(numPbc[masqDir&&(masqRet==0)])+iNotCstr+iTied+iRetAndNDir+iRetAndDir;
_iNotDir=ndof*Mh.getNumberOfNodes()-bc.getDirichletDofs().size();
bc.actualizePbc(iDir, iRet, iTied, iNotCstr, iRetAndDir, iRetAndNDir, iNRetAndDir);


{//    numPbc[masqRet]=std::valarray<size_t>(numPbc[masqRet])+iNotCstr;
//    numPbc[masqTied]=std::valarray<size_t>(numPbc[masqTied])+iNotCstr+iRet;
    //numPbc[masqDir]=std::valarray<size_t>(numPbc[masqDir])+iNotCstr+iRet+iTied;
    //_iNotDir=ndof*Mh.getNumberOfNodes()-iDir; //_iNotDir is the number of dofs where dirichlet BCs are not applied
//    _iNotDir=ndof*Mh.getNumberOfNodes()-bc.getDirichletDofs().size(); //_iNotDir is the number of dofs where dirichlet BCs are not applied

//    std::valarray<size_t> num(ndof*Mh.getNumberOfNodes());
//    for(size_t p=0; p<ndof*Mh.getNumberOfNodes(); p++) num[p]=p;
//    std::valarray<size_t> valTiedTemp = std::valarray<size_t>(num[masqTied]);
//    std::valarray<size_t> valRetTemp=std::valarray<size_t>(num[masqRet]);//
//    std::vector<size_t> retDofs(std::begin(valRetTemp),std::end(valRetTemp));
//    std::vector<size_t> tiedDofs(std::begin(valTiedTemp),std::end(valTiedTemp));
//    bc.actualizePbc(tiedDofs, retDofs);

    //for(size_t i=0; i<bc.getDirichletDofs().size(); i++) masqDir[bc.getDirichletDofs()[i]]=1;

   // bc.setRenumRetNumber(std::valarray(masqRet[masqRet]).size());
   // bc.setRenumTiedNumber(std::valarray(masqTied[masqTied]).size());
}

}


//***************************** assemble dirichlet related elements in the overall matrix ***************************
template <typename T, size_t D>
void dimMatPbcDir(const MeshPbc<T,D>& Mh, const std::valarray<size_t>& num, size_t ndof,const BCs& bc,
                  matrix<T>& a4, matrix<T>& a14,matrix<T>& a24,matrix<T>& a34)
{
    /*
     * a4={a41, a42, a43, a44}
     * This function dimensions a4, a14, a24, a34;
     * This function is used in fem::assemblyPbc
     * */
    std::vector<std::list<size_t> > s4, s14, s24, s34;
    size_t nDir=bc.getDirNumber();  // dof number where dirichlet is applied
    size_t nRet=bc.getRetainedNumber();
    size_t nTied=bc.getTiedNumber();
    size_t nNotCstr=ndof*Mh.getNumberOfNodes()-nDir-nTied-nRet;
    size_t nNotDir=ndof*Mh.getNumberOfNodes()-nDir;
    size_t dirOffset=ndof*Mh.getNumberOfNodes()-bc.getDirNumber();
    s4.resize(nDir);
    s14.resize(nNotCstr);  s24.resize(nRet);  s34.resize(nTied);
    for (size_t e = 0; e < Mh.getNumberOfElements(); e++){
        size_t nv = Mh.getElements()[e].getElementSize();

        for (size_t ie = 0; ie < nv; ie++){
            for (size_t kie = 0; kie < ndof; kie++){
                size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;

                if (bc.isDirichlet(i)) //dimension s41, s42, s43, s44 eq s4
                {
                     unique_insert(s4[num[i]-dirOffset], num[i]);
                    for (size_t je = 0; je < nv; je++) {
                    for (size_t kje = 0; kje < ndof; kje++){
                        size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                        if (j == i ) continue;
                        unique_insert(s4[ num[i]-dirOffset ], num[j]);}
                    }}
                else //dimension s14, s24, s34; they are all 3 off-diagonal
                    for (size_t je = 0; je < nv; je++) {
                        {
                        for (size_t kje = 0; kje < ndof; kje++){
                            size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;

                            if ( (num[i]<nNotCstr) && (num[j] >= nNotDir) )
                            {unique_insert(s14[ num[i] ], num[j]-nNotDir); continue;}

                            if (num[i]>=nNotCstr && num[i]<(nNotCstr+nRet) && num[j]>=nNotDir)
                            {unique_insert(s24[ num[i] - nNotCstr], num[j]-nNotDir); continue;}

                            if (num[i]>=(nNotCstr+nRet) && num[i]<(nNotCstr+nRet+nTied) && num[j]>=nNotDir)
                            {unique_insert(s34[ num[i]-(nNotCstr+nRet) ], num[j]-nNotDir); continue;}
                        }
                    }

                }
            }

        }
    }
    a4.resize (s4, ndof*Mh.getNumberOfNodes());
    a14.resize(s14,nDir); a24.resize(s24, nDir); a34.resize(s34,nDir);
}

template <typename T, size_t D>
void fillMatPbcDir(const MeshPbc<T,D>& Mh,  const std::valarray<size_t>& num, size_t ndof, const BCs& bc,
                   T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
                   matrix<T>& a4, matrix<T>& a14, matrix<T>& a24, matrix<T>& a34 )
{
    /*
     * a4={a41, a42, a43, a44}
     * This function fills a4, a14, a24, a34;
     *This function is used in fem::assemblyPbc
     * */

    size_t nDir=bc.getDirNumber();  // dof number where dirichlet is applied
    size_t nRet=bc.getRetainedNumber();
    size_t nTied=bc.getTiedNumber();
    size_t nNotCstr=ndof*Mh.getNumberOfNodes()-nDir-nTied-nRet;
    size_t nNotDir=ndof*Mh.getNumberOfNodes()-nDir;

    size_t dirOffset=ndof*Mh.getNumberOfNodes()-bc.getDirNumber();
    for (size_t e = 0; e < Mh.getNumberOfElements(); e++) {
        size_t nv=Mh.getElements()[e].getElementSize();

        for (size_t ie = 0; ie < nv; ie++) {
            for (size_t kie = 0; kie < ndof; kie++){
                size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
                if (bc.isDirichlet(i)) //Fill (K41, K42, K43, K44) eq a4
                {
                a4.at(num[i]-dirOffset,num[i])   += matrixValue(Mh, e, ie, ie,kie, kie/*, ndof*/);
                for (size_t je = 0; je < nv; je++) {
                for (size_t kje = 0; kje < ndof; kje++){
                    size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                    if (j == i ) continue;
                    a4.at(num[i]-dirOffset,num[j]) += matrixValue(Mh, e, ie, je,kie,kje/*, ndof*/);}
                }}
                else//fill K14, K24, K34; they are all 3 off-diagonal
                for (size_t je = 0; je < nv; je++) {
                    {
                        for (size_t kje = 0; kje < ndof; kje++){
                            size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                            if (num[i]<nNotCstr && num[j]>=nNotDir) {a14.at(num[i], num[j]-nNotDir) += matrixValue(Mh, e, ie, je,kie,kje) ;continue;}
                            if ( num[i]>=nNotCstr && num[i]<(nNotCstr+nRet) && num[j]>=nNotDir) {a24.at(num[i] - nNotCstr, num[j]-nNotDir) += matrixValue(Mh, e, ie, je,kie,kje);continue;}
                            if (num[i]>=(nNotCstr+nRet) && num[i]<(nNotCstr+nRet+nTied) && num[j]>=nNotDir) {a34.at (num[i]-(nNotCstr+nRet) , num[j]-nNotDir) += matrixValue(Mh, e, ie, je,kie,kje);continue;}
                        }
                    }

                }
            }
        }
    }
}

//********************************* AssemblyPbc ***********************************
template<typename T,size_t D>
void assemblyPbc(const MeshPbc<T,D>& Mh, const std::valarray<size_t>& numPbc, const matrix<T>& matGal,
const matrix<T>& constrMat, const std::valarray<T>& constrVec, size_t ndof,const BCs& bc,
 T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
                 matrix<T>& a4, matrix<T>& redMat, std::valarray<T>& redRhs, const std::valarray<bool>& masqDir){

//This function computes the reduced matrix redMat and reduced rhs redRhs for taking in acounts pbc;
//This function also exposes a4={a41,a42,a43,a44} which helps later in computing nodal forces Fd

    std::vector<std::list<size_t> > s11,s12,s13,s21,s22,s23,s31,s32,s33;
    matrix<T> a14, a24, a34;

    size_t nColGal;
    size_t sl1=ndof*Mh.getNumberOfNodes()-bc.getDirNumber()-bc.getTiedNumber()-bc.getRetainedNumber(),
    sl2=bc.getRetainedNumber(), sl3=bc.getTiedNumber();

    std::vector<std::list<size_t> > s; //Structure of the galerkin matrix
    matGal.getRowStruct(s, nColGal);

    //splitting the galerkin matrix

    std::vector<std::list<size_t> > s1, s2, s3;
    s1.assign(s.begin(), s.begin()+sl1);
    s2.assign(s.begin()+sl1, s.begin()+sl1+sl2);
    s3.assign(s.begin()+sl1+sl2, s.end());

    s11.resize(sl1); s12.resize(sl1); s13.resize(sl1);
    s21.resize(sl2); s22.resize(sl2); s23.resize(sl2);
    s31.resize(sl3); s32.resize(sl3); s33.resize(sl3);

    // These predicates help splitting the 3 rows s1, s2, s3 to s11, s12, ..., s33
    auto pred1=[sl1](size_t i){ if(i>=sl1) return true; return false;};
    auto pred2=[sl1, sl2](size_t i){ if( i>=(sl1+sl2) ) return true; return false;};
    std::list<size_t>::iterator it1, it2;

    for(size_t i=0; i<sl1; i++){
    it1=std::find_if(s1[i].begin(), s1[i].end(),pred1);
    it2=std::find_if(it1, s1[i].end(),pred2);
    s11[i].assign(s1[i].begin(),it1);
    s12[i].assign(it1,it2);
    s13[i].assign(it2,s1[i].end());
    }
    for(size_t i=0; i<sl2; i++){
    it1=std::find_if(s2[i].begin(), s2[i].end(),pred1);
    it2=std::find_if(it1, s2[i].end(),pred2);
    s21[i].assign(s2[i].begin(),it1);
    s22[i].assign(it1,it2);
    s23[i].assign(it2,s2[i].end());
    }
    for(size_t i=0; i<( s.size()-(sl1+sl2) ); i++){
    it1=std::find_if(s3[i].begin(), s3[i].end(),pred1);
    it2=std::find_if(it1, s3[i].end(),pred2);
    s31[i].assign(s3[i].begin(),it1);
    s32[i].assign(it1,it2);
    s33[i].assign(it2,s3[i].end());
    }
 //The 2 functions minus1 and minus 2 help the columns indexes in s11, s12, ..., s33 start from 0
    auto minus1=[sl1](size_t& i){return i-=sl1;};
    auto minus2=[sl1,sl2](size_t& i){return i-=(sl1+sl2);};

    for(size_t i=0; i<sl1; i++){
            std::for_each(s12[i].begin(), s12[i].end(), minus1);
            std::for_each(s13[i].begin(), s13[i].end(), minus2);
        }
    for(size_t i=0; i<sl2; i++){
            std::for_each(s22[i].begin(), s22[i].end(), minus1);
            std::for_each(s23[i].begin(), s23[i].end(), minus2);
    }
    for(size_t i=0; i<sl3; i++){
            std::for_each(s32[i].begin(), s32[i].end(), minus1);
            std::for_each(s33[i].begin(), s33[i].end(), minus2);
    }

    matrix<T> K11, K12, K13, K21, K22, K23, K31, K32, K33;
    K11.resize(s11,sl1); K21.resize(s21,sl1); K31.resize(s31,sl1);
    K12.resize(s12,sl2); K22.resize(s22,sl2); K32.resize(s32,sl2);
    K13.resize(s13,sl3); K23.resize(s23,sl3); K33.resize(s33,sl3);

    //Filling K11, K12, ..., K33 from matGal
    for(size_t i=0; i<s11.size(); i++) for(auto j=s11[i].begin(); j!=s11[i].end(); j++)
            K11.at(i,*j) = matGal(i,*j);

    for(size_t i=0; i<s12.size(); i++) for(auto j=s12[i].begin(); j!=s12[i].end(); j++)
            K12.at(i,*j) = matGal(i,*j + sl1);

    for(size_t i=0; i<s13.size(); i++) for(auto j=s13[i].begin(); j!=s13[i].end(); j++)
            K13.at(i,*j) = matGal(i,*j + sl1+sl2);
    //****************************************
    for(size_t i=0; i<s21.size(); i++) for(auto j=s21[i].begin(); j!=s21[i].end(); j++)
            K21.at(i,*j) = matGal(i+sl1,*j);

    for(size_t i=0; i<s22.size(); i++) for(auto j=s22[i].begin(); j!=s22[i].end(); j++)
            K22.at(i,*j) = matGal(i+sl1,*j + sl1);

    for(size_t i=0; i<s23.size(); i++) for(auto j=s23[i].begin(); j!=s23[i].end(); j++)
            K23.at(i,*j) = matGal(i+sl1,*j + sl1+sl2);
    //******************************************
    for(size_t i=0; i<s31.size(); i++) for(auto j=s31[i].begin(); j!=s31[i].end(); j++)
            K31.at(i,*j) = matGal(i+sl1+sl2,*j);

    for(size_t i=0; i<s32.size(); i++) for(auto j=s32[i].begin(); j!=s32[i].end(); j++)
            K32.at(i,*j) = matGal(i+sl1+sl2,*j + sl1);

    for(size_t i=0; i<s33.size(); i++) for(auto j=s33[i].begin(); j!=s33[i].end(); j++)
            K33.at(i,*j) = matGal(i+sl1+sl2,*j + sl1+sl2);
    //********************************************

    matrix<T> KRed11, KRed12, KRed21, KRed22;
    //KRed22=((K22.plus(((constrMat.transpose()).mult(K32)))).plus((K23.mult(constrMat)))).plus((((constrMat.transpose()).mult(K33))).mult(constrMat));
    KRed11 = K11;
    KRed12 = K12 + K13*constrMat;
    KRed21 = K21 + (constrMat.transpose())*K31;
    KRed22 = K22 + (constrMat.transpose())*K32 + K23*constrMat + (constrMat.transpose())*K33*constrMat ;

    //Construction and filling of the reduced matrix res

    std::vector<std::list<size_t> >sRed11, sRed12, sRed21, sRed22;
    size_t ncolRed11, ncolRed12, ncolRed21, ncolRed22;
    KRed11.getRowStruct(sRed11, ncolRed11); KRed12.getRowStruct(sRed12, ncolRed12);
    KRed21.getRowStruct(sRed21, ncolRed21); KRed22.getRowStruct(sRed22, ncolRed22);

    auto add1=[sl1](size_t& i){return i+=sl1;};
    //auto minus2=[sl1,sl2](size_t& i){return i-=(sl1+sl2);};
    std::vector<std::list<size_t> > sRed12Abs=sRed12, sRed22Abs=sRed22;

    for(size_t i=0; i<sl1; i++){std::for_each(sRed12Abs[i].begin(), sRed12Abs[i].end(), add1);}
    for(size_t i=0; i<sl2; i++){std::for_each(sRed22Abs[i].begin(), sRed22Abs[i].end(), add1);}

    std::vector<std::list<size_t> > resStruct(sl1+sl2);
    for(size_t i=0; i<sl1; i++){
            //std::merge(sRed11[i].begin(),sRed11[i].end(), sRed12Abs[i].begin(), sRed12Abs[i].end(), std::back_inserter(resStruct[i]));
            std::copy(sRed11[i].begin(),sRed11[i].end(),std::back_inserter(resStruct[i]));
            std::copy(sRed12Abs[i].begin(), sRed12Abs[i].end(),std::back_inserter(resStruct[i]));
    }
    for(size_t i=0; i<sl2; i++){
    //   std::merge(sRed21[i].begin(),sRed21[i].end(), sRed22Abs[i].begin(), sRed22Abs[i].end(), std::back_inserter(resStruct[i]));
    std::copy(sRed21[i].begin(),sRed21[i].end(),std::back_inserter(resStruct[i+sl1]));
    std::copy(sRed22Abs[i].begin(), sRed22Abs[i].end(),std::back_inserter(resStruct[i+sl1]));

    }
    redMat.resize(resStruct, sl1+sl2);
    //Filling res from KRed11, KRed12, ..., KRed22
    for(size_t i=0; i<sRed11.size(); i++) for(auto j=sRed11[i].begin(); j!=sRed11[i].end(); j++)
            redMat.at(i,*j) = KRed11(i,*j);

    for(size_t i=0; i<sRed12.size(); i++) for(auto j=sRed12[i].begin(); j!=sRed12[i].end(); j++)
            redMat.at(i,*j+sl1) = KRed12(i,*j);

    for(size_t i=0; i<sRed21.size(); i++) for(auto j=sRed21[i].begin(); j!=sRed21[i].end(); j++)
            redMat.at(i+sl1,*j) = KRed21(i,*j);

    for(size_t i=0; i<sRed22.size(); i++) for(auto j=sRed22[i].begin(); j!=sRed22[i].end(); j++)
            redMat.at(i+sl1,*j+sl1) = KRed22(i,*j);
    //********************************************
    fem::dimMatPbcDir(Mh, numPbc, ndof, bc, a4, a14, a24, a34);
    fem::fillMatPbcDir(Mh, numPbc, ndof, bc, matrixValue, a4, a14, a24, a34);

    std::valarray<T> galRhs1, galRhs2, galRhs3;
    rhsPbc(a14, a24, a34, bc, ndof, galRhs1, galRhs2, galRhs3, numPbc);
    std::valarray<T> redRhs1=galRhs1 -1*(K13*constrVec);
    std::valarray<T> redRhs2=galRhs2 + (constrMat.transpose())*galRhs3 -1*(K23*constrVec) -((constrMat.transpose())*(K33*constrVec));
    redRhs.resize(sl1+sl2,0);
    for(size_t i=0; i<sl1; i++) redRhs[i]=redRhs1[i];
    for(size_t i=0; i<sl2; i++) redRhs[i+sl1]=redRhs2[i];

    }

    //********************************* rhs computing for pbc ****************************************
template <typename T>
void rhsPbc(const matrix<T>& K14,const matrix<T>& K24, const matrix<T>& K34,
                const BCs& bc, size_t ndof, std::valarray<T>& galRhs1,std::valarray<T>& galRhs2,std::valarray<T>& galRhs3, const std::valarray<size_t>& numPbc)
    {
    /*
     * This function helps compution the right side needed in solving matGal*U_ndir=Rhs_ndir
    galRhs doesnt inclue dirichlet rhs values
    galRhs1 -> for non constrained dofs;
    galRhs2 -> for constrained dofs;
    galRhs3 -> for constrained to dofs;
    */
    size_t offset=K14.nrow()+K24.nrow()+K34.nrow();
    std::vector<size_t> dirdofs=bc.getDirichletDofs();

    std::valarray<T> dirValuesVec(dirdofs.size());
    for(size_t i=0; i<dirdofs.size(); i++) dirValuesVec[ numPbc[ dirdofs[i] ]-offset ]= bc.pbcDirichletValue(dirdofs[i], ndof);
    galRhs1=-1*(K14*dirValuesVec);
    galRhs2=-1*(K24*dirValuesVec);
    galRhs3=-1*(K34*dirValuesVec);

    }

//************************************* pbc Disp and force Values ****************************************************
template <typename T, size_t D>
void pbcDispForceValues(MeshPbc<T,D>& Mh, /*std::valarray<size_t>& numPbc,*/
    size_t ndof, BCs& bc, vector<pbcEquation>& pbcLinks,
        T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
      int solType, std::valarray<T>& disp, std::valarray<T>& force
    ){
    /*Given the type of sollicitation, this function computes the nodal displacements
    /and nodal forces*/

    /*
     * One have to solve K.U=F
    Due to pbc, K is divided to K11, K12, K13, K14, K21, K22, K23, K24, K31, K32, K33, K34, K41, K42, K43, K44
    fem::femAssembly, galMat = K11, K12, K13,K21, K22, K23, K31, K32, K33
    dirMat4 = K41, K42, K43, K44
    *redMat, redRhs are modified matrices due to constraints equations related to periodic boundary conditions
    */

     pbcLinks.clear();
     matrix<T> galMat;
     matrix<T> constrMat;
     std::valarray<double> constrVec;

     size_t nNotDir;
     std::valarray<size_t> numPbc;
     std::valarray<bool> masqDir;
     std::valarray<bool> masqTied;
     std::valarray<bool> masqRet;
     std::valarray<bool> masqNotConstrained;
     size_t NbLinks; vector<size_t> sommets;
     scaletag::models::PbcUtilities pbc;

     pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
     bc.setSommets(sommets); //natural numerotation

     //0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
     if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
     if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

     bc.setPbc(pbcLinks, ndof, sommets);
     renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);
     size_t nNotCstr = ndof*Mh.getNumberOfNodes()-bc.getDirNumber()-bc.getTiedNumber()-bc.getRetainedNumber();
     size_t nRet = bc.getRetainedNumber();
     size_t nDir=bc.getDirNumber();
     size_t nTied=bc.getTiedNumber();
     fem::femAssembly(Mh, numPbc, galMat, ndof, bc, matrixValue); // dimension and fill {{K11,K12,K13},{K21,K22,K33},{K31, K32, K33}}

     matrix<T> dirMat4;      //matrices elements with relation with dirichlet dofs : K41, K42, K43, K44
     matrix<T> redMat;
     std::valarray<T> redRhs;

     pbc.createConstraintMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //gamma
     pbc.createConstraintVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S
     assemblyPbc(Mh, numPbc,galMat, constrMat, constrVec, ndof, bc, matrixValue, dirMat4, redMat, redRhs, masqDir);

     scaletag::fem::GeneralSolver<T> solver;
     solver.solveMumpsAssembledNonSym(redMat,redRhs);

    // Evaluation of displacement vector

     disp.resize( ndof*Mh.getNumberOfNodes(), 0 );
     std::valarray<T> sol = solver.getSolution();

     disp[masqNotConstrained]=sol[std::slice(0,nNotCstr,1)];
     disp[masqRet]=sol[std::slice(nNotCstr,nRet,1)];

     std::valarray<double> retValues = disp[masqRet];
     disp[masqTied] = constrMat*(retValues) + constrVec;

     for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     disp[ (bc.getDirichletDofs()[i]) ] = bc.pbcDirichletValue( (bc.getDirichletDofs()[i]) , ndof);

    // Evaluation of nodal forces at retained nodes
     force.resize(ndof*Mh.getNumberOfNodes(), 0);
     std::valarray<T> dispNumPbc( disp.size() );
     for(size_t i=0; i<sol.size(); i++) dispNumPbc[i]=sol[i];
     std::valarray<double> tiedValues=constrMat*(retValues) + constrVec;
     for(size_t i=0; i<nTied; i++) dispNumPbc[nNotCstr+nRet+i]=tiedValues[i];
     for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     dispNumPbc[ numPbc[(bc.getDirichletDofs()[i])] ]=bc.pbcDirichletValue( (bc.getDirichletDofs()[i]) , ndof);
     std::valarray<T> rhsDir=dirMat4*dispNumPbc; //rhsNonDir=0
     force[masqDir]=rhsDir;
     cout<<"meas(Mh) = "<<meas(Mh)<<endl;


{
    //Testing of matrices (see mathematica at "/home/ahose/Desktop/SCALETAG/ScaleTag/stlib/source/models")
//     std::valarray<double> val1{1,2,1,2,3}, val2{2,1,1,2,3};
//     std::valarray<size_t> idx1{0,1,3,3,3}, idx2{0,2,1,0,3},ptr1{0,2,3,4,5},ptr2{0,2,3,4,5};
//     matrix<double> mat1(ptr1,idx1,val1,4), mat2{ptr2,idx2,val2,4};
//     matrix<double> matprod = mat1.mult(mat2);
//     matrix<double> matsum = mat1.plus(mat2);
//     matrix<double> mat1trans=mat1.transpose();

/*
 * This to analyze singular matrix
 *      std::valarray<double> val2{20., 5.}, val1{4.0, 8.0, 1.0, 2.0};
     std::valarray<size_t> idx1{0,1,0,1}, ptr1{0,2,4};
     matrix<double> mat1(ptr1, idx1, val1,2);
     scaletag::fem::GeneralSolver<T> solver1;
     solver1.solveMumpsAssembledNonSym(mat1,val2);
     std::cout<<"sol : "<<solver1.getSolution()[0]<<" "<<solver1.getSolution()[1]<<std::endl;*/
}

    }



/**************************   SECOND METHOD *******************************/

template <typename T, size_t D>
void Gamma(const MeshPbc<T,D>& Mh, const matrix<T>& constrMat, const std::valarray<T>& constrVec,
               size_t ndof,const BCs& bc, matrix<T>& matGamma, std::valarray<T>& vecC){

/*This function computes Gamma and C*/
//Gamma
size_t nNotCstr= ndof*Mh.getNumberOfNodes()-bc.getTiedNumber()-bc.getRetainedNumber(); //-bc.getDirNumber()
size_t nRet=bc.getRetainedNumber();
size_t nTied=bc.getTiedNumber();

size_t nrow=nNotCstr+nRet+nTied; // ndof*Mh.getNumberOfNodes()-bc.getDirNumber(); //nNotCstr+nRet+nTied
size_t ncol= nNotCstr+nRet; // nrow-bc.getTiedNumber(); //nRet+nNotCstr;
size_t nnz=constrMat.nnz()+nNotCstr+nRet; //constrMat.nnz()+ncol; //constrMat.nnz()+nRet+nNotCstr;
std::valarray<size_t> ptr; ptr.resize(nrow+1);
for(size_t i=0; i<ncol; i++) ptr[i]=i;

std::valarray<size_t> ptrG=constrMat.getPtr();
for(size_t i=0; i<nTied; i++) ptr[ncol+i]=ncol+ptrG[i];
ptr[nrow]=nnz;

std::valarray<size_t> idx; idx.resize(nnz,0);
for(size_t i=0; i<ncol; i++) idx[i]=i;
for(size_t i=0; i<constrMat.nnz(); i++) idx[ncol+i]=nNotCstr+constrMat.getColumnsIndex()[i];

std::valarray<T> val; val.resize(nnz,0);
for(size_t i=0; i<ncol; i++) val[i]=1.0;
for(size_t i=0; i<constrMat.nnz(); i++) val[ncol+i]=constrMat.getVal()[i];

matGamma=matrix<T>(ptr, idx, val, ncol);

vecC.resize(nNotCstr+nRet+nTied,0);
for(size_t i=0; i<nTied; i++) vecC[ncol+i]=constrVec[i];
}


template<typename T,size_t D>
void assemblyPbc2(const MeshPbc<T,D>& Mh, const std::valarray<size_t>& numPbc, const matrix<T>& matGal,
                      const matrix<T>& constrMat, const std::valarray<T>& constrVec, size_t ndof, const BCs& bc,
                      T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
                      matrix<T>& redMat, std::valarray<T>& redRhs,
                      matrix<T>& a4, matrix<T>& a14, matrix<T>& a24, matrix<T>& a34){

//may be remove a14 a24 a34
fem::dimMatPbcDir(Mh, numPbc, ndof, bc, a4, a14, a24, a34);
fem::fillMatPbcDir(Mh, numPbc, ndof, bc, matrixValue, a4, a14, a24, a34);

matrix<T> matGamma; std::valarray<T> vecC;
Gamma(Mh, constrMat, constrVec,ndof,bc, matGamma, vecC);
redMat=((matGamma.transpose())*matGal)*matGamma;

std::valarray<T> tempValue, u4; u4.resize(bc.getDirNumber(),0);  //tempValue=-1*[a14 a24 a34]*u4
size_t dirOffset=ndof*Mh.getNumberOfNodes()-bc.getDirNumber();
for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     u4[ numPbc[(bc.getDirichletDofs()[i])] -dirOffset]=bc.pbcDirichletValue( (bc.getDirichletDofs()[i]) , ndof);
std::valarray<T> tempValue1=-1*(a14*u4);
std::valarray<T> tempValue2=-1*(a24*u4);
std::valarray<T> tempValue3=-1*(a34*u4);
tempValue.resize(tempValue1.size()+tempValue2.size()+tempValue3.size(), 0);
for(size_t i=0; i<tempValue1.size(); i++) tempValue[i]=tempValue1[i];
for(size_t i=0; i<tempValue2.size(); i++) tempValue[tempValue1.size()+i]=tempValue2[i];
for(size_t i=0; i<tempValue3.size(); i++) tempValue[tempValue1.size()+tempValue2.size()+i]=tempValue3[i];
redRhs=(matGamma.transpose())*tempValue -((matGamma.transpose())*matGal)*vecC;
    }



template <typename T, size_t D>
void pbcDispForceValues2(MeshPbc<T,D>& Mh, /*std::valarray<size_t>& numPbc,*/
                             size_t ndof, BCs& bc, vector<pbcEquation>& pbcLinks,
                             T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
                             int solType, std::valarray<T>& disp, std::valarray<T>& force
                             ){
/*Given the type of sollicitation, this function computes the nodal displacements
    /and nodal forces*/

/*
     * One have to solve K.U=F
    Due to pbc, K is divided to K11, K12, K13, K14, K21, K22, K23, K24, K31, K32, K33, K34, K41, K42, K43, K44
    fem::femAssembly, galMat = K11, K12, K13,K21, K22, K23, K31, K32, K33
    dirMat4 = K41, K42, K43, K44
    *redMat, redRhs are modified matrices due to constraints equations related to periodic boundary conditions
    */

pbcLinks.clear();
matrix<T> galMat;
matrix<T> constrMat;
std::valarray<double> constrVec;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool>masqDir;
std::valarray<bool>masqTied;
std::valarray<bool>masqRet;
std::valarray<bool> masqNotConstrained;
size_t NbLinks; vector<size_t> sommets;
scaletag::models::PbcUtilities pbc;

pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
bc.setSommets(sommets); //natural numerotation

//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

bc.setPbc(pbcLinks, ndof, sommets);
renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);
size_t nNotCstr = ndof*Mh.getNumberOfNodes()-bc.getDirNumber()-bc.getTiedNumber()-bc.getRetainedNumber();
size_t nRet = bc.getRetainedNumber();
size_t nTied=bc.getTiedNumber();
size_t nDir=bc.getDirNumber();
fem::femAssembly(Mh, numPbc, galMat, ndof, bc, matrixValue); // dimension and fill {{K11,K12,K13},{K21,K22,K33},{K31, K32, K33}}

matrix<T> dirMat4;      //matrices elements with relation with dirichlet dofs : K41, K42, K43, K44
matrix<T> redMat;
std::valarray<T> redRhs;

pbc.createConstraintMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //gamma
pbc.createConstraintVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S
//assemblyPbc2(Mh, numPbc,galMat, constrMat, constrVec, ndof, bc, matrixValue, dirMat4, redMat, redRhs, masqDir);
matrix<T> a4, a14, a24, a34;
assemblyPbc2(Mh, numPbc, galMat,constrMat, constrVec, ndof, bc,matrixValue, redMat, redRhs, a4, a14, a24,a34);

scaletag::fem::GeneralSolver<T> solver;
solver.solveMumpsAssembledNonSym(redMat,redRhs);

// Evaluation of displacement vector

disp.resize( ndof*Mh.getNumberOfNodes(), 0 );
std::valarray<T> sol = solver.getSolution();

disp[masqNotConstrained]=sol[std::slice(0,nNotCstr,1)];
disp[masqRet]=sol[std::slice(nNotCstr,nRet,1)];

std::valarray<double> retValues = disp[masqRet];
disp[masqTied] = constrMat*(retValues) + constrVec;

for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     disp[ (bc.getDirichletDofs()[i]) ] = bc.pbcDirichletValue( (bc.getDirichletDofs()[i]) , ndof);

// Evaluation of nodal forces at retained nodes
force.resize(ndof*Mh.getNumberOfNodes(), 0);
std::valarray<T> dispNumPbc( disp.size() );
for(size_t i=0; i<sol.size(); i++) dispNumPbc[i]=sol[i];
std::valarray<double> tiedValues=constrMat*(retValues) + constrVec;
for(size_t i=0; i<nTied; i++) dispNumPbc[nNotCstr+nRet+i]=tiedValues[i];
for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     dispNumPbc[ numPbc[(bc.getDirichletDofs()[i])] ]=bc.pbcDirichletValue( (bc.getDirichletDofs()[i]) , ndof);
std::valarray<T> rhsDir=/*dirMat4*/a4*dispNumPbc; //rhsNonDir=0
force[masqDir]=rhsDir;
    }

/********************************   Computation with diagonal dirichlet Section *****************************/
/****************************** Global analysis ****************************/
    template <typename T, size_t D>
    void globGamma(const MeshPbc<T,D>& Mh, const matrix<T>& constrMat, const std::valarray<T>& constrVec,
               size_t ndof,const BCs& bc, matrix<T>& matGamma, std::valarray<T>& vecC){

/*This function computes Gamma and C and takes in account every dofs*/
//Gamma
size_t nNotCstr= ndof*Mh.getNumberOfNodes()-bc.getDirNumber()-bc.getTiedNumber()-bc.getRetainedNumber();
size_t nRet=bc.getRetainedNumber();
size_t nTied=bc.getTiedNumber();
size_t nDir=bc.getDirNumber();

size_t nrow=nNotCstr+nRet+nTied+nDir; //=ndof*Mh.getNumberOfNodes();
size_t ncol= nNotCstr+nRet+nDir;
size_t nnz=constrMat.nnz()+nNotCstr+nRet+nDir;

std::valarray<size_t> ptr; ptr.resize(nrow+1);
for(size_t i=0; i<(nNotCstr+nRet); i++) ptr[i]=i;

std::valarray<size_t> ptrG=constrMat.getPtr();
for(size_t i=0; i<nTied; i++) ptr[nNotCstr+nRet+i]=nNotCstr+nRet+ptrG[i];
for(size_t i=0; i<nDir; i++) ptr[nNotCstr+nRet+nTied+i]=nNotCstr+nRet+constrMat.nnz()+i;
ptr[nrow]=nnz;

std::valarray<size_t> idx; idx.resize(nnz,0);
for(size_t i=0; i<(nNotCstr+nRet); i++) idx[i]=i;
for(size_t i=0; i<constrMat.nnz(); i++) idx[(nNotCstr+nRet)+i]=nNotCstr+constrMat.getColumnsIndex()[i];
for(size_t i=0; i<nDir; i++) idx[nNotCstr+nRet+constrMat.nnz()+i]=nNotCstr+nRet+i;

std::valarray<T> val; val.resize(nnz,0);
for(size_t i=0; i<(nNotCstr+nRet); i++) val[i]=1.0;
for(size_t i=0; i<constrMat.nnz(); i++) val[(nNotCstr+nRet)+i]=constrMat.getVal()[i];
for(size_t i=0; i<nDir; i++) val[nNotCstr+nRet+constrMat.nnz()+i]=1.;

matGamma=matrix<T>(ptr, idx, val, ncol);

vecC.resize(nNotCstr+nRet+nTied+nDir,0);
for(size_t i=0; i<nTied; i++) vecC[(nNotCstr+nRet)+i]=constrVec[i];
    }


template <typename T, size_t D>
void pbcDispForceValues3(MeshPbc<T,D>& Mh, /*std::valarray<size_t>& numPbc,*/
                             size_t ndof, BCs& bc, vector<pbcEquation>& pbcLinks,
                             T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
                             int solType, std::valarray<T>& disp, std::valarray<T>& force
                             ){
/*Given the type of sollicitation, this function computes the nodal displacements
    /and nodal forces*/

/*
     * One have to solve K.U=F
    Due to pbc, K is divided to K11, K12, K13, K14, K21, K22, K23, K24, K31, K32, K33, K34, K41, K42, K43, K44
    fem::femAssembly, galMat = K11, K12, K13,K21, K22, K23, K31, K32, K33
    dirMat4 = K41, K42, K43, K44
    *redMat, redRhs are modified matrices due to constraints equations related to periodic boundary conditions
    */

pbcLinks.clear();
matrix<T> KG, KD, KRed, matGamma;
std::valarray<double> FD, FRed;

matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool>masqDir;
std::valarray<bool>masqTied;
std::valarray<bool>masqRet;
std::valarray<bool> masqNotConstrained;
size_t NbLinks; vector<size_t> sommets;
scaletag::models::PbcUtilities pbc;

pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
bc.setSommets(sommets); //natural numerotation

//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

bc.setPbc(pbcLinks, ndof, sommets);
renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);
size_t nNotCstr = ndof*Mh.getNumberOfNodes()-bc.getDirNumber()-bc.getTiedNumber()-bc.getRetainedNumber();
size_t nRet = bc.getRetainedNumber();
size_t nTied=bc.getTiedNumber();
size_t nDir=bc.getDirNumber();
fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, matrixValue); // Galerkin matrix with every components calculated
fem::assemblyMatWithDir(Mh, numPbc, KD, ndof, bc, matrixValue); // Galerkin matrix with every components calculated
//matrix<T> dirMat4;      //matrices elements with relation with dirichlet dofs : K41, K42, K43, K44
//matrix<T> redMat;
//std::valarray<T> redRhs;

pbc.createConstraintMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createConstraintVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S
//assemblyPbc2(Mh, numPbc,galMat, constrMat, constrVec, ndof, bc, matrixValue, dirMat4, redMat, redRhs, masqDir);
//matrix<T> a4, a14, a24, a34;
//assemblyPbc2(Mh, numPbc, galMat,constrMat, constrVec, ndof, bc,matrixValue, redMat, redRhs, a4, a14, a24,a34);

FD.resize(ndof*Mh.getNumberOfNodes(),0);
for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     FD[ numPbc[(bc.getDirichletDofs()[i])] ] = bc.pbcDirichletValue(bc.getDirichletDofs()[i], ndof);

globGamma( Mh, constrMat, constrVec, ndof, bc,  matGamma,  vecC);
KRed=(matGamma.transpose())*KD*matGamma;
FRed=(matGamma.transpose())*(FD-KD*vecC);

/*****  Tempation to solve singularity ********/
matrix<T> KRedDir;
std::valarray<double> FRedDir=FRed;
for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     FRedDir[ numPbc[(bc.getDirichletDofs()[i])] -nTied ] = bc.pbcDirichletValue(bc.getDirichletDofs()[i], ndof);

KRedDir=KRed;
KRedDir.nFirstLines(nNotCstr+nRet);

matrix<T> appMat;
size_t appMatCol = KRed.ncol();
std::valarray<size_t> appMatPtr, appMatIdx;
std::valarray<T> appMatVal;
appMatPtr.resize(nDir+1);
appMatIdx.resize(nDir, 0);
appMatVal.resize(nDir, 0);
for(size_t i=0; i<nDir; i++) appMatPtr[i]=i;
appMatPtr[nDir]=nDir;
size_t horizontalOffset=nNotCstr+nRet;
for(size_t i=0; i<nDir; i++) appMatIdx[i] = horizontalOffset+i;//numPbc[(bc.getDirichletDofs()[i])] -nTied
for(size_t i=0; i<nDir; i++) appMatVal[i] = 1.0;

appMat = matrix<T>(appMatPtr, appMatIdx, appMatVal, appMatCol);
KRedDir.appendMatrixToBottom(appMat);

/***** End Tempation to solve singularity ********/


//cout<<"max: "<<FRed.max()<< "; min: "<<FRed.min()<<std::endl;
scaletag::fem::GeneralSolver<T> solver;
//solver.solveMumpsAssembledNonSym(KRed, FRed);
solver.solveMumpsAssembledNonSym(KRedDir, FRedDir);

// Evaluation of displacement vector

disp.resize( ndof*Mh.getNumberOfNodes(), 0 );
std::valarray<T> sol = solver.getSolution();

disp[masqNotConstrained]=sol[std::slice(0,nNotCstr,1)];
disp[masqRet]=sol[std::slice(nNotCstr,nRet,1)];
std::valarray<double> Uret=sol[std::slice(nNotCstr,nRet,1)];
disp[masqTied]=constrMat*Uret +constrVec;

/*for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     disp[ (bc.getDirichletDofs()[i]) ] = bc.pbcDirichletValue( (bc.getDirichletDofs()[i]) , ndof);*/
disp[masqDir] =sol[std::slice(nNotCstr+nRet, nDir, 1)];

// Evaluation of nodal forces
force.resize(ndof*Mh.getNumberOfNodes(), 0);
std::valarray<T> dispNumPbc( disp.size() );
for(size_t i=0; i<(nNotCstr+nRet); i++) dispNumPbc[i]=sol[i];
std::valarray<double> Ut=constrMat*Uret + constrVec; //Uret
for(size_t i=0; i<nTied; i++) dispNumPbc[nNotCstr+nRet+i]=Ut[i];
//for(size_t i=0; i<bc.getDirichletDofs().size(); i++)
     //dispNumPbc[ std::slice(nNotCstr+nRet+nTied, nDir, 1)] ]=sol[std::slice(nNotCstr+nRet, nDir, 1)];
dispNumPbc[ std::slice(nNotCstr+nRet+nTied, nDir, 1) ]=sol[std::slice(nNotCstr+nRet, nDir, 1)];
     //dispNumPbc[ numPbc[(bc.getDirichletDofs()[i])] ] = sol[nNotCstr+nRet+i];
//std::valarray<T> rhsDir=/*dirMat4*/a4*dispNumPbc; //rhsNonDir=0
//force[masqDir]=rhsDir;

std::valarray<double> forceNum = KG*dispNumPbc;
force[masqNotConstrained]=forceNum[std::slice(0,nNotCstr,1)];
force[masqRet]=forceNum[std::slice(nNotCstr,nRet,1)];
force[masqTied] = forceNum[std::slice(nNotCstr+nRet,nTied,1)];
force[masqDir] = forceNum[std::slice(nNotCstr+nRet+nTied,nDir,1)];;
    }

    template <typename T, size_t D>
    void pbcDispForceValues4(MeshPbc<T,D>& Mh, /*std::valarray<size_t>& numPbc,*/
                             size_t ndof, BCs& bc, vector<pbcEquation>& pbcLinks,
                             T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
                             int solType, std::valarray<T>& disp, std::valarray<T>& force
                             ){
/*Given the type of sollicitation, this function computes the nodal displacements
    /and nodal forces*/

/*
     * One have to solve K.U=F
    Due to pbc, K is divided to K11, K12, K13, K14, K21, K22, K23, K24, K31, K32, K33, K34, K41, K42, K43, K44
    fem::femAssembly, galMat = K11, K12, K13,K21, K22, K23, K31, K32, K33
    dirMat4 = K41, K42, K43, K44
    *redMat, redRhs are modified matrices due to constraints equations related to periodic boundary conditions
    */

/*  Elimination method 4*/
pbcLinks.clear();
matrix<T> KG/*, KD, KRed, matGamma*/;
//std::valarray<double> FD, FRed;

matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool> masqDir;
std::valarray<bool> masqTied;
std::valarray<bool> masqRet;
std::valarray<bool> masqNotConstrained;
size_t NbLinks;
vector<size_t> sommets;
scaletag::models::PbcUtilities pbc;

pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
bc.setSommets(sommets); //natural numerotation

//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

bc.setPbc(pbcLinks, ndof, sommets);
renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);
size_t nNotCstr = ndof*Mh.getNumberOfNodes()-bc.getTiedNumber()-bc.getRetainedNumber(); //-bc.getDirNumber()
size_t nRet = bc.getRetainedNumber();
size_t nTied=bc.getTiedNumber();
size_t nDir=bc.getDirNumber();
//size_t nDirBar = ndof*Mh.getNumberOfNodes()-nDir;
fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, matrixValue); // Galerkin matrix with every components calculated

//double alpha = std::max( std::fabs(KG.min()), std::fabs(KG.max()) );
//alpha = alpha*1e6;

pbc.createConstraintMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createConstraintVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S

matrix<T> matGamma;
Gamma(Mh, constrMat, constrVec, ndof, bc, matGamma, vecC);
matrix<T> KRed = (matGamma.transpose())*(KG*matGamma);
std::valarray<double> FRed =-1*((matGamma.transpose())*(KG*vecC));

/*** Imposing dirichlet dofs *****/
std::vector<size_t> dirdofs = bc.getDirichletDofs();
for(size_t i=0; i<dirdofs.size(); i++){
   size_t  dof = numPbc[ dirdofs[i] ],
         iStart = KRed.getPtr()[dof],
         iEnd = KRed.getPtr()[dof+1],
           ne   = iEnd-iStart;
     for(size_t e=0; e<ne; e++) KRed.at( dof, KRed.getColumnsIndex()[iStart+e] ) = 0.;
     KRed.at(dof, dof) = 1.0;
     FRed[dof] = bc.pbcDirichletValue(dirdofs[i], ndof);
}
/*** End Imposing dirichlet dofs ***/


scaletag::fem::GeneralSolver<T> solver;
solver.solveMumpsAssembledNonSym(KRed, FRed );

disp.resize( ndof*Mh.getNumberOfNodes(), 0 );
std::valarray<double> sol = solver.getSolution();

disp[masqNotConstrained] = sol[std::slice(0,nNotCstr,1)];
std::valarray<double> dispRet = sol[std::slice(nNotCstr,nRet,1)];
disp[masqRet]=dispRet;
std::valarray<double> dispTied=constrMat*dispRet+constrVec;
disp[masqTied] = dispTied;
//disp[masqDir] = sol[std::slice(nNotCstr+nRet+nTied,nDir,1)];

// Evaluation of nodal forces
force.resize(ndof*Mh.getNumberOfNodes(), 0);
std::valarray<T> dispNumPbc( disp.size() );
//for(size_t i=0; i<(nNotCstr+nRet+nTied); i++) dispNumPbc[i]=sol[i];
//dispNumPbc[ std::slice(nNotCstr+nRet+nTied, nDir, 1) ]=u4;
dispNumPbc[std::slice(0, nNotCstr+nRet, 1)]= sol;
dispNumPbc[std::slice(nNotCstr+nRet, nTied, 1)]=dispTied;

std::valarray<double> forceNum = KG*dispNumPbc;
force[masqNotConstrained]=forceNum[std::slice(0,nNotCstr,1)];
force[masqRet]=forceNum[std::slice(nNotCstr,nRet,1)];
force[masqTied] = forceNum[std::slice(nNotCstr+nRet,nTied,1)];

    }


    /*************  Tempations for penalty method and lagrange multiplier  ***********************/

    template <typename T, size_t D>
    void pbcDispForceValues5(MeshPbc<T,D>& Mh, /*std::valarray<size_t>& numPbc,*/
                             size_t ndof, BCs& bc, vector<pbcEquation>& pbcLinks,
                             T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
                             int solType, std::valarray<T>& disp, std::valarray<T>& force
                             ){
/*Given the type of sollicitation, this function computes the nodal displacements
    /and nodal forces*/

/*
     * One have to solve K.U=F
    Due to pbc, K is divided to K11, K12, K13, K14, K21, K22, K23, K24, K31, K32, K33, K34, K41, K42, K43, K44
    fem::femAssembly, galMat = K11, K12, K13,K21, K22, K23, K31, K32, K33
    dirMat4 = K41, K42, K43, K44
    *redMat, redRhs are modified matrices due to constraints equations related to periodic boundary conditions
    */

/* Penalty method */
pbcLinks.clear();
matrix<T> KG/*, KD, KRed, matGamma*/;
//std::valarray<double> FD, FRed;

matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool> masqDir;
std::valarray<bool> masqTied;
std::valarray<bool> masqRet;
std::valarray<bool> masqNotConstrained;
size_t NbLinks; vector<size_t> sommets;
scaletag::models::PbcUtilities pbc;

pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
bc.setSommets(sommets); //natural numerotation

//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

bc.setPbc(pbcLinks, ndof, sommets);
renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);
size_t nNotCstr = ndof*Mh.getNumberOfNodes()-bc.getTiedNumber()-bc.getRetainedNumber(); //-bc.getDirNumber()
size_t nRet = bc.getRetainedNumber();
size_t nTied=bc.getTiedNumber();
size_t nDir=bc.getDirNumber();
std::valarray<bool> ret=masqRet[masqRet];
std::valarray<bool> tied=masqTied[masqTied];
//size_t nDirBar = ndof*Mh.getNumberOfNodes()-nDir;
fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, matrixValue); // Galerkin matrix with every components calculated

double alpha = std::max( std::fabs(KG.min()), std::fabs(KG.max()) );
alpha = alpha*1e4;

pbc.createConstraintGlobMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createConstraintGlobVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S

matrix<double> KGMod = KG+( (constrMat.transpose())*constrMat)*alpha;
std::valarray<double> FGMod = alpha*( (constrMat.transpose())*constrVec );
matrix<double> KGModDir = KGMod ;
std::valarray<double> FGModDir = FGMod;
/*** Imposing dirichlet dofs *****/
//std::vector<size_t> dirdofs = bc.getDirichletDofs();
//for(size_t i=0; i<dirdofs.size(); i++){
//     size_t  dof = numPbc[ dirdofs[i] ],
//         iStart = KGModDir.getPtr()[dof],
//         iEnd = KGModDir.getPtr()[dof+1],
//         ne   = iEnd-iStart;
//     for(size_t e=0; e<ne; e++) KGModDir.at( dof, KGModDir.getColumnsIndex()[iStart+e] ) = 0.;
//     KGModDir.at(dof, dof) = 1.0;
//     FGModDir[dof] = bc.pbcDirichletValue(dirdofs[i], ndof);
//}
/*** End Imposing dirichlet dofs ***/
scaletag::fem::GeneralSolver<T> solver;
solver.solveMumpsAssembledNonSym(KGModDir, FGModDir );
//solver.solveMumpsAssembledNonSym(KGMod, FGMod );

disp.resize( ndof*Mh.getNumberOfNodes(), 0 );
std::valarray<double> sol = solver.getSolution();

disp[masqNotConstrained]=sol[std::slice(0,nNotCstr,1)];
disp[masqRet]=sol[std::slice(nNotCstr,nRet,1)];
disp[masqTied] = sol[std::slice(nNotCstr+nRet,nTied,1)];

// Evaluation of nodal forces
force.resize(ndof*Mh.getNumberOfNodes(), 0);
std::valarray<T> dispNumPbc( disp.size() );
dispNumPbc=sol;

std::valarray<double> forceNum = KG*dispNumPbc;
force[masqNotConstrained]=forceNum[std::slice(0,nNotCstr,1)];
force[masqRet]=forceNum[std::slice(nNotCstr,nRet,1)];
force[masqTied] = forceNum[std::slice(nNotCstr+nRet,nTied,1)];


/*****  Tempation to solve singularity ********/
//std::valarray<double> u4(nDir);
//for(size_t i=0; i<nDir; i++)
//     u4[ numPbc[ bc.getDirichletDofs()[i] ] -nNotDir ] = bc.pbcDirichletValue( (bc.getDirichletDofs()[i]) , ndof);
//matrix<double> KGMod = KG+( (constrMat.transpose())*constrMat)*alpha;//+
//std::valarray<double> FGMod = alpha*( (constrMat.transpose())*constrVec );
//FGMod[std::slice(nNotDir, nDir , 1)] = u4;

//matrix<T> appMat;
//size_t appMatCol = KGMod.ncol();
//std::valarray<size_t> appMatPtr, appMatIdx;
//std::valarray<T> appMatVal;
//appMatPtr.resize(nDir+1);
//appMatIdx.resize(nDir, 0);
//appMatVal.resize(nDir, 0);
//for(size_t i=0; i<nDir; i++) appMatPtr[i]=i;
//appMatPtr[nDir]=nDir;
//size_t horizontalOffset=nNotCstr+nRet+nTied;
//for(size_t i=0; i<nDir; i++) appMatIdx[i] = horizontalOffset+i;//numPbc[(bc.getDirichletDofs()[i])] -nTied
//for(size_t i=0; i<nDir; i++) appMatVal[i] = 1.0;

//appMat = matrix<T>(appMatPtr, appMatIdx, appMatVal, appMatCol);
//KGMod.nFirstLines(nNotDir);
//KGMod.appendMatrixToBottom(appMat);



//scaletag::fem::GeneralSolver<T> solver;
//solver.solveMumpsAssembledNonSym(KGMod, FGMod );

//disp.resize( ndof*Mh.getNumberOfNodes(), 0 );
//std::valarray<double> sol = solver.getSolution();

//disp[masqNotConstrained]=sol[std::slice(0,nNotCstr,1)];
//disp[masqRet]=sol[std::slice(nNotCstr,nRet,1)];
//disp[masqTied] = sol[std::slice(nNotCstr+nRet,nTied,1)];
////disp[masqDir] = sol[std::slice(nNotCstr+nRet+nTied,nDir,1)];

//// Evaluation of nodal forces
//force.resize(ndof*Mh.getNumberOfNodes(), 0);
//std::valarray<T> dispNumPbc( disp.size() );
////for(size_t i=0; i<(nNotCstr+nRet+nTied); i++) dispNumPbc[i]=sol[i];
////dispNumPbc[ std::slice(nNotCstr+nRet+nTied, nDir, 1) ]=u4;
//dispNumPbc=sol;

//std::valarray<double> forceNum = KG*dispNumPbc;
//force[masqNotConstrained]=forceNum[std::slice(0,nNotCstr,1)];
//force[masqRet]=forceNum[std::slice(nNotCstr,nRet,1)];
//force[masqTied] = forceNum[std::slice(nNotCstr+nRet,nTied,1)];

/*****  End Tempation to solve singularity ********/

    }


    /*************  Tempations for penalty method and lagrange multiplier  ***********************/

    template <typename T, size_t D>
    void pbcDispForceValues6(MeshPbc<T,D>& Mh, /*std::valarray<size_t>& numPbc,*/
                             size_t ndof, BCs& bc, vector<pbcEquation>& pbcLinks,
                             T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
                             int solType, std::valarray<T>& disp, std::valarray<T>& force
                             ){
/*Given the type of sollicitation, this function computes the nodal displacements
    /and nodal forces*/

/*
     * One have to solve K.U=F
    Due to pbc, K is divided to K11, K12, K13, K14, K21, K22, K23, K24, K31, K32, K33, K34, K41, K42, K43, K44
    fem::femAssembly, galMat = K11, K12, K13,K21, K22, K23, K31, K32, K33
    dirMat4 = K41, K42, K43, K44
    *redMat, redRhs are modified matrices due to constraints equations related to periodic boundary conditions
    */

/* Lagrange multipliers method */

pbcLinks.clear();
matrix<T> KG/*, KD, KRed, matGamma*/;
//std::valarray<double> FD, FRed;

matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool> masqDir;
std::valarray<bool> masqTied;
std::valarray<bool> masqRet;
std::valarray<bool> masqNotConstrained;
size_t NbLinks; vector<size_t> sommets;
scaletag::models::PbcUtilities pbc;


pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
bc.setSommets(sommets); //natural numerotation

//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

bc.setPbc(pbcLinks, ndof, sommets);
renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);
size_t nNotCstr = ndof*Mh.getNumberOfNodes()-bc.getTiedNumber()-bc.getRetainedNumber(); //-bc.getDirNumber()
size_t nRet = bc.getRetainedNumber();
size_t nTied=bc.getTiedNumber();
size_t nDir=bc.getDirNumber();
std::valarray<bool> ret=masqRet[masqRet];
std::valarray<bool> tied=masqTied[masqTied];
//size_t nDirBar = ndof*Mh.getNumberOfNodes()-nDir;
fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, matrixValue); // Galerkin matrix with every components calculated

pbc.createConstraintGlobMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createConstraintGlobVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S


std::vector<std::list<size_t> > s(ndof*Mh.getNumberOfNodes());
matrix<double> zero;
zero.resize(s, ndof*Mh.getNumberOfNodes());
matrix<double> constrMatTranspose = constrMat.transpose();
constrMatTranspose.appendMatrixToBottom(zero);
matrix<double> KGMod = KG;
KGMod.appendMatrixToBottom(constrMat);
KGMod.appendMatrixToRight(constrMatTranspose);
std::valarray<double> FGMod;
FGMod.resize(2*ndof*Mh.getNumberOfNodes(), 0);
FGMod[std::slice(ndof*Mh.getNumberOfNodes(), ndof*Mh.getNumberOfNodes(), 1)] = constrVec;

/*** Imposing dirichlet dofs *****/
//std::vector<size_t> dirdofs = bc.getDirichletDofs();
//for(size_t i=0; i<dirdofs.size(); i++){
//     size_t  dof = numPbc[ dirdofs[i] ],
//         iStart = KGModDir.getPtr()[dof],
//         iEnd = KGModDir.getPtr()[dof+1],
//         ne   = iEnd-iStart;
//     for(size_t e=0; e<ne; e++) KGModDir.at( dof, KGModDir.getColumnsIndex()[iStart+e] ) = 0.;
//     KGModDir.at(dof, dof) = 1.0;
//     FGModDir[dof] = bc.pbcDirichletValue(dirdofs[i], ndof);
//}
/*** End Imposing dirichlet dofs ***/
scaletag::fem::GeneralSolver<T> solver;
//solver.solveMumpsAssembledNonSym(KGModDir, FGModDir );
solver.solveMumpsAssembledNonSym(KGMod, FGMod );

disp.resize( ndof*Mh.getNumberOfNodes(), 0 );
std::valarray<double> sol = solver.getSolution();

disp[masqNotConstrained]=sol[std::slice(0,nNotCstr,1)];
disp[masqRet]=sol[std::slice(nNotCstr,nRet,1)];
disp[masqTied] = sol[std::slice(nNotCstr+nRet,nTied,1)];

// Evaluation of nodal forces
force.resize(ndof*Mh.getNumberOfNodes(), 0);
std::valarray<T> dispNumPbc( disp.size() );
dispNumPbc=sol[std::slice(0, ndof*Mh.getNumberOfNodes(),1)];

std::valarray<double> forceNum = KG*dispNumPbc;
force[masqNotConstrained]=forceNum[std::slice(0,nNotCstr,1)];
force[masqRet]=forceNum[std::slice(nNotCstr,nRet,1)];
force[masqTied] = forceNum[std::slice(nNotCstr+nRet,nTied,1)];

    }
    /************ Custom solution using Petsc *******************************/

    template <typename T, size_t D>
    void pbcDispForceValuesPetsc(int* argc, char*** argv,MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, std::vector<pbcEquation>& pbcLinks,
                             T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
                             int solType, std::valarray<T>& disp, std::valarray<T>& force
                             ){

matrix<T> KG;
matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool> masqDir;
std::valarray<bool> masqTied;
std::valarray<bool> masqRet;
std::valarray<bool> masqNotConstrained;
size_t NbLinks; vector<size_t> sommets;
scaletag::models::PbcUtilities pbc;

std::valarray<double> dispNumPbc, forceNumPbc;
models::DataForPetsc petscData;

pbcLinks.clear();
pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
bc.setSommets(sommets); //natural numerotation

//Setting boundary conditions
//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

bc.setPbc(pbcLinks, ndof, sommets);  //Fill tiedDofs and retainedDofs in bc
renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);

size_t nNotCstr, nTied, nRet, nDir;

nRet = bc.getRetainedNumber();
nDir = bc.getDirNumber();
std::valarray<bool> ret=masqRet[masqRet];
std::valarray<bool> tied=masqTied[masqTied];

//Reevaluations of some of the parameters due to the new renumbering
nNotCstr = std::valarray<bool>(masqNotConstrained[masqNotConstrained]).size();
nTied = std::valarray<bool>(masqTied[masqTied]).size();

int ws, wr;
MPI_Comm_size(MPI_COMM_WORLD, &ws);
MPI_Comm_rank(MPI_COMM_WORLD, &wr);
if(wr==0){
fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, matrixValue); // Galerkin matrix. Dirichlet components are included
pbc.createPbcConstraintsMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createPbcConstraintsVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S

{//PetscMPIInt rank=0;
//int petscArgc =1;
//char* testPurpose="testPurpose";
//char** petscArgv=&testPurpose;

//PetscInitialize(NULL, NULL,      NULL, NULL); // Testing NULL, NULL,    , NULL, NULL

//PetscInitialize(&petscArgc, &petscArgv, NULL, NULL);
//MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
//if(rank==0){
//models::DataForPetsc petscData(Mh, ndof, KG, constrMat, constrVec);
//petscData.petscSolve(dispNumPbc);
//PetscFinalize();}
//models::DataForPetsc::petscSolve(Mh, ndof, KG, constrMat, constrVec, dispNumPbc);
//petscData.init();
    }
//models::DataForPetsc petscData(&KG, &constrMat, &constrVec, models::objAndGradFunc, models::hessianFunc ,models::constraintsFunc, models::constraintsGradFunc);
petscData=std::move(models::DataForPetsc(&KG, &constrMat, &constrVec, models::objAndGradFunc, models::hessianFunc ,models::constraintsFunc, models::constraintsGradFunc));

dispNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);
forceNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);}

petscData.petscSolve(argc, argv, Mh, ndof, KG, constrMat, constrVec, dispNumPbc);

if(wr==0){
forceNumPbc=KG*dispNumPbc;
disp.resize( ndof*Mh.getNumberOfNodes(), 0.0 );
force.resize( ndof*Mh.getNumberOfNodes(), 0.0 );

//To be reanalyzed as we choose now a different numbering
size_t nRetAndNDir, nRetAndDir, nNRetAndDir;
nRetAndNDir = std::valarray<T>( disp[masqRet&&(masqDir==0)] ).size();
nRetAndDir = std::valarray<T>( disp[masqRet&&masqDir] ).size();
nNRetAndDir = std::valarray<T>( disp[masqDir&&(masqRet==0)] ).size();

disp[masqNotConstrained] = dispNumPbc[std::slice(0,nNotCstr,1)]; //Change sol to dispNumPbc
disp[masqTied] = dispNumPbc[std::slice(nNotCstr,nTied,1)];
disp[masqRet&&(masqDir==0)] = dispNumPbc[std::slice(nNotCstr+nTied,nRetAndNDir,1)]; //Needs nRetAndNDir, nRetAndDir, nNRetAndDir in the sequel
disp[masqRet&&masqDir] = dispNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir,nRetAndDir,1)];
disp[masqDir&&(masqRet==0)] = dispNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir+nRetAndDir,nNRetAndDir,1)];

//force valarray is computed in the same way as disp valarray.
force[masqNotConstrained] = forceNumPbc[std::slice(0,nNotCstr,1)]; //Change sol to dispNumPbc
force[masqTied] = forceNumPbc[std::slice(nNotCstr,nTied,1)];
force[masqRet&&(masqDir==0)] = forceNumPbc[std::slice(nNotCstr+nTied,nRetAndNDir,1)]; //Needs nRetAndNDir, nRetAndDir, nNRetAndDir in the sequel
force[masqRet&&masqDir] = forceNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir,nRetAndDir,1)];
force[masqDir&&(masqRet==0)] = forceNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir+nRetAndDir,nNRetAndDir,1)];}

    }

}}
#endif

