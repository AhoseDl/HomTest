#include<pbcmeshtools.h>

namespace scaletag{
namespace fem{

    PBCMeshTool::PBCMeshTool(){Initialize();}

//    PBCMeshTool::PBCMeshTool(TopoDS_Shape* _shape): theShape(_shape){
//    //Build the theShapeDataSource
//      Initialize();
//    }

    PBCMeshTool::PBCMeshTool(IVtkTools_ShapeDataSource* _theShapeSource):
        theShapeSource(_theShapeSource){
    //Build the Shape
     theShape = theShapeSource->GetShape()->GetShape();
    Initialize();
    }

    PBCMeshTool::~PBCMeshTool(){
    //if(meshData) delete meshData;
    //if(theShapeSource)  /*theShapeSource->Delete();*/ delete theShapeSource;
    }

//    PBCMeshTool::PBCMeshTool(const IVtkTools_ShapeDataSource& _theShapeSource, const TopoDS_Shape& _theShape)
//    : theShapeSource(&_theShapeSource), theShape(&_theShape){
//      Initialize();
//    }

    void PBCMeshTool::Initialize(){
        gmsh::initialize();
        gmsh::clear();
        gmsh::model::add("ModelName");

        gmsh::vectorpair entities;
        gmsh::model::occ::importShapesNativePointer(static_cast<const void*>(&theShape), entities);
        gmsh::model::occ::synchronize();


            }


    void PBCMeshTool::Finalize(){
        gmsh::finalize();
    }

