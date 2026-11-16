#ifndef SCALETAGFEM_H
#define SCALETAGFEM_H

#include<matrix.h>
#include<MeshPbc.h>
#include<list>
#include<valarray>
#include<pbcutilities.h>
#include<bcs.h>

#include<physics.h>

//#include<femPbc.h>
//#include "/home/ahose/Desktop/ThinkCell/kuu.h"

namespace scaletag{
namespace fem{

/*utilities for fem computation*/

//********************************* unique_insert **********************
template <typename T>
void unique_insert (std::list<T>& L, const T& j) {
 typename std::list<T>::iterator iter = L.begin(), last = L.end();
 while (iter != last && *iter < j) ++iter;
 if (iter == last || *iter != j) L.insert(iter, j);
}

//********************************* renumbering *******************************
template<typename T, size_t D>
void renumbering(const MeshPbc<T,D>& Mh, std::valarray<size_t>& num, size_t& _iNotDir, size_t ndof,const BCs& bc){

// num is the total number of dofs
// ndof is the degree of freedom per node
  num.resize(ndof*Mh.getNumberOfNodes());
  size_t iDir= 0, iNotDir = 0;
  for (size_t i = 0; i < ndof*Mh.getNumberOfNodes(); i++)
      if (bc.isDirichlet(i))
      num[i] = iDir++;
    else
      num[i] = iNotDir++;
      
_iNotDir=iNotDir; //_iNotDir is the number of dofs where dirichlet BCs are not applied
}

//********************************* Searching an integer in a valarray *************************

bool findValue(std::valarray<size_t>& vec, size_t i);


//********************************* Dimensionning ********************

template <typename T, size_t D>
void dimensionMatrix(const MeshPbc<T,D>& Mh, const std::valarray<size_t>& num, matrix<T>& a, size_t ndof,const BCs& bc,
                     std::vector<std::list<size_t> >& s)
{

  size_t n = ndof*Mh.getNumberOfNodes()-bc.getDirNumber();  // dof number where dirichlet is not applied
  s.resize(n);
  for (size_t e = 0; e < Mh.getNumberOfElements(); e++){
  size_t nv = Mh.getElements()[e].getElementSize();

    for (size_t ie = 0; ie < nv; ie++){
      for (size_t kie = 0; kie < ndof; kie++){
          size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
          if ( bc.isDirichlet(i) ) continue;
           unique_insert(s[num[i]], num[i]);
       for (size_t je = 0; je < nv; je++) {
         for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                if (j == i || bc.isDirichlet(j)) continue; //((ndof*ie+kie)==(ndof*je+kje))
                unique_insert (s[num[i]], num[j]);
            }}
      }

    }
    }
  a.resize (s, n);
}

//********************************* Filling *************************
template <typename T, size_t D>
void fillMatrix(const MeshPbc<T,D>& Mh,  const std::valarray<size_t>& num, matrix<T>& a, size_t ndof, const BCs& bc,
                T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje)
               )
{
  for (size_t e = 0; e < Mh.getNumberOfElements(); e++) {
    size_t nv=Mh.getElements()[e].getElementSize();

    for (size_t ie = 0; ie < nv; ie++) {
            for (size_t kie = 0; kie < ndof; kie++){
            size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
            if (bc.isDirichlet(i)) continue;
            a.at(num[i],num[i])   += matrixValue(Mh, e, ie, ie,kie, kie);
            for (size_t je = 0; je < nv; je++) {
            for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                if (j == i || bc.isDirichlet(j)) continue;
                a.at(num[i],num[j]) += matrixValue(Mh, e, ie, je,kie,kje);

            }}
      }
    }//End of element e computation
  }
}


//********************************* Assembly *****************************
template <typename T, size_t D>
void femAssembly(const MeshPbc<T,D>& Mh, std::valarray<size_t>& num,
    matrix<T>& a, size_t ndof,const BCs& bc,
    T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje)
    ){

  std::vector<std::list<size_t> > s;
     dimensionMatrix(Mh, num, a, ndof, bc,s);
     fillMatrix(Mh, num,a, ndof, bc, matrixValue);
    }
/********************************   Global Section *****************************/
//********************************* Global Dimensionning ********************

