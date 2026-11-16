 /*=========================================================================
    2
    3   Program:   SCALETAG
    4   Module:    visualization.h
    5
    6   Copyright (c) Dela Ahose
    7   All rights reserved.
    8   See Copyright.txt or http://www.scaletag.com/Copyright.htm for details.
    9
   10      This software is distributed WITHOUT ANY WARRANTY; without even
   11      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   12      PURPOSE.  See the above copyright notice for more information.
   13
   14 =========================================================================*/

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include<QScopedPointer>
#include <QObject>
#include<stlib_global.h>
#include <frames/iobjectFactory.h>
#include <visualization/visualization.h>
//#include<frames/mainwindow.h>
#include<QQuickWidget>
#include<vtkOrientationMarkerWidget.h>
#include<frames/project.h>
#include<QStringList>
#include<frames/workdirectory.h>
#include<window/mainwindow.h>
#include<QWindow>
#include <vtkDoubleArray.h>
#include <vtkSmartPointer.h>
#include<controllers/mainwindowcontroller.h>
#include<QDebug>

#include<frames/heavytask.h>


 //#include<scaleTagTreeView.h>
#include<QTreeView>
#include<QStandardItem>
#include<QStandardItemModel>
#include <QVBoxLayout>

 #include<window/mainTreeView.h>

#include<masterpbc.h>

#include<cassert>


namespace scaletag {
namespace frames {

class STLIB_EXPORT ObjectFactory :public IObjectFactory
{
    Q_OBJECT
    Q_PROPERTY(scaletag::visualization::Visualization* ui_Visualization READ getVisualization CONSTANT)
    Q_PROPERTY(QStringList ui_ModelsNames READ getModelsNamesList /*CONSTANT*/ NOTIFY modelsNamesListChanged)
   //Q_PROPERTY(QStringList ui_ModelsNames READ getModelsNamesList /*CONSTANT*/  /*NOTIFY modelsNamesListChanged(project->getModelsNamesList())*/)
    //Q_PROPERTY(QStringList /*ui_modelsNames*/ui_modelsNamesList READ getModelsNamesList /*WRITE updateModelsNamesList*/  NOTIFY modelsNamesListChanged)
    Q_PROPERTY(scaletag::frames::WorkDirectory* ui_workDirectory READ getWorkDirectory CONSTANT)
    Q_PROPERTY(scaletag::controllers::MainWindowController* ui_mainWindowController READ getMainWindowController CONSTANT)
    Q_PROPERTY(scaletag::frames::Project* ui_project READ getProject CONSTANT)
public:
    explicit ObjectFactory(/*window::MainWindow* _widgetMainWindow=nullptr,*/
                           /*QWindow* _qmlMainWindow=nullptr,*/ QQmlApplicationEngine* engine=nullptr);
    // Q_INVOKABLE scaletag::visualization::Visualization* initializeView();
    Project* getProject() const;
    Q_INVOKABLE void initializeView();

    Q_INVOKABLE scaletag::visualization::Visualization* getVisualization();
    //MainWindow* createMainWindow();

   // Q_INVOKABLE QStringList getModelsNamesList() const;

    Q_INVOKABLE QStringList getModelsNamesList() const;
  // void updateModelsNamesList(/*const QStringList& stringListView = QStringList()*//*project->getModelsNamesList()*/){
       //  project->setModelsNamesList(project->getModelsNamesList());
       // emit modelsNamesListChanged();
       //project->updateModelsNamesList();
   //}//To be modified

    Q_INVOKABLE QString nameAt(qsizetype i) const;

    WorkDirectory* getWorkDirectory()const;

    //    void setQmlMainWindow(QWindow*_window){mainWindowController->setQmlMainWindow(_window);
    //       visualization->setParent((QObject*)_window);}

