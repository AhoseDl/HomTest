#ifndef SCALETAGFEMPBC_H
#define SCALETAGFEMPBC_H

#include<matrix.h>
#include<MeshPbc.h>
#include<list>
#include<valarray>
#include<bcs.h>
#include<fem.h>
//#include<solver.h>
#include<iostream>
#include<algorithm>
#include<physics.h>

//#include<petscutilities.h>
#include<taoanalysis.h>

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

//Brief verification of the renumbering
size_t ndofs; //Total number of dofs
std::valarray<bool> masqRetAndNDir=std::valarray<bool>(masqRet&&(masqDir==0));
std::valarray<bool> masqRetAndDir=std::valarray<bool>(masqRet&&masqDir);
std::valarray<bool> masqNotRetAndDir=std::valarray<bool>((masqRet==0) && masqDir);
size_t n1 = std::count(std::begin(masqNotConstrained), std::end(masqNotConstrained), true);
size_t n2 = std::count(std::begin(masqTied), std::end(masqTied), true);
size_t n3 = std::count(std::begin(masqRetAndNDir), std::end(masqRetAndNDir), true);
size_t n4 = std::count(std::begin(masqRetAndDir), std::end(masqRetAndDir), true);
size_t n5 = std::count(std::begin(masqNotRetAndDir), std::end(masqNotRetAndDir), true);

ndofs=n1+n2+n3+n4+n5;
std::cout<<" The partition in renumbering gives "<<ndofs<< " dofs "<<std::endl;
std::cout<<" The expected number of degree of freedoms is : "<<ndof*Mh.getNumberOfNodes()<<std::endl;
std::cout<<" Not retained Dirichlet dofs : "<<n5<<std::endl;
std::cout<<" Retained Dirichlet dofs with applied dirichlet boundary conditions : "<<n4<<std::endl;

}

template<typename T, size_t D>
void createMatrixWithNotMeshNodes(const std::vector<size_t>& nodes, /*const*/ fem::BCs& bc, const std::valarray<size_t>& numPbc,
                                  size_t ndof, fem::matrix<T>& mat){
std::valarray<size_t> ptr;
std::valarray<size_t> idx;
std::valarray<T> val;

std::vector<size_t> notMeshDofs = bc.getNotMeshDofs(ndof);

size_t nrow = notMeshDofs.size();
size_t ncol = numPbc.size();

ptr.resize(nrow+1);
idx.resize(nrow);
val.resize(nrow, 0.);

for(size_t i=0; i<nrow; i++){
        ptr[i] =i;
        idx[i] = numPbc[ notMeshDofs[i] ];
        val[i] = 1.;

}
ptr[nrow]=nrow;

mat = fem::matrix<T>(ptr, idx, val, ncol);

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
    PetscErrorCode pbcTaoDispForce(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, std::vector<pbcEquation>& pbcLinks,
                             T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
                             int solType, std::valarray<T>& disp, std::valarray<T>& force
                             ){

//std::vector<pbcEquation> pbcLinks;

matrix<T> KG;
matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool> masqDir;
std::valarray<bool> masqTied;
std::valarray<bool> masqRet;
std::valarray<bool> masqNotConstrained;
size_t NbLinks; vector<size_t> sommets; std::vector<size_t> notMeshNodes;

scaletag::models::PbcUtilities pbc;

std::valarray<double> dispNumPbc, forceNumPbc, lambda;
models::DataForPetsc petscData;

//Mat petscKG, petscConstrMat;
//Vec petscConstrVec;

PetscFunctionBeginUser;

//pbcLinks.clear();
pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets, notMeshNodes); //natural numerotation
bc.setSommets(sommets); //natural numerotation
bc.setNotMeshNodes(notMeshNodes, ndof);

//Setting boundary conditions
//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

bc.setPbc(pbcLinks, ndof, sommets);  //Fill tiedDofs and retainedDofs in bc
renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);

size_t nNotCstr, nTied, nRet, nDir;