template <typename T, size_t D>
void globDimensionMatrix(const MeshPbc<T,D>& Mh, const std::valarray<size_t>& num, matrix<T>& a, size_t ndof,const BCs& bc,
                     std::vector<std::list<size_t> >& s)
{

     size_t n = ndof*Mh.getNumberOfNodes();
     size_t nm = bc.getNotMeshDofs(ndof).size();
     n -= nm; //Kelvin Marc algo need this
   //  if( (bc.getNotMeshDofs(ndof).size()) != 0) n -= (bc.getNotMeshDofs(ndof).size()) ;
     s.resize(n);
     for (size_t e = 0; e < Mh.getNumberOfElements(); e++){
    size_t nv = Mh.getElements()[e].getElementSize();

    for (size_t ie = 0; ie < nv; ie++){
      for (size_t kie = 0; kie < ndof; kie++){
            size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
            unique_insert(s[num[i]], num[i]);
            for (size_t je = 0; je < nv; je++) {
            for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                if (j == i ) continue;
                unique_insert (s[num[i]], num[j]);
            }}
      }

    }
     }
     a.resize (s, n);
}

//********************************* Global Filling *************************
template <typename T, size_t D>
void globFillMatrix(const MeshPbc<T,D>& Mh,  const std::valarray<size_t>& num, matrix<T>& a, size_t ndof, const BCs& bc,
//                T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje)
 ElementalMatrix& elementaltMatrix )
{

     //size_t n = Mh.getNumberOfElements();
     for (size_t e = 0; e < Mh.getNumberOfElements(); e++) {
    elementaltMatrix(e); //This generates relevant data for element e for per dof contribution computing
    size_t nv=Mh.getElements()[e].getElementSize();

    for (size_t ie = 0; ie < nv; ie++) {
      for (size_t kie = 0; kie < ndof; kie++){
            size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
            a.at(num[i],num[i])   += /*matrixValue(Mh, e, ie, ie,kie, kie)*/elementaltMatrix( e, ie, ie,kie, kie);
            for (size_t je = 0; je < nv; je++) {
            for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                if (j == i ) continue;
                a.at(num[i],num[j]) += /*matrixValue(Mh, e, ie, je,kie,kje)*/elementaltMatrix( e, ie, je,kie, kje);
            }}
      }
    }//End of element e computation
     }
}

//********************************* Global Assembly *****************************
template <typename T, size_t D>
void globFemAssembly(const MeshPbc<T,D>& Mh, std::valarray<size_t>& num,
                 matrix<T>& a, size_t ndof,const BCs& bc,
//                 T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/)
    ElementalMatrix& elementaltMatrix){

     std::vector<std::list<size_t> > s;
     globDimensionMatrix(Mh, num, a, ndof, bc,s);
     globFillMatrix(Mh, num,a, ndof, bc, elementaltMatrix);
}

    /********************************* Endo of Global Section ********************/


/********************************   Computation with diagonal dirichlet Section *****************************/
template <typename T, size_t D>
void dimMatWithDir(const MeshPbc<T,D>& Mh, const std::valarray<size_t>& num, matrix<T>& a, size_t ndof,const BCs& bc,
                     std::vector<std::list<size_t> >& s)
{
     size_t n = ndof*Mh.getNumberOfNodes();
     s.resize(n);
     for (size_t e = 0; e < Mh.getNumberOfElements(); e++){
    size_t nv = Mh.getElements()[e].getElementSize();
    for (size_t ie = 0; ie < nv; ie++){
      for (size_t kie = 0; kie < ndof; kie++){
            size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
            if ( bc.isDirichlet(i) ) continue;
            unique_insert(s[num[i]], num[i]);
            for (size_t je = 0; je < nv; je++) {
            for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                if (j == i ) continue;
                unique_insert (s[num[i]], num[j]);
            }}
      }
    }
     }
     std::vector<size_t> dirDofs=bc.getDirichletDofs();
     for(size_t i=0; i<dirDofs.size(); i++) unique_insert(s[num[dirDofs[i]]], num[dirDofs[i]]);
     a.resize (s, n);
}


template <typename T, size_t D>
void fillMatWithDir(const MeshPbc<T,D>& Mh,  const std::valarray<size_t>& num, matrix<T>& a, size_t ndof, const BCs& bc,
                T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje)
                )
{
     size_t n = ndof*Mh.getNumberOfNodes();

     for (size_t e = 0; e < Mh.getNumberOfElements(); e++){
    size_t nv = Mh.getElements()[e].getElementSize();

    for (size_t ie = 0; ie < nv; ie++){
      for (size_t kie = 0; kie < ndof; kie++){
            size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
            if ( bc.isDirichlet(i) ) continue;
            //a.at(num[i],num[i]) += matrixValue(Mh, e, ie, je,kie,kje);
            for (size_t je = 0; je < nv; je++) {
            for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                //if (j == i ) continue;
                a.at(num[i],num[j]) += matrixValue(Mh, e, ie, je,kie,kje);
            }}
      }
    }
    }
     std::vector<size_t> dirDofs=bc.getDirichletDofs();
     for(size_t i=0; i<dirDofs.size(); i++) a.at( num[dirDofs[i]], num[dirDofs[i]]) += 1.0;
}


