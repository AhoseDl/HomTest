#include<visualization/scaletagviewport.h>

namespace scaletag {
namespace visualization {

ScaleTagViewPort::ScaleTagViewPort(QObject *_parent , QQmlApplicationEngine* _engine ) : QObject(_parent){
 engine = _engine;
engine->load(viewUrl);
int viewUrlRank = _engine->rootObjects().size()-1;
win.reset(reinterpret_cast<QWindow*>(_engine->rootObjects().at(viewUrlRank)));
reinterpret_cast<QWindow*>(_engine->rootObjects().at(viewUrlRank))->setTitle(name);
//ren = reinterpret_cast<QQuickVTKRenderItem*>(_engine->rootObjects().at(viewUrlRank)->findChild<QQuickVTKRenderItem*>("visual"))->renderer();
this->setParent(win.data());

//dataForVis.reset(new visualization::GenerateDataForVis());

 QObject::connect(win.data(), &QWindow::destroyed, this, &scaletag::visualization::ScaleTagViewPort::onWindowDeleted);
// QObject::connect(this, SIGNAL(viewPortDeleted(int)),this->parent(), SLOT(deleteViewPort(int))); //because didn't find a way to reference receiver type here
qquickvtkItem =_engine->rootObjects().at(viewUrlRank)->findChild<QQuickVTKRenderItem*>("visual");
 //ren=qquickvtkItem->renderer();
//ren = vtk::TakeSmartPointer(qquickvtkItem->renderer());
ren = vtkSmartPointer<vtkRenderer>::Take(qquickvtkItem->renderer());
meshActor->SetMapper(meshMapper);
shapeActor->SetMapper(shapeMapper);
ren->AddActor(meshActor);
qquickvtkItem->renderer()->AddActor(meshActor);

    addOrientationGizmo();

   QQuickVTKRenderItem *qquickvtkItem =_engine->rootObjects().at(viewUrlRank)->findChild<QQuickVTKRenderItem*>("visual");
   //qDebug()<<"qquickvtkItem : "<<qquickvtkItem;
   /*vtkSmartPointer<vtkAxesActor> */
    //orientationMarker=vtkOrientationMarkerWidget::New();

    vtkNew<vtkNamedColors> colors;
    double rgba[4]={0.0, 0.0, 0.0, 0.0};
    unsigned char scaleTagColor[4]={0, 51, 102,0};
    unsigned char solidColor[4]={204, 204, 204,0};
    colors->GetColor("Carrot", rgba);
    colors->SetColor("scaleTagColor", scaleTagColor);
    colors->SetColor("solidColor", solidColor);

    _engine->rootObjects().at(viewUrlRank)->findChild<QQuickVTKRenderItem*>("visual")->renderer()->GradientBackgroundOn();
    qquickvtkItem->renderer()->GradientBackgroundOn();
    qquickvtkItem->renderer()->SetGradientMode(vtkRenderer::GradientModes::VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_CORNER);
    qquickvtkItem->renderer()->SetBackground(colors->GetColor3d("cobalt"/*"cobalt"*//*"MidnightBlue"*/).GetData()); //0.047,0.87,0.93//colors->GetColor3d("MidnightBlue").GetData()
    qquickvtkItem->renderer()->SetBackground2(colors->GetColor3d("scaleTagColor"/*"MidnightBlue"*/).GetData());

    //vtkNew<vtkOrientationMarkerWidget> orientationMarker;


    qquickvtkItem->update();
    qquickvtkItem->renderer()->ResetCamera();
    qquickvtkItem->renderWindow()->renderWindow()->Render();

}

void ScaleTagViewPort::addOrientationGizmo(){

    vtkNew<vtkNamedColors> colors;
    double rgba[4]={0.0, 0.0, 0.0, 0.0};
    unsigned char scaleTagColor[4]={0, 51, 102,0};
    unsigned char solidColor[4]={204, 204, 204,0};
    colors->GetColor("Carrot", rgba);
    colors->SetColor("scaleTagColor", scaleTagColor);
    colors->SetColor("solidColor", solidColor);

    int viewUrlRank = engine->rootObjects().size()-1;
    QQuickVTKRenderItem *qquickvtkItem =engine->rootObjects().at(viewUrlRank)->findChild<QQuickVTKRenderItem*>("visual");

    axesActor = vtkSmartPointer<vtkAxesActor>::New();
    orientationMarker = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    orientationMarker->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    orientationMarker->SetOrientationMarker(axesActor);
    orientationMarker->SetInteractor(qquickvtkItem->renderWindow()->renderWindow()->GetInteractor());
    orientationMarker->SetViewport(0.0, 0.0, 0.207, 0.3);

    //qmlOrientationMarker->SetZoom(1.5); //to reduce the padding
    axesActor->SetTotalLength(1.0, 1.0, 1.0);      // default is often smaller effectively
    axesActor->SetShaftTypeToCylinder();
    axesActor->SetCylinderRadius(0.08);            // thinner shafts read as "less padded"
    axesActor->SetConeRadius(0.6);

    vtkCaptionActor2D* xCaption = axesActor->GetXAxisCaptionActor2D();
    vtkCaptionActor2D* yCaption = axesActor->GetYAxisCaptionActor2D();
    vtkCaptionActor2D* zCaption = axesActor->GetZAxisCaptionActor2D();
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
    orientationMarker->SetEnabled(1);
    orientationMarker->InteractiveOff();
}

ScaleTagViewPort::~ScaleTagViewPort(){
    emit viewPortDeleted(this->getNumViewPort());
}

vtkRenderer* ScaleTagViewPort::getRenderer(){return ren.GetPointer();}
QWindow* ScaleTagViewPort::getWindow(){return win.data();}

void ScaleTagViewPort::setNumViewPort(int _num){numViewport=_num;}
int ScaleTagViewPort::getNumViewPort(){return numViewport;}

//QSharedPointer<visualization::GenerateDataForVis> ScaleTagViewPort::getDataForVis(){return dataForVis;}

visualization::GenerateDataForVis* ScaleTagViewPort::getDataForVis(){return dataForVis;}
void ScaleTagViewPort::setDataForVis(visualization::GenerateDataForVis* _dataForVis){
    //dataForVis.reset(_dataForVis);
    dataForVis = _dataForVis;
}




void ScaleTagViewPort::setCurrentModel(frames::Model* _model){model.reset(_model);
    win->setTitle(model->getName());
    name=model->getName();
    ren->GetRenderWindow()->Render();}

void ScaleTagViewPort::onWindowDeleted(QObject* _window){
    // model->onViewPortDeleted(numViewport);
    emit viewPortDeleted(numViewport);}

//void ScaleTagViewPort::deleteViewPort(int viewportNum){qDebug()<< "viewport "<<viewportNum<<" deleted";}


void ScaleTagViewPort::view(){
    std::cout<<"Viewing in a specific viewport  ..."<<std::endl;
    //meshMapper->SetInputConnection(dataForVis->getAA()->GetOutputPort());
    meshMapper->SetInputConnection(dataForVis->getWarpVector()->GetOutputPort());
    qquickvtkItem->update();
    qquickvtkItem->renderer()->GetRenderWindow()->Render();
    if(!dataForVis->getAA()->GetOutput() || !dataForVis->getWarpVector()->GetOutput()) {std::cout<<"No data object at output."<<std::endl; return;}
}
}}
