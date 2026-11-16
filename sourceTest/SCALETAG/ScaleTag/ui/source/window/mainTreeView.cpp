//#ifndef SCALETAGTREEVIEW_H
//#define SCALETAGTREEVIEW_H

//#include<QAbstractItemModel>
//#include<QTreeView>
//#include<QDockWidget>
//#include<window/mainwindow.h>
//#include<controllers/mainwindowcontroller.h>


//class Project ;
//class TreeItem{
//};



//class scaleTagTreeModel : public QAbstractItemModel{

//public:
//scaleTagTreeModel(Project* _project) : project(_project){}


//private:
//Project* project;
//};




//class scaleTagTreeView : public QTreeView{

//private:
//	//scaleTagTreeModel* treeModel;
//	//MainWindow* mainWindow;



//public:
//  void setupMainWindowTreeView(){
//        //**QStandardItemModel* treeModel = getMainWindowController()->getWidgetWindow()->getTreeModel();
//        //**QTreeView* treeView = getMainWindowController()->getWidgetWindow()->getTreeView();
//        this->setMinimumWidth(30);
//        //treeView->setMinimumHeight(100);
//        treeModel->setData(QModelIndex(), "Project", Qt::DisplayRole);
//        this->setModel(treeModel);
//        //QModelIndex rootIndex = treeModel->createIndex(0,0,project.data());
//        this->setRootIndex( treeModel->index(0,0, QModelIndex()) );
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

//};

//#endif

#include<mainTreeView.h>

QStringList ScaleTagTreeView::capturedExpandedState() {
    QStringList expanded;
    std::function<void(const QModelIndex&, const QString&)> walk =
        [&](const QModelIndex& parent, const QString& parentPath) {
            int rows = treeModel->rowCount(parent);
            for (int i = 0; i < rows; ++i) {
                QModelIndex idx = treeModel->index(i, 0, parent);
                const QString path = parentPath + "/" + idx.data(Qt::DisplayRole).toString();
                if (this->isExpanded(idx))
                    expanded << path;
                walk(idx, path);
            }
        };
    walk(QModelIndex(), QString());
    return expanded;
}

void ScaleTagTreeView::restoreExpandedState(const QStringList& expandedPaths) {
    std::function<void(const QModelIndex&, const QString&)> walk =
        [&](const QModelIndex& parent, const QString& parentPath) {
            int rows = treeModel->rowCount(parent);
            for (int i = 0; i < rows; ++i) {
                QModelIndex idx = treeModel->index(i, 0, parent);
                const QString path = parentPath + "/" + idx.data(Qt::DisplayRole).toString();
                if (expandedPaths.contains(path))
                    this->setExpanded(idx, true);
                walk(idx, path);
            }
        };
    walk(QModelIndex(), QString());
}