    void PBCMeshTool::ImposePeriodicity(const std::vector<EntityPair>& pbcFacesVIS){

        //Nothing guarantees that pairs provided in pbcFacesVIS are organized in free entities - tied entities manner;
        //And actually there is no need to rearrange them in this manner here

        std::vector<EntityPair> gmshPairs = VIStoGMSH(pbcFacesVIS);
        std::vector<EntityPair> gmshStructuredPairs(gmshPairs.size()); //To contain slave/master structured PBC

        /* For every pair
        -Verify similarity of faces
        -Compute the translation vector and affine transformation matrix
        -give free enity/tied entity structure -> this modifies pairs ordering in gmshPairs
        -Invoke setPeriodic on gmshPairs
        */


        /* Building tied and retained pairs */

        //Tied Faces = faces closer to A
        // Retained faces = faces further from A

        std::vector<double> A(3); // may be corner vertex (xmin, ymin, zmin)
        int tg1, tg2; //pairs in gmshPairs;
        std::vector<double> pA1, pA2; // Projections of face tg1 and face tg2;
        double d1, d2;

        gmsh::vectorpair theSolid;
        gmsh::model::occ::getEntities(theSolid, 3) ; //Retrieve the volume tag
        double xmin, xmax, ymin, ymax, zmin, zmax;
        gmsh::model::occ::getBoundingBox(3, theSolid[0].second, xmin, ymin, zmin, xmax, ymax, zmax);

        A[0]=xmin; A[1] = ymin; A[2] = zmin;

        int i=0;
        for(auto& p : gmshPairs){

            tg1 = p.tagLo; tg2 = p.tagHi;
            std::vector<double> dummyParCoord;
            gmsh::model::getClosestPoint(2, tg1, A, pA1, dummyParCoord);
            gmsh::model::getClosestPoint(2, tg2, A, pA2, dummyParCoord);

            d1 = std::pow( std::pow((A[0]-pA1[0]),2) + std::pow((A[1]-pA1[1]),2) + std::pow((A[2]-pA1[2]),2),  0.5);
            d2 = std::pow( std::pow((A[0]-pA2[0]),2) + std::pow((A[1]-pA2[1]),2) + std::pow((A[2]-pA2[2]),2),  0.5);

        int tiedP, retP;
        if(d1<=d2) {tiedP = tg1; retP = tg2;} else
        {tiedP = tg2; retP = tg1;}

        gmshStructuredPairs[i] = {tiedP, retP};

        // --- debug ---
        std::ofstream ofs3("/home/ahose/Desktop/pbc3.dat", std::ios::app);
        double ct[3], cr[3];
        gmsh::model::occ::getCenterOfMass(2, tiedP, ct[0], ct[1], ct[2]);
        gmsh::model::occ::getCenterOfMass(2, retP,  cr[0], cr[1], cr[2]);
        ofs3 << "pair " << i << " tied=" << tiedP << " ret=" << retP
                  << " d1=" << d1 << " d2=" << d2
                  << " | tied COM=(" << ct[0] << "," << ct[1] << "," << ct[2] << ")"
                  << " ret COM=("  << cr[0] << "," << cr[1] << "," << cr[2] << ")"
                  << " | A=(" << A[0] << "," << A[1] << "," << A[2] << ")\n";
        // --- end debug ---


        i++;



        } //End of gmmshStructuredPairs construction

        /*Computing the translation vectors */
        std::vector<std::vector<double> > trVecs(gmshStructuredPairs.size());
        std::vector<double> aTrVec (3);
        std::vector<double> coordG1(3), coordG2(3);

        int ppair=0;
        for(auto& p : gmshStructuredPairs){
        gmsh::model::occ::getCenterOfMass(2, p.tagLo, coordG1[0], coordG1[1], coordG1[2]);
        gmsh::model::occ::getCenterOfMass(2, p.tagHi, coordG2[0], coordG2[1], coordG2[2]);
//        aTrVec[0] = coordG2[0]-coordG1[0];
//        aTrVec[1] = coordG2[1]-coordG1[1];
//        aTrVec[2] = coordG2[2]-coordG1[2];
        aTrVec[0] = coordG1[0]-coordG2[0];
        aTrVec[1] = coordG1[1]-coordG2[1];
        aTrVec[2] = coordG1[2]-coordG2[2];  //GMSH meshes master faces, applies "affine transformation" master surface's node to map nodes on slave surface.
        trVecs[ppair] = aTrVec;
        ppair++;
        }


        /* Using translation vectors to impose periodic constraints */
        std::vector<double> affTr(16,0.); double tx, ty, tz;
        affTr[0] = 1.; affTr[5] = 1. ; affTr[10] = 1. ; affTr[15] = 1.;
        ppair=0;
        for(auto& p : gmshStructuredPairs){
        //for(auto& trVec : trVecs){
        //Compute the 4x4 translation matrix
        //Use setPeriodic() to impose periodicity

        tx = trVecs[ppair][0] ; ty = trVecs[ppair][1] ; tz = trVecs[ppair][2] ;
        affTr[3] = tx; affTr[7] = ty; affTr[11] = tz;

        std::vector<int> tiedTags(1), retTags(1);

        tiedTags[0] = p.tagLo; retTags[0] = p.tagHi;

        gmsh::model::mesh::setPeriodic(2, tiedTags, retTags, affTr);

        ppair++;
        }
    setStructuredFacePairs(gmshStructuredPairs);
    setEdgesPeriodicity(gmshStructuredPairs);
    //setStructuredEdgePairs(gmshStructuredPairs);  //use gmshStructuredPairs to build edge mappings and keep
    //gmshStructuredEdgePairs in PBCMeshTool
    }

