#ifndef PBCUTILITIES_H
#define PBCUTILITIES_H

#include <QObject>
#include<MeshPbc.h>
#include<bitset>
#include<node.h>
#include<cmath>
#include<cassert>
#include<QDebug>
#include<bcs.h>
#include<valarray>
#include<vector>
#include<list>
#include<matrix.h>
#include<pbcequation.h>

namespace scaletag {
namespace models {

//struct pbcEquation{
//    size_t nRet;
//    std::vector<size_t> ptIds; //<tn, rt1, rt2, ..., rt_nRet>
//    std::vector<double> coeffs; //nRet coefficients

//    pbcEquation(size_t _nRet, std::vector<size_t> _ptIds, std::vector<double> _coeffs){
//        nRet=_nRet; ptIds=_ptIds; coeffs=_coeffs;
//    }
//    size_t getTiedId() const{return ptIds[0];}
//    size_t getRetId(size_t i) const {return ptIds[i+1];}  // i=0 returns the firs ret id
//    double getCoeff(size_t i) const {return coeffs[i];}  //returns the coeff of ret with id=i

//};

class PbcUtilities : public QObject
{
    Q_OBJECT
public:
    explicit PbcUtilities(QObject *parent = nullptr);
    void structBox(const MeshPbc<double,3>&, double& Xmin, double& Xmax, double& Ymin, double& Ymax, double& Zmin, double& Zmax) ;
    void nodesSelect(const MeshPbc<double,3>& meshPbc, /*string box,*/ bitset<6>bs, double epsi, vector<size_t>& Output) ;

    void edgesLinks(const MeshPbc<double,3>& meshPbc, const vector<size_t>& Sommets, int ind, bool trans,const vector<size_t>& v1,
          const vector<size_t>& v2,double epsi, std::vector<pbcEquation>& sortie) ;

    void facesLinks(const MeshPbc<double,3>& meshPbc, const vector<size_t>& Sommets, int ind, const vector<size_t>& v1,
    const vector<size_t>& v2, double epsi, vector<pbcEquation>& sortie) ;

    void createPbcLinks(MeshPbc<double,3>& meshPbc, vector<pbcEquation>& pbcLinks, size_t& NbLinks,
    vector<size_t>& sommets) ;

    std::vector<std::list<size_t> > getMatrixStructure(const MeshPbc<double,3>& Th, const vector<pbcEquation>& pbcLinks,
      const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof) const;

    std::vector<std::list<size_t> > getGlobMatrixStructure(const MeshPbc<double,3>& Th, const vector<pbcEquation>& pbcLinks,
                                                       const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof) const;

    void createConstraintMatrix(const MeshPbc<double,3>& Th, const vector</*array<size_t,3> */pbcEquation>& pbcLinks,
                                const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                fem::matrix<double>& mat)const;
    void createConstraintVector(const MeshPbc<double,3>& Th, const vector</*array<size_t,3> */pbcEquation>& pbcLinks,
                                const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                std::valarray<double>& vec) const;

    void createConstraintGlobMatrix(const MeshPbc<double,3>& Th, const vector</*array<size_t,3> */pbcEquation>& pbcLinks,
                                const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                fem::matrix<double>& mat)const;
    void createConstraintGlobVector(const MeshPbc<double,3>& Th, const vector</*array<size_t,3> */pbcEquation>& pbcLinks,
                                const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                std::valarray<double>& vec) const;

    void createPbcConstraintsMatrix(const MeshPbc<double,3>& Th, const vector</*array<size_t,3> */pbcEquation>& pbcLinks,
                                    const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                    fem::matrix<double>& mat)const;
    void createPbcConstraintsVector(const MeshPbc<double,3>& Th, const vector</*array<size_t,3> */pbcEquation>& pbcLinks,
                                    const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                    std::valarray<double>& vec) const;

private:
    std::vector<pbcEquation> pbcEqs;
    std::vector<size_t> sommets;
signals:

};

} // namespace models
} // namespace scaletag

#endif // SCALETAG_MODELS_PBCUTILITIES_H