nRet = bc.getRetainedNumber();
nDir = bc.getDirNumber();
//std::valarray<bool> ret=masqRet[masqRet];
//std::valarray<bool> tied=masqTied[masqTied];

//Reevaluations of some of the parameters due to the new renumbering
//nNotCstr = std::count(begin(masqNotConstrained), end(masqNotConstrained), true);
//nNotCstr = std::valarray<bool>(masqNotConstrained[masqNotConstrained]).size();
//nTied = std::valarray<bool>(masqTied[masqTied]).size();

nNotCstr=std::count(std::begin(masqNotConstrained), std::end(masqNotConstrained), true);
nTied = std::count(std::begin(masqTied), std::end(masqTied), true);

int ws, wr;
MPI_Comm_size(MPI_COMM_WORLD, &ws);
MPI_Comm_rank(MPI_COMM_WORLD, &wr);
//if(wr==0){
fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, matrixValue); // Galerkin matrix. Dirichlet components are included
pbc.createPbcConstraintsMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createPbcConstraintsVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S

//fem::matrix<double> matNotMeshDofs;
//fem::createMatrixWithNotMeshNodes<double,3>(bc.getNotMeshNodes(), bc, numPbc, ndof, matNotMeshDofs);
//KG.nFirstLines(KG.nrow()-matNotMeshDofs.nrow());
//KG.appendMatrixToBottom(matNotMeshDofs);


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

//petscData=std::move(models::DataForPetsc(&KG, &constrMat, &constrVec, models::objAndGradFunc, models::hessianFunc ,models::constraintsFunc, models::constraintsGradFunc));
  //  models::DataForPetsc::init(petscKG, petscConstrMat, petscConstrVec);
dispNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);
forceNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);
//}


MPI_Barrier(MPI_COMM_WORLD);
//petscData.petscSolve(argc, argv, Mh, ndof, KG, constrMat, constrVec, dispNumPbc);
//models::DataForPetsc::petscSolve(Mh, ndof, KG, constrMat, constrVec, &petscKG, &petscConstrMat, &petscConstrVec, dispNumPbc);
//models::DataForPetsc dataForPetsc;

//PetscCall(petscData.petscSolve(Mh, ndof, KG, constrMat, constrVec, /*&petscKG, &petscConstrMat, &petscConstrVec,*/ dispNumPbc));
//PetscCall(petscData.petscSolve2(Mh, ndof, KG,constrMat, constrVec, dispNumPbc, numPbc, bc));

//matrix<double> KGTest; Mat petscKuu;
//fem::KuuAssembly(Mh, numPbc, KGTest, ndof, bc, matrixValue);
//std::valarray<size_t> idxt=KGTest.getColumnsIndex();
//size_t ndirt=idxt.size()-bc.getDirichletDofs().size();
//for(size_t i=0; i<idxt.size(); i++) if(idxt[i]>=ndirt) std::cout<<"exceeded column indexes : "<<i<<" "<< idxt[i]<<std::endl;
//std::cout<<"Beginning KuuTest .."<<std::endl;
//petscData.matrixToPetscMat(KGTest, petscKuu);

//MPI_Barrier(MPI_COMM_WORLD);
////petscData.kuuTest(Mh, ndof, petscKuu, constrMat, constrVec, dispNumPbc,     numPbc, bc);
//std::cout<<"Ending KuuTtest ..."<<std::endl;

MPI_Barrier(MPI_COMM_WORLD);

PetscCall(petscData.petscSolveLinearRedMumpsCond(Mh, ndof, KG, constrMat, constrVec,     numPbc, bc, dispNumPbc, lambda));

