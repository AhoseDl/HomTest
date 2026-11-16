#ifndef BCS_H
#define BCS_H

#include<vector>
#include<algorithm>
#include<array>
#include<list>
#include<pbcequation.h>
//#include<pbcutilities.h>
#include<MeshPbc.h>

namespace scaletag{

namespace models {
class PbcUtilities;
}
	namespace fem{
// A B C D Ap Bp Cp Dp  sommets
// 0 1 2 3 4  5  6  7


class BCs{
private:
    scaletag::models::PbcUtilities* pbc;
    std::vector<size_t> notMeshNodes;
    std::vector<size_t> notMeshDofs;
    std::vector<size_t> sommets;  //computer memory numbers
	std::vector<size_t> DirichletDofs;
    size_t loadingType=0;//0->TrX, 1->TrY, 2->TrZ, 3->CisXY, 4->CisXZ, 5->CisYZ
    double delta = 0.01/*0.2*//*0.45*/;
    std::vector<size_t> retainedDofs;
    std::vector<size_t> tiedDofs; // retained and tied nodes did not include dirichlet dofs
  //  std::vector<size_t> notConstrainedDofs; // this set does not include neither Dirichlet neither retained neither tied dofs
    //size_t renumRetNumber=0;
    //size_t renumTiedNumber=0;
    size_t nDir, nRet, nTied, nNotCstr, nRetAndDir, nRetAndNDir, nNRetAndDir;
    size_t meshAndNDir=0, meshAndDir=0, NMeshAndDir=0, NMeshAndNDir=0;

    std::vector<std::vector<double>> tilingDirs;



public:

    void initialize();

    models::PbcUtilities* getPbcUtilities() const;
    void setPbcUtilities(models::PbcUtilities* _pbc);

    void setTilingDirs(const std::vector<std::vector<double>>& _tilingDirs);
    std::vector<std::vector<double>> getTilingDirs() const;

    void setRefCount(size_t _meshAndNDir, size_t _meshAndDir, size_t _NMeshAndDir,
                     size_t _NMeshAndNDir);

    void getRefCount(size_t& _meshAndNDir, size_t& _meshAndDir, size_t& _NMeshAndDir,
                     size_t& _NMeshAndNDir) const;

    std::vector<size_t> getDirichletDofs() const;
    //std::valarray<double>getDirichletValues(){}

    //size_t getRenumRetainedNumber()const{return renumRetNumber;}
   // size_t getRenumTiedNumber()const{return renumTiedNumber;}
    //void setRenumRetNumber(size_t _renumRetNumber){renumRetNumber=_renumRetNumber;}
    //void setRenumTiedNumber(size_t _renumTiedNumber){renumTiedNumber=_renumTiedNumber;}
    size_t getDirNumber() const;
    size_t getTiedNumber() const;
    size_t getRetainedNumber() const;
    size_t getNotConstraindedDofs() const;
    double getPertubation() const;
    void setPertubation(double _delta) ;
    void setLoadingType(size_t solType);
    size_t getLoadingType() const;


    void actualizePbc(const std::vector<size_t>& tdofs,const std::vector<size_t>& rdofs);

    void actualizePbc(size_t _nDir, size_t _nRet, size_t _nTied, size_t _nNotCstr,
                      size_t _nRetAndDir, size_t _nRetAndNDir, size_t _nNRetAndDir);
    //template<typename T, size_t D>

    std::vector<size_t> getNotMeshNodes() const;

    std::vector<size_t> getNotMeshDofs(size_t ndof) const;

    void setNotMeshDofs(size_t ndof);

    void setNotMeshNodes(const std::vector<size_t>& _notMeshNodes, size_t ndof);

    void setPbc(const std::vector<pbcEquation>& pbcLinks, size_t ndof, const std::vector<size_t>& _sommets);

    bool isTied(const size_t _dof) const;

    bool isRetained(const size_t _dof) const;

    bool isDirichlet(size_t i) const ;

    bool isNotConstrainedDofs(size_t i) const;

    bool isMeshDof(size_t i) const;


    void addDirichlet(std::vector<size_t> vec);

    void setSommets(std::vector<size_t> _sommets);

    vector<size_t> getSommets() const;
	
    //TractionX
    void setTractionX(size_t ndof=1);
	
    //TractionY
    void setTractionY(size_t ndof=1);
	
    //TractionZ
    void setTractionZ(size_t ndof=1);
	
    //CisXY
    void setCisXY(size_t ndof=1);
	
    //CisXZ
    void setCisXZ(size_t ndof=1);
	
    //CisYZ
    void setCisYZ(size_t ndof=1);

    //dirichletValue
    double pbcDirichletValue(size_t i, size_t ndof) const;


    void setGeneralPbcBoundaryCondition();
//    double pbcDirichletValue(const MeshPbc<double, 3>& meshPbc, const std::vector<int>& controlNodes,
//                             int sollType, size_t vertDof, double delta) const;
    double pbcDirichletValue(const MeshPbc<double,3>& meshPbc, const std::vector<int>& controlNodes,
                             int sollType, size_t vertDof, double delta) const;

};


// bool isDirichlet(const BCs& bc, size_t i){
// return bc.isDirichlet(i);
//}

}}
#endif
