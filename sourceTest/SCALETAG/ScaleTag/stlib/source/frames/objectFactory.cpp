#include <frames/objectFactory.h>

#include <QMdiSubWindow>

#include<QTimer>

namespace scaletag {
namespace frames {

ObjectFactory::ObjectFactory(/*window::MainWindow* _widgetMainWindow,*/ /*QWindow* _qmlMainWindow,*/ QQmlApplicationEngine* _engine):scaletag::frames::IObjectFactory(_engine/*_qmlMainWindow*/)
{
    engine.reset(_engine);
    auto _qmlMainWindow=reinterpret_cast<QWindow*>(engine->rootObjects()[0]);
    visualization.reset(new visualization::Visualization((QObject*)_qmlMainWindow, engine.data()));
    //visualization->initialize();
    //createView();
    project.reset(new Project(this));
    workdir.reset(new WorkDirectory(this));
    //widgetMainWindow.reset(_widgetMainWindow);
   // qmlMainWindow.reset(_qmlMainWindow);
    mainWindowController.reset(new controllers::MainWindowController(_qmlMainWindow));


    //scaleTageTreeView.reset(new ScaleTagTreeView());
    //scaleTageTreeView->setupMainWindowTreeView();

   //setupMainWindowTreeView();

    QObject::connect(this, &ObjectFactory::qmlWelcomeViewPassed, this, &ObjectFactory::showMainWindow);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::startSelectionFired,
                     this, &frames::ObjectFactory::StartSelection);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::endSelectionFired,
                     this, &frames::ObjectFactory::EndSelection);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::importCADFired,
                     project.data(), &frames::Project::projectCADModify);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::importCADWithShapeFired,
                     project.data(), &frames::Project::/*projectCADWithShapeModifyAsHeavyTask*/projectCADWithShapeModify);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::createViewPortFired,
                     this, &frames::ObjectFactory::createViewport);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::createNoNameModelFired,
                    project.data(), &frames::Project::createNoNameModel);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::deleteModelFired,
                     project.data(), &frames::Project::deleteCurrentModel);

  //  QObject::connect( project.data(), &frames::Project::modelNameChanged, &frames::Project::update);  // delete later
    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::renameModelFired,
                   this, &frames::ObjectFactory::createActionsForRenamingInWindow);



    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::editingEndedWith,
                   this, &frames::ObjectFactory::renameModel);
    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::editingMaterialNameEndedWith,
                     this, &frames::ObjectFactory::renameMaterial);

    QObject::connect(project.data(), &Project::modelListChanged, this, &ObjectFactory::updateTreeView);
    QObject::connect(project->getCurrentModel(), &Model::needTreeViewUpdate, this, &ObjectFactory::updateTreeView);



    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::runPBCAnalysisFired,
                     project.data()/*->getCurrentModel()*/, /*&Model::*/&Project::runPBCAnalysisLauncher);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::viewResultActionFired,
                     project->getCurrentModel(), &Model::viewResults); //askViewResults is emitted here

//    QObject::connect(project->getCurrentModel(), &Model::askViewResults, this, &ObjectFactory::viewResult);


    QObject::connect(project->getCurrentModel(), &Model::askViewResults, [this](){
        std::cout<<"To open view pannel"<<std::endl;

        auto currentModel = getProject()->getCurrentModel();

        auto dataForVis = currentModel->getDataForVis();
        if(!dataForVis) return;

        //this->visualization->setDataForVis(dataForVis);
        dataForVis->setFields(getProject()->getCurrentModel()->getFields());

        auto* viewPannel = new window::ViewPannel(currentModel/*this->mainWindowController->getWidgetWindow()*/);
        QObject::connect(viewPannel, &window::ViewPannel::viewResultsWith, this->getVisualization(), &visualization::Visualization::viewFieldResults, Qt::QueuedConnection);

        //viewPannel->setModel(currentModel);
    } );

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::showMeshEdgesFired,
                     this, &ObjectFactory::showMeshEdges);








    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::createMaterialFired,
                 project->getCurrentModel(), &Model::addMaterial);

    QObject::connect(mainWindowController->getWidgetWindow(), &window::MainWindow::modifyMaterialFired, this,  &ObjectFactory::modifyMaterial);
    QObject::connect(mainWindowController->getWidgetWindow(), &window::MainWindow::updateMaterialFired, project->getCurrentModel(),  &Model::updateMaterial);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::deleteMaterialFired,
                         this, &frames::ObjectFactory::createActionsForMaterialDeletingInWindow);
    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::renameMaterialFired,
                 this, &frames::ObjectFactory::createActionsForMaterialRenamingInWindow);

    QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::assignMaterialFired,
                     this, &frames::ObjectFactory::createActionsForMaterialAssigning);

