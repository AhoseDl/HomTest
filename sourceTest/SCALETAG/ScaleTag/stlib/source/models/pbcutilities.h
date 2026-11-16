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
#include<limits>

#include<boost/qvm/mat.hpp>
#include<boost/qvm/mat_operations.hpp>
#include<boost/qvm/mat_access.hpp>
#include <boost/qvm/map_mat_mat.hpp>

namespace scaletag {
namespace models {


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
    vector<size_t>& sommets, std::vector<size_t>& notMeshNodes) ;

    void createPbcLinks(MeshPbc<double,3>& meshPbc, /*std::vector<double>& tilingDirs,*/ /*const fem::MeshData& meshData,*/  size_t& NbLinks,
                        vector<size_t>& sommets, std::vector<size_t>& notMeshNodes, vector<pbcEquation>& pbcLinks) ;

    std::vector<std::list<size_t> > getMatrixStructure(const MeshPbc<double,3>& Th, const vector<pbcEquation>& pbcLinks,
      const std::valarray<size_t>& numPbc, const fem::BCs& bc, size_t ndof) const;

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


    std::vector<std::vector<double>> getTilingDirs() const;
    void setTilingDirs(const std::vector<std::vector<double>>& _tilingDirs);

private:
//    std::vector<pbcEquation> pbcEqs;
//    std::vector<size_t> sommets;
    std::vector<std::vector<double>> tilingDirs;
signals:

};

} // namespace models
} // namespace scaletag

#endif // SCALETAG_MODELS_PBCUTILITIES_H