 void PBCMeshTool::setEdgesPeriodicity(const std::vector<EntityPair>& structuredFacePairs){
  std::vector<std::vector<EntityPair>> edgePairs;

 //Building edges pairs
 int nMappedFaces = gmshStructuredFacePairs.size();

 edgePairs.resize(nMappedFaces);

 std::vector<int> tiedEdgeTags ; //vector to contain tied tags.  the tag is repeated only once in the vector

 std::vector<std::vector<double>> trVecs(nMappedFaces); // To hold per surface mapping translation vector.
 int dirNum=0; // To every mapped face correspond a dirNum
 for(auto& surPair : gmshStructuredFacePairs){

 int tiedSurfTag = surPair.tagLo;
 int retSurfTag = surPair.tagHi;
 std::vector<double> GLo(3), GHi(3), trVec(3);
 gmsh::model::occ::getCenterOfMass(2, tiedSurfTag, GLo[0], GLo[1], GLo[2]);
 gmsh::model::occ::getCenterOfMass(2, retSurfTag, GHi[0], GHi[1], GHi[2]);

 trVec[0] = GHi[0] - GLo[0];
 trVec[1] = GHi[1] - GLo[1];
 trVec[2] = GHi[2] - GLo[2];

//  trVec[0] = GLo[0] - GHi[0];
//  trVec[1] = GLo[1] - GHi[1];
//  trVec[2] = GLo[2] - GHi[2];

 trVecs[dirNum] = trVec;
 gmsh::vectorpair tiedBoundaryEdges; //boundaryNodes;
 gmsh::vectorpair retBoundaryEdges;
 gmsh::model::getBoundary({{2, tiedSurfTag}}, tiedBoundaryEdges, false, false, false);
 gmsh::model::getBoundary({{2, retSurfTag}}, retBoundaryEdges, false, false, false);

 std::vector<int> retBoundaryEdgesList(retBoundaryEdges.size());
 for(int i=0; i<retBoundaryEdges.size(); i++) retBoundaryEdgesList[i] = retBoundaryEdges[i].second;
    for(auto& [dim, tg] : tiedBoundaryEdges){
     if(!insert_once(tg, tiedEdgeTags)) continue;
        int nSamples = 5 ; double tol = 1.e-5;//1.e-6
    int mappedEdge = findEdgeBySamplePoints(tg, /*amongEdges*/retBoundaryEdgesList, trVec[0] , trVec[1] , trVec[2], nSamples, tol);
        if(mappedEdge != -1) edgePairs[dirNum].push_back( {tg, mappedEdge} );

        }
    dirNum++;
    }

    /***** Setting periodicity on the edges ******/

    for(int dirNum=0; dirNum<nMappedFaces; dirNum++){
    std::vector<int> edgeTiedTagsDir, edgeRetTagsDir;
    edgeTiedTagsDir.reserve( edgePairs[dirNum].size());
    edgeRetTagsDir.reserve( edgePairs[dirNum].size());

    for(auto& [edgeTied, edgeRet, prec1 , prec2] : edgePairs[dirNum]){ edgeTiedTagsDir.push_back(edgeTied);  edgeRetTagsDir.push_back(edgeRet); }

    std::vector<double> affTr(16,0.); double tx = trVecs[dirNum][0], ty = trVecs[dirNum][1], tz = trVecs[dirNum][2];
    affTr[0] = 1.; affTr[5] = 1. ; affTr[10] = 1. ; affTr[15] = 1.;
    affTr[3] = -tx, affTr[7]=-ty, affTr[11]=-tz;//to be reanalyzed
    gmsh::model::mesh::setPeriodic(1, edgeTiedTagsDir, edgeRetTagsDir, affTr );
    }


    gmshStructuredEdgePairs = edgePairs ;
    }



    std::vector<std::vector<EntityPair>> PBCMeshTool::getEdgesPeriodicity() const{
    return gmshStructuredEdgePairs;
    }


    void PBCMeshTool::SetMeshSize(const double& meshsize){
        gmsh::vectorpair pts;
        gmsh::model::getEntities(pts, 0);
        gmsh::model::mesh::setSize(pts, meshsize);
        }