MPI_Barrier(MPI_COMM_WORLD);
if(wr==0){
forceNumPbc=KG*dispNumPbc; //+(constrMat.transpose())*Lambda
std::vector<size_t> dirdofs = bc.getDirichletDofs();
size_t nd = dirdofs.size();
//std::valarray<double> ud(nd, 0.);
//ud = dispNumPbc[std::slice( (KG.nrow()-nd), nd, 1)];
matrix<double> Gcool = constrMat;
Gcool.nFirstLines( constrMat.nrow() - dirdofs.size() );
//for(size_t m=0; m<( constrMat.nrow() - dirdofs.size() ); m++ )
//            if(std::fabs(Gcool(m, numPbc[dirdofs[3]])) >= 1.e-50) Gcool.at(m, numPbc[dirdofs[3]]) = 0. ;
//            for(size_t dd=0; dd<dirdofs.size(); dd++) Gcool.at(m, numPbc[dirdofs[dd]]) = 0. ;
forceNumPbc += (Gcool.transpose())*lambda;
//matrix<double> Gd = Gcool;
//Gd.nLastColumns(nd);
//forceNumPbc[std::slice(KG.nrow()-Gcool.nrow(), Gcool.nrow(), 1)] += -1*(Gd*ud);

for(size_t i=0; i<dirdofs.size(); i++) std::cout<<" force numPbc "<<dirdofs[i]<<" : "<<forceNumPbc[numPbc[dirdofs[i]]]<<std::endl;
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
force[masqDir&&(masqRet==0)] = forceNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir+nRetAndDir,nNRetAndDir,1)];
}

PetscFunctionReturn(PETSC_SUCCESS);
    }



//    template <typename T, size_t D>
//    PetscErrorCode pbcDispForceValuesPetsc(int* argc, char*** argv,MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, std::vector<pbcEquation>& pbcLinks,
//                                           T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
//                                           int solType, std::valarray<T>& disp, std::valarray<T>& force
//                                           ){

//matrix<T> KG;
//matrix<T> constrMat;
//std::valarray<double> constrVec, vecC;

//size_t nNotDir;
//std::valarray<size_t> numPbc;
//std::valarray<bool> masqDir;
//std::valarray<bool> masqTied;
//std::valarray<bool> masqRet;
//std::valarray<bool> masqNotConstrained;
//size_t NbLinks; vector<size_t> sommets;
//scaletag::models::PbcUtilities pbc;

//std::valarray<double> dispNumPbc, forceNumPbc;
//models::DataForPetsc petscData;

//Mat petscKG, petscConstrMat;
//Vec petscConstrVec;

//PetscFunctionBeginUser;

//pbcLinks.clear();
//pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets); //natural numerotation
//bc.setSommets(sommets); //natural numerotation

////Setting boundary conditions
////0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
//if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
//if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);

//bc.setPbc(pbcLinks, ndof, sommets);  //Fill tiedDofs and retainedDofs in bc
//renumberingPbc(Mh, numPbc, nNotDir, ndof, bc, masqDir, masqTied, masqRet, masqNotConstrained);

//size_t nNotCstr, nTied, nRet, nDir;

//nRet = bc.getRetainedNumber();
//nDir = bc.getDirNumber();
//std::valarray<bool> ret=masqRet[masqRet];
//std::valarray<bool> tied=masqTied[masqTied];

////Reevaluations of some of the parameters due to the new renumbering
//nNotCstr = std::valarray<bool>(masqNotConstrained[masqNotConstrained]).size();
//nTied = std::valarray<bool>(masqTied[masqTied]).size();

//int ws, wr;
//MPI_Comm_size(MPI_COMM_WORLD, &ws);
//MPI_Comm_rank(MPI_COMM_WORLD, &wr);
////if(wr==0){
//fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, matrixValue); // Galerkin matrix. Dirichlet components are included
//pbc.createPbcConstraintsMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
//pbc.createPbcConstraintsVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S

//{//PetscMPIInt rank=0;
////int petscArgc =1;
////char* testPurpose="testPurpose";
////char** petscArgv=&testPurpose;

////PetscInitialize(NULL, NULL,      NULL, NULL); // Testing NULL, NULL,    , NULL, NULL

