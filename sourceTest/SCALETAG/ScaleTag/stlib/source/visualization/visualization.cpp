#include <visualization/visualization.h>

#include <vtkExtractEdges.h>
#include <vtkGeometryFilter.h>
#include<iostream>
#include<vtkNamedColors.h>
//#include<vtkViewport.h>
#include <QWeakPointer>
#include<vtkTemporalInterpolator.h>

using namespace std;


#include<frames/project.h>

namespace scaletag {
namespace visualization {

Visualization::Visualization(QObject *parent, QQmlApplicationEngine* _engine)
    : QObject(parent)
{
    viewParent.reset(qobject_cast<QObject*>(parent));
    engine.reset(_engine);
    //orientationMarker=(nullptr);
    numberOfViewPorts=0;


    //qmlMainWindowRenderItem.reset(viewParent->findChild<QQuickVTKRenderItem*>("visual"));
//    qDebug()<<"qmlMainWin : "<<qmlMainWindowRenderItem;
    InitScaleTagColors();
    //qmlMainWindowRenderItem->renderWindow()->renderWindow()->GetInteractor()->SetInteractorStyle(qmlStyle);
    //qmlStyle->SetDefaultRenderer(qmlMainWindowRenderItem->renderer());

//    QScopedPointer<ScaleTagViewPort> mainViewPort;
//    mainViewPort.reset( new ScaleTagViewPort(this, _engine));
    //currentViewPort.reset(new visualization::ScaleTagViewPort());
    //currentDataForVis.reset(new visualization::GenerateDataForVis());
 //QObject::connect(currentViewPort.data(),&ScaleTagViewPort::selected, this, &visualization::Visualization::setCurrentViewPort); //Delay the connection
    //QObject::connect(project.data(),&frames::Project::deleteView, this, &Visualization::deleteViewPort);
    //createViewport();

   //currentDataForVis=new GenerateDataForVis(this);


    QObject::connect(this, &Visualization::currentViewPortChanged, this, &Visualization::setCurrentViewPort );

}


Visualization::~Visualization(){}

ScaleTagViewPort* Visualization::createViewport(QObject* parent){
    ScaleTagViewPort* viewPort = new ScaleTagViewPort(this, engine.data());
    viewPort->setParent(parent);
    numberOfViewPorts++;
    viewPort->setNumViewPort(numberOfViewPorts-1);
    viewPorts.append(viewPort);
emit viewPortCreated(viewPort);
    return viewPort;
}

QList<visualization::ScaleTagViewPort*>& Visualization::getViewPorts(){return viewPorts;}

/*QSharedPointer<*/visualization::ScaleTagViewPort*/*>*/ Visualization::getCurrentViewPort(){return currentViewPort;}

void Visualization::setProject(frames::Project& _project){project.reset(&_project);}

void Visualization::setPipelineEnd(int mapperType){
//Function to be removed as GenerateDataForVis owns now the pipeline to the vtkActor;
    vtkNew<vtkActor> testActor;
//    if(mapperType==0) qmlActor->SetMapper(qmlMapper);
//    if(mapperType==1) qmlActor->SetMapper(qmlCompMapper);
    if(mapperType==0) testActor->SetMapper(qmlMapper);
    if(mapperType==1) testActor->SetMapper(qmlCompMapper);
    qmlMainWindowRenderItem->renderer()->AddActor(testActor);


    {
//    qmlMainWindowRenderItem->renderWindow()->renderWindow()->GetInteractor()->SetInteractorStyle(qmlStyle);
//    qmlStyle->SetDefaultRenderer(qmlMainWindowRenderItem->renderer());
    //qmlMainWindowRenderItem->renderer()->AddActor(actor1);
    //qmlMainWindowRenderItem->renderer()->AddActor(edgesActor);
    //qmlMainWindowRenderItem->renderer()->AddActor(qmlActor);
    //qmlMainWindowRenderItem->renderer()->GradientBackgroundOn();
    //qmlMainWindowRenderItem->renderer()->SetGradientMode(vtkRenderer::GradientModes::VTK_GRADIENT_VERTICAL);
    //qmlMainWindowRenderItem->renderer()->SetBackground(scaleTagColors->GetColor3d("cobalt"/*"MidnightBlue"*/).GetData()); //0.047,0.87,0.93//colors->GetColor3d("MidnightBlue").GetData()
    //qmlMainWindowRenderItem->renderer()->SetBackground2(scaleTagColors->GetColor3d("scaleTagColor"/*"MidnightBlue"*/).GetData());
    //qmlActor->GetProperty()->SetColor(scaleTagColors->GetColor3d("solidColor"/*"MistyRose"*/).GetData());
}

    update();



}

void Visualization::update(){
    //getDataForVis()->update();
    qmlMainWindowRenderItem->update();
    qmlMainWindowRenderItem->renderer()->ResetCamera();   //uncomment this for final setup
    qmlMainWindowRenderItem->renderWindow()->renderWindow()->Render();
}

void Visualization::InitScaleTagColors(){
    double rgba[4]={0.0, 0.0, 0.0, 0.0};
    unsigned char scaleTagColor[4]={0, 51, 102,0};
    unsigned char solidColor[4]={204, 204, 204,0};
    scaleTagColors->GetColor("Carrot", rgba);
    scaleTagColors->SetColor("scaleTagColor", scaleTagColor);
    scaleTagColors->SetColor("solidColor", solidColor);
}


vtkMapper* Visualization::getQmlMapper(int mapperType){
    if (mapperType==0)return qmlMapper;
    else return qmlCompMapper;
}

vtkRenderer* Visualization::getRenderer(){return qmlMainWindowRenderItem->renderer();}

void Visualization::renderView(vtkAlgorithm* algo, int mapperType){

    // renderView takes an algorithm and render its output data through qmlMapper
    qmlMapper->SetInputConnection(algo->GetOutputPort());

    setPipelineEnd(mapperType);
}
void Visualization::renderView( frames::Part* _part){
    //vtkSmartPointer<vtkOCCTReader> reader;
    //reader->SetFileName(_part->get)

    //to be reworked may be the choice of mapper is the problem
    //vtkNew<vtkCompositePolyDataMapper> qmlCompMapper;

    // renderView takes an algorithm and render its output data through qmlMapper
    _part->initializeOCCTReader();
    qmlCompMapper->SetInputConnection(_part->getCADReader()->GetOutputPort());
    setPipelineEnd(visualization::cadMapper);
    getRenderer()->AddActor(qmlActor);
    auto* win = qmlMainWindowRenderItem->renderWindow()->renderWindow();
    win->AddRenderer(getRenderer());

    update();
    win->GetInteractor()->SetInteractorStyle(qmlStyle);
    win->GetInteractor()->Start();
    win->Render();
}

void Visualization::viewShapeActor(GenerateDataForVis* dataForVis){

//    TopoDS_Shape cadShape ;
//    _part->readCADFile(_part->getCADFile(), cadShape);

//    _part->createVISSource();
//    auto* _dataSource = _part->getVISSource();
//    _dataSource->SetShape(new IVtkOCC_Shape(cadShape));
//    _dataSource->Update();

//        auto dataForVis = new GenerateDataForVis(_part); //parameter can be nullptr
//        dataForVis->setShapeDataSource(_dataSource);
//        dataForVis->setCADPipeline();

    qmlMainWindowRenderItem->renderer()->AddActor(dataForVis->getShapeActor());
    getRenderer()->ResetCamera();
    update();
}

void Visualization::renderCADWithShapeView(frames::Part* _part){

    std::cout<<"Rendering TopoDS_Shape from : "<<_part->getCADFile().toStdString()<<std::endl;

    //_part.readStepFile("/home/ahose/Desktop/GMSH/Diabolo/build/diabolo.step");
    //_part.readStepFile("/home/ahose/Desktop/GMSH/Haltere/build/HalterePlein.step");
    //_part.readStepFile("/home/ahose/Desktop/GMSH/KelvinCellGmsh/hollow/build/kelvin.step");
    //_part.readIGESFile("/home/ahose/Desktop/GMSH/Octet/Octet-Truss.stp"); //stp or igs
    // _part.readIGESFile("/home/ahose/Desktop/GMSH/Octet/Kelvin.iges"); //stp or igs
    //_part.readIGESFile("/home/ahose/Desktop/GMSH/Octet/Altere2.igs");

    TopoDS_Shape cadShape ;
    _part->readCADFile(_part->getCADFile(), cadShape);
    /* Wrap into a VIS source that remembers CAD topology */
    //QThread::currentThread()->wait(100000);

    _part->createVISSource();
    auto* dataSource = _part->getVISSource();
    dataSource->SetShape(new IVtkOCC_Shape(cadShape));
    dataSource->Update();

    auto dataForVis = new GenerateDataForVis(_part); //parameter can be nullptr
    dataForVis->setShapeDataSource(dataSource);
    dataForVis->setCADPipeline();

    qmlMainWindowRenderItem->renderer()->AddActor(dataForVis->getShapeActor());
    getRenderer()->ResetCamera();
    update();
    //this->moveToThread(QApplication::instance()->thread());
   // dataForVis->moveToThread(QApplication::instance()->thread());
   // QMetaObject::invokeMethod(this, [this, dataForVis](){this->viewShapeActor(dataForVis);}, Qt::QueuedConnection);
   // viewShapeActor(dataForVis);

}

void Visualization::setDataForVis(GenerateDataForVis* _dataForVis){
    //currentDataForVis.reset(_dataForVis);
    currentDataForVis = _dataForVis;
}

GenerateDataForVis* Visualization::getDataForVis(){
    return currentDataForVis;
}

void Visualization::initialize(){
    //reinterpret_cast<QWindow*>(viewParent.data())->setVisible(false); //This is important and make the container/qwindow not be appeared in a separated window
    //qobject_cast<QWindow*>(viewParent.data())->destroy();
    //QQuickVTKRenderItem *qmlMainWindowRenderItem =viewParent->findChild<QQuickVTKRenderItem*>("visual");

    qmlMainWindowRenderItem.reset(viewParent->findChild<QQuickVTKRenderItem*>("visual"));//cout<<endl<<qmlMainWindowRenderItem;
    vtkSmartPointer<vtkAxesActor> axes = vtkAxesActor::New();
    qmlAxes = vtkAxesActor::New();
    //orientationMarker=vtkOrientationMarkerWidget::New();

    //vtkNew<vtkNamedColors> colors;

    double rgba[4]={0.0, 0.0, 0.0, 0.0};
//    unsigned char scaleTagColor[4]={0, 51, 102,0};
//    unsigned char solidColor[4]={204, 204, 204,0};
//    scaleTagColors->GetColor("Carrot", rgba);
//    scaleTagColors->SetColor("scaleTagColor", scaleTagColor);
//    scaleTagColors->SetColor("solidColor", solidColor);

    qmlMainWindowRenderItem->renderer()->GradientBackgroundOn();
    //qmlMainWindowRenderItem->renderer()->SetGradientMode(vtkRenderer::GradientModes::VTK_GRADIENT_VERTICAL);
    qmlMainWindowRenderItem->renderer()->SetGradientMode(vtkRenderer::GradientModes::VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_SIDE);
    //qmlMainWindowRenderItem->renderer()->SetGradientMode(vtkRenderer::GradientModes::VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_CORNER);
    //prefer background1->cobalt && background2 Midnightblue
    qmlMainWindowRenderItem->renderer()->SetBackground(scaleTagColors->GetColor3d("cobalt"/*"cobalt"*//*"MidnightBlue"*/).GetData()); //0.047,0.87,0.93//colors->GetColor3d("MidnightBlue").GetData()
    qmlMainWindowRenderItem->renderer()->SetBackground2(scaleTagColors->GetColor3d("scaleTagColor"/*"MidnightBlue"*/).GetData());

    //qmlMainWindowRenderItem->renderer()->SetBackground(colors->GetColor3d("GhostWhite"/*"WhiteSmoke"*//*"MintCream"*//*"cobalt"*//*"MidnightBlue"*/).GetData()); //0.047,0.87,0.93


    qmlOrientationMarker->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    qmlOrientationMarker->SetOrientationMarker(axes);
    qmlOrientationMarker->SetInteractor(qmlMainWindowRenderItem->renderWindow()->renderWindow()->GetInteractor());
    //qmlOrientationMarker->SetViewport(0.0, 0.0, 0.25, 0.25);
    qmlOrientationMarker->SetViewport(0.0, 0.0, 0.207, 0.3);
    //qmlOrientationMarker->SetZoom(1.5); //to reduce the padding
    axes->SetTotalLength(1.0, 1.0, 1.0);      // default is often smaller effectively
    axes->SetShaftTypeToCylinder();
    axes->SetCylinderRadius(0.08);            // thinner shafts read as "less padded"
    axes->SetConeRadius(0.6);

    vtkCaptionActor2D* xCaption = axes->GetXAxisCaptionActor2D();
    vtkCaptionActor2D* yCaption = axes->GetYAxisCaptionActor2D();
    vtkCaptionActor2D* zCaption = axes->GetZAxisCaptionActor2D();
    // Width/height are normalized viewport-fraction-of-actor sizes
    xCaption->SetWidth(0.05);
    xCaption->SetHeight(0.05);
    yCaption->SetWidth(0.05);
    yCaption->SetHeight(0.05);
    zCaption->SetWidth(0.05);
    zCaption->SetHeight(0.05);
    for (auto* caption : {xCaption, yCaption, zCaption})
    {
    caption->GetTextActor()->SetTextScaleModeToNone(); // stop auto-scaling to caption box
    caption->GetCaptionTextProperty()->SetFontSize(18); // now this actually takes effect
    caption->GetCaptionTextProperty()->BoldOn();
    caption->GetCaptionTextProperty()->SetColor(1, 1, 1); // optional: make sure it's visible
    }

    qmlOrientationMarker->SetEnabled(1);
    //qmlOrientationMarker->InteractiveOn(); //One can move it
    qmlOrientationMarker->InteractiveOff(); //One cannot move it

    qmlMainWindowRenderItem->renderWindow()->renderWindow()->GetInteractor()->SetInteractorStyle(qmlStyle);
   //qmlStyle->SetDefaultRenderer(qmlMainWindowRenderItem->renderer());
    qmlMainWindowRenderItem->renderer()->AddActor(qmlActor);
    qmlActor->GetProperty()->SetColor(scaleTagColors->GetColor3d("solidColor"/*"MistyRose"*/).GetData());

   // qmlMainWindowRenderItem->update();
   // qmlMainWindowRenderItem->renderer()->ResetCamera();
    qmlMainWindowRenderItem->renderWindow()->renderWindow()->Render();



    //visualization->initialize();
    //initSelection(SM_Face);

}



void Visualization::initSelection(IVtk_SelectionMode mode){

    selection = std::make_unique<SelectionTool>();
    //        selection->In

    auto* ren = getRenderer();
    auto* iren = getRenderer()->GetRenderWindow()->GetInteractor();
    vtkSmartPointer<IVtkTools_ShapePicker> picker = vtkSmartPointer<IVtkTools_ShapePicker>::New();

    if(ren==nullptr || iren==nullptr ||picker==nullptr) {
    std::cout<<"Problem in the view configuration "<<std::endl;
    return;
    }
    selection->Initialize(iren, ren, picker, mode);
    std::cout<<"Selectiion mode has been initialized"<<std::endl;

}

void Visualization::endSelection(){selection.reset();}


void Visualization::init(const QString& filepath){

//vtkNew<vtkNamedColors> colors;
//    unsigned char scaleTagColor[4]={0, 51, 102,0};
//    unsigned char solidColor[4]={204, 204, 204,0};
//    colors->SetColor("scaleTagColor", scaleTagColor);
//    colors->SetColor("solidColor", solidColor);

 //cout<<"Called"<<endl;

string stringFile=filepath.toStdString();

MeshPbc<double,3> meshpbc;
meshpbc.marcInitialize(stringFile);
//meshpbc.reorder();
cout<<"file : "<<stringFile<< " Imported "<<std::endl;
vtkNew<vtkUnstructuredGrid> ugrid;
vtkNew<vtkPoints> points;
vtkIdType npts=meshpbc.getNumberOfNodes();
vtkIdType ne=meshpbc.getElements().size();
vtkIdType vtkne=ne;
vtkIdType vtkesize=meshpbc.getElements()[0].getElementSize();
ugrid->AllocateExact(vtkne, vtkesize);
double* pt;
for(vtkIdType i=0; i<npts; i++) {
    pt=meshpbc.getNodes()[i].getX();
    points->InsertNextPoint(pt);}

ugrid->SetPoints(points);

vtkIdType* elementalIdList;
vtkIdType esize;
for(vtkIdType e=0; e<ne; e++){
    esize=meshpbc.getElements()[e].getElementSize();
    elementalIdList=new vtkIdType[esize];
    for(vtkIdType i=0; i<esize; i++){
        //elementalIdList[i]=meshpbc.getElements()[e].getConnectivity()[i]-1;
        elementalIdList[i]=meshpbc.getConnectivity(e)[i];
    }
    ugrid->InsertNextCell(VTK_TETRA/*VTK_QUADRATIC_TETRA*/, esize, elementalIdList);}
ugrid->Squeeze();

vtkNew<vtkGeometryFilter> geoFilter;
geoFilter->SetInputData(ugrid);
vtkNew<vtkPolyDataMapper> polyMapper;
polyMapper->SetInputConnection(geoFilter->GetOutputPort());
polyMapper->ScalarVisibilityOff();
vtkNew<vtkActor> actor1;
actor1->SetMapper(polyMapper);
actor1->GetProperty()->SetColor(scaleTagColors->GetColor3d("solidColor"/*"Gray"*//*"LightSlateGray"*//*"Silver"*//*"DimGray"*//*"MistyRose"*/).GetData());
{//The diffuse light might be switched off to make the
    //of the actor independent of the orientation
    actor1->GetProperty()->SetDiffuse(0);
    actor1->GetProperty()->SetAmbient(1);
    //To avoid any fancy shading
    actor1->GetProperty()->SetInterpolationToFlat();
}

vtkNew<vtkExtractEdges> edgesFilter;
edgesFilter->SetInputConnection(geoFilter->GetOutputPort());
vtkNew<vtkPolyDataMapper> edgesMapper;
edgesMapper->SetInputConnection(edgesFilter->GetOutputPort());
vtkNew<vtkActor> edgesActor;
edgesActor->SetMapper(edgesMapper);
edgesActor->GetProperty()->SetColor(0,0,0);
edgesActor->GetProperty()->SetDiffuse(0);
edgesActor->GetProperty()->SetAmbient(1);
edgesActor->GetProperty()->SetLineWidth(2);



        vtkNew<vtkDataSetMapper> mapper;
        mapper->SetInputData(ugrid);
        vtkNew<vtkActor> actor;
        //actor->GetProperty()->EdgeVisibilityOn();
       //actor->GetProperty()->SetRenderLinesAsTubes(1);
       qmlActor->SetMapper(qmlMapper);

        //QQuickVTKRenderItem * qmlMainWindowRenderItem =viewParent->findChild<QQuickVTKRenderItem*>("visual");
        vtkNew<vtkInteractorStyleTrackballCamera> style;
        qmlMainWindowRenderItem->renderWindow()->renderWindow()->GetInteractor()->SetInteractorStyle(style);
        style->SetDefaultRenderer(qmlMainWindowRenderItem->renderer());


        if(qmlMainWindowRenderItem!=nullptr){
        qmlMainWindowRenderItem->renderer()->AddActor(actor1);
         qmlMainWindowRenderItem->renderer()->AddActor(edgesActor);
        qmlMainWindowRenderItem->renderer()->GradientBackgroundOn();
        //qmlMainWindowRenderItem->renderer()->SetGradientMode(vtkRenderer::GradientModes::VTK_GRADIENT_VERTICAL);
        qmlMainWindowRenderItem->renderer()->SetBackground(scaleTagColors->GetColor3d("cobalt"/*"MidnightBlue"*/).GetData()); //0.047,0.87,0.93//colors->GetColor3d("MidnightBlue").GetData()
        qmlMainWindowRenderItem->renderer()->SetBackground2(scaleTagColors->GetColor3d("scaleTagColor"/*"MidnightBlue"*/).GetData());
        actor->GetProperty()->SetColor(scaleTagColors->GetColor3d("solidColor"/*"MistyRose"*/).GetData());
         qmlMainWindowRenderItem->update();}


         qmlMainWindowRenderItem->renderer()->ResetCamera();
         qmlMainWindowRenderItem->renderWindow()->renderWindow()->Render();


}

void Visualization::viewResults(){
         std::cout<<"View Results from Visualization class"<<std::endl;

         size_t nt = getDataForVis()->getDO2DS()->GetOutput()->GetNumberOfPoints();
         size_t nc = 3;
         vtkNew<vtkDoubleArray> disp0;
         disp0->SetName("disp0");
         //disp0->SetNumberOfTuples(nt);
         disp0->SetNumberOfComponents(nc);

         for(vtkIdType np=0; np<nt ; np++ )
         {
         double tpl[nc];
         for(vtkIdType c=0; c<nc; c++) tpl[c] = getDataForVis()->getField("disp0", 0)[np*nc+c]; //tps=0
         //disp0->SetValue3 [np]=tpl;
         disp0->InsertNextTuple(tpl);
         }

         getDataForVis()->getDO2DS()->GetOutput()->GetFieldData()->AddArray(disp0);
         getDataForVis()->getRF()->AddOperation(
             vtkRearrangeFields::COPY,          // Type d'opération (MOVE ou COPY)
             "disp0",                     // Nom du tableau cible
             vtkRearrangeFields::DATA_OBJECT,    // Emplacement d'origine (Source)  //for field data
             vtkRearrangeFields::POINT_DATA     // Emplacement de destination (Target)
             );
         getDataForVis()->getAA()->Assign("disp0",vtkDataSetAttributes::SCALARS/*VECTORS*/, vtkAssignAttribute::POINT_DATA);
         getDataForVis()->getAA()->Update();

         vtkNew<vtkLookupTable> lut;
         lut->SetNumberOfTableValues(256);
         lut->SetHueRange(0.667, 0.0); // Du bleu au rouge
         lut->Build();

         //qmlMapper->SetScalarRange(-0.005, 0.011);

         vtkNew<vtkScalarBarActor> act;
         act->SetLookupTable(lut);
         act->SetTitle("disp0");
         act->SetNumberOfLabels(5);
         act ->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
         act->SetPosition(/*0.85, 0.1*/0.05, 0.50);  // Coin inférieur gauche (X, Y)
         act->GetPosition2Coordinate()->SetCoordinateSystemToViewport();
         act->SetWidth(0.08);           // Largeur relative à la fenêtre
         act->SetHeight(0.35);
         act->SetMaximumWidthInPixels(90);
         act->SetMaximumHeightInPixels(300);
         //vtkNew<vtkScalarBarWidget> actwid;
         //actwid->SetInteractor(qmlMainWindowRenderItem->renderWindow()->renderWindow()->GetInteractor());
         //actwid->SetScalarBarActor(act);


         act->GetTitleTextProperty()->SetColor(1, 1, 1); // Noir
         act->GetLabelTextProperty()->SetColor(1, 1, 1);
         act->SetVerticalTitleSeparation(20); // pixels of gap between title and bar
         act->GetTitleTextProperty()->SetFontSize(14);
         act->SetLabelFormat("%-6.2f"); // Affiche 2 décimales
         act->UnconstrainedFontSizeOn(); //Si le texte des étiquettes devient illisible ou trop gros

         //getQmlMapper(0)->ScalarVisibilityOn();

         //getQmlMapper(0)->SetLookupTable(lut);
         //getQmlMapper(0)->SetScalarModeToUsePointData(); // Without this, when CAD(.stp) is visualized, fields are hidden after
         getDataForVis()->getMapper(0)->SetLookupTable(lut);
         getDataForVis()->getMapper(0)->SetScalarModeToUsePointData();
         getDataForVis()->getMapper(0)->SetScalarRange(-0.005, 0.011);
         if( !(qmlMainWindowRenderItem->renderer()->HasViewProp(act)))
         qmlMainWindowRenderItem->renderer()->AddActor2D(act);
         //qmlMainWindowRenderItem->renderer()->AddActor2D(act);
         //actwid->EnabledOn();//On();


         update();

         getDataForVis()->setMeshEdgesPipeline();
         getRenderer()->AddActor( getDataForVis()->getMeshEdgesActor() );

         update();
}

void Visualization::viewFieldResults(const QString& fieldName, int comp, int timeRank, double scaleFactor, bool showDeformed){

         //timeRank=0 corresponds to the undeformed shape

         if (fieldName.isEmpty()) {
         std::cerr << "viewFieldResults: empty field name (comp=" << comp
                   << ", timeRank=" << timeRank << ")" << std::endl;
         return;
         }

         std::cout<<"Showing results for "<<fieldName.toStdString()<<" at time step : "<<timeRank<<"."<<std::endl;

         QString warpFieldName = "default";

         if(fieldName == QString("disp0") || fieldName=="disp1" || fieldName == "disp2" || fieldName == "disp3"
             || fieldName == QString("disp4") || fieldName == QString("disp5")  ) warpFieldName = fieldName;

         if(fieldName == QString("force0")) warpFieldName = "disp0";
         if(fieldName == QString("force1")) warpFieldName = "disp1";
         if(fieldName == QString("force2")) warpFieldName = "disp2";
         if(fieldName == QString("force3")) warpFieldName = "disp3";
         if(fieldName == QString("force4")) warpFieldName = "disp4";
         if(fieldName == QString("force5")) warpFieldName = "disp5";


         size_t nt = getDataForVis()->getDO2DS()->GetOutput()->GetNumberOfPoints();
         size_t nc = 3;

         auto fieldData = getDataForVis()->getDO2DS()->GetOutput()->GetFieldData();
                  fieldData->Initialize();


         vtkNew<vtkDoubleArray> fieldToShow;  //Instead of disp0
         fieldToShow->SetName(fieldName.toStdString().data());
         fieldToShow->SetNumberOfComponents(nc);
         fieldToShow->SetNumberOfTuples(nt);



         if(timeRank==0) fieldToShow->Fill(0.0);
         else
         for(vtkIdType np=0; np<nt ; np++ )
         {
         double tpl[nc];
         for(vtkIdType c=0; c<nc; c++){
        //if(timeRank==0) tpl[c] = 0. ; //Undeformed
        //else
        tpl[c] = getDataForVis()->getField(fieldName.toStdString(), timeRank)[np*nc+c]; //tps=1
         }

         //fieldToShow->InsertNextTuple(tpl);
        fieldToShow->InsertTuple(np, tpl);
         }




         /*getDataForVis()->getDO2DS()->GetOutput()->GetFieldData()*/fieldData->AddArray(fieldToShow);

         vtkNew<vtkDoubleArray> warpField;


         if(fieldName != warpFieldName && warpFieldName != "default") {

         warpField->SetName(warpFieldName.toStdString().c_str());
         warpField->SetNumberOfComponents(nc);
         warpField->SetNumberOfTuples(nt);


        if(timeRank==0) warpField->Fill(0.0);
        else

        for(vtkIdType np = 0; np < nt; np++) {
        double tpl[3];
        for(vtkIdType c = 0; c < nc; c++) {
            tpl[c] = getDataForVis()->getField(warpFieldName.toStdString(), timeRank)[np * nc + c];
        }
        //warpField->InsertNextTuple(tpl);
        warpField->InsertTuple(np, tpl);
         }

         fieldData->AddArray(warpField);
         }

         getDataForVis()->getRF()->RemoveAllOperations();
         getDataForVis()->getRF()->AddOperation(
             vtkRearrangeFields::COPY,          // Type d'opération (MOVE ou COPY)
             fieldName.toStdString().data(),                     // Nom du tableau cible
             vtkRearrangeFields::DATA_OBJECT,    // Emplacement d'origine (Source)  //for field data
             vtkRearrangeFields::POINT_DATA     // Emplacement de destination (Target)
             );

         if(fieldName != warpFieldName && warpFieldName != "default") {
         getDataForVis()->getRF()->AddOperation(
             vtkRearrangeFields::COPY, warpFieldName.toStdString().c_str(),
             vtkRearrangeFields::DATA_OBJECT, vtkRearrangeFields::POINT_DATA
             );
         }

         getDataForVis()->getAA()->Assign(fieldName.toStdString().data(),vtkDataSetAttributes::SCALARS/*VECTORS*/, vtkAssignAttribute::POINT_DATA);
         getDataForVis()->getWarpVector()->SetInputArrayToProcess(0,0,0,  vtkDataObject::FIELD_ASSOCIATION_POINTS, /*fieldName*/warpFieldName.toStdString().data());
                  getDataForVis()->getEdgeWarpVector()->SetInputArrayToProcess(0,0,0,  vtkDataObject::FIELD_ASSOCIATION_POINTS, /*fieldName*/warpFieldName.toStdString().data());
         getDataForVis()->setScaleFactor(scaleFactor);

                  if(showDeformed){
         getDataForVis()->getWarpVector()->SetScaleFactor(scaleFactor);
         getDataForVis()->getEdgeWarpVector()->SetScaleFactor(scaleFactor);}
                  else {getDataForVis()->getWarpVector()->SetScaleFactor(0.0);
         getDataForVis()->getEdgeWarpVector()->SetScaleFactor(0.0);}

         getDataForVis()->getAA()->Update();
         getDataForVis()->getWarpVector()->Update();

         vtkNew<vtkLookupTable> lut;
         if(comp==4)  {lut->SetVectorModeToMagnitude(); std::cout<<"Executing Magn branch"<<std::endl;}
         else{
         lut->SetVectorModeToComponent();
         //comp == 4 ? lut->SetVectorComponent(-1) : lut->SetVectorComponent(comp-1);
         lut->SetVectorComponent(comp-1);
            }
         lut->SetNumberOfTableValues(256);
         lut->SetHueRange(0.667, 0.0); // Du bleu au rouge
         lut->Build();

         //qmlMapper->SetScalarRange(-0.005, 0.011);

         //vtkNew<vtkScalarBarActor> act;
         act->SetLookupTable(lut);
         act->SetTitle(fieldName.toStdString().data());
         act->SetNumberOfLabels(5);
         act ->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
         act->SetPosition(/*0.85, 0.1*/0.05, 0.50);  // Coin inférieur gauche (X, Y)
         act->GetPosition2Coordinate()->SetCoordinateSystemToViewport();
         act->SetWidth(0.08);           // Largeur relative à la fenêtre
         act->SetHeight(0.35);
         act->SetMaximumWidthInPixels(90);
         act->SetMaximumHeightInPixels(300);
         //vtkNew<vtkScalarBarWidget> actwid;
         //actwid->SetInteractor(qmlMainWindowRenderItem->renderWindow()->renderWindow()->GetInteractor());
         //actwid->SetScalarBarActor(act);


         act->GetTitleTextProperty()->SetColor(1, 1, 1); // Noir
         act->GetLabelTextProperty()->SetColor(1, 1, 1);
         act->SetVerticalTitleSeparation(20); // pixels of gap between title and bar
         act->GetTitleTextProperty()->SetFontSize(14);
         act->SetLabelFormat("%-6.2f"); // Affiche 2 décimales
         act->UnconstrainedFontSizeOn(); //Si le texte des étiquettes devient illisible ou trop gros

         //getQmlMapper(0)->ScalarVisibilityOn();

         //getQmlMapper(0)->SetLookupTable(lut);

         //getQmlMapper(0)->SetScalarModeToUsePointData(); // Without this, when CAD(.stp) is visualized, fields are hidden after

         double* range; double eps;

//         auto* valArray = static_cast<vtkDoubleArray*>(getDataForVis()->getWarpVector()->
//                          GetOutput()->GetPoints()->GetData());
         if(comp==4)
         range = fieldToShow->GetRange(-1); //Provide -1 to get magnitude range. 0,1,2... for components magnitude
         else range = fieldToShow->GetRange(comp-1);

         eps = (range[1]-range[0])/( std::max( std::fabs(range[1]), std::fabs(range[0])) ) ;
         eps = eps*1.e-5;
        // std::cout<<"eps : "<<eps<<std::endl;
         getDataForVis()->getMapper(0)->SetLookupTable(lut);
         getDataForVis()->getMapper(0)->SetScalarModeToUsePointFieldData(); //SetScalarToUsePointData();
         getDataForVis()->getMapper(0)->SetScalarVisibility(true);
         getDataForVis()->getMapper(0)->SelectColorArray(fieldName.toStdString().data());
//         getDataForVis()->getMapper(0)->SetScalarRange(-0.005, 0.011);
         if(std::fabs(range[0] - range[1]) < 1.e-9)
                getDataForVis()->getMapper(0)->SetScalarRange(range[0]-0.01, range[1]+0.01);
         else
             getDataForVis()->getMapper(0)->SetScalarRange(range[0]-eps, range[1]+eps);

         if( !(qmlMainWindowRenderItem->renderer()->HasViewProp(act)))
         qmlMainWindowRenderItem->renderer()->AddActor2D(act);
         //actwid->EnabledOn();//On();


        // update();

         getDataForVis()->setMeshEdgesPipeline();
         getRenderer()->AddActor( getDataForVis()->getMeshEdgesActor() );

         update();
}

void Visualization::setCurrentViewPort(visualization::ScaleTagViewPort* viewport)
{
  //currentViewPort.reset(viewport);
         currentViewPort = viewport ;

         std::cout<<"ViewPort has been changed."<<currentViewPort<<std::endl;
}

void Visualization::deleteViewPort(int _numViewPort){
         int n=viewPorts.size();
         if(_numViewPort <n){
         for(int i=_numViewPort; i<n; i++){viewPorts.at(i)->setNumViewPort(i-1);
        viewPorts.removeAt(_numViewPort-1);
         }
         }

}


} // namespace visualization
} // namespace scaletag