    void PBCMeshTool::mesh(const double& meshsize){

        //Class constructors have already done any initialization task
        ImposePeriodicity(facePairs);
        //SetMeshSize(meshsize);
        SetMeshSize(meshsize);
        //gmsh::model::mesh::setOrder(2);
        //gmsh::option::setNumber("Geometry.Tolerance", 1e-4); //May be useful for tolerances when matching surfaces
        //gmsh::option::setNumber("Mesh.RecombineAll", 1);
        //gmsh::model::mesh::setTransfiniteAutomatic();

        //gmsh::model::mesh::setOrder(2); //for quadratic elements //Works only if there is an existing mesh


        gmsh::option::setNumber("Mesh.ElementOrder", 2);
        gmsh::option::setNumber("Mesh.MeshSizeFactor", 0.5);
        gmsh::option::setNumber("Mesh.MeshSizeFromCurvature", 1);
        gmsh::model::mesh::optimize("Netgen");
        gmsh::model::mesh::optimize(""); //Gmsh
        gmsh::model::mesh::generate(3);
        //gmsh::write("ModelName.msh");
        //gmsh::write("ModelName.inp");
        gmsh::model::mesh::removeDuplicateNodes();
        gmsh::model::mesh::removeDuplicateElements();
        //use custom method to import mesh to Scaltag
        //gmsh::write("ModelName.step");
        //gmsh::model::occ::synchronize();

        /************   Retrieving mesh data (MeshData) in GMSH  *************/
        std::vector<std::size_t> nodeTags;
        std::vector<double> coord;
        std::vector<int>  elementTypes;
        std::vector<std::vector<std::size_t> > elementTags; //nodeTags;
        std::vector<size_t> elementSizes;

        std::vector<double> parametricCoord;
        gmsh::model::mesh::getNodes(nodeTags, coord, parametricCoord, 3, -1, true, false);

        std::vector<std::vector<size_t>> connectivity;
        gmsh::model::mesh::getElements(elementTypes, elementTags, connectivity, 3, -1);

//        size_t nelts = 0;
//        for(auto gEltType : elementTypes) nelts += elementTags[gEltType].size();
//        elementSizes.reserve( elementTypes.size() );
//        for(auto gEltType : elementTypes)
//        {
//        //size_t gEltType = elementTypes[e];
//        int eSize;

//        std::string  elementName; int dim, order, numPrimaryNodes; std::vector<double>  localNodeCoord;
//        gmsh::model::mesh::getElementProperties(gEltType, elementName, dim, order, eSize, localNodeCoord, numPrimaryNodes);
//        elementSizes.push_back(eSize);

//        }
        size_t nelts = 0;
        elementSizes.reserve(elementTypes.size());
        for(size_t e = 0; e < elementTypes.size(); ++e)
        {
    nelts += elementTags[e].size();

    std::string elementName; int dim, order, eSize, numPrimaryNodes;
    std::vector<double> localNodeCoord;
    gmsh::model::mesh::getElementProperties(elementTypes[e], elementName, dim,
                                            order, eSize, localNodeCoord,
                                            numPrimaryNodes);
    elementSizes.push_back(eSize);
        }

        meshData.initialize(nodeTags, coord, elementTypes, elementTags, connectivity, elementSizes); //For importing the mesh to Scaletag purpose


        //gmsh::fltk::run();
        //std::vector<std::vector<s

        /*******   Retrieving the periodic mapping *******************/
    //this->buildConstraints(nodesConstraints);

        //this->Finalize();
    }

    MeshData PBCMeshTool::getMeshData(){return meshData;}
	
