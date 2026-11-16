#include<pbcutilities.h>
#include<bcs.h>

namespace scaletag{
namespace fem{


//double BCs::pbcDirichletValue(const MeshPbc<double,3>& meshPbc, const std::vector<int>& controlNodes,
//                              int sollType, size_t vertDof, double delta) const{

//        //vertDof is a given dof on a control vertex.
//        //This function returns Dirichlet periodic boundary conditions related to vertDof

//        if(vertDof.size() != 4) return std::numeric_limits<double>::infinity ;
//        int nA = controlNodes[0];
//        int nB = controlNodes[1];
//        int nD = controlNodes[2];
//        int nAp = controlNodes[3];

////        int nA = controlNodes[0];
////        int nB = controlNodes[1];
////        int nD = controlNodes[3];
////        int nAp = controlNodes[4];

//        double xBA, xDA, xApA,   yBA, yDA, yApA,  zBA, zDA, zApA; //coordMM' =coord(M')-coord(M)
//        xBA = meshPbc.getNodes()[nB].getXi(0) - meshPbc.getNodes()[nA].getXi(0);
//        xDA = meshPbc.getNodes()[nD].getXi(0) - meshPbc.getNodes()[nA].getXi(0);
//        xApA = meshPbc.getNodes()[nAp].getXi(0) - meshPbc.getNodes()[nA].getXi(0);

//        yBA = meshPbc.getNodes()[nB].getXi(1) - meshPbc.getNodes()[nA].getXi(1);
//        yDA = meshPbc.getNodes()[nD].getXi(1) - meshPbc.getNodes()[nA].getXi(1);
//        yApA = meshPbc.getNodes()[nAp].getXi(1) - meshPbc.getNodes()[nA].getXi(1);

//        zBA = meshPbc.getNodes()[nB].getXi(2) - meshPbc.getNodes()[nA].getXi(2);
//        zDA = meshPbc.getNodes()[nD].getXi(2) - meshPbc.getNodes()[nA].getXi(2);
//        zApA = meshPbc.getNodes()[nAp].getXi(2) - meshPbc.getNodes()[nA].getXi(2);


//        int nAdof0 = 3*nA+0; int nAdof1 = 3*nA+1; in nAdof2 = 3*nA+2;

//        int nBdof0 = 3*nB+0; int nBdof1 = 3*nB+1; in nBdof2 = 3*nB+2;

//        int nDdof0 = 3*nD+0; int nDdof1 = 3*nD+1; in nDdof2 = 3*nD+2;

//        int nApdof0 = 3*nAp+0; int nApdof1 = 3*nAp+1; in nApdof2 = 3*nAp+2;

//    /* Uniaxial deformations case */

//        if(sollType == 0 /*deformation xx*/){
//            if(vertDof == nBdof0) return delta*xBA; else
//                if(vertDof == nDdof0) return delta*xDA; else
//                if(vertDof == nApdof0) return delta*xApA;
//            else return 0. ;
//        }

//        if(sollType == 1 /*deformation yy*/){
//            if(vertDof == nBdof1) return delta*yBA; else
//                if(vertDof == nDdof1) return delta*yDA; else
//                if(vertDof == nApdof1) return delta*yApA;
//            else return 0. ;
//        }

//        if(sollType == 2 /*deformation zz*/){
//            if(vertDof == nBdof2) return delta*zBA; else
//                if(vertDof == nDdof2) return delta*zDA; else
//                if(vertDof == nApdof2) return delta*zApA;
//            else return 0. ;
//        }

//    /* Shear deformations case */

//        if(sollType == 3 /*shear xy*/){
//            if(vertDof == nBdof0) return delta*yBA; else
//                if(vertDof == nBdof1) return delta*xBA; else /**/
//                if(vertDof == nDdof0) return delta*yDA; else
//                if(vertDof == nDdof1) return delta*xDA; else /**/
//                if(vertDof == nApdof0) return delta*yApA; else
//                if(vertDof == nApdof1) return delta*xApA;
//            else return 0. ;
//        }

//        if(sollType == 4 /*shear xz*/){
//            if(vertDof == nBdof0) return delta*zBA; else
//                if(vertDof == nBdof2) return delta*xBA; else /**/
//                if(vertDof == nDdof0) return delta*zDA; else
//                if(vertDof == nDdof2) return delta*xDA; else /**/
//                if(vertDof == nApdof0) return delta*zApA; else
//                if(vertDof == nApdof2) return delta*xApA;
//            else return 0. ;
//        }

//        if(sollType == 5 /*shear yz*/){
//            if(vertDof == nBdof1) return delta*zBA; else
//            if(vertDof == nBdof2) return delta*yBA; else /**/
//                if(vertDof == nDdof1) return delta*zDA; else
//                if(vertDof == nDdof2) return delta*yDA; else /**/
//                if(vertDof == nApdof1) return delta*zApA; else
//                if(vertDof == nApdof2) return delta*yApA;
//            else return 0. ;
//        }

//    }



void BCs::initialize(){
    pbc=nullptr;
    nDir=nRet=nTied=nNotCstr=nRetAndDir=nRetAndNDir=nNRetAndDir
        =meshAndNDir=0, meshAndDir=0, NMeshAndDir=0, NMeshAndNDir=0;
    notMeshNodes.clear(); notMeshDofs.clear(); sommets.clear(); DirichletDofs.clear();
    retainedDofs.clear();
    tiedDofs.clear();
    delta=0.01;
}
models::PbcUtilities* BCs::getPbcUtilities() const {return pbc;}
void BCs::setPbcUtilities(models::PbcUtilities* _pbc){pbc=_pbc;}

void BCs::setTilingDirs(const std::vector<std::vector<double>>& _tilingDirs){tilingDirs=_tilingDirs;}
std::vector<std::vector<double>> BCs::getTilingDirs() const{return tilingDirs;}

void BCs::setRefCount(size_t _meshAndNDir, size_t _meshAndDir, size_t _NMeshAndDir,
                 size_t _NMeshAndNDir)
{meshAndDir=_meshAndDir; meshAndNDir=_meshAndNDir;
    NMeshAndDir=_NMeshAndDir; NMeshAndNDir=_NMeshAndNDir;}

void BCs::getRefCount(size_t& _meshAndNDir, size_t& _meshAndDir, size_t& _NMeshAndDir,
                 size_t& _NMeshAndNDir) const
{_meshAndDir=meshAndDir; _meshAndNDir=meshAndNDir;
    _NMeshAndDir=NMeshAndDir; _NMeshAndNDir=NMeshAndNDir;}

std::vector<size_t> BCs::getDirichletDofs() const{return DirichletDofs;}
//std::valarray<double>getDirichletValues(){}

//size_t getRenumRetainedNumber()const{return renumRetNumber;}
// size_t getRenumTiedNumber()const{return renumTiedNumber;}
//void setRenumRetNumber(size_t _renumRetNumber){renumRetNumber=_renumRetNumber;}
//void setRenumTiedNumber(size_t _renumTiedNumber){renumTiedNumber=_renumTiedNumber;}
size_t BCs::getDirNumber()const{return DirichletDofs.size();}
size_t BCs::getTiedNumber()const{return tiedDofs.size();}
size_t BCs::getRetainedNumber()const{return retainedDofs.size();}
size_t BCs::getNotConstraindedDofs()const{return nNotCstr;}
double BCs::getPertubation() const {return delta;}
void BCs::setPertubation(double _delta) {delta = _delta;}
void BCs::setLoadingType(size_t solType){loadingType = solType;}
size_t BCs::getLoadingType() const {return loadingType;}


void BCs::actualizePbc(const std::vector<size_t>& tdofs,const std::vector<size_t>& rdofs){tiedDofs=tdofs;
    retainedDofs=rdofs;}

void BCs::actualizePbc(size_t _nDir, size_t _nRet, size_t _nTied, size_t _nNotCstr,
                  size_t _nRetAndDir, size_t _nRetAndNDir, size_t _nNRetAndDir){
    nDir=_nDir; nRet=_nRet; nTied=_nTied; nNotCstr=_nNotCstr;
    nRetAndDir=_nRetAndDir; nRetAndNDir=_nRetAndNDir; nNRetAndDir=_nNRetAndDir;
}
//template<typename T, size_t D>

std::vector<size_t> BCs::getNotMeshNodes() const {return notMeshNodes;}

std::vector<size_t> BCs::getNotMeshDofs(size_t ndof) const {
    std::vector<size_t> dofs;
    //if( !(notMeshNodes.empty()) ){
    dofs.resize(ndof*notMeshNodes.size());

    for(size_t i=0; i<notMeshNodes.size(); i++)
        for(size_t j=0; j<ndof; j++) dofs[ndof*i+j] = ndof*notMeshNodes[i]+j;

    return dofs;//}
    //else return std::vector<size_t>();
}
void BCs::setNotMeshDofs(size_t ndof){
    notMeshDofs.clear();
    notMeshDofs=getNotMeshDofs(ndof);
}

void BCs::setNotMeshNodes(const std::vector<size_t>& _notMeshNodes, size_t ndof){
    //ndof is the number of dofs per node
    notMeshNodes=_notMeshNodes;
    notMeshDofs.clear();
    setNotMeshDofs(ndof);}

void BCs::setPbc(const std::vector<pbcEquation>& pbcLinks, size_t ndof, const std::vector<size_t>& _sommets){
    //This function takes the parameter pbcLinks composed of <tn, rt1, rt2, ... , rtn> and fills
    //tiedDofs and retainedDofs of boundary condition Bcs. One may note that in general a tied node
    //cannot be also a retained one. In preprocessing(ie creating pbcLinks), it is a good idea to choose
    //retained and tied sets totally disjointed and boundary conditions are put only on retained nodes.
    tiedDofs.clear();
    retainedDofs.clear();
    //        size_t iA = _sommets[0];
    //        retainedDofs.push_back(ndof*iA);
    //        retainedDofs.push_back(ndof*iA+1);
    //        retainedDofs.push_back(ndof*iA+2);

    for(size_t kndof=0; kndof<ndof; kndof++){

        for(size_t i=0; i<pbcLinks.size(); i++){
            size_t tId = pbcLinks[i].getTiedId();
            size_t tdof=ndof*( tId ) + kndof;
            if(!isDirichlet(tdof))   tiedDofs.push_back( tdof );

            for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
                size_t retId = pbcLinks[i].getRetId(iRetPbc);
                size_t rtdof=ndof*( retId ) + kndof;
                if(/*!isDirichlet( rtdof  )*/ true)  retainedDofs.push_back( rtdof );//modified 21-07-2025 --true
            }}
    }