////PetscInitialize(&petscArgc, &petscArgv, NULL, NULL);
////MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
////if(rank==0){
////models::DataForPetsc petscData(Mh, ndof, KG, constrMat, constrVec);
////petscData.petscSolve(dispNumPbc);
////PetscFinalize();}
////models::DataForPetsc::petscSolve(Mh, ndof, KG, constrMat, constrVec, dispNumPbc);
////petscData.init();
//}
////models::DataForPetsc petscData(&KG, &constrMat, &constrVec, models::objAndGradFunc, models::hessianFunc ,models::constraintsFunc, models::constraintsGradFunc);
////   PetscFunctionBeginUser;
//petscData=std::move(models::DataForPetsc(&KG, &constrMat, &constrVec, models::objAndGradFunc, models::hessianFunc ,models::constraintsFunc, models::constraintsGradFunc));

//dispNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);
//forceNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);
////}

//MPI_Barrier(MPI_COMM_WORLD);
//petscData.petscSolve(argc, argv, Mh, ndof, KG, constrMat, constrVec, dispNumPbc);
////models::DataForPetsc::petscSolve(Mh, ndof, KG, constrMat, constrVec, &petscKG, &petscConstrMat, &petscConstrVec, dispNumPbc);

//MPI_Barrier(MPI_COMM_WORLD);
//if(wr==0){
//forceNumPbc=KG*dispNumPbc;
//disp.resize( ndof*Mh.getNumberOfNodes(), 0.0 );
//force.resize( ndof*Mh.getNumberOfNodes(), 0.0 );

////To be reanalyzed as we choose now a different numbering
//size_t nRetAndNDir, nRetAndDir, nNRetAndDir;
//nRetAndNDir = std::valarray<T>( disp[masqRet&&(masqDir==0)] ).size();
//nRetAndDir = std::valarray<T>( disp[masqRet&&masqDir] ).size();
//nNRetAndDir = std::valarray<T>( disp[masqDir&&(masqRet==0)] ).size();

//disp[masqNotConstrained] = dispNumPbc[std::slice(0,nNotCstr,1)]; //Change sol to dispNumPbc
//disp[masqTied] = dispNumPbc[std::slice(nNotCstr,nTied,1)];
//disp[masqRet&&(masqDir==0)] = dispNumPbc[std::slice(nNotCstr+nTied,nRetAndNDir,1)]; //Needs nRetAndNDir, nRetAndDir, nNRetAndDir in the sequel
//disp[masqRet&&masqDir] = dispNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir,nRetAndDir,1)];
//disp[masqDir&&(masqRet==0)] = dispNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir+nRetAndDir,nNRetAndDir,1)];

////force valarray is computed in the same way as disp valarray.
//force[masqNotConstrained] = forceNumPbc[std::slice(0,nNotCstr,1)]; //Change sol to dispNumPbc
//force[masqTied] = forceNumPbc[std::slice(nNotCstr,nTied,1)];
//force[masqRet&&(masqDir==0)] = forceNumPbc[std::slice(nNotCstr+nTied,nRetAndNDir,1)]; //Needs nRetAndNDir, nRetAndDir, nNRetAndDir in the sequel
//force[masqRet&&masqDir] = forceNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir,nRetAndDir,1)];
//force[masqDir&&(masqRet==0)] = forceNumPbc[std::slice(nNotCstr+nTied+nRetAndNDir+nRetAndDir,nNRetAndDir,1)];
//}

////PetscFunctionReturnVoid();
//PetscFunctionReturn(PETSC_SUCCESS);
//    }


    template <typename T, size_t D>
    PetscErrorCode pbcTaoDispForce2(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, std::vector<pbcEquation>& pbcLinks,
//                                   T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
  ElementalMatrix& elementalMatrix, int solType, std::valarray<T>& disp, std::valarray<T>& force
                                   ){

//std::vector<pbcEquation> pbcLinks;

matrix<T> KG;
matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool> meshAndNDir;
std::valarray<bool> meshAndDir;
std::valarray<bool> NMeshAndDir;
std::valarray<bool> NMeshAndNDir;
size_t NbLinks; std::vector<size_t> sommets; std::vector<size_t> notMeshNodes;

scaletag::models::PbcUtilities pbc;

std::valarray<double> dispNumPbc, forceNumPbc, lambda;
models::DataForPetsc petscData;

bc.initialize();

PetscFunctionBeginUser;

//pbcLinks.clear();
pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets, notMeshNodes); //natural numerotation
//pbc.createPbcLinks(Mh, NbLinks, sommets, notMeshNodes, pbcLinks); //For Gen
bc.setSommets(sommets); //natural numerotation
bc.setNotMeshNodes(notMeshNodes, ndof);