    std::vector<EntityPair> PBCMeshTool::VIStoGMSH(const std::vector<EntityPair>& surfPairVis){

        //Here we are using EntityPair without Master/Slave notion

        std::vector<EntityPair> gmshPair(surfPairVis.size());

        //one need a mapping of tags in GMSH to Ids in VIS
        if(!theShapeSource) return{} ;

        //Building the root TopoDS_Shape from theShapeSource
        Handle(IVtk_IShape) iVtkShape = Handle(IVtk_IShape)::DownCast(theShapeSource->GetShape());

        TopoDS_Shape theShape;
        std::unordered_map<int, TopoDS_Shape> shapesTags;

        if (!iVtkShape.IsNull())
        {
            Handle(IVtkOCC_Shape) visShape = Handle(IVtkOCC_Shape)::DownCast(iVtkShape);
            theShape = visShape->GetShape();

        }
        if (!theShape.IsNull())
            {

            TopTools_IndexedMapOfShape faceMap;
            TopExp::MapShapes(theShape, TopAbs_FACE, faceMap); // We assume indexes in faceMap are GMSH tags of the TopoDS_Shape

            for (int i = 1; i <= faceMap.Extent(); ++i) {
                shapesTags[i] = TopoDS::Face(faceMap(i));
            }

            }


            std::unordered_map<IVtk_IdType, int> VisGmshMap;
            std::vector<int> tagsVector;
            std::vector<int> gTags;
            std::vector<IVtk_IdType> vIds;

            tagsVector.reserve(shapesTags.size());
            for(const auto& tgSh : shapesTags) tagsVector.push_back(tgSh.first);

            vIds.reserve(2*surfPairVis.size());
            for(const auto& p:surfPairVis){vIds.push_back(p.tagLo); vIds.push_back(p.tagHi);}

            gTags.resize(2*surfPairVis.size());

            for(int i=0; i< vIds.size(); i++)
            {
            IVtk_IdType anId = vIds[i];
                for(auto tg: tagsVector)
                if( isVISIdGMSHTag(theShapeSource , shapesTags, anId, tg ) ) gTags[i] = tg;

            }
            for(int i=0; i<gTags.size(); i++) VisGmshMap[ vIds[i] ] = gTags[i];

            int i=0;
            for(auto& p : surfPairVis) {
                //Convert every VIS index in SurPairVis to GMSH tag;
                gmshPair[i] = {VisGmshMap[p.tagLo], VisGmshMap[p.tagHi]}; i++;
            }


    return gmshPair;
    }




    bool PBCMeshTool::isVISIdGMSHTag( IVtkTools_ShapeDataSource* shapeSource ,const std::unordered_map<int, TopoDS_Shape>& shapesTags, int id, int tag) const{

        /* Verify if an entity in GMSH corresponds to an Id used in VIS*/

       /* One can use an algo based on centroids' distances, boundary boxes
        or take N points - caracterized by their parametric coordinates and
        and verify if those points are on both of the surfaces. */

        /* Compare TopoDS_Shape-GMSH(tag) vs TopoDS_Shape_VIS(id) - This needs to include
        GModelIO_OCC.h */

        // shapesTags holds the mapping gmsh tag -> TopoDS_Shape

        TopoDS_Shape shapeVIS, shapeGMSH;

        shapeGMSH = shapesTags.at(tag);

        //Computing shapeVis
//        Handle(IVtkOCC_Shape) occShape = dataSource->GetShape();
//        if (occShape.IsNull()) {std::cout<<"The shape source must exist."<<std::endl; return;}
//        IVtk_IdType shapeId = occShape->GetId();
//        IVtk_ShapeIdList subIds = myPicker->GetPickedSubShapesIds(shapeId);
//        if (!subIds.IsEmpty()) {
//            IVtk_IdType subId = subIds.First();
//            shapeVIS = occShape->GetSubShape(subId);
//        }

        Handle(IVtk_IShape) iVtkShape = Handle(IVtk_IShape)::DownCast(shapeSource->GetShape());

        if (!iVtkShape.IsNull())
        {
            Handle(IVtkOCC_Shape) occShape = Handle(IVtkOCC_Shape)::DownCast(iVtkShape);
            shapeVIS = occShape->GetSubShape((IVtk_IdType)id);

            if (!shapeVIS.IsNull() && !shapeGMSH.IsNull())
            {
              return areShapesCoincident(shapeVIS ,shapeGMSH); // We could also use IsGeometricallyEqual();
            }

            return false;

//        std::vector<TopoDS_Face> occ_faces;
//        for (TopExp_Explorer exp(*theShape, TopAbs_FACE); exp.More(); exp.Next()) {
//            occ_faces.push_back(TopoDS::Face(exp.Current()));
//        }

    }
    }


    void PBCMeshTool::setStructuredFacePairs(const std::vector<EntityPair>& _gmshStructuredFacePairs){gmshStructuredFacePairs = _gmshStructuredFacePairs;}
    std::vector<EntityPair> PBCMeshTool::getStructuredFacePairs() const{ return gmshStructuredFacePairs;}