template <typename T, size_t D>
void assemblyMatWithDir(const MeshPbc<T,D>& Mh, std::valarray<size_t>& num,
                 matrix<T>& a, size_t ndof,const BCs& bc,
                 T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje)
                 ){
     std::vector<std::list<size_t> > s;
     dimMatWithDir(Mh, num, a, ndof, bc,s);
     fillMatWithDir(Mh, num,a, ndof, bc, matrixValue);
}
/********************************   End of Computation with diagonal dirichlet Section *****************************/



//********************************* matrix value *************************
template <typename T=double, size_t D=3>
T matrixValueTetra(const MeshPbc<T,D>& Mh, size_t e,size_t ie, size_t je, size_t kie, size_t kje/*, size_t ndof*/){
//T matrixValueTetra(const MeshPbc<T,D>& Mh, size_t e,size_t ie, size_t je, size_t kie, size_t kje){
     double E, nu, lambda, mu;
     E=/*0.5*/608.; nu=0.4;
     lambda=(nu*E)/((1+nu)*(1-2*nu));
     mu=(E)/(2*(1+nu));

/*T N1(double xi, double eta, double zeta){return (1 - xi - eta - zeta);};
T N2(double xi, double eta, double zeta){return (xi);};
T N3(double xi, double eta, double zeta){return (eta);};
T N4(double xi, double eta, double zeta){return (zeta);};*/

//U(*)(double, double, double, double) tabN[4]={N1, N2, N3, N4};

     //size_t i=ndof*Mh.getConnectivity(e)[ie]+kie;
    // size_t j=ndof*Mh.getConnectivity(e)[je]+kje;
double xA,yA,zA, xB,yB,zB, xC,yC,zC, xD,yD,zD,   detTau;
double A11,A22,A33, A12,A21, A13,A31, A23,A32; // commatrice coefficients
//i.e To invert the transformation matrix, one need to transpose the matrix A.

//xA=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[0] -1].getXi(0);
//yA=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[0] -1].getXi(1);
//zA=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[0] -1].getXi(2);
xA=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[0]]).getXi(0);
yA=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[0]]).getXi(1);
zA=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[0]]).getXi(2);


//xB=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[1] -1].getXi(0);
//yB=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[1] -1].getXi(1);
//zB=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[1] -1].getXi(2);
xB=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[1]]).getXi(0);
yB=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[1]]).getXi(1);
zB=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[1]]).getXi(2);

//xC=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[2] -1].getXi(0);
//yC=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[2] -1].getXi(1);
//zC=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[2] -1].getXi(2);
xC=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[2]]).getXi(0);
yC=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[2]]).getXi(1);
zC=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[2]]).getXi(2);

//xD=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[3] -1].getXi(0);
//yD=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[3] -1].getXi(1);
//zD=Mh.getNodes()[ Mh.getElements()[e].getConnectivity()[3] -1].getXi(2);
xD=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[3] ]).getXi(0);
yD=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[3]]).getXi(1);
zD=Node<T,D>(Mh.getNodes()[ Mh.getConnectivity(e)[3]]).getXi(2);


detTau= (-xA+xB)*(-yA+yC)*(-zA+zD)-(-xA+xB)*(-zA+zC)*(-yA+yD)
         -(-yA+yB)*(-xA+xC)*(-zA+zD)+(-yA+yB)*(-zA+zC)*(-xA+xD)
         +(-zA+zB)*(-xA+xC)*(-yA+yD)-(-zA+zB)*(-yA+yC)*(-xA+xD);

if(detTau<=1.e-7) std::cout<< "volume of element "<<e<<" is less than 1.e-7. vol : "<<detTau<<std::endl;
//Aij for commatrix of transformation
//Tau-1 is then the transpose of A divided by detTau
A11=( (-yA+yC)*(-zA+zD) - (-zA+zC)*(-yA+yD) );
A22=( (-xA+xB)*(-zA+zD) - (-zA+zB)*(-xA+xD)  );
A33=( (-xA+xB)*(-yA+yC) - (-yA+yB)*(-xA+xC) );

A12=-1*( (-yA+yB)*(-zA+zD) - (-zA+zB)*(-yA+yD) );
A21=-1*( (-xA+xC)*(-zA+zD) - (-zA+zC)*(-xA+xD) );

A13=( (-yA+yB)*(-zA+zC) - (-zA+zB)*(-yA+yC) );
A31=( (-xA+xC)*(-yA+yD) - (-yA+yC)*(-xA+xD) );