QObject::connect(mainWindowController->getWidgetWindow(), &window::MainWindow::mapSurfaceFired, this, &ObjectFactory::mapPBCSurfaces);
QObject::connect(mainWindowController->getWidgetWindow(), &window::MainWindow::verifyPBCFired, this, &ObjectFactory::verifyPBCSurfaces);
QObject::connect(mainWindowController->getWidgetWindow(), &window::MainWindow::meshActionFired, this, &ObjectFactory::generatePBCMesh);


QObject::connect(mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::importModelFired,
project.data(), &frames::Project::projectModify);

//QObject::connect(project.data(), &Project::currentModelChanged, this, /*this, &ObjectFactory::updateModelView*/[this](frames::Model* model){
//        heavyTaskLauncher(this, &ObjectFactory::updateModelView, model) ;}, Qt::QueuedConnection);
QObject::connect(project.data(), &Project::currentModelChanged, this, &ObjectFactory::updateModelView);
//QObject::connect(project->getCurrentModel(), &Model::modelModified, this, &ObjectFactory::updateModelView, Qt::QueuedConnection); //doubting
QObject::connect(project.data(), &Project::currentModelModified, this, &ObjectFactory::updateModelView, Qt::QueuedConnection);
QObject:connect(getVisualization(), &visualization::Visualization::viewPortCreated, this, &ObjectFactory::showCurrentModel);
//QObject::connect(project.data(), &Project::currentModelChanged, this, &ObjectFactory::showCurrentModelInCurrentViewPort);




//QObject::connect(mainWindowController->getWidgetWindow()->get)
{
//visualization->createViewport(mainWindowController->getWidgetWindow());
//visualization->getViewPorts().at(0)->getWindow()->setTitle(" A new one");
//QWidget* viewPortWiget=QWidget::createWindowContainer((visualization->getViewPorts().at(0)->getWindow()));

//QDockWidget* dock=new QDockWidget("Hello",mainWindowController->getWidgetWindow(), Qt::SubWindow);


//dock->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
//dock->setAllowedAreas(Qt::TopDockWidgetArea);

//dock->setWidget(viewPortWiget);
////mainWindowController->getWidgetWindow()->addDockWidget(Qt::TopDockWidgetArea, dock);
//viewPortWiget->setWindowFlags(Qt::SubWindow|Qt::WindowCloseButtonHint|Qt::WindowMaximizeButtonHint|Qt::WindowMinimizeButtonHint);

////mainWindowController->getWidgetWindow()->setCentralWidget(viewPortWiget);
////mainWindowController->getWidgetWindow()->setCentralWidget(dock);

//viewPortWiget->setParent(mainWindowController->getWidgetWindow()->centralWidget());
//dock->setVisible(true);
//dock->setFloating(true);
}





    //mainWindowController->getWidgetWindow()->centralWidget()->mainWindowController->getWidgetWindow()->getMdiArea());

  // visualization->getViewPorts().at(0)->getWindow()->hide();
    //viewPortWiget->setParent(mainWindowController->getQmlMainWindowContainer());

//    QMdiSubWindow* sub1=new QMdiSubWindow;
//    sub1->setWidget(viewPortWiget);
//    sub1->setAttribute(Qt::WA_DeleteOnClose);
    //mainWindowController->getWidgetWindow()->setCentralWidget(viewPortWiget);