    void PBCMeshTool::buildMappingDirs(){

    mappingDirs.clear();
    if(gmshStructuredFacePairs.empty()) return;
    for(auto& p:gmshStructuredFacePairs){
            int tiedTg = p.tagLo;
            int retTg = p.tagHi;
            std::vector<double> c1(3), c2{3}; std::vector<double> dirVec(3);
            gmsh::model::occ::getCenterOfMass(2, tiedTg, c1[0], c1[1], c1[2] );
            gmsh::model::occ::getCenterOfMass(2, retTg, c2[0], c2[1], c2[2] );
            dirVec[0] = c2[0]-c1[0];
            dirVec[1] = c2[1]-c1[1];
            dirVec[2] = c2[2]-c1[2];
           mappingDirs.push_back(dirVec);
    }

   // mappingDirs = {{1.,0,0}, {0.,1.,0.}, {0., 0., 1.}};
    meshData.setMappingDirs(mappingDirs);
    }

    std::vector<std::vector<double>> PBCMeshTool::getGMappingDirs() const{ return mappingDirs;}


    void PBCMeshTool::buildConstraints(std::vector</*std::vector<EntityPair*/std::unordered_map<int, int> >& nodesConstraints){

    //This system yiels a non singular system of equations


//    std::vector<EntityPair> gmshStructuredFacePairs; // EntityPair's in gmshFacePairs are structured <slave, master> through ImposePeriodicity()
//    std::vector<std::vector<EntityPair>>  gmshStructuredEdgePairs;


    /****************** First retrieve all boundary edges nodes *******************/
    int nDir = gmshStructuredFacePairs.size();
    std::vector<int> allEdgeTags;
    for(int i=0; i<nDir; i++){

            int tg1, tg2;
           // std::vector<int>
            gmsh::vectorpair edgeTags1, edgeTags2;
            tg1=gmshStructuredFacePairs[i].tagLo;
            tg2=gmshStructuredFacePairs[i].tagHi;

            gmsh::model::getBoundary({{2, tg1}}, edgeTags1, false, false, false);
            gmsh::model::getBoundary({{2, tg2}}, edgeTags2, false, false, false);

            for(auto [dim, p]:edgeTags1) insert_once(p, allEdgeTags);
            for(auto [dim, p]:edgeTags2) insert_once(p, allEdgeTags);
    }

//    auto isNotEdgeNode =[&allEdgeTags](int tg){

//        std::vector<size_t> nt; std::vector<double> xyz, par;
//        gmsh::model::mesh::getNodes(nt, xyz, par, dim, tag, /*includeBoundary=*/true, false);

//       auto it = std::find(allEdgeTags.begin(), allEdgeTags.end(), tg);
//        if(it == allEdgeTags.end()) return true;
//        return false;
//    };
    auto isNotEdgeNode = [&allEdgeTags](size_t nodeTag){
        std::vector<double> xyz, par;
        int entDim = -1, entTag = -1;
        gmsh::model::mesh::getNode(nodeTag, xyz, par, entDim, entTag);

        if(entDim == 0) return false;
        if(entDim == 1)
            return std::find(allEdgeTags.begin(), allEdgeTags.end(), entTag) == allEdgeTags.end();
        return true;
    };
    /**********************************/

    /* Building of edge nodes pairs */
    //std::vector<std::vector<std::unordered_map<int, int>>> edgeNodesConstraints;
    std::vector<std::unordered_map<int, int> > edgeNodesConstraints;

    /*int*/ nDir = gmshStructuredEdgePairs.size();
    //edgeNodesConstraints.reserve(nDir);
    //edgeNodesConstraints.resize(nDir);
    for(int i=0; i<nDir; i++){
            std::vector<EntityPair> aDirPairs = gmshStructuredEdgePairs[i];
            std::unordered_map<int, int> aDirPairNodes ;

            for(int iPairEdges = 0; iPairEdges<aDirPairs.size(); iPairEdges++){
              EntityPair anEdgePair = aDirPairs[ iPairEdges ] ;

              std::vector<size_t> tiedNodesTags, retNodesTags;   std::vector<double> affTr;
              int tagMaster;
              gmsh::model::mesh::getPeriodicNodes(1, anEdgePair.tagLo, tagMaster, tiedNodesTags, retNodesTags, affTr, true );
              if(tagMaster != anEdgePair.tagHi) continue;

              for(int nTag=0; nTag<tiedNodesTags.size(); nTag++)
                //  aDirPairNodes.push_back( {tiedNodesTags[nTag], retNodesTags[nTag]} );
              aDirPairNodes.insert( {tiedNodesTags[nTag], retNodesTags[nTag]}  );

            }

            edgeNodesConstraints.push_back(aDirPairNodes);
            //edgeNodesConstraints[i] = aDirPairNodes;
    }

    /*  Building face nodes pairs */

    std::vector<std::unordered_map<int, int> >  faceNodesConstraints;
    for(int i=0; i<nDir; i++){
        EntityPair aDirPairs = gmshStructuredFacePairs[i];
        std::unordered_map<int, int> aDirPairNodes ;

        std::vector<size_t> tiedNodesTags, retNodesTags; std::vector<double> affTr;
        int tiedFaceTag = aDirPairs.tagLo;
        int tagMaster;
        gmsh::model::mesh::getPeriodicNodes(2, tiedFaceTag, tagMaster, tiedNodesTags, retNodesTags, affTr, true );
        if(tagMaster != aDirPairs.tagHi) continue;

        for(int nTag=0; nTag<tiedNodesTags.size(); nTag++)
              if( isNotEdgeNode(tiedNodesTags[nTag]) && isNotEdgeNode(retNodesTags[nTag]))
                  aDirPairNodes.insert( {tiedNodesTags[nTag], retNodesTags[nTag]} );

        faceNodesConstraints.push_back(aDirPairNodes);
    }

    //Merging the face and edge constraints to nodesConstraint
    nodesConstraints.resize(nDir);
    for(int i = 0; i < nDir; i++){
        nodesConstraints[i] = std::move(faceNodesConstraints[i]);

        //if (i < edgeNodesConstraints.size()) {
              for(const auto& [slaveNode, masterNode] : edgeNodesConstraints[i]) {
                  nodesConstraints[i][slaveNode] = masterNode;
              }
        //}
    }

    meshData.setNodesPeriodicity(nodesConstraints);
    }





















