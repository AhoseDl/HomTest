//#ifndef PBCANALYSIS_H
//#define PBCANALYSIS_H

#pragma once
#include<femPbc.h>
#include<bcs.h>
//#include<pbcutilities.h>
#include<taoanalysis.h>

#include<boost/qvm/mat.hpp>
#include<boost/qvm/mat_operations.hpp>
#include<boost/qvm/mat_access.hpp>
#include <boost/qvm/map_mat_mat.hpp>
//#include <boost/qvm/all.hpp> To avoid including individually the previous header file

namespace scaletag{
namespace fem{

//****************************** TractionX *********************
template <typename T, size_t D>
//PetscErrorCode ElasXAnalysis(/*int* argc, char*** argv, */MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
//    std::valarray<T>& disp, std::valarray<T>& force, T& E11, T& nu12, T& nu13)
PetscErrorCode ElasXAnalysis(/*int* argc, char*** argv, */MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                             ElementalMatrix& elementalMatrix,
                             std::valarray<T>& disp, std::valarray<T>& force, T& E11, T& nu12, T& nu13)
{
    std::cout<<"Running PBC analysis. Loading in X direction ..."<<std::endl;

    std::vector<pbcEquation> pbcLinks; /*=std::vector<pbcEquation>();*/
    PetscFunctionBeginUser;
    //scaletag::fem::pbcDispForceValues6(Mh, ndof, bc, pbcLinks, matrixValue, 0, disp, force);
    // scaletag::fem::pbcDispForceValuesPetsc(argc, argv, Mh, ndof, bc, pbcLinks, matrixValue, 0, disp, force);
     /*PetscCall(*/scaletag::fem::pbcTaoDispForce2(Mh, ndof, bc, pbcLinks, elementalMatrix, 0, disp, force)/*)*/;

    MPI_Barrier(MPI_COMM_WORLD);
    //double V=meas(Mh);
    //double Vd=meas(Mh, ndof, disp);

    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){

    //size_t testId=pbcLinks[0].getRetId(0);

    size_t iA = bc.getSommets()[0];
    size_t iB = bc.getSommets()[1];
    size_t iD = bc.getSommets()[3];
    size_t iAp = bc.getSommets()[4];

    size_t iC = bc.getSommets()[2];
    size_t iBp = bc.getSommets()[5];
    size_t iCp = bc.getSommets()[6];
    size_t iDp = bc.getSommets()[7];

    double Lx = Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
    double Ly = Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
    double Lz = Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

//    double V = Lx*Ly*Lz;
//    double sigma11, eps11, eps22, eps33;
//    double dxx=disp[ndof*iB];
//    size_t iA = bc.getSommets()[0];
//    size_t iB = bc.getSommets()[1];
//    size_t iD = bc.getSommets()[2];
//    size_t iAp = bc.getSommets()[3];


//    double Lx = Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
//    double Ly = Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
//    double Lz = Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

    double V = Lx*Ly*Lz;
    double sigma11, eps11, eps22, eps33;
    double dxx=disp[ndof*iB];

//    eps11=dxx/(Lx+dxx);
//    sigma11=((Lx+dxx)/Vd)*force[ndof*iB];
//    eps22=disp[ndof*iD+1]/(Ly+disp[ndof*iD+1]);
//    eps33=disp[ndof*iAp+2]/(Lz+disp[ndof*iAp+2]);

    eps11=dxx/(Lx);
    sigma11=((Lx)/V)*force[ndof*iB];
    eps22=disp[ndof*iD+1]/(Ly);
    eps33=disp[ndof*iAp+2]/(Lz);

    E11=sigma11/eps11; nu12=-1*eps22/eps11; nu13=-1*eps33/eps11;
    cout<<"E11 : "<< E11 <<" nu12 : "<< nu12 <<" nu13 : "<<nu13<<std::endl;
    std::cout<<"disp[ndof*iB] : "<<disp[ndof*iB]<<std::endl;
    std::cout<<"uA("<<disp[ndof*iA]<<" , "<<disp[ndof*iA+1]<<" , "<<disp[ndof*iA+2]<<")"<<std::endl;
    std::cout<<"uB("<<disp[ndof*iB]<<" , "<<disp[ndof*iB+1]<<" , "<<disp[ndof*iB+2]<<")"<<std::endl;
    std::cout<<"uD("<<disp[ndof*iD]<<" , "<<disp[ndof*iD+1]<<" , "<<disp[ndof*iD+2]<<")"<<std::endl;
    std::cout<<"uAp("<<disp[ndof*iAp]<<" , "<<disp[ndof*iAp+1]<<" , "<<disp[ndof*iAp+2]<<")"<<std::endl;
    cout<<"force[ndof*iB] : "<<force[ndof*iB]<<std::endl;

  //  std::cout<<"uC("<<disp[ndof*iC]<<" , "<<disp[ndof*iC+1]<<" , "<<disp[ndof*iC+2]<<")"<<std::endl;
   // std::cout<<"uBp("<<disp[ndof*iBp]<<" , "<<disp[ndof*iBp+1]<<" , "<<disp[ndof*iBp+2]<<")"<<std::endl;
  //  std::cout<<"uCp("<<disp[ndof*iCp]<<" , "<<disp[ndof*iCp+1]<<" , "<<disp[ndof*iCp+2]<<")"<<std::endl;
  //  std::cout<<"uDp("<<disp[ndof*iDp]<<" , "<<disp[ndof*iDp+1]<<" , "<<disp[ndof*iDp+2]<<")"<<std::endl;
    std::cout<<"sigma11 at ndof*iB : "<<sigma11<<std::endl;}