//mainWindowController->getWidgetWindow()->getMdiArea()->setParent(nullptr);
//   mainWindowController->getWidgetWindow()->getMdiArea()->addSubWindow(sub1);
//    viewPortWiget->mapToParent(QPoint(0,0));

//createViewport();

}




//QStringList* ObjectFactory::getModelsNamesList() const
//{

//    return project->getModelsNamesList();
//}

WorkDirectory* ObjectFactory::getWorkDirectory()const{

    return workdir.data();
}

/*MainWindow* ObjectFactory::createMainWindow(){
    return new MainWindow;
}*/


//scaletag::visualization::Visualization*
Project* ObjectFactory::getProject() const{return project.data();}

void ObjectFactory::initializeView(){
    visualization->initialize();

   /* return visualization;
    MainWindow* mainWin=createMainWindow();

    QQuickWidget* widget;
    widget->setParent(mainWin);
    widget->setSource(QUrl("/home/ahose/Desktop/SCALETAG/ScaleTag/ui/views/vtk.qml"));

    QQuickVTKRenderItem *qquickvtkItem = widget->findChild<QQuickVTKRenderItem*>("visual");
    vtkSmartPointer<vtkAxesActor> axes = vtkAxesActor::New();

    vtkOrientationMarkerWidget* orientationMarker=vtkOrientationMarkerWidget::New();

    vtkNew<vtkNamedColors> colors;
    double rgba[4]={0.0, 0.0, 0.0, 0.0};
    colors->GetColor("Carrot", rgba);
    qquickvtkItem->renderer()->SetBackground(colors->GetColor3d("MidnightBlue").GetData());
    orientationMarker->SetOutlineColor(rgba[0], rgba[1], rgba[2]);
    orientationMarker->SetOrientationMarker(axes);
    orientationMarker->SetInteractor(qquickvtkItem->renderWindow()->renderWindow()->GetInteractor());
    orientationMarker->SetViewport(0.0, 0.0, 0.2, 0.2);
    orientationMarker->SetEnabled(1);
    orientationMarker->InteractiveOn();

    qquickvtkItem->update();
    qquickvtkItem->renderer()->ResetCamera();
    qquickvtkItem->renderWindow()->renderWindow()->Render();
    return nullptr;*/

}


scaletag::visualization::Visualization* ObjectFactory::getVisualization(){return visualization.data();}


QStringList ObjectFactory::getModelsNamesList() const{
    return project->getModelsNamesList();
    //emit modelsNamesListChanged(project->getModelsNamesList());
}


QString ObjectFactory::nameAt(qsizetype i) const{
    return project->getModelsNamesList()[i];

}

controllers::MainWindowController* ObjectFactory::getMainWindowController(){
    return mainWindowController.data();
}

void ObjectFactory::showMainWindow(){
    //mainWindowController->hideQmlMainWindow();

    qDebug()<<"qml button fired";
                     visualization->initialize(); // set the frame for visualization
    //initializeView();

    mainWindowController->createMainWindow();
    //mainWindowController->show();

    //setupMainWindowTreeView();
    scaleTagTreeView.reset(new ScaleTagTreeView(project.data()));
    scaleTagTreeView->setupMainWindowTreeView(getMainWindowController()->getWidgetWindow());



    QObject::connect(scaleTagTreeView.data(), &ScaleTagTreeView::itemClicked, this, &ObjectFactory::updateItemInTreeView);

}

void ObjectFactory::renameModel(const QString& fromName, const QString& toName){
    project->renameModel(fromName, toName);
    project->setCurrentModel(toName);
}
void ObjectFactory::renameMaterial(const QString& fromName, const QString& toName){
    project->renameMaterial(fromName, toName);
    updateTreeView();
}