   /* bool areShapesCoincident(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2, double tolerance = 1e-6) {
    // This function gives check if minimal distance between two shapes is inferior to a tolerance
        // Initializing the distance algorithm with both shapes
        BRepExtrema_DistShapeShape distCalc(shape1, shape2);

        // Performing the geometric computation
        distCalc.Perform();

        if (!distCalc.IsDone()) {
            std::cerr << "Distance computation failed." << std::endl;
            return false;
        }

        // Getting the minimum distance
        double minDist = distCalc.Value();

        // Checking if the distance falls within your allowed tolerance
        return (minDist <= tolerance);
    }*/


    bool areShapesCoincident(const TopoDS_Shape& shapeA, const TopoDS_Shape& shapeB, double linearTolerance)
    {

        /**
     * Checks if two shapes occupy the exact same 3D spatial boundaries.
     * Works even if TShape pointers differ or if files were read independently.
     */

        //First   the Boolean Intersection is computed - common part
        BRepAlgoAPI_Common commonAlgo(shapeA, shapeB);

        //Adding a fuzzy value for shapes with tiny modeling inaccuracies
        commonAlgo.SetFuzzyValue(1e-9);

        commonAlgo.Build();
        if (!commonAlgo.IsDone()) {
            return false; // For failed boolean operation
        }

        TopoDS_Shape commonShape = commonAlgo.Shape();
        if(commonShape.IsNull()) {return false; // Shapes do not intersect
        }

        //Measuring properties
        GProp_GProps propsA, propsB, propsCommon;

        // Measuring volumes for solids - Dim=3, surfaces for faces - Dim=2, Linear for Edges - Dim=1

        BRepGProp::VolumeProperties(shapeA, propsA);
        double valueA = propsA.Mass(); //Mass() returns volume for volume properties

        if (valueA > Precision::Confusion()) {
            // Evaluate as 3D Volumes
            BRepGProp::VolumeProperties(shapeB, propsB);
            BRepGProp::VolumeProperties(commonShape, propsCommon);
        } else {
            // Fallback to 2D Surfaces (For faces comparison)
            BRepGProp::SurfaceProperties(shapeA, propsA);
            BRepGProp::SurfaceProperties(shapeB, propsB);
            BRepGProp::SurfaceProperties(commonShape, propsCommon);
            valueA = propsA.Mass(); // Mass() returns area for surface properties

            if (valueA < Precision::Confusion()) {
              // Fallback to 1D Lines (if comparing isolated wires or edges)
              BRepGProp::LinearProperties(shapeA, propsA);
              BRepGProp::LinearProperties(shapeB, propsB);
              BRepGProp::LinearProperties(commonShape, propsCommon);
              valueA = propsA.Mass(); // Mass() returns length for linear properties
            }
        }

        double valueB = propsB.Mass();
        double valueCommon = propsCommon.Mass();

        // The common measure must match both original shapes
        double deltaA = std::abs(valueA - valueCommon);
        double deltaB = std::abs(valueB - valueCommon);

        return (deltaA <= linearTolerance && deltaB <= linearTolerance);
    }