A23=-1*( (-xA+xB)*(-zA+zC) - (-zA+zB)*(-xA+xC) );
A32=-1*( (-xA+xB)*(-yA+yD) - (-yA+yB)*(-xA+xD) );

//T GradN1[]{-1, -1, -1}; T GradN2[]{1, 0, 0}; T GradN3[]{0, 1, 0}; T GradN4[]{0, 0, 1};  // The real values must be gotten by multiplying every line by Grad xi/x.

T GradN1[]{(-A11-A12-A13)/detTau, (-A21-A22-A23)/detTau, (-A31-A32-A33)/detTau};
T GradN2[]{A11/detTau, A21/detTau, A31/detTau};
T GradN3[]{A12/detTau, A22/detTau, A32/detTau};
T GradN4[]{A13/detTau, A23/detTau, A33/detTau};  // The real values must be gotten by multiplying every line by Grad xi/x.


T tabGradN[4][3] = { {GradN1[0],GradN1[1],GradN1[2]}, {GradN2[0],GradN2[1],GradN2[2]}, {GradN3[0],GradN3[1],GradN3[2]}, {GradN4[0],GradN4[1],GradN4[2]} };

//return (tabGradN[ie][kie])*C*(tabGradN[je][kje]);

return (1./6.)*detTau*( lambda*(tabGradN[ie][kie])*(tabGradN[je][kje]) + mu*( (tabGradN[ie][kje])*(tabGradN[je][kie])
    +(kie==kje)*( (tabGradN[ie][0])*(tabGradN[je][0])+(tabGradN[ie][1])*(tabGradN[je][1])+(tabGradN[ie][2])*(tabGradN[je][2]) ) )  );

//One (1/6) is a scaling factor to have the exact value. See Thomas Hughes, The fem; page 199

}

//************************************************************************












template<typename T, size_t D>
void KuuDimensionMatrix(const MeshPbc<T,D>& Mh, const std::valarray<size_t>& num, matrix<T>& a, size_t ndof,const BCs& bc,
                        std::vector<std::list<size_t> >& s)
{

size_t n = ndof*Mh.getNumberOfNodes();
size_t nu = n-bc.getDirichletDofs().size();
s.resize(n-bc.getDirichletDofs().size());
for (size_t e = 0; e < Mh.getNumberOfElements(); e++){
    size_t nv = Mh.getElements()[e].getElementSize();

    for (size_t ie = 0; ie < nv; ie++){
      for (size_t kie = 0; kie < ndof; kie++){
            size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
            if(bc.isDirichlet(i)) continue;
            unique_insert(s[num[i]], num[i]);
            for (size_t je = 0; je < nv; je++) {
            for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                if (j == i || bc.isDirichlet(j)) continue;
                if(num[j]>=nu)std::cout<<"col issue in mat dim j : "<<j<<" numPbc[j] : "<<num[j]<<std::endl;
                unique_insert(s[num[i]], num[j]);
            }}
      }

    }
}
a.resize (s, n-bc.getDirichletDofs().size());
}


template <typename T, size_t D>
void KuuFillMatrix(const MeshPbc<T,D>& Mh,  const std::valarray<size_t>& num, matrix<T>& a, size_t ndof, const BCs& bc,
                   T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje)
                   )
{
for (size_t e = 0; e < Mh.getNumberOfElements(); e++) {
    size_t nv=Mh.getElements()[e].getElementSize();

    for (size_t ie = 0; ie < nv; ie++) {
      for (size_t kie = 0; kie < ndof; kie++){
            size_t i = ndof*(Mh.getConnectivity(e)[ie])+kie;
            if(bc.isDirichlet(i)) continue;
            a.at(num[i],num[i])   += matrixValue(Mh, e, ie, ie,kie, kie);
            for (size_t je = 0; je < nv; je++) {
            for (size_t kje = 0; kje < ndof; kje++){
                size_t j = ndof*(Mh.getConnectivity(e)[je])+kje;
                if (j == i || bc.isDirichlet(j)) continue;
                a.at(num[i],num[j]) += matrixValue(Mh, e, ie, je,kie,kje);

            }}
      }
    }//End of element e computation
}
}



template <typename T=double, size_t D=3>
void KuuAssembly(const MeshPbc<T,D>& Mh, std::valarray<size_t>& num,
                 matrix<T>& a, size_t ndof,const BCs& bc,
                 T (*matrixValue)(const MeshPbc<T,D>& Th,size_t e,size_t ie, size_t je, size_t kie, size_t kje)
                 ){

std::vector<std::list<size_t> > s;
KuuDimensionMatrix(Mh, num, a, ndof, bc,s);
KuuFillMatrix(Mh, num,a, ndof, bc, matrixValue);
}
}}

#endif