    Q_INVOKABLE scaletag::controllers::MainWindowController* getMainWindowController();



//    void setupMainWindowTreeView(){
//        QStandardItemModel* treeModel = getMainWindowController()->getWidgetWindow()->getTreeModel();
//        QTreeView* treeView = getMainWindowController()->getWidgetWindow()->getTreeView();
//        treeView->setMinimumWidth(30);
//        //treeView->setMinimumHeight(100);
//        treeModel->setData(QModelIndex(), "Project", Qt::DisplayRole);
//        treeView->setModel(treeModel);
//        //QModelIndex rootIndex = treeModel->createIndex(0,0,project.data());
//        treeView->setRootIndex( treeModel->index(0,0, QModelIndex()) );
//        QDockWidget *dock = new QDockWidget(("Project Structure"), getMainWindowController()->getWidgetWindow(), Qt::Widget);
//        dock->setAllowedAreas(Qt::LeftDockWidgetArea /*| Qt::RightDockWidgetArea*/);
//        //QVBoxLayout* layout=new QVBoxLayout;
//        //dock->setLayout(layout);

//        //treeView->setLayout(layout);
//       // dock->setFixedWidth(4);

//        treeView->show();
//        dock->setWidget(treeView);
//        getMainWindowController()->getWidgetWindow()->addDockWidget(Qt::LeftDockWidgetArea,dock);

//        //dock->setFloating(false);
//         dock->setFeatures(QDockWidget::NoDockWidgetFeatures /*|QDockWidget::DockWidgetVerticalTitleBar*/);
//        //dock->setWindowTitle("Project\nStructure");
//         dock->setVisible(true);
//         //treeView->resize(50, {});
//getMainWindowController()->getWidgetWindow()->resizeDocks({dock}, {125}, Qt::Horizontal);
//    }




private:
    //QScopedPointer<scaletag::visualization::GenerateDataForVis> dataForvis;
    QSharedPointer<scaletag::visualization::Visualization> visualization;
    QScopedPointer<scaletag::frames::Project> project;
    QScopedPointer<scaletag::frames::WorkDirectory> workdir;
    //    QScopedPointer<QWindow>qmlMainWindow;
    //    QScopedPointer<QMainWindow> widgetMainWindow;
    QScopedPointer<controllers::MainWindowController> mainWindowController;
    QScopedPointer<QQmlApplicationEngine> engine;

    QScopedPointer<ScaleTagTreeView> scaleTagTreeView;

 //   QScopedPointer<scaletag::models::MasterPBC> masterPBC;




signals:
    void qmlWelcomeViewPassed();
    void modelsNamesListChanged();
    //void modelsNamesListChanged(/*QStringList modelsNamesList*/);

public slots:
    Q_INVOKABLE void showMainWindow();
    //    visualization::ScaleTagViewPort* createViewPort();
    //    int deleteViewPort(int); QScopedPointer<QList<Model*> > models;


    void renameModel(const QString& fromName, const QString& toName);
    void renameMaterial(const QString& fromName, const QString& toName);
    void modifyMaterial();
    void assignMaterial();

    void createActionsForRenamingInWindow();
    void createActionsForMaterialDeletingInWindow();
    void createActionsForMaterialRenamingInWindow();
    void createActionsForMaterialAssigning();

    void updateModelView(frames::Model* _model);

    void updateModelCADView(frames::Part* _part);

    void updateModelCADWithShapeView(frames::Part* _part);


    void showCurrentModel(visualization::ScaleTagViewPort*);
    void viewResult(/*frames::Model* _model*//*=this->getProject()->getCurrentModel()*/);
    void showMeshEdges();


    visualization::ScaleTagViewPort* createViewport();
    void showCurrentModelInCurrentViewPort(Model* _model);

    ScaleTagTreeView* getTreeView();

    void updateItemInTreeView(const QString& category, const QString& itemName);


    void updateTreeView();

    void StartSelection();

    void EndSelection();


    void mapPBCSurfaces();

    void verifyPBCSurfaces();

    void generatePBCMesh();
};



} // namespace frames
} // namespace scaletag

#endif // SCALETAG_FRAMES_OBJECTFACTORY_H
