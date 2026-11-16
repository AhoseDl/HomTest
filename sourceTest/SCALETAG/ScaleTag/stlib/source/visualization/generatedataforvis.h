#ifndef GENERATEDATAFORVIS
#define GENERATEDATAFORVIS
#include<QObject>

#include<visualization/visualization.h>

#include<frames/model.h>
#include<vtkProgrammableDataObjectSource.h>
#include<vtkDataObjectToDataSetFilter.h>
#include<vtkRearrangeFields.h>
#include<vtkAssignAttribute.h>
#include<vtkWarpVector.h>
#include<vtkDataArray.h>
#include<vtkIdTypeArray.h>
#include<vtkUnstructuredGrid.h>
#include<vtkExtractEdges.h>

#include<vtkLookupTable.h>
#include<vtkScalarBarActor.h>
#include<vtkScalarBarWidget.h>
#include <vtkTextProperty.h>

#include <vtkInformationIntegerKey.h>
#include<vtkInformation.h>
#include<models/MeshPbc.h>
#include<vtkFieldData.h>

#include <QSharedPointer>
#include<functional>
#include <vtkDoubleArray.h>
#include<QDebug>

#include<map>


#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkOCC_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <STEPControl_Reader.hxx>
#include<IGESControl_Reader.hxx>
#include<IVtkTools.hxx>
#include <IFSelect_ReturnStatus.hxx>  //For checking if file read succeeds
#include <IVtk_Types.hxx>
#include<IVtkTools_ShapeObject.hxx>
#include<BRepCheck_Analyzer.hxx>
#include<BRepMesh_IncrementalMesh.hxx>

#include<cad.h>
#include<memory.h>





namespace scaletag{
namespace visualization{

//class Visualization;

using namespace std;

enum mapperType{meshMapper=0, cadMapper=1};


class GenerateDataForVis : public QObject{

    Q_OBJECT


private :

vtkNew<vtkProgrammableDataObjectSource> pdo;
vtkNew<vtkDataObjectToDataSetFilter> do2ds;
vtkNew<vtkRearrangeFields> rf;
vtkNew<vtkAssignAttribute> aa;

vtkNew<vtkWarpVector> warpVec;
vtkNew<vtkWarpVector> warpVecEdges;

vtkNew<vtkDataSetMapper> theMapper;
vtkNew<vtkCompositePolyDataMapper> theCompMapper;
vtkNew<vtkActor> theActor;
vtkNew<vtkActor> meshEdgesActor;


//QSharedPointer<frames::Model> model;
frames::Model* model;

std::vector<std::vector<vtkDoubleArray*> > data; // To be remove ; use fields instead

std::map<std::string, std::vector<std::valarray<double>> > fields;

std::string currentFieldName = "name"; int currentFrame=0;

vtkInformationIntegerKey *natureDataKey, *typeDataFieldKey, *typeDataAttributeKey, *nTimeStepKey;

//vtkSmartPointer<vtkDoubleArray> points;
//vtkSmartPointer<vtkIdTypeArray> cellsTypes;
//vtkSmartPointer<vtkIdTypeArray> cellsConnectivity; int ncells;


IVtkTools_ShapeDataSource* shapeDataSource; //For CAD viewing purpose
vtkNew<vtkCompositePolyDataMapper> shapeMapper;
vtkNew<vtkActor> shapeActor;

double scaleFactor;

public:
GenerateDataForVis(QObject* parent=nullptr);
void setModel(frames::Model* _model);
void update();
void initialize();
void initInformationKeys();
void setData(const vector<vector<vtkDataArray*> >& data);
//void setInterface(const MeshPbc<double,3>& mesh, vtkDataArray* points, vtkIdTypeArray* cellsTypes, vtkIdTypeArray* cellsConnectivity, int& ncells);
void setInterface( frames::Model& model, vtkDoubleArray* points, vtkIdTypeArray* cellsTypes, vtkIdTypeArray* cellsConnectivity, int& ncells);
void setGeometry(vtkDoubleArray* points, vtkIdTypeArray* cellsTypes, vtkIdTypeArray* cellsConnectivity/*, int ncells*/);
void addArray(vector<vtkDoubleArray*>& varr, const std::string& name="", int natureData=0, int typeData=0);
friend void executeFunction(void* arg);
//std::function<void*(void*)> f=&GenerateDataForVis::executeFunction;
void setExecuteMethod();
void setPipeline(mapperType=meshMapper);
void setCurrentDataArray(const string& name, const int& frame);
void updateDataObjectToDataSet();

void setMeshEdgesPipeline();

void setFields(const std::map< std::string , std::vector<std::valarray<double> > >& _fields);

std::valarray<double>  getField(const std::string& fieldName, size_t tps=0);


void next();

vtkProgrammableDataObjectSource* getPDO();
vtkDataObjectToDataSetFilter* getDO2DS();
vtkRearrangeFields* getRF();
vtkAssignAttribute* getAA();

vtkWarpVector* getWarpVector();
vtkWarpVector* getEdgeWarpVector();
void setScaleFactor(const double& _scaleFactor) ;
double getScaleFactor() const ;

vtkMapper* getMapper(int mapperType);  //to be changed. #rent fcts for geo and mesh
vtkActor* getActor();
vtkActor* getShapeActor();
vtkActor* getMeshEdgesActor();


void setShapeDataSource(IVtkTools_ShapeDataSource* _shapeDataSource);
void setCADPipeline();






};

}}
#endif

/*

in addArray :
natureData = vtkDataSetAttributes::{SCALARS, VECTORS, ...}
typeDataField = vtkRearrangeFields::{DATA_OBJECT, POINT_DATA, CELL_DATA, ...}
typeDataAttribute = vtkAssignAttribute::{POINT_DATA, CELL_DATA, ...}

in setGeometry :
points : vtkDataArray
cellsType : vtkIdTypeArray 
cellsConnectivity : vtkIdTypeArray 

*/