void ObjectFactory::modifyMaterial(){
    std::cout<<"Modifying a material."<<std::endl; // remove this function in MainWindow

    if (!mainWindowController || !mainWindowController->getWidgetWindow()) return;

    window::MainWindow* mainWindow = mainWindowController->getWidgetWindow();
    QMenu* modifyMenu = mainWindow->getModifyMaterialMenu();

    modifyMenu->clear();
    // auto materialsList = project->getCurrentModel()->getMaterialsNamesList();

    //QTimer::singleShot(0, this, [this, modifyMenu]() {
    auto materialsList = project->getCurrentModel()->getMaterialsNamesList();
    if (materialsList.isEmpty()) {
QAction* noMatAction = new QAction("(Aucun matériau)", modifyMenu);
                       noMatAction->setEnabled(false);
modifyMenu->addAction(noMatAction);
return;
    }

    for(const auto& matName : materialsList){
QAction* matAction = new QAction(matName, modifyMenu);
modifyMenu->addAction(matAction);

QObject::connect(matAction, &QAction::triggered, this, [this, matName]() {

    //            if (this->getProject() && this->getProject()->getCurrentModel()) {
    //                this->getProject()->getCurrentModel()->deleteMaterial(matName);
    //            }
    //            this->createActionsForMaterialDeletingInWindow();  //optionnel

    QString purposeTitle = "Modify a material";
    double E = project->getCurrentModel()->getMaterial(matName.toStdString()).at("E");
    double nu= project->getCurrentModel()->getMaterial(matName.toStdString()).at("nu");
    auto* materialWid = mainWindowController->getWidgetWindow()->modifyMaterial(
        purposeTitle, matName, E, nu);
});


    }

}

void ObjectFactory::assignMaterial(){
    //project->getCurrentModel()->exchangeMatPos(pos, oldPos);
}

   // });
//}



void ObjectFactory::createActionsForRenamingInWindow(){
    //sender here is always of type window::MainWindow*
    qobject_cast<window::MainWindow*>(sender())->getRenameModelMenu()->clear();
    QList<QAction*> actions;
    for(auto* mod : project->getModelList()){
    QAction* action= new QAction(mod->getName());
    QObject::connect(action, &QAction::triggered, mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::renameModel);
    actions.append(action);

    }
    qobject_cast<window::MainWindow*>(sender())->getRenameModelMenu()->addActions(actions);
}


void ObjectFactory::createActionsForMaterialDeletingInWindow(){

    if (!mainWindowController || !mainWindowController->getWidgetWindow()) return;

    window::MainWindow* mainWindow = mainWindowController->getWidgetWindow();
    QMenu* deleteMenu = mainWindow->getDeleteMaterialMenu();

    // On nettoie et reconstruit au cycle suivant de Qt
    // Cela évite que Qt ne se mélange les pinceaux si le menu est déjà ouvert ou en cours de fermeture.
    QTimer::singleShot(0, this, [this, deleteMenu]() {
        if (!deleteMenu || !project || !project->getCurrentModel()) return;

        deleteMenu->clear();

        auto materialsList = project->getCurrentModel()->getMaterialsNamesList();

        if (materialsList.isEmpty()) {
        QAction* noMatAction = new QAction("(Aucun matériau)", deleteMenu);
        noMatAction->setEnabled(false);
        deleteMenu->addAction(noMatAction);
        return;
        }

        for(const auto& matName : materialsList){
            QAction* matAction = new QAction(matName, deleteMenu);

            QObject::connect(matAction, &QAction::triggered, this, [this, matName]() {

                 if (this->getProject() && this->getProject()->getCurrentModel()) {
                    this->getProject()->getCurrentModel()->deleteMaterial(matName);
                }


                this->createActionsForMaterialDeletingInWindow();  //optionnel
            });

            deleteMenu->addAction(matAction);
        }
    });
}



void ObjectFactory::createActionsForMaterialRenamingInWindow(){
    mainWindowController->getWidgetWindow()->getRenameMaterialMenu()->clear();
    QList<QAction*> actions;

    QMenu* renameMenu = mainWindowController->getWidgetWindow()->getRenameMaterialMenu();
    if (getProject()->getCurrentModel()->getMaterialsNamesList().isEmpty()) {
    QAction* noMatAction = new QAction("(Aucun matériau)", renameMenu);
    noMatAction->setEnabled(false);
    renameMenu->addAction(noMatAction);
    return;
    }
    for(auto matName : getProject()->getCurrentModel()->getMaterialsNamesList()){
    QAction* matAction = new QAction(matName);
    QObject::connect(matAction, &QAction::triggered, mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::renameMaterial);
    actions.append(matAction);
    }


//    qobject_cast<window::MainWindow*>(sender())->getRenameMaterialMenu()->clear();
//    QList<QAction*> actions;
//    for(auto mats : project->getCurrentModel()->getMaterialsNamesList()){
//    QAction* matAction = new QAction(mats);
//    QObject::connect(matAction, &QAction::triggered, mainWindowController->getWidgetWindow(), &window::MainWindow::renameMaterial);
//    actions.append(matAction);
//    }
    renameMenu->addActions(actions);

}