    std::sort( tiedDofs.begin(), tiedDofs.end());
    std::sort( retainedDofs.begin(), retainedDofs.end());
    auto lastTiedDofs = std::unique(tiedDofs.begin(), tiedDofs.end());
    auto lastRetainedDofs = std::unique(retainedDofs.begin(), retainedDofs.end());
    tiedDofs.erase(lastTiedDofs, tiedDofs.end());
    retainedDofs.erase(lastRetainedDofs, retainedDofs.end());

    /* The fixed vertex dofs(A) is also member of retained dofs set.
         * This is taken in account when renumbering the dofs(via actualize pbc)*/

    //        std::vector<size_t> retret=retainedDofs;
    //        std::vector<size_t> interTiedRet, interTiedDir, interRetDir;
    //        std::set_intersection(tiedDofs.begin(), tiedDofs.end(), retainedDofs.begin(), retainedDofs.end(), std::back_inserter(interTiedRet));
    //        std::set_intersection(tiedDofs.begin(), tiedDofs.end(), DirichletDofs.begin(), DirichletDofs.end(), std::back_inserter(interTiedDir));
    //        std::set_intersection(retainedDofs.begin(), retainedDofs.end(), DirichletDofs.begin(), DirichletDofs.end(), std::back_inserter(interRetDir));

}

bool BCs::isTied(const size_t _dof)const{
    const auto it = std::find(tiedDofs.begin(), tiedDofs.end(), _dof);
    if(it!=tiedDofs.end()) return true;
    return false;

}

bool BCs::isRetained(const size_t _dof)const{
    const auto it = std::find(retainedDofs.begin(), retainedDofs.end(), _dof);
    if(it!=retainedDofs.end()) return true;
    return false;

}

bool BCs::isDirichlet(size_t i) const{
    //	if(DirichletDofs.empty()) return false;
    //    for(auto j:DirichletDofs){if(j==i) return true;}
    //	return false;
    const auto it = std::find(DirichletDofs.begin(), DirichletDofs.end(), i);
    if(it!=DirichletDofs.end()) return true;
    return false;
}

bool BCs::isNotConstrainedDofs(size_t i) const{
    if( isDirichlet(i)||isTied(i)||isRetained(i) ) return false;
    return true;
}

bool BCs::isMeshDof(size_t i) const{
    const auto it = std::find(notMeshDofs.begin(), notMeshDofs.end(), i);
    if(it != notMeshDofs.end()) return false;
    return true;
}


void BCs::addDirichlet(std::vector<size_t> vec){
    DirichletDofs.insert(DirichletDofs.end(), vec.begin(), vec.end());
    std::sort(DirichletDofs.begin(), DirichletDofs.end());
    std::vector<size_t>::iterator it = std::unique(DirichletDofs.begin(), DirichletDofs.end());
    DirichletDofs.erase(it, DirichletDofs.end());
}

void BCs::setSommets(std::vector<size_t> _sommets){
    sommets.clear();
    sommets.insert(sommets.begin(), _sommets.begin(), _sommets.end());
}
vector<size_t> BCs::getSommets() const {return sommets;}

//TractionX
void BCs::setTractionX(size_t ndof){
    DirichletDofs.clear();
    // sommet A
    size_t i = (ndof*(sommets[0]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet B
    i = (ndof*(sommets[1]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet D
    i = (ndof*(sommets[3]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+2);
    // sommet Ap
    i = (ndof*(sommets[4]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1);

    //std::sort(DirichletDofs.begin(), DirichletDofs.end());
}

//TractionY
void BCs::setTractionY(size_t ndof){
    DirichletDofs.clear();
    // sommet A
    size_t i = (ndof*(sommets[0]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet B
    i = (ndof*(sommets[1]))+0;
    DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet D
    i = (ndof*(sommets[3]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet Ap
    i = (ndof*(sommets[4]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1);

    //std::sort(DirichletDofs.begin(), DirichletDofs.end());
}

//TractionZ
void BCs::setTractionZ(size_t ndof){
    DirichletDofs.clear();
    // sommet A
    size_t i = (ndof*(sommets[0]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);

    // sommet B
    i = (ndof*(sommets[1]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1);

    // sommet D
    i = (ndof*(sommets[3]))+0;
    DirichletDofs.push_back(i);  DirichletDofs.push_back(i+2);

    // sommet Ap
    i = (ndof*(sommets[4]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);

    //std::sort(DirichletDofs.begin(), DirichletDofs.end());
}

//CisXY
void BCs::setCisXY(size_t ndof){
    DirichletDofs.clear();
    // sommet A
    size_t i = (ndof*(sommets[0]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet B
    i = (ndof*(sommets[1]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet D
    i = (ndof*(sommets[3]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet Ap
    i = (ndof*(sommets[4]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);

    //std::sort(DirichletDofs.begin(), DirichletDofs.end());
}

//CisXZ
void BCs::setCisXZ(size_t ndof){
    DirichletDofs.clear();
    // sommet A
    size_t i = (ndof*(sommets[0]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet B
    i = (ndof*(sommets[1]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet D
    i = (ndof*(sommets[3]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet Ap
    i = (ndof*(sommets[4]))+0;

    //std::sort(DirichletDofs.begin(), DirichletDofs.end());
}

//CisYZ
void BCs::setCisYZ(size_t ndof){
    DirichletDofs.clear();
    // sommet A
    size_t i = (ndof*(sommets[0]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet B
    i = (ndof*(sommets[1]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet D
    i = (ndof*(sommets[3]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);
    // sommet Ap
    i = (ndof*(sommets[4]))+0;
    DirichletDofs.push_back(i); DirichletDofs.push_back(i+1); DirichletDofs.push_back(i+2);

    //std::sort(DirichletDofs.begin(), DirichletDofs.end());
}

//dirichletValue
double BCs::pbcDirichletValue(size_t i, size_t ndof)const{

    if(loadingType==0){
        //TrX
        if( i == (ndof*(sommets[1])+0) ) return delta;
        else return 0;
    }
    else
        if(loadingType==1){
            //TrY
            if( i == (ndof*(sommets[3])+1) ) return delta;
            else return 0;
        }
        else
            if(loadingType==2){
                //TrZ
                if( i == (ndof*(sommets[4])+2) ) return delta;
                else return 0;
            }
            else
                if(loadingType==3){
                    //CisXY
                    if( i == (ndof*(sommets[1])+1) ) return delta;
                    else return 0;
                }
                else
                    if(loadingType==4){
                        //CisXZ
                        if( i == (ndof*(sommets[1])+2) ) return delta;
                        else return 0;
                    }
                    else
                        if(loadingType==5){
                            //CisYZ
                            if( i == (ndof*(sommets[3])+2) ) return delta;
                            else return 0;
                        }
}


void BCs::setGeneralPbcBoundaryCondition(){
    //This function is intended to replace setTractionX(), ..., setCisYZ()

    DirichletDofs.clear();
    //        size_t nA = sommets[0];
    //        size_t nB = sommets[1];
    //        size_t nD = sommets[3];
    //        size_t nAp = sommets[4];

    size_t nA = sommets[0];
    size_t nB = sommets[1];
    size_t nD = sommets[2];
    size_t nAp = sommets[3];

    DirichletDofs = {3*nA, 3*nA+1, 3*nA+2,      3*nB, 3*nB+1, 3*nB+2,
                     3*nD, 3*nD+1, 3*nD+2,     3*nAp, 3*nAp+1, 3*nAp+2};
    //        DirichletDofs.append({3*nA, 3*nA+1, 3*nA+2,   3*nB, 3*nB+1, 3*nB+2,
    //                              3*nD, 3*nD+1, 3*nD+2, 3*nAp, 3*nAp+1, 3*nAp+2});



}
//    double pbcDirichletValue(const MeshPbc<double, 3>& meshPbc, const std::vector<int>& controlNodes,
//                             int sollType, size_t vertDof, double delta) const;
double BCs::pbcDirichletValue(const MeshPbc<double,3>& meshPbc, const std::vector<int>& controlNodes,
                         int sollType, size_t vertDof, double delta) const{

    //vertDof is a given dof on a control vertex.
    //This function returns Dirichlet periodic boundary conditions related to vertDof

    if(controlNodes.size() != 4) return std::numeric_limits<double>::infinity() ;
    int nA = controlNodes[0];
    int nB = controlNodes[1];
    int nD = controlNodes[2];
    int nAp = controlNodes[3];

    //        int nA = controlNodes[0];
    //        int nB = controlNodes[1];
    //        int nD = controlNodes[3];
    //        int nAp = controlNodes[4];

    double xBA, xDA, xApA,   yBA, yDA, yApA,  zBA, zDA, zApA; //coordMM' =coord(M')-coord(M)
    xBA = meshPbc.getNodes()[nB].getXi(0) - meshPbc.getNodes()[nA].getXi(0);
    xDA = meshPbc.getNodes()[nD].getXi(0) - meshPbc.getNodes()[nA].getXi(0);
    xApA = meshPbc.getNodes()[nAp].getXi(0) - meshPbc.getNodes()[nA].getXi(0);

    yBA = meshPbc.getNodes()[nB].getXi(1) - meshPbc.getNodes()[nA].getXi(1);
    yDA = meshPbc.getNodes()[nD].getXi(1) - meshPbc.getNodes()[nA].getXi(1);
    yApA = meshPbc.getNodes()[nAp].getXi(1) - meshPbc.getNodes()[nA].getXi(1);

    zBA = meshPbc.getNodes()[nB].getXi(2) - meshPbc.getNodes()[nA].getXi(2);
    zDA = meshPbc.getNodes()[nD].getXi(2) - meshPbc.getNodes()[nA].getXi(2);
    zApA = meshPbc.getNodes()[nAp].getXi(2) - meshPbc.getNodes()[nA].getXi(2);


    int nAdof0 = 3*nA+0; int nAdof1 = 3*nA+1; int nAdof2 = 3*nA+2;

    int nBdof0 = 3*nB+0; int nBdof1 = 3*nB+1; int nBdof2 = 3*nB+2;

    int nDdof0 = 3*nD+0; int nDdof1 = 3*nD+1; int nDdof2 = 3*nD+2;

    int nApdof0 = 3*nAp+0; int nApdof1 = 3*nAp+1; int nApdof2 = 3*nAp+2;

    /* Uniaxial deformations case */

    if(sollType == 0 /*deformation xx*/){
        if(vertDof == nBdof0) return delta*xBA; else
            if(vertDof == nDdof0) return delta*xDA; else
                if(vertDof == nApdof0) return delta*xApA;
                else return 0. ;
    }

    if(sollType == 1 /*deformation yy*/){
        if(vertDof == nBdof1) return delta*yBA; else
            if(vertDof == nDdof1) return delta*yDA; else
                if(vertDof == nApdof1) return delta*yApA;
                else return 0. ;
    }

    if(sollType == 2 /*deformation zz*/){
        if(vertDof == nBdof2) return delta*zBA; else
            if(vertDof == nDdof2) return delta*zDA; else
                if(vertDof == nApdof2) return delta*zApA;
                else return 0. ;
    }

    /* Shear deformations case */

    if(sollType == 3 /*shear xy*/){
        if(vertDof == nBdof0) return delta*yBA; else
            if(vertDof == nBdof1) return delta*xBA; else /**/
                if(vertDof == nDdof0) return delta*yDA; else
                    if(vertDof == nDdof1) return delta*xDA; else /**/
                        if(vertDof == nApdof0) return delta*yApA; else
                            if(vertDof == nApdof1) return delta*xApA;
                            else return 0. ;
    }

    if(sollType == 4 /*shear xz*/){
        if(vertDof == nBdof0) return delta*zBA; else
            if(vertDof == nBdof2) return delta*xBA; else /**/
                if(vertDof == nDdof0) return delta*zDA; else
                    if(vertDof == nDdof2) return delta*xDA; else /**/
                        if(vertDof == nApdof0) return delta*zApA; else
                            if(vertDof == nApdof2) return delta*xApA;
                            else return 0. ;
    }

    if(sollType == 5 /*shear yz*/){
        if(vertDof == nBdof1) return delta*zBA; else
            if(vertDof == nBdof2) return delta*yBA; else /**/
                if(vertDof == nDdof1) return delta*zDA; else
                    if(vertDof == nDdof2) return delta*yDA; else /**/
                        if(vertDof == nApdof1) return delta*zApA; else
                            if(vertDof == nApdof2) return delta*yApA;
                            else return 0. ;
    }

}





}}