    PetscFunctionReturn(PETSC_SUCCESS);
}

//****************************** TractionY *********************
//template <typename T, size_t D>
//void ElasYAnalysis(int* argc, char*** argv, MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
//                   std::valarray<T>& disp, std::valarray<T>& force, T& E22, T& nu21, T& nu23){

//vector<pbcEquation> pbcLinks;
////scaletag::fem::pbcDispForceValues(Mh, ndof, bc, pbcLinks, matrixValue, 1, disp, force);
//    scaletag::fem::pbcDispForceValuesPetsc(argc, argv, Mh, ndof, bc, pbcLinks, matrixValue, 1, disp, force);
//size_t iA = bc.getSommets()[0];
//size_t iB = bc.getSommets()[1];
//size_t iD = bc.getSommets()[3];
//size_t iAp = bc.getSommets()[4];

//double Lx=Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
//double Ly=Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
//double Lz=Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

//double sigma22, eps11, eps22, eps33;

//double V=meas(Mh);

//sigma22=(Ly/V)*force[ndof*iD+1];
//eps11=disp[ndof*iB]/(Lx+disp[ndof*iB]);
//eps22=disp[ndof*iD+1]/(Ly+disp[ndof*iD+1]);
//eps33=disp[ndof*iAp+2]/(Lz+disp[ndof*iAp+2]);

//E22=sigma22/eps22; nu21=-1*eps11/eps22; nu23=-1*eps33/eps22;

//cout<<"E22 : "<< E22 <<" nu21 : "<< nu21 <<" nu31 : "<<nu23<<std::endl;
//}

////****************************** TractionZ *********************
//template <typename T, size_t D>
//void ElasZAnalysis(int* argc, char*** argv, MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
//                   std::valarray<T>& disp, std::valarray<T>& force, T& E33, T& nu31, T& nu32){
//vector<pbcEquation> pbcLinks;
////scaletag::fem::pbcDispForceValues(Mh, ndof, bc, pbcLinks, matrixValue, 2, disp, force);
//    scaletag::fem::pbcDispForceValuesPetsc(argc, argv, Mh, ndof, bc, pbcLinks, matrixValue, 2, disp, force);
//size_t iA = bc.getSommets()[0];
//size_t iB = bc.getSommets()[1];
//size_t iD = bc.getSommets()[3];
//size_t iAp = bc.getSommets()[4];

//double Lx=Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
//double Ly=Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
//double Lz=Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

//double sigma33, eps11, eps22, eps33;

//double V=meas(Mh);

//sigma33=(Lz/V)*force[ndof*iAp+2];
//eps11=disp[ndof*iB]/(Lx+disp[ndof*iB]);
//eps22=disp[ndof*iD+1]/(Ly+disp[ndof*iD+1]);
//eps33=disp[ndof*iAp+2]/(Lz+disp[ndof*iAp+2]);

//E33=sigma33/eps33; nu31=-1*eps11/eps33; nu32=-1*eps22/eps33;

//cout<<"E33 : "<< E33 <<" nu31 : "<< nu31 <<" nu32 : "<<nu32<<std::endl;
//}

////****************************** CisXY *********************
//template <typename T, size_t D>
//void ElasXYAnalysis(int* argc, char*** argv, MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
//                    std::valarray<T>& disp, std::valarray<T>& force, T& G12){
//vector<pbcEquation> pbcLinks;
////scaletag::fem::pbcDispForceValues(Mh, ndof, bc, pbcLinks, matrixValue, 3, disp, force);
//    scaletag::fem::pbcDispForceValuesPetsc(argc, argv, Mh, ndof, bc, pbcLinks, matrixValue, 3, disp, force);
//size_t iA = bc.getSommets()[0];
//size_t iB = bc.getSommets()[1];
//size_t iD = bc.getSommets()[3];
//size_t iAp = bc.getSommets()[4];

//double Lx=Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
//double Ly=Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
//double Lz=Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

//double tau12, gamma12;//eps11, eps22, eps33;

//double V=meas(Mh);

//tau12=(Lx/V)*force[ndof*iB+1];
//gamma12=2*disp[ndof*iB]/(Lx+disp[ndof*iB]);

//G12=tau12/gamma12;
//cout<<"G12 : "<< G12 <<std::endl;
//}

////****************************** CisXZ *********************
//template <typename T, size_t D>
//void ElasXZAnalysis(int* argc, char*** argv, MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
//                    std::valarray<T>& disp, std::valarray<T>& force, T& G13){
//vector<pbcEquation> pbcLinks;
////scaletag::fem::pbcDispForceValues(Mh, ndof, bc, pbcLinks, matrixValue, 4, disp, force);
//    scaletag::fem::pbcDispForceValuesPetsc(argc, argv, Mh, ndof, bc, pbcLinks, matrixValue, 4, disp, force);
//size_t iA = bc.getSommets()[0];
//size_t iB = bc.getSommets()[1];
//size_t iD = bc.getSommets()[3];
//size_t iAp = bc.getSommets()[4];

//double Lx=Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
//double Ly=Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
//double Lz=Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

//double tau13, gamma13;

//double V=meas(Mh);

//tau13=(Lx/V)*force[ndof*iB+2];
//gamma13=2*disp[ndof*iB+2]/(Lx+disp[ndof*iB+2]);

//G13=tau13/gamma13;
//cout<<"G13 : "<< G13 <<std::endl;
//}

////****************************** CisYZ *********************
//template <typename T, size_t D>
//void ElasYZAnalysis(int* argc, char*** argv, MeshPbc<T,D>& Mh, size_t ndof, BCs& bc, T (*matrixValue)(const MeshPbc<T,D>& Mh,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/),
//                    std::valarray<T>& disp, std::valarray<T>& force, T& G23){
//vector<pbcEquation> pbcLinks;
////scaletag::fem::pbcDispForceValues(Mh, ndof, bc, pbcLinks, matrixValue, 5, disp, force);
//    scaletag::fem::pbcDispForceValuesPetsc(argc, argv, Mh, ndof, bc, pbcLinks, matrixValue, 5, disp, force);
//size_t iA = bc.getSommets()[0];
//size_t iB = bc.getSommets()[1];
//size_t iD = bc.getSommets()[3];
//size_t iAp = bc.getSommets()[4];

//double Lx=Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
//double Ly=Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
//double Lz=Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

//double tau23, gamma23;

//double V=meas(Mh);

//tau23=(Lx/V)*force[ndof*iD+2];
//gamma23=2*disp[ndof*iD+2]/(Lx+disp[ndof*iD+2]);

//G23=tau23/gamma23;
//cout<<"G23 : "<< G23 <<std::endl;
//}

////****************************** Full analysis *********************
//void ElasFullAnalysis();



/************************ General pbc analysis(patern solely defined by (a1, a2, a3) *******************/


template <typename T, size_t D>
PetscErrorCode ElasXAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                             ElementalMatrix& elementalMatrix,
                             std::valarray<T>& disp, std::valarray<T>& force, T& E11, T& nu12, T& nu13);
template <typename T, size_t D>
PetscErrorCode ElasYAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force);
template <typename T, size_t D>
PetscErrorCode ElasZAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force);
template <typename T, size_t D>
PetscErrorCode ElasXYAnalysisGen(/*int* argc, char*** argv, */MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force);
template <typename T, size_t D>
PetscErrorCode ElasXZAnalysisGen(/*int* argc, char*** argv, */MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force);
template <typename T, size_t D>
PetscErrorCode ElasYZAnalysisGen(/*int* argc, char*** argv, */MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force);

template <typename T, size_t D>
PetscErrorCode ElasFullAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                   ElementalMatrix& elementalMatrix,
                                   std::vector<std::valarray<T>>& disps, std::vector<std::valarray<T>>& forces, T& E11, T& E22, T& E33, T& nu12, T& nu13,
                                       T& nu23, T&G12, T& G13, T& G23);
}
}

#include<pbcanalysis.tpp>


//#endif