void ObjectFactory::createActionsForMaterialAssigning(){

    auto* currentModel = getProject()->getCurrentModel();
    mainWindowController->getWidgetWindow()->getAssignMaterialMenu()->clear();
    QList<QAction*> actions;

    QMenu* assignMaterialMenu = mainWindowController->getWidgetWindow()->getAssignMaterialMenu();
    if (currentModel->getMaterialsNamesList().isEmpty()) {
    QAction* noMatAction = new QAction("(Aucun matériau)", assignMaterialMenu);
                           noMatAction->setEnabled(false);
    assignMaterialMenu->addAction(noMatAction);
    return;
    }
    for(auto matName : currentModel->getMaterialsNamesList()){
    QAction* matAction = new QAction(matName);
    //QObject::connect(matAction, &QAction::triggered, mainWindowController->getWidgetWindow(), &scaletag::window::MainWindow::renameMaterial);
    QObject::connect(matAction, &QAction::triggered, project.data(), &frames::Project::assignMaterial);
    actions.append(matAction);
    }

    assignMaterialMenu->addActions(actions);

}



void ObjectFactory::updateModelView(frames::Model* _model){

    std::cout<< "model "<<_model->getName().toStdString()<<" at "<<_model<<" modified "<<std::endl;

    getVisualization()->getRenderer()->RemoveAllViewProps();/*
    getVisualization()->getQmlMapper()->RemoveAllInputConnections(0);
    getVisualization()->getQmlMapper(1)->RemoveAllInputConnections(0);
    getVisualization()->getQmlMapper()->RemoveAllInputs();
    getVisualization()->getQmlMapper(0)->RemoveAllInputs();*/

    if (_model == nullptr
        //        &&( (_model->getCurrentPart()==nullptr) ||_model->getMesh()==nullptr)
        ) return;
    //{

    if(_model->getMesh()!=nullptr)
    {
    //auto dataForVis = new visualization::GenerateDataForVis(/*visualization.data()*/);
    auto dataForVis = _model->getDataForVis();
    dataForVis->initialize();
    if(dataForVis==nullptr) return;
    visualization->setDataForVis(dataForVis);
    dataForVis->setPipeline(visualization::meshMapper);

    dataForVis->setModel(_model);

    dataForVis->setExecuteMethod();
    dataForVis->updateDataObjectToDataSet();

    dataForVis->update();
    _model->setDataForVis(dataForVis);

    dataForVis->getMapper(visualization::meshMapper)->SetInputConnection( dataForVis->/*getAA()*/getWarpVector()->GetOutputPort());
    dataForVis->getMapper(visualization::meshMapper)->SetInputConnection(dataForVis->getWarpVector()->GetOutputPort()) ;
    dataForVis->getActor()->SetMapper(dataForVis->getMapper(visualization::meshMapper));
    getVisualization()->getRenderer()->AddActor( dataForVis->getActor() );


    //    mapper->SetInputConnection(dataForVis->getAA()->GetOutputPort());
    //    actor->SetMapper(mapper);
    //    visualization->getRenderer()->AddActor(actor);



    visualization->getRenderer()->ResetCamera();
    visualization->update();
    std::cout<<"updateModelView() has been executed."<<std::endl;
    }

    if( _model->getCurrentPart()!=nullptr){
    if(_model->getMesh()) return;  //To avoid displaying part when mesh exist. // To be changed
    updateModelCADWithShapeView(_model->getCurrentPart());
    //heavyTaskLauncher(this, &ObjectFactory::updateModelCADWithShapeView, _model->getCurrentPart());
    //visualization->update();


    // Think about putting this part in GenerateDataForVis
    }

}