    bool IsGeometricallyEqual(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2) {
        //Thinking about another way to check coincidence of two TopoDS_Shape
        // Compute the symmetric difference: (Shape1 \ Shape2)
        BRepAlgoAPI_Cut cut1(shape1, shape2);

        cut1.Build();
        if (!cut1.IsDone() || !cut1.Shape().IsNull()) {
            return false;
        }

        // Compute the inverse symmetric difference: (Shape2 \ Shape1)
        BRepAlgoAPI_Cut cut2(shape2, shape1);
        cut2.Build();
        if ( !cut2.IsDone() || !cut2.Shape().IsNull() ) {
            return false;
        }

        return true;
    }



    /*********************** Definition of the class MeshData ***************************/
    void MeshData::initialize(std::vector<std::size_t> _nodeTags, std::vector<double> _coord,
            std::vector<int>  _elementTypes, std::vector<std::vector<std::size_t> > _elementTags,
            std::vector<std::vector<std::size_t>> _connectivity, std::vector<size_t> _elementSizes){
        nodeTags = _nodeTags;
        coord = _coord;
        elementTypes = _elementTypes;
        elementTags = _elementTags;
        connectivity = _connectivity;
        elementSizes = _elementSizes;
    }

    std::vector<std::size_t> MeshData::getNodeTags() const { return nodeTags;}
    std::vector<double> MeshData::getNodesCoord() const { return coord;}

    std::vector<int> MeshData::getElementTypes() const { return elementTypes;}
    std::vector<std::vector<std::size_t> > MeshData::getElementTags() const { return elementTags;}
    std::vector<std::vector<std::size_t> > MeshData::getConnectivity() const {return connectivity;}
    std::vector<std::size_t> MeshData::getElementSizes() const {return elementSizes;}

    void MeshData::setNodesPeriodicity(const std::vector<std::unordered_map<int, int>>& _nodesConstraints){
        nodesConstraints = _nodesConstraints;
    }
    std::vector<std::unordered_map<int, int>> MeshData::getNodesPeriodicity() const{
        return nodesConstraints;
    }

    void MeshData::setMappingDirs(const std::vector<std::vector<double>>& _mappingDirs){
        mappingDirs = _mappingDirs;
    }
    std::vector<std::vector<double>> MeshData::getMappingDirs() const{

        return mappingDirs;
    }


    bool insert_once(const int& tg, std::vector<int>& vec){

        std::vector<int>::iterator it = std::find(vec.begin(), vec.end(), tg);
        if(it != vec.end()) return false;
        vec.push_back(tg);
        return true;
    }
}
}