//Setting boundary conditions
//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);
//bc.setLoadingType(solType);
//bc.setGeneralPbcBoundaryCondition();
//bc.setPbcUtilities(&pbc);
//bc.setTilingDirs(pbc.getTilingDirs());

bc.setPbc(pbcLinks, ndof, sommets);  //Fill tiedDofs and retainedDofs in bc
renumberingPbc2(Mh, numPbc,  ndof, bc, meshAndNDir, meshAndDir, NMeshAndDir, NMeshAndNDir);
size_t nm, nm1, nm2, nr, nr1, nr2;

nm1 = std::count(std::begin(meshAndNDir), std::end(meshAndNDir), true);
nm2 = std::count(std::begin(meshAndDir), std::end(meshAndDir), true);
nm = nm1+nm2;

nr1 = std::count(std::begin(NMeshAndDir), std::end(NMeshAndDir), true);
nr2 = std::count( std::begin(NMeshAndNDir), std::end(NMeshAndNDir), true);
nr = nr1+nr2;

bc.setRefCount(nm1, nm2, nr1, nr2);
//size_t mbar, md, rd, rdbar; //mesh and not dir, mesh and dir, ref and dir, ref and not Dir

int ws, wr;
MPI_Comm_size(MPI_COMM_WORLD, &ws);
MPI_Comm_rank(MPI_COMM_WORLD, &wr);

fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, elementalMatrix); // Galerkin matrix. Dirichlet components are included
pbc.createPbcConstraintsMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createPbcConstraintsVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S

dispNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);
forceNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);


MPI_Barrier(MPI_COMM_WORLD);

PetscCall(petscData.petscSolveLinearRedMumpsCond2(Mh, ndof, KG, constrMat, constrVec,     numPbc, bc, dispNumPbc, lambda));

MPI_Barrier(MPI_COMM_WORLD);
if(wr==0){
std::vector<size_t> dirdofs = bc.getDirichletDofs();
size_t nd = dirdofs.size();

//matrix<double> GBar = constrMat ; GBar.nFirstLines(constrMat.nrow() -nd);
//GBar = constrMat ; GBar.nFirstLines(constrMat.nrow() -nr2); //To be reviewed
//GBar=constrMat; // To be deleted

//matrix<double> Gm =GBar; Gm.nFirstColumns(nm);
//matrix<double> Gr = GBar; Gr.nLastColumns(nr);

std::valarray<double> um = dispNumPbc[(meshAndNDir||meshAndDir)];
//forceNumPbc[(meshAndNDir||meshAndDir)] += KG*um + (Gm.transpose())*lambda;
//forceNumPbc[(NMeshAndDir||NMeshAndNDir)] += (Gr.transpose())*lambda;
forceNumPbc = 0. ;
forceNumPbc[(meshAndNDir||meshAndDir)] += KG*um ;//+ (Gm.transpose())*lambda;
forceNumPbc/*[(NMeshAndDir||NMeshAndNDir)]*/ += (constrMat.transpose())*lambda;
for(size_t i=0; i<dirdofs.size(); i++) forceNumPbc[numPbc[dirdofs[i]]] = -1*lambda[constrMat.nrow()-nd+i];

disp.resize( nm+nr, 0.0 );
force.resize( nm+nr, 0.0 );

disp[meshAndNDir] = dispNumPbc[std::slice(0, nm1, 1)];
disp[meshAndDir] = dispNumPbc[std::slice(nm1, nm2, 1)];
disp[NMeshAndDir] = dispNumPbc[std::slice(nm, nr1, 1)];
disp[NMeshAndNDir] = dispNumPbc[std::slice(nm+nr1, nr2, 1)];

force[meshAndNDir] = forceNumPbc[std::slice(0, nm1, 1)];
force[meshAndDir] = forceNumPbc[std::slice(nm1, nm2, 1)];
force[NMeshAndDir] = forceNumPbc[std::slice(nm, nr1, 1)];
force[NMeshAndNDir] = forceNumPbc[std::slice(nm+nr1, nr2, 1)];

}