void ObjectFactory::updateModelCADView(frames::Part* _part){
    /*qDebug()*/cout<<"Part : "<< _part->getName().toStdString()<< " of model : "<<_part->getModel()->getName().toStdString()<<" initialized"<<std::endl;
    //auto dataForVis=new visualization::GenerateDataForVis(_part->getModel());
    // dataForVis->setModel(_part->getModel());


    // implement cad visualization in Visualization

    //visualization->viewCADPart();

    std::cout<<"updateModelCADView called"<<std::endl;
    visualization->renderView(_part);
}

void ObjectFactory::updateModelCADWithShapeView(frames::Part* _part){
    std::cout<<"updateModelCADWithShapeView called"<<std::endl;
    visualization->renderCADWithShapeView(_part);
    //heavyTaskLauncher(visualization.data(), &visualization::Visualization::renderCADWithShapeView,_part);
    //visualization->update();
    //getVisualization()->initSelection(SM_Face);
}


void ObjectFactory::showCurrentModel(visualization::ScaleTagViewPort* viewPort){
    std::cout<<"showing current model."<<std::endl;
    if(!viewPort) return;
    auto* currentModel = getProject()->getCurrentModel();
    if(!currentModel) return;
    if(!currentModel->getMesh()) return;
    viewPort->setDataForVis(currentModel->getDataForVis());
    viewPort->view();
    viewPort->getRenderer()->ResetCamera();

}

void ObjectFactory::showCurrentModelInCurrentViewPort(Model* _model){
    if(!getVisualization()->getCurrentViewPort()) {std::cout<<"Viewport is not initialized."<<std::endl; return;}
    showCurrentModel(getVisualization()->getCurrentViewPort());
}

void ObjectFactory::viewResult(/*frames::Model* _model*//*=this->getProject()->getCurrentModel()*/){

    qDebug()<< "model "<<this->getProject()->getCurrentModel()->getName()<<" at "
             <<getProject()->getCurrentModel()<<" will be viewed";

    std::cout<<"viewResult() from ObjectFactory class called"<<std::endl;
    //updateModelView(getProject()->getCurrentModel());
    //auto dataForVis = visualization->getDataForVis();
    auto dataForVis = getProject()->getCurrentModel()->getDataForVis();
    if(!dataForVis) return;
    //dataForVis->setModel(getProject()->getCurrentModel());
    //dataForVis->update();

    std::cout<<"currentDataForVis : "<< dataForVis<<std::endl
        <<" curr model: "<<getProject()->getCurrentModel()->getName().toStdString()<<std::endl;
    visualization->setDataForVis(dataForVis);
    dataForVis->setFields(getProject()->getCurrentModel()->getFields());
    visualization->viewResults();
    visualization->update();
}

void ObjectFactory::showMeshEdges(){

    if(!project->getCurrentModel()->getMesh()) return;
    visualization->getDataForVis()->setMeshEdgesPipeline();
    if(! visualization->getRenderer()->HasViewProp(   visualization->getDataForVis()->getMeshEdgesActor()  ))
    visualization->getRenderer()->AddActor( visualization->getDataForVis()->getMeshEdgesActor() );
    else
    visualization->getRenderer()->RemoveActor( visualization->getDataForVis()->getMeshEdgesActor() );


//    bool vis = visualization->getDataForVis()->getMeshEdgesActor()->GetVisibility();
//    if(vis==true) visualization->getRenderer()->RemoveActor( visualization->getDataForVis()->getMeshEdgesActor() );
//    if(vis==false) visualization->getRenderer()->AddActor( visualization->getDataForVis()->getMeshEdgesActor() );
    visualization->update();
}

