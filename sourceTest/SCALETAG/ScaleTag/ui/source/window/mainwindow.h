#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QToolBar>
#include<QMenuBar>
#include<QAction>
#include<frames/workdirectory.h>
#include<QStatusBar>

//#include<scaleTagTreeView.h>
#include<QTreeView>
#include<QStandardItemModel>
#include<QStandardItem>

#include<QMdiArea>
#include<QMdiSubWindow>
#include <QQuickItem>
//#include<QComboBox>
#include<iostream>
#include<QFileDialog>
#include<QTextEdit>
#include<sstream>
#include<QString>
#include <QGraphicsAnchorLayout>
#include <QHBoxLayout>
#include<QLabel>
#include<QSizePolicy>
#include <QSplitter>
#include <QList>
#include <QDockWidget>
#include<QTabWidget>

#include<iostream>
#include<cstdio>
#include<QTextStream>
#include<window/outputhandle.h>
#include<window/scaletagdialogs.h>
//#include<physics.h>

namespace scaletag {
namespace window {

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    QMdiArea* getMdiArea(){return mdiArea.data();}

    QMenu* getRenameModelMenu(){return renameModelMenu;}
    QMenu* getRenameMaterialMenu(){return renameMaterialMenu;}
    QMenu* getDeleteMaterialMenu(){return deleteMaterialMenu;}
    QMenu* getModifyMaterialMenu(){return modifyMaterialMenu;}
    QMenu* getAssignMaterialMenu(){return assignMaterialMenu;}


    void init();


    //QTreeView* getTreeView(){return treeView.data();}
    //QStandardItemModel* getTreeModel(){return treeModel.data();}

private:

    void createActions();

    QScopedPointer<QMdiArea> mdiArea;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu* editMenu;
    QMenu* modelMenu;
    QMenu* renameModelMenu;
    QMenu *selectionMenu;

    QMenu* pbcMeshMenu;

    QMenu* materialMenu;
    QMenu* createMaterialMenu;
    QMenu* modifyMaterialMenu;
    QMenu* renameMaterialMenu;
    QMenu* deleteMaterialMenu;
    QMenu* assignMaterialMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *modelToolBar;

    QStatusBar* mainStatusBar;

    QTextEdit* scaleTagInfo;
    QTextEdit* scaleTagConsole;



    QSharedPointer<frames::WorkDirectory> workdir;

    QQuickItem* rightButtons;


   // QScopedPointer<QTreeView> treeView;
    //QScopedPointer<QStandardItemModel> treeModel;



signals:
    void importModelFired(const QString& dir);
    void importCADFired(const QString& dir);
    void importCADWithShapeFired(const QString& dir);
    void runPBCAnalysisFired();
    void viewResultActionFired();
    void showMeshEdgesFired();
    void createViewPortFired();
    void createModelFired(const QString& _name="" );
    void createNoNameModelFired( );
    void deleteModelFired(/*Model* _model=nullptr*/);
    void renameModelFired(/*const QString& _newName="newName"*/);
    void editingEndedWith(const QString& from, const QString& toName);
    void editingMaterialNameEndedWith(const QString& from, const QString& toName);

    void mapSurfaceFired();
    void verifyPBCFired();
    void meshActionFired();

    void startSelectionFired();
    void endSelectionFired();

    void createMaterialFired(/*const QString& materialName, double E, double nu*/Material _mat);
    void modifyMaterialFired();
    void updateMaterialFired(Material _mat, QString _materialToModfyName);
    void deleteMaterialFired(/*const QString& materialName*/);
    void renameMaterialFired(/*const QString& materialName*/);
    void assignMaterialFired();



public slots:

    void renameModel(){
        /*The old name is provided through signal sender*/
        QString fromName=qobject_cast<QAction*>(sender())->text();
        RenameModelWidget* renameWid = new RenameModelWidget( fromName);
        renameWid->setOldName( (qobject_cast<QAction*>(sender()))->text());
        QObject::connect(renameWid, &RenameModelWidget::editingEndedWith, this,
                         &MainWindow::editingEndedWith);
        std::cout<<"renameModel() to be implemented in MainWindow"<<std::endl;
        //renameWid->deleteLater();
    }

    void renameMaterial(){
        /*The old name is provided through signal sender*/
        QString fromName=qobject_cast<QAction*>(sender())->text();
        RenameMaterialWidget* renameWid = new RenameMaterialWidget( fromName);
        renameWid->setOldName( (qobject_cast<QAction*>(sender()))->text());
        QObject::connect(renameWid, &RenameMaterialWidget::editingEndedWith, this,
                         &MainWindow::editingMaterialNameEndedWith);
        std::cout<<"renameModel() to be implemented in MainWindow"<<std::endl;

        //renameWid->deleteLater();
    }

    void createMaterial(){
        window::LinearElasticMaterialWidget* materialWid = new window::LinearElasticMaterialWidget(""/*,this*/);
        QObject::connect(materialWid, &LinearElasticMaterialWidget::editingEndedWith, this, &window::MainWindow::createMaterialFired);
        //materialWid->setOldName( (qobject_cast<QAction*>(sender()))->text());
//        QObject::connect(materialWid, &RenameModelWidget::LinearElasticMaterialWidget, this,
//                         &MainWindow::editingEndedWith);
//       std::cout<<"A new material is created"<<std::endl;

    }

    window::LinearElasticMaterialWidget* modifyMaterial(const QString& purposeTitle, const QString& matName, double E, double nu){
        window::LinearElasticMaterialWidget* materialWid = new window::LinearElasticMaterialWidget(purposeTitle, matName, E, nu);
        //materialWid->setPurposeTitle("Modify a material"); //already done during class creation
        QObject::connect(materialWid, &LinearElasticMaterialWidget::editingMaterialEndedWith, this, &window::MainWindow::updateMaterialFired);

        return materialWid;
    }


};

} // namespace frames
} // namespace scaletag

#endif // MAINWINDOW_H