PetscFunctionReturn(PETSC_SUCCESS);
    }


template <typename T, size_t D>
PetscErrorCode pbcTaoDispForceGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, std::vector<pbcEquation>& pbcLinks,
                                    //                                   T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje),
                                    ElementalMatrix& elementalMatrix, int solType, std::valarray<T>& disp, std::valarray<T>& force
                                    ){

//std::vector<pbcEquation> pbcLinks;

matrix<T> KG;
matrix<T> constrMat;
std::valarray<double> constrVec, vecC;

size_t nNotDir;
std::valarray<size_t> numPbc;
std::valarray<bool> meshAndNDir;
std::valarray<bool> meshAndDir;
std::valarray<bool> NMeshAndDir;
std::valarray<bool> NMeshAndNDir;
size_t NbLinks; std::vector<size_t> sommets; std::vector<size_t> notMeshNodes;

scaletag::models::PbcUtilities pbc;

std::valarray<double> dispNumPbc, forceNumPbc, lambda;
models::DataForPetsc petscData;

bc.initialize();

PetscFunctionBeginUser;

//pbcLinks.clear();
//pbc.createPbcLinks(Mh,pbcLinks, NbLinks, sommets, notMeshNodes); //natural numerotation
pbc.createPbcLinks(Mh, NbLinks, sommets, notMeshNodes, pbcLinks);
bc.setSommets(sommets); //natural numerotation
bc.setNotMeshNodes(notMeshNodes, ndof);

//Setting boundary conditions
//0->Tx, 1->Ty, 2->Tz, 3->CisXY, 4->cisXZ, 5->cisYZ
//if(solType==0) bc.setTractionX(3); if(solType==1) bc.setTractionY(3); if(solType==2) bc.setTractionZ(3);
//if(solType==3) bc.setCisXY(3); if(solType==4) bc.setCisXZ(3); if(solType==5) bc.setCisYZ(3);
bc.setLoadingType(solType);
bc.setGeneralPbcBoundaryCondition();
bc.setPbcUtilities(&pbc);
bc.setTilingDirs(pbc.getTilingDirs());

bc.setPbc(pbcLinks, ndof, sommets);  //Fill tiedDofs and retainedDofs in bc
renumberingPbc2(Mh, numPbc,  ndof, bc, meshAndNDir, meshAndDir, NMeshAndDir, NMeshAndNDir);
size_t nm, nm1, nm2, nr, nr1, nr2;

nm1 = std::count(std::begin(meshAndNDir), std::end(meshAndNDir), true);
nm2 = std::count(std::begin(meshAndDir), std::end(meshAndDir), true);
nm = nm1+nm2;

nr1 = std::count(std::begin(NMeshAndDir), std::end(NMeshAndDir), true);
nr2 = std::count( std::begin(NMeshAndNDir), std::end(NMeshAndNDir), true);
nr = nr1+nr2;

bc.setRefCount(nm1, nm2, nr1, nr2);
//size_t mbar, md, rd, rdbar; //mesh and not dir, mesh and dir, ref and dir, ref and not Dir

int ws, wr;
MPI_Comm_size(MPI_COMM_WORLD, &ws);
MPI_Comm_rank(MPI_COMM_WORLD, &wr);

fem::globFemAssembly(Mh, numPbc, KG, ndof, bc, elementalMatrix); // Galerkin matrix. Dirichlet components are included
pbc.createPbcConstraintsMatrix(Mh, pbcLinks, numPbc, bc, ndof, constrMat);  //G
pbc.createPbcConstraintsVector(Mh, pbcLinks, numPbc, bc, ndof, constrVec); //S

dispNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);
forceNumPbc.resize(ndof*Mh.getNumberOfNodes(), 0.);