visualization::ScaleTagViewPort* ObjectFactory::createViewport(){
    auto* scaleTagViewPort=visualization->createViewport(mainWindowController->getWidgetWindow());
    scaleTagViewPort->getWindow()->setTitle(" A new one");
    scaleTagViewPort->getWindow()->hide();
    auto* scaleTagViewPortWidget = QWidget::createWindowContainer(scaleTagViewPort->getWindow(), mainWindowController->getWidgetWindow()/*->centralWidget()*/,/*Qt::Window*/Qt::Widget );

    //scaleTagViewPortWidget->setWindowFlags(Qt::FramelessWindowHint);
    //scaleTagViewPortWidget->setMinimumHeight(1);
    scaleTagViewPortWidget->setMinimumSize(500,500);
    //        auto* subWindow = mainWindowController->getWidgetWindow()->getMdiArea()->addSubWindow(scaleTagViewPortWidget,Qt::CustomizeWindowHint | Qt::Tool | Qt::WindowTitleHint);
    //subWindow->setWindowFlags(subWindow->windowFlags()|Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::SubWindow);
    auto* subWindow = mainWindowController->getWidgetWindow()->getMdiArea()->addSubWindow(scaleTagViewPortWidget,Qt::CustomizeWindowHint | Qt::SubWindow);
    subWindow->show();
    //subWindow->showMaximized();
    //QObject::connect(mainWindowController->getWidgetWindow()->getMdiArea(), &QMdiArea::subWindowActivated, )

    QObject::connect(subWindow, &QMdiSubWindow::aboutToActivate, getVisualization(), [this, scaleTagViewPort](){
        QMetaObject::invokeMethod(this->getVisualization(), "currentViewPortChanged", Qt::QueuedConnection, scaleTagViewPort);}
                     );
    return scaleTagViewPort;
}

ScaleTagTreeView* ObjectFactory::getTreeView(){return scaleTagTreeView.data();}

void ObjectFactory::updateItemInTreeView(const QString& category, const QString& itemName){
    if(category.toStdString()=="Materials" || itemName.toStdString()=="Materials") std::cout<<"Going to modify materials"<<std::endl;
    if(itemName.toStdString()=="Mesh") std::cout<<"Mesh manager to be created"<<std::endl;

}


void ObjectFactory::updateTreeView(){
    scaleTagTreeView->refreshModels();
    std::cout<<"TreeView has been modified "<<std::endl;
}


void ObjectFactory::StartSelection(){getVisualization()->initSelection(SM_Face);}

void ObjectFactory::EndSelection(){getVisualization()->endSelection();}





void  ObjectFactory::mapPBCSurfaces(){
    //std::cout<<"Engaged in faces mapping "<<std::endl;
    getVisualization()->initSelection(SM_Face);
    auto* masterPBC = getProject()->getCurrentModel()->getMasterPBC();
    getProject()->getCurrentModel()->getMasterPBC()->initialize((ObjectFactory*)this);
    //masterPBC.reset( new models::MasterPBC((ObjectFactory*)this)  );
    auto* pbcWidget = new models::PBCConstraintsManagerWidget(mainWindowController->getWidgetWindow());


    QObject::connect(pbcWidget, &models::PBCConstraintsManagerWidget::closeWidget, this->getVisualization(), &visualization::Visualization::endSelection);
    QObject::connect(pbcWidget, &models::PBCConstraintsManagerWidget::aSurfaceIsSelected,
                     masterPBC/*.data()*/, &models::MasterPBC::setSurfaceId);

    QObject::connect(pbcWidget, &models::PBCConstraintsManagerWidget::clearAllFired,
                     masterPBC/*.data()*/, &models::MasterPBC::clearAllMappedFaces);

    //        std::cout<<"Select the first face "<<std::endl;
    //        std::cout<<"Select the second face "<<std::endl;

    //        std::string surfLabel1 = "Select the first face:";
    //        std::string surfLabel2 = "Select the second face:";

    //        auto* part = getProject()->getCurrentModel()->getCurrentPart();

    //        /*
    //         * In the file containing MasterPBC, create a custom class which
    //         * manages ids acquision
    //         * The class has a signal which is emitted when "validate" is clicked;
    //         *
    //         *
    //         * /

    std::cout<<"Select the first face."<<std::endl;

}



