#ifndef PBCMESHTOOL_H
#define PBCMESHTOOL_H

#include<gmsh.h>
#include<cad.h>

#include<gmshtools.h> // will contain scaletag's api to gmsh

#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>

#include<TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Cut.hxx>

#include <BRepExtrema_DistShapeShape.hxx>

#include <IVtk_IShape.hxx>
#include <IVtk_Types.hxx>

#include <BRepAlgoAPI_Common.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <Precision.hxx>

#include<map>
#include<unordered_map>

#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>

namespace scaletag{
namespace fem{

/*utilities PBC mesh */

//class MeshData;

bool areShapesCoincident(const TopoDS_Shape& shape1, const TopoDS_Shape& shape2,double linearTolerance = 1e-6 );

class MeshData{
    /* This class intents to facilitate gmsh data transfer to scaletag*/
private :
    std::vector<std::size_t> nodeTags;
    std::vector<double> coord;

    std::vector<int>  elementTypes;
    std::vector<std::vector<std::size_t> > elementTags; //nodeTags;
     std::vector<std::vector<size_t>> connectivity;
    std::vector<size_t> elementSizes; //same size as elementTypes

    size_t D=3;

    //std::vector<EntityPair> gmshStructuredFacePairs; // EntityPair's in gmshFacePairs are structured <slave, master> through ImposePeriodicity()

    std::vector<std::vector<double>> mappingDirs;
    std::vector<std::unordered_map<int, int>> nodesConstraints;


public:
    MeshData() = default;

    void initialize(std::vector<std::size_t> nodeTags, std::vector<double> coord,
     std::vector<int>  elementTypes, std::vector<std::vector<std::size_t> > elementTags,
     std::vector<std::vector<size_t>> connectivity, std::vector<size_t> elementSizes);

    std::vector<std::size_t> getNodeTags() const;
    std::vector<double> getNodesCoord() const;

    std::vector<int> getElementTypes() const;
    std::vector<std::vector<std::size_t> > getElementTags() const ;
    std::vector<std::vector<std::size_t> > getConnectivity() const;
    std::vector<std::size_t>  getElementSizes() const;

    //void setStructuredFacePairs(const std::vector<EntityPair>& _gmshStructuredFacePairs);
    //std::vector<EntityPair> getStructuredFacePairs() const;

    void setNodesPeriodicity(const std::vector<std::unordered_map<int, int>>& _nodesConstraints);
    std::vector<std::unordered_map<int, int>> getNodesPeriodicity() const;


    void setMappingDirs(const std::vector<std::vector<double>>& _mappingDirs);
    std::vector<std::vector<double>> getMappingDirs() const;




};

class PBCMeshTool{

private:

    TopoDS_Shape theShape;
    vtkSmartPointer<IVtkTools_ShapeDataSource> theShapeSource;
    std::vector<EntityPair> facePairs; //Contains ids from IVtkOCC_Shape/IVtkTools_ShapeDataSource
    double meshSize;

    scaletag::fem::MeshData meshData;

     std::vector<EntityPair> gmshStructuredFacePairs; // EntityPair's in gmshFacePairs are structured <slave, master> through ImposePeriodicity()
    std::vector<std::vector<EntityPair>>  gmshStructuredEdgePairs;
     std::vector<std::vector<double>> mappingDirs;
public:
    PBCMeshTool();
    PBCMeshTool(IVtkTools_ShapeDataSource* _theShapeSource);
    ~PBCMeshTool();
    //PBCMeshTool(TopoDS_Shape* _theShape);
    //PBCMeshTool(const IVtkTools_ShapeDataSource& _theShapeSource, const TopoDS_Shape& _theShape);

    void setFacePairs(const /*std::unordered_map*/std::vector<std::pair<IVtk_IdType, IVtk_IdType>>& mappedFaces){
        facePairs.clear();
        facePairs.reserve(mappedFaces.size());
        for(const auto& p : mappedFaces) facePairs.push_back({p.first, p.second});

        //for(auto p:mappedFaces) facePairs.push_back({static_cast<int>(p.first), static_cast<int>(p.second)});
    }

	void Initialize();
    void Finalize();
    void ImposePeriodicity(const std::vector<EntityPair>& pbcFaces);
    void setEdgesPeriodicity(const std::vector<EntityPair>& structuredFacePairs);
    std::vector<std::vector<EntityPair>> getEdgesPeriodicity() const;
    void SetMeshSize(const double& meshSize);

	void mesh(const double& meshsize);

    MeshData getMeshData();
	
    std::vector<EntityPair> VIStoGMSH(const std::vector<EntityPair>& surfPairVis);

    bool isVISIdGMSHTag(IVtkTools_ShapeDataSource* shapeSource ,const std::unordered_map<int, TopoDS_Shape>& shapesTags, int id, int tag) const;


    void setStructuredFacePairs(const std::vector<EntityPair>& _gmshStructuredFacePairs);
    std::vector<EntityPair> getStructuredFacePairs() const;


    void buildMappingDirs();
    std::vector<std::vector<double>> getGMappingDirs() const;
    void buildConstraints(std::vector</*std::vector<EntityPair>>&*/std::unordered_map<int, int> > & constraints);

};





bool insert_once(const int& tg, std::vector<int>& vec);
}}

#endif
