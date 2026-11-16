#include<visualization/generatedataforvis.h>




namespace scaletag {
namespace visualization {

void executeFunction(void* arg){
    GenerateDataForVis* obj = reinterpret_cast<GenerateDataForVis*>(arg);
    //frames::Model* _model;
    vtkSmartPointer<vtkDoubleArray> points =vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkIdTypeArray> cellsTypes=vtkSmartPointer<vtkIdTypeArray>::New();
    vtkSmartPointer<vtkIdTypeArray> cellsConnectivity=vtkSmartPointer<vtkIdTypeArray>::New();
    int ncells;
    obj->setInterface(*(obj->model/*.data()*/), points, cellsTypes, cellsConnectivity, ncells) ;
    obj->pdo->GetOutput()->GetFieldData()->AddArray(points);
    obj->pdo->GetOutput()->GetFieldData()->AddArray(cellsTypes);
    obj->pdo->GetOutput()->GetFieldData()->AddArray(cellsConnectivity);
    //obj->setGeometry(points, cellsTypes, cellsConnectivity);
    if(!((obj->data).empty())) {
        for(auto& field : obj->data) for(auto& fieldTimeStep : field) obj->pdo->GetOutput()->GetFieldData()->AddArray(fieldTimeStep);
    }
}


GenerateDataForVis::GenerateDataForVis(QObject* _parent) : QObject(_parent), pdo(), do2ds(), rf(), aa()
{
//do2ds->SetInputConnection(pdo->GetOutputPort());
//rf->SetInputConnection(do2ds->GetOutputPort());
//aa->SetInputConnection(rf->GetOutputPort());
//setExecuteMethod();
//updateDataObjectToDataSet();


//data=vtkSmartPointer<vtkDataArray>::New();

initInformationKeys();
}

void GenerateDataForVis::update(){
pdo->Update();
do2ds->Update();
rf->Update();
aa->Update();
warpVec->Update();
}

void GenerateDataForVis::initialize(){
pdo->GetOutput()->Initialize(); //This clear points, cells, etc. accumulated in the dataObject associated with pdo
initInformationKeys();
pdo->Modified();
}

void GenerateDataForVis::initInformationKeys(){
nTimeStepKey = new vtkInformationIntegerKey("nTimeStepKey", "vtkIntArray");
typeDataFieldKey = new vtkInformationIntegerKey("typeDataFieldKey", "vtkIntArray");
typeDataAttributeKey = new vtkInformationIntegerKey("typeDataAttributeKey", "vtkIntArray");
natureDataKey = new vtkInformationIntegerKey("natureDataKey", "vtkIntArray");
}

void GenerateDataForVis::setInterface( frames::Model& model, vtkDoubleArray* points, vtkIdTypeArray* cellsTypes, vtkIdTypeArray* cellsConnectivity, int& ncells){
MeshPbc<double,3>* mesh=(model.getMesh());
points->SetName("points");
points->SetNumberOfComponents(3);
//points->SetNumberOfTuples(mesh->getNumberOfNodes());

cellsTypes->SetNumberOfComponents(1);
cellsTypes->SetName("cellsTypes");

cellsConnectivity->SetNumberOfComponents(1);
cellsConnectivity->SetName("cellsConnectivity");


for(size_t i=0; i<mesh->getNumberOfNodes(); i++) {
        points->InsertNextTuple(mesh->getNodes()[i].getX());

//        points->InsertNextTuple3(mesh->getNodes()[i][0],
//                                 mesh->getNodes()[i][1], mesh->getNodes()[i][2] );
       // qDebug()<<"points :" <<points->GetTuple(i)[0]<<" ";
//qDebug()<<"nodes :" <<mesh->getNodes()[i][0]<<" ";
}


size_t scaleTagElementType, elementSize;
size_t vtkElementType;
ncells = mesh->getElements().size();
//qDebug()<<"numb cells before : "<<cellsTypes->GetNumberOfTuples();
for(int i=0; i<ncells; i++){
    scaleTagElementType=mesh->getElements()[i].getElementType();
    elementSize=mesh->getElements()[i].getElementSize();

    if(i==10)
        std::cout<<"elType : "<<scaleTagElementType<<" esize : "<<elementSize<<std::endl;

    if(scaleTagElementType==4) vtkElementType=VTK_TETRA; /*VTK_HEXAHEDRON;*///To be redefined
    if(scaleTagElementType==11) vtkElementType=VTK_QUADRATIC_TETRA;
    cellsTypes->InsertNextTuple1(vtkElementType);
    cellsConnectivity->InsertNextTuple1(elementSize);

    if(scaleTagElementType != 11)
    for(size_t ie=0; ie<elementSize; ie++) cellsConnectivity->InsertNextTuple1(mesh->getConnectivity(i)[ie]);

    if(scaleTagElementType == 11){
    for(size_t ie=0; ie<elementSize-2; ie++) cellsConnectivity->InsertNextTuple1(mesh->getConnectivity(i)[ie]);
    cellsConnectivity->InsertNextTuple1(mesh->getConnectivity(i)[elementSize-1]);
    cellsConnectivity->InsertNextTuple1(mesh->getConnectivity(i)[elementSize-2]);

    //the 2 last mid-edge nodes inside GMSH and VTK are inverted
    }
}
//qDebug()<<"numb cells after: "<<cellsTypes->GetNumberOfTuples();


}


void GenerateDataForVis::setPipeline(mapperType _mapperType){
//This pipeline is used to visualize mesh and fiels from f.e.m.
do2ds->SetInputConnection(pdo->GetOutputPort());
rf->SetInputConnection(do2ds->GetOutputPort());
aa->SetInputConnection(rf->GetOutputPort());
//qobject_cast<scaletag::visualization::Visualization*>(this->parent())->setPipelineEnd((int)_mapperType);

warpVec->SetInputConnection(aa->GetOutputPort());
warpVec->SetScaleFactor(scaleFactor);

//qobject_cast<scaletag::visualization::Visualization*>(this->parent()) ->getQmlMapper(0)->SetInputConnection(aa->GetOutputPort());
getMapper(_mapperType)->SetInputConnection(/*aa*/warpVec->GetOutputPort());

getActor()->SetMapper(getMapper(_mapperType));
//qobject_cast<scaletag::visualization::Visualization*>(this->parent())  ->getRenderer()->AddActor(getActor());
//qobject_cast<scaletag::visualization::Visualization*>(this->parent())  ->update();

}


void GenerateDataForVis::setMeshEdgesPipeline(){

//vtkNew<vtkExtractEdges> edgesFilter;
//edgesFilter->SetInputConnection(do2ds->GetOutputPort());
//warpVecEdges->SetInputConnection(edgesFilter->GetOutputPort());

///*edgesMapper*/warpVecEdges->SetInputConnection(edgesFilter->GetOutputPort());
//warpVecEdges->SetScaleFactor(scaleFactor);

//vtkNew<vtkPolyDataMapper> edgesMapper;
//edgesMapper->SetInputConnection(warpVecEdges->GetOutputPort());
//meshEdgesActor->SetMapper(edgesMapper);
//meshEdgesActor->GetProperty()->SetColor(0,0,0);
//meshEdgesActor->GetProperty()->SetDiffuse(0);
//meshEdgesActor->GetProperty()->SetAmbient(1);
//meshEdgesActor->GetProperty()->SetLineWidth(2);

vtkNew<vtkExtractEdges> edgesFilter;
vtkNew<vtkPolyDataMapper> edgesMapper;

warpVecEdges->SetScaleFactor(scaleFactor);

warpVecEdges->SetInputConnection(do2ds->GetOutputPort());
edgesFilter->SetInputConnection(warpVec->GetOutputPort());
edgesMapper->SetInputConnection(edgesFilter->GetOutputPort());
meshEdgesActor->SetMapper(edgesMapper);
meshEdgesActor->GetProperty()->SetColor(0,0,0);
meshEdgesActor->GetProperty()->SetDiffuse(0);
meshEdgesActor->GetProperty()->SetAmbient(1);
meshEdgesActor->GetProperty()->SetLineWidth(2);

edgesMapper->ScalarVisibilityOff();
}


void GenerateDataForVis::setShapeDataSource(IVtkTools_ShapeDataSource* _shapeDataSource){shapeDataSource=_shapeDataSource;}
void GenerateDataForVis::setCADPipeline(){

int stepMode=1, igesMode=1; IFSelect_ReturnStatus status;
//STEPControl_Reader stepReader;
//IGESControl_Reader igesReader;
//TopoDS_Shape cadShape ;
//if(shapeDataSource==nullptr) return;
//shapeDataSource->SetShape(new IVtkOCC_Shape(cadShape));
shapeDataSource->Update();
shapeMapper->SetInputConnection(shapeDataSource->GetOutputPort());
IVtkTools::InitShapeMapper(shapeMapper); //Initializes the mapper using standard/default OCCT colors
shapeActor->SetMapper(shapeMapper);
IVtkTools_ShapeObject::SetShapeSource(shapeDataSource, shapeActor);//This ensures that the Open CASCADE shape source is properly stored inside the VTK actor or dataset's internal information object using a unique key, allowing selection tools like IVtkTools_ShapePicker to find it later

}

void GenerateDataForVis::setScaleFactor(const double& _scaleFactor){
scaleFactor=_scaleFactor;
warpVec->SetScaleFactor(scaleFactor);
warpVecEdges->SetScaleFactor(scaleFactor);
}

double GenerateDataForVis::getScaleFactor() const {return scaleFactor;}


vtkProgrammableDataObjectSource* GenerateDataForVis::getPDO(){return pdo;}
vtkDataObjectToDataSetFilter* GenerateDataForVis::getDO2DS(){return do2ds;}
vtkRearrangeFields* GenerateDataForVis::getRF(){return rf;}
vtkAssignAttribute* GenerateDataForVis::getAA(){return aa;}

vtkWarpVector* GenerateDataForVis::getWarpVector(){return warpVec;}
vtkWarpVector* GenerateDataForVis::getEdgeWarpVector(){return warpVecEdges;}

vtkMapper* GenerateDataForVis::getMapper(int mapperType){
if(mapperType==0) return theMapper;
if(mapperType==1) return theCompMapper;
}

vtkActor* GenerateDataForVis::getActor(){return theActor;}

vtkActor* GenerateDataForVis::getShapeActor(){return shapeActor;}

vtkActor* GenerateDataForVis::getMeshEdgesActor(){return meshEdgesActor;}

void GenerateDataForVis::setFields(const std::map< std::string , std::vector<std::valarray<double>> >& _fields){fields=_fields;}

std::valarray<double>  GenerateDataForVis::getField(const std::string& fieldName, size_t tps){
return fields.at(fieldName)[tps-1];
}

void GenerateDataForVis::setModel(frames::Model* _model){
//model.reset(_model);
model = _model;
}



void GenerateDataForVis::addArray(vector<vtkDoubleArray*>& varr, const string& name, int natureData, int typeData)
{
//vtkInformationIntegerKey *natureDataKey, *typeDataFieldKey, *typeDataAttributeKey, *nTimeStepKey;
int typeDataField, typeDataAttribute;

int n=varr.size();
int nTimeStep=n;
//nTimeStepKey = new vtkInformationIntegerKey("nTimeStepKey", "vtkDataArray");
//typeDataFieldKey = new vtkInformationIntegerKey("typeDataFieldKey", "vtkDataArray");
//typeDataAttributeKey = new vtkInformationIntegerKey("typeDataAttributeKey", "vtkDataArray");
//natureDataKey = new vtkInformationIntegerKey("natureDataKey", "vtkDataArray");

if(n>0) for(int i=0; i<n; i++){
    if(i==0){
            if(typeData == vtkAssignAttribute::POINT_DATA){
            typeDataField=vtkRearrangeFields::POINT_DATA;
            typeDataAttribute=vtkAssignAttribute::POINT_DATA;
            }else if(typeData ==vtkAssignAttribute::CELL_DATA){
            typeDataField=vtkRearrangeFields::CELL_DATA;
            typeDataAttribute=vtkAssignAttribute::CELL_DATA;
            }else{
            typeDataField=vtkRearrangeFields::POINT_DATA;
            typeDataAttribute=vtkAssignAttribute::POINT_DATA;}
        varr[0]->GetInformation()->Set(nTimeStepKey, nTimeStep);
        varr[0]->GetInformation()->Set(typeDataFieldKey, typeDataField);
        varr[0]->GetInformation()->Set(typeDataAttributeKey, typeDataAttribute);
        varr[0]->GetInformation()->Set(natureDataKey, natureData);

        varr[0]->SetName(name.c_str());
        //data.push_back(varr);
        } else
        {
        string s=name+to_string(i);
        varr[i]->SetName(s.c_str());
        //data.push_back(varr[i]);
        }
        data.push_back(varr);
    }

}


void GenerateDataForVis::setGeometry(vtkDoubleArray* points, vtkIdTypeArray* cellsTypes, vtkIdTypeArray* cellsConnectivity/*, int ncells*/){
// parameters may be built from meshPbc

cellsTypes->SetName("cellsTypes");
cellsConnectivity->SetName("cellsConnectivity");
points->SetName("points");
/*
cellsTypes->SetNumberOfComponent(1);
cellsConnectivity->SetNumberOfComponent(1);
*/

//for(size_t i=0; i<ncells; i++){cellsTypes->InsertNextTuple1(/*VTK_HEXAHEDRON*/cellsTypes[i]);}
/*vtkIdType ptIds[ncells+s]={};
for(size_t i=0; i<ncells+s; i++){cellsConnectivity->InsertNextTuple1(ptIds[i]);}
*/

pdo->GetOutput()->GetFieldData()->AddArray(points);
pdo->GetOutput()->GetFieldData()->AddArray(cellsTypes);
pdo->GetOutput()->GetFieldData()->AddArray(cellsConnectivity);

do2ds->SetDataSetTypeToUnstructuredGrid();
do2ds->SetPointComponent(0, "points", 0);
do2ds->SetPointComponent(1, "points", 1);
do2ds->SetPointComponent(2, "points", 2);
do2ds->SetCellTypeComponent("cellsTypes", 0);
do2ds->SetCellConnectivityComponent("cellsConnectivity", 0);
do2ds->Update();

}

void GenerateDataForVis::updateDataObjectToDataSet()
{
do2ds->SetDataSetTypeToUnstructuredGrid();
do2ds->SetPointComponent(0, "points", 0);
do2ds->SetPointComponent(1, "points", 1);
do2ds->SetPointComponent(2, "points", 2);
do2ds->SetCellTypeComponent("cellsTypes", 0);
do2ds->SetCellConnectivityComponent("cellsConnectivity", 0);
do2ds->Update();
}





// void GenerateDataForVis::executeFunction(void* arg){
//vtkSmartPointer<vtkProgrammableDataObjectSource> obj = reinterpret_cast<vtkProgrammableDataObjectSource*>(arg);
//for(auto& field : data) for(auto& fieldTimeStep : field) pdo->GetOutput()->GetFieldData()->AddArray(fieldTimeStep);
//}
void GenerateDataForVis::setExecuteMethod(){
pdo->SetExecuteMethod(executeFunction, this);
}



void GenerateDataForVis::setCurrentDataArray(const string& name, const int& frame){

rf->RemoveAllOperations();
int nTimeStep = pdo->GetOutput()->GetFieldData()->GetArray("name")->GetInformation()->Get(nTimeStepKey);
int natureData = pdo->GetOutput()->GetFieldData()->GetArray("name")->GetInformation()->Get(natureDataKey);
int typeDataField = pdo->GetOutput()->GetFieldData()->GetArray("name")->GetInformation()->Get(typeDataFieldKey);
int typeDataAttribute = pdo->GetOutput()->GetFieldData()->GetArray("name")->GetInformation()->Get(typeDataAttributeKey);

for(int i=0; i<nTimeStep; i++){rf->AddOperation(vtkRearrangeFields::COPY, (name+to_string(i)).c_str(), vtkRearrangeFields::DATA_OBJECT, typeDataField);}

if(frame<0 || frame>nTimeStep-1){currentFrame=0; aa->Assign(name.c_str(), natureData, typeDataAttribute);}
if(frame>=0 && frame<=nTimeStep-1){aa->Assign((name+to_string(frame)).c_str(), natureData, typeDataAttribute);}


currentFieldName=name; currentFrame=frame;

}

void GenerateDataForVis::next(){
// -currentFieldName : string, -currentFrame : int

vtkInformationIntegerKey *natureDataKey, *typeDataAttributeKey;
if( !(pdo->GetOutput()->GetFieldData()->GetArray(currentFieldName.c_str())) ){return;}

int n = pdo->GetOutput()->GetFieldData()->GetArray(currentFieldName.c_str())->GetNumberOfTuples();

if(0<=currentFrame && currentFrame<n-1){
currentFrame +=1;
    aa->Assign((currentFieldName+to_string(currentFrame)).c_str(), pdo->GetOutput()->GetFieldData()->GetArray(currentFieldName.c_str())->GetInformation()->Get(natureDataKey), pdo->GetOutput()->GetFieldData()->GetArray(currentFieldName.c_str())->GetInformation()->Get(typeDataAttributeKey));
}

}

}}

//previous(), firstFrame(), lastFrame()