MPI_Barrier(MPI_COMM_WORLD);

PetscCall(petscData.petscSolveLinearRedMumpsCond2(Mh, ndof, KG, constrMat, constrVec,     numPbc, bc, dispNumPbc, lambda));

MPI_Barrier(MPI_COMM_WORLD);
if(wr==0){
std::vector<size_t> dirdofs = bc.getDirichletDofs();
size_t nd = dirdofs.size();

//matrix<double> GBar = constrMat ; GBar.nFirstLines(constrMat.nrow() -nd);
//GBar = constrMat ; GBar.nFirstLines(constrMat.nrow() -nr2); //To be reviewed
//GBar=constrMat; // To be deleted

//matrix<double> Gm =GBar; Gm.nFirstColumns(nm);
//matrix<double> Gr = GBar; Gr.nLastColumns(nr);

std::valarray<double> um = dispNumPbc[(meshAndNDir||meshAndDir)];
//forceNumPbc[(meshAndNDir||meshAndDir)] += KG*um + (Gm.transpose())*lambda;
//forceNumPbc[(NMeshAndDir||NMeshAndNDir)] += (Gr.transpose())*lambda;
forceNumPbc = 0. ;
forceNumPbc[(meshAndNDir||meshAndDir)] += KG*um ;//+ (Gm.transpose())*lambda;
forceNumPbc/*[(NMeshAndDir||NMeshAndNDir)]*/ += (constrMat.transpose())*lambda;
for(size_t i=0; i<dirdofs.size(); i++) forceNumPbc[numPbc[dirdofs[i]]] = -1*lambda[constrMat.nrow()-nd+i];

disp.resize( nm+nr, 0.0 );
force.resize( nm+nr, 0.0 );

disp[meshAndNDir] = dispNumPbc[std::slice(0, nm1, 1)];
disp[meshAndDir] = dispNumPbc[std::slice(nm1, nm2, 1)];
disp[NMeshAndDir] = dispNumPbc[std::slice(nm, nr1, 1)];
disp[NMeshAndNDir] = dispNumPbc[std::slice(nm+nr1, nr2, 1)];

force[meshAndNDir] = forceNumPbc[std::slice(0, nm1, 1)];
force[meshAndDir] = forceNumPbc[std::slice(nm1, nm2, 1)];
force[NMeshAndDir] = forceNumPbc[std::slice(nm, nr1, 1)];
force[NMeshAndNDir] = forceNumPbc[std::slice(nm+nr1, nr2, 1)];

}
    }
    template<typename T, size_t D>
    void renumberingPbc2(const MeshPbc<T,D>& Mh, std::valarray<size_t>& numPbc, size_t ndof, BCs& bc,
                        std::valarray<bool>& meshAndNDir, std::valarray<bool>& meshAndDir, std::valarray<bool>& NMeshAndDir,
                        std::valarray<bool>& NMeshAndNDir){

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

size_t nnode = Mh.getNumberOfNodes();
numPbc.resize(ndof*Mh.getNumberOfNodes(), 0);
//masqDir.resize(ndof*Mh.getNumberOfNodes(), 0);
//masqTied.resize(ndof*Mh.getNumberOfNodes(), 0);
//masqRet.resize(ndof*Mh.getNumberOfNodes(), 0);
//masqNotConstrained.resize(ndof*Mh.getNumberOfNodes(),0);

//std::valarray<bool> meshAndNDir, meshAndDir, NMeshAndDir, NMeshAndNDir;

meshAndNDir.resize(ndof*Mh.getNumberOfNodes(), 0);
meshAndDir.resize(ndof*Mh.getNumberOfNodes(), 0);
NMeshAndDir.resize(ndof*Mh.getNumberOfNodes(), 0);
NMeshAndNDir.resize(ndof*Mh.getNumberOfNodes(), 0);

size_t imd = 0, imNd = 0, ird =0, irNd =0; //m->mesh; d->Dir ; N->Not, r-> not mesh dof (related to reference points)
for(size_t ik=0; ik<ndof*nnode ; ik++){
if(bc.isMeshDof(ik) && !bc.isDirichlet(ik)){numPbc[ik]=imNd++; meshAndNDir[ik]=1; continue;}
if(bc.isMeshDof(ik) && bc.isDirichlet(ik)){numPbc[ik]=imd++; meshAndDir[ik]=1; continue;}
if(!bc.isMeshDof(ik) && bc.isDirichlet(ik)){numPbc[ik]=ird++; NMeshAndDir[ik]=1; continue;}
if(!bc.isMeshDof(ik) && !bc.isDirichlet(ik)){numPbc[ik]=irNd++; NMeshAndNDir[ik]=1; continue;}
}
numPbc[meshAndDir] = std::valarray<size_t>(numPbc[meshAndDir]) + imNd;
numPbc[NMeshAndDir] = std::valarray<size_t>(numPbc[NMeshAndDir]) + imNd+imd;
numPbc[NMeshAndNDir] = std::valarray<size_t>(numPbc[NMeshAndNDir]) + imNd+imd+ird;

size_t iDir = 0, iRet = 0, iTied=0, iNotCstr=0; size_t ik=0;

size_t iRetAndDir=0, iRetAndNDir=0, iNRetAndDir=0;
for(size_t ik=0; ik<ndof*nnode; ik++){


{//if(bc.isTied(ik)){ numPbc[ik] = iTied++; masqTied[ik]=1; continue; }

//numPbc[ik] = iNotCstr++; masqNotConstrained[ik]=1;
//}
//numPbc[masqTied]=std::valarray<size_t>(numPbc[masqTied])+iNotCstr;
//numPbc[masqRet&&(masqDir==0)]=std::valarray<size_t>(numPbc[masqRet&&(masqDir==0)])+iNotCstr+iTied;
//numPbc[masqRet&&masqDir]=std::valarray<size_t>(numPbc[masqRet&&masqDir])+iNotCstr+iTied+iRetAndNDir;
//numPbc[masqDir&&(masqRet==0)]=std::valarray<size_t>(numPbc[masqDir&&(masqRet==0)])+iNotCstr+iTied+iRetAndNDir+iRetAndDir;
//_iNotDir=ndof*Mh.getNumberOfNodes()-bc.getDirichletDofs().size();
//bc.actualizePbc(iDir, iRet, iTied, iNotCstr, iRetAndDir, iRetAndNDir, iNRetAndDir);


////Brief verification of the renumbering
//size_t ndofs; //Total number of dofs
//std::valarray<bool> masqRetAndNDir=std::valarray<bool>(masqRet&&(masqDir==0));
//std::valarray<bool> masqRetAndDir=std::valarray<bool>(masqRet&&masqDir);
//std::valarray<bool> masqNotRetAndDir=std::valarray<bool>((masqRet==0) && masqDir);
//size_t n1 = std::count(std::begin(masqNotConstrained), std::end(masqNotConstrained), true);
//size_t n2 = std::count(std::begin(masqTied), std::end(masqTied), true);
//size_t n3 = std::count(std::begin(masqRetAndNDir), std::end(masqRetAndNDir), true);
//size_t n4 = std::count(std::begin(masqRetAndDir), std::end(masqRetAndDir), true);
//size_t n5 = std::count(std::begin(masqNotRetAndDir), std::end(masqNotRetAndDir), true);

//ndofs=n1+n2+n3+n4+n5;
//std::cout<<" The partition in renumbering gives "<<ndofs<< " dofs "<<std::endl;
//std::cout<<" The expected number of degree of freedoms is : "<<ndof*Mh.getNumberOfNodes()<<std::endl;
//std::cout<<" Not retained Dirichlet dofs : "<<n5<<std::endl;
//std::cout<<" Retained Dirichlet dofs with applied dirichlet boundary conditions : "<<n4<<std::endl;

    }
    }
    }

}}
#endif