void ObjectFactory::verifyPBCSurfaces(){
    std::cout<<"Verifying constraints on PBC faces."<<std::endl;

    /******* Retrieving a pair of TopoDS_Shape corresponding to a pair of tags ***********/
    auto* masterPBC = getProject()->getCurrentModel()->getMasterPBC();
    if(masterPBC/*.data()*/ == nullptr) return ;

    /*std::unordered_map<IVtk_IdType, IVtk_IdType>*/ std::vector<std::pair<IVtk_IdType, IVtk_IdType>> mappedFaces = masterPBC->getMappedFaces();

    auto* dataSource = getProject()->getCurrentModel()->getCurrentPart()->getVISSource();

    auto visShape =  dataSource->GetShape();
    //IVtk_IdType pId = visShape->GetId();

    bool areMapped = true;
    for(const auto& p : mappedFaces){
    TopoDS_Shape shape1 = visShape->GetSubShape(p.first);
    TopoDS_Shape shape2 = visShape->GetSubShape(p.second);
    GProp_GProps props1, props2;
    BRepGProp::SurfaceProperties(shape1, props1);
    BRepGProp::SurfaceProperties(shape2, props2);
    gp_Pnt G1 = props1.CentreOfMass();
    gp_Pnt G2 = props2.CentreOfMass();
    Standard_Real x1, y1, z1,  x2, y2, z2;
    x1 = G1.X();    x2 = G2.X();
    y1 = G1.Y();    y2 = G2.Y();
    z1 = G1.Z();    z2 = G2.Z();
    std::vector<double> aTr{x2-x1, y2-y1, z2-z1};

    gp_Trsf trsf;
    trsf.SetTranslation( gp_Vec(aTr[0], aTr[1], aTr[2]) );
    BRepBuilderAPI_Transform transformer(shape1, trsf, /*Copy=*/false);
    TopoDS_Shape movedShape = transformer.Shape();

    areMapped = areMapped && fem::areShapesCoincident(movedShape, shape2);

    if(areMapped) std::cout<<"Faces are correctly mapped."<<std::endl;
    if(!areMapped) std::cout<<"Faces are not correctly mapped."<<std::endl;
    }

}
void ObjectFactory::generatePBCMesh(){
    auto* currentModel = getProject()->getCurrentModel();

    auto* dataSource = currentModel->getCurrentPart()->getVISSource();

    if(dataSource){
    auto* pbcMeshTool = new fem::PBCMeshTool(currentModel->getCurrentPart()->getVISSource());

    auto* masterPBC = getProject()->getCurrentModel()->getMasterPBC();
    if (!masterPBC) {
        std::cerr << "Erreur : masterPBC est nul !" << std::endl;
        return;
    }

   // std::unordered_map<IVtk_IdType, IVtk_IdType> mfaces = masterPBC->getMappedFaces();
    std::vector<std::pair<IVtk_IdType, IVtk_IdType>> mfaces = masterPBC->getMappedFaces();
    pbcMeshTool->setFacePairs(mfaces);
    //pbcMeshTool->SetMeshSize(0.01);

    pbcMeshTool->mesh(1./10.);  //1./40. 1./10. 1./50. 1./15. 1./20. 1./25. (converge pour 1./25.)
    pbcMeshTool->buildMappingDirs();
    std::cout<<"Meshing geometry ..."<<std::endl;

    std::vector<std::unordered_map<int, int>> nodesConstraints;
    pbcMeshTool->buildConstraints(nodesConstraints);  // meshData holds also the values
    pbcMeshTool->Finalize();
    fem::MeshData meshData = pbcMeshTool->getMeshData();
    MeshPbc<double,3>* mesh = new MeshPbc<double,3>();
    mesh->gmshInitialize(meshData);
    //mesh->setM
    currentModel->setMesh(mesh);
    //currentModel->getMesh()->gmshInitialize(meshData);
    QMetaObject::invokeMethod(getProject(), "currentModelChanged", Qt::QueuedConnection,  currentModel);
    //currentModelChanged
    //currentModelModified
    //model


    }

}


} // namespace frames
} // namespace scaletag
