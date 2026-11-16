#ifndef SCALETAGTREEVIEW_H
#define SCALETAGTREEVIEW_H

#include<QStandardItemModel>

#include<QScopedPointer>
#include<QAbstractItemModel>
#include<QTreeView>
#include<QDockWidget>
#include<window/mainwindow.h>
#include<controllers/mainwindowcontroller.h>
#include<frames/project.h>

#include<QStringList>
#include<QFont>
#include<QVariant>
//class Project ;


class ScaletagTreeItem {
public:
    explicit ScaletagTreeItem(const QString& _name, const QString& _decoration = QString(), ScaletagTreeItem* _parentItem = nullptr)
        : name(_name), decoration(_decoration), parentItem(_parentItem) {}

    ~ScaletagTreeItem() { qDeleteAll(childItems); }

    void appendChild(ScaletagTreeItem* child) { childItems.append(child); }
    ScaletagTreeItem* child(int row) const { return childItems.value(row); }
    int childCount() const { return childItems.count(); }
    int row() const {
        if (parentItem)
            return parentItem->childItems.indexOf(const_cast<ScaletagTreeItem*>(this));
        return 0;
    }
    ScaletagTreeItem* parentTreeItem() const { return parentItem; }
    QString displayName() const { return name; }

private:
    QString name;
    QString decoration;
    ScaletagTreeItem* parentItem;
    QList<ScaletagTreeItem*> childItems;
};


class ScaleTagTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit ScaleTagTreeModel(scaletag::frames::Project* _project, QObject* _parent = nullptr)
        : QAbstractItemModel(_parent), project(_project) {
        rootItem.reset(new ScaletagTreeItem("root"));
        setupModelData();
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        ScaletagTreeItem* parentItem = parent.isValid()
                                   ? static_cast<ScaletagTreeItem*>(parent.internalPointer())
                                   : rootItem.get();

        ScaletagTreeItem* childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        return QModelIndex();
    }

    QModelIndex parent(const QModelIndex &index) const override {
        if (!index.isValid())
            return QModelIndex();

        ScaletagTreeItem* childItem = static_cast<ScaletagTreeItem*>(index.internalPointer());
        ScaletagTreeItem* parentItem = childItem->parentTreeItem();

        if (parentItem == rootItem.get() || !parentItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
    }

    int rowCount(const QModelIndex &parent = {}) const override {
        ScaletagTreeItem* parentItem = parent.isValid()
                                   ? static_cast<ScaletagTreeItem*>(parent.internalPointer())
                                   : rootItem.get();
        return parentItem->childCount();
    }

    int columnCount(const QModelIndex & = {}) const override { return 1; }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() /*|| role != Qt::DisplayRole*/)
            return QVariant();
        ScaletagTreeItem* item = static_cast<ScaletagTreeItem*>(index.internalPointer());

        if (role == Qt::DisplayRole)
            return item->displayName();

        if (role == Qt::FontRole) {
            ScaletagTreeItem* parent = item->parentTreeItem();
            if (parent == modelsItem && item->displayName() == currentModelName) {
                QFont font;
                font.setUnderline(true);
                return font;
            }
    }
        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid())
            return Qt::NoItemFlags;
        return QAbstractItemModel::flags(index);
    }

//    QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const override {
//        return "Project";/* Not a root Item, i just want to see a caption like ("Project") sitting above the tree
//        This is not an expandable row*/
//        return QVariant();

//    }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (orientation == Qt::Horizontal && section == 0 && role == Qt::DisplayRole)
            return "Project"; /* Not a root Item, i just want to see a caption like ("Project") sitting above the tree
//        This is not an expandable row*/
        return QVariant();
    }

    bool hasChildren(const QModelIndex &parent) const override
    {//Implementing hasChildren is not necessary. Did it just to show + symbol for Analysis and Materials as they
        //don't have children (yet). (row ->0).
        if (!parent.isValid())
            return true; // root always "has children" (top-level items exist)

        ScaletagTreeItem *item = static_cast<ScaletagTreeItem*>(parent.internalPointer());

        // Force the branch indicator for these two specific nodes
        if ( (item->displayName() == "Materials" || item->displayName() == "Analysis") && item->parentTreeItem()==rootItem.get() )
        {
            //std::cout<<item->displayName().toStdString()<<std::endl;
            return true;
        }

        // Default behavior for everything else
        return rowCount(parent) > 0;
    }



    // Call after models are added/removed elsewhere in the project
    void refreshModels() {
        beginResetModel();
        rootItem.reset(new ScaletagTreeItem("root"));
        setupModelData();
        endResetModel();
    }

private:
    void setupModelData() {
        modelsItem = new ScaletagTreeItem("Models", QString(), rootItem.get());
        rootItem->appendChild(modelsItem);

        if (project) {
            const QStringList modelNames = project->getModelsNamesList();
            currentModelName = project->getCurrentModel()->getName();
            for (const QString& name : modelNames){

            //modelsItem->appendChild(new ScaletagTreeItem(name, QString(), modelsItem));

            ScaletagTreeItem* modelItem = new ScaletagTreeItem(name, QString(), modelsItem);

            // "Mesh" node under this model
            ScaletagTreeItem* meshItem = new ScaletagTreeItem("Mesh", QString(), modelItem);
            modelItem->appendChild(meshItem);

            // "Materials" node under this model, populated with that model's materials
            ScaletagTreeItem* materialsItem = new ScaletagTreeItem("Materials", QString(), modelItem);
            modelItem->appendChild(materialsItem);

            if (auto* modelObj = project->getModel(name)) {
                const QStringList materialNames = modelObj->getMaterialsNamesList(); // adjust name
                for (const QString& matName : materialNames)
                    materialsItem->appendChild(new ScaletagTreeItem(matName, QString(), materialsItem));
                modelsItem->appendChild(modelItem);
            }

            }

        }

        ScaletagTreeItem* analysisItem = new ScaletagTreeItem("Analysis", QString(), rootItem.get());
        rootItem->appendChild(analysisItem);

        ScaletagTreeItem* materialsItem = new ScaletagTreeItem("Materials", QString(), rootItem.get());
        rootItem->appendChild(materialsItem);




    }

    scaletag::frames::Project* project;
    std::unique_ptr<ScaletagTreeItem> rootItem;
    ScaletagTreeItem* modelsItem = nullptr; // kept for quick access when refreshing
    QString currentModelName;
};


class ScaleTagTreeView : public QTreeView {
    Q_OBJECT

private:
    QScopedPointer<ScaleTagTreeModel> treeModel;

public:
    ScaleTagTreeView(scaletag::frames::Project* _project) : QTreeView() {
        treeModel.reset(new ScaleTagTreeModel(_project));
        this->setModel(treeModel.data());
        this->setRootIndex(QModelIndex());

        connect(this, &QTreeView::clicked, this, &ScaleTagTreeView::onItemClicked);

//        QString treeStyleSheet{"QTreeView::branch:open:has-children{border-image: url(:/icons/borderexpand) 0;}"
//                               "QTreeView::branch:closed:has-children{border-image: url(:/icons/bordercollapse) 0;}"
//                               //"QTreeView::branch:has-siblings:!adjoins-item{border-image:url(:/icons/borderline) 0;}"
//                               "QTreeView::branch:has-siblings:adjoins-item{border-image:url(:/icons/bordermore) 0;}"
//                               "QTreeView::branch:!has-children:!has-siblings:adjoins-item{border-image:url(:/icons/borderend) 0;}"
//                               "QTreeView::branch:closed:has-children:!has-siblings{border-image: url(:/icons/bordercollapse) 0;}"
//                               "QTreeView::branch:closed:has-children:has-siblings{border-image: url(:/icons/bordercollapsemore) 0;}"
//                               "QTreeView::branch:open:has-children:!has-siblings{border-image: url(:/icons/borderexpand) 0;}"
//                               "QTreeView::branch:open:has-children:has-siblings{border-image: url(:/icons/borderexpandmore) 0;}"
//        };

        QString treeStyleSheet{
            // 1. Verrouillage géométrique
            "QTreeView::branch { width: 16px; height: 16px; }"

            // 2. LIGNES VERTICALES DE PASSAGE (Le cœur du problème)
            // Pour les enfants en arrière-plan :
            "QTreeView::branch:has-siblings:!adjoins-item { image: url(:/icons/borderline); }"

            // AJOUT : Pour un nœud parent OUVERT qui a des frères en dessous (ex: "Models")
            "QTreeView::branch:open:has-children:has-siblings:!adjoins-item { image: url(:/icons/borderline); }"

            // 3. JONCTIONS SIMPLES (Fichiers / Feuilles)
            "QTreeView::branch:has-siblings:adjoins-item { image: url(:/icons/bordermore); }"
            "QTreeView::branch:!has-siblings:adjoins-item { image: url(:/icons/borderend); }"

            // 4. BOUTONS FERMÉS [+]
            "QTreeView::branch:closed:has-children:!has-siblings { image: url(:/icons/bordercollapse); }"
            "QTreeView::branch:closed:has-children:has-siblings { image: url(:/icons/bordercollapsemore); }"

            // 5. BOUTONS OUVERTS [-]
            "QTreeView::branch:open:has-children:!has-siblings { image: url(:/icons/borderexpand); }"
            "QTreeView::branch:open:has-children:has-siblings { image: url(:/icons/borderexpandmore); }"
        };



        this->setStyleSheet(treeStyleSheet);
        //this->setStyleSheet("QTreeView { show-decoration-selected: 1; }");

        this->setHeaderHidden(false); //To enable caption "Project" to appear. It is defined in heaData() function

        //this->setRootIsDecorated(false);
    }
//    "QTreeView::branch:open:has-children{border-image: url(:/icons/borderexpand) 0;}"
//    "QTreeView::branch:closed:has-children{border-image: url(:/icons/bordercollapse) 0;}"
////    "QTreeView::branch:open:!has-children{border-image: url(:/icons/borderexpand) 0;}"
//    "QTreeView::branch:closed:!has-children{border-image: url(:/icons/bordercollapse) 0;}"
//    "QTreeView::branch:has-siblings:!adjoins-item{border-image:url(:/icons/borderline) 0;}"
//    "QTreeView::branch:has-siblings:adjoins-item{border-image:url(:/icons/bordermore) 0;}"
//    "QTreeView::branch:!has-children:!has-siblings:adjoins-item{border-image:url(:/icons/borderend) 0;}"
//    "QTreeView::branch:closed:has-children:!has-siblings{border-image: url(:/icons/bordercollapse) 0;}"
//    "QTreeView::branch:closed:has-children:has-siblings{border-image: url(:/icons/bordercollapsemore) 0;}"
//    "QTreeView::branch:open:has-children:!has-siblings{border-image: url(:/icons/borderexpand) 0;}"
//    "QTreeView::branch:open:has-children:has-siblings{border-image: url(:/icons/borderexpandmore) 0;}"

    void setupMainWindowTreeView(scaletag::window::MainWindow* mainWindow) {
        QDockWidget *dock = new QDockWidget("Project Structure", mainWindow, Qt::Widget);
        dock->setAllowedAreas(Qt::LeftDockWidgetArea);
        dock->setWidget(this);
        mainWindow->addDockWidget(Qt::LeftDockWidgetArea, dock);
        dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dock->setVisible(true);
        mainWindow->resizeDocks({dock}, {125}, Qt::Horizontal);
        dock->show();
        this->show();

    }


    QStringList capturedExpandedState();
    void restoreExpandedState(const QStringList& expandedNames);
    void refreshModels() {
        const QStringList expandedNames = capturedExpandedState();
        treeModel->refreshModels();
        restoreExpandedState(expandedNames);}

signals:
    // Emitted for every click, category tells you which top-level branch it belongs to
    void itemClicked(const QString& category, const QString& itemName);
    // Convenience signal fired only when a model entry (child of "Models") is clicked
    void modelClicked(const QString& modelName);

private slots:
    void onItemClicked(const QModelIndex &index) {

        if (!index.isValid())
            return;

        const QString name = index.data(Qt::DisplayRole).toString();
        const QModelIndex parentIndex = index.parent();
        const QModelIndex grandparentIndex = parentIndex.parent();

        const QString parentName = parentIndex.data(Qt::DisplayRole).toString();
        const QString grandparentName = grandparentIndex.data(Qt::DisplayRole).toString();


        emit itemClicked(parentName, name);
/*
        if (!parentIndex.isValid()) {
            // top-level category itself was clicked, e.g. "Models"
            emit itemClicked(name, QString());
            return;
        }

        const QString category = parentIndex.data(Qt::DisplayRole).toString();
        emit itemClicked(category, name);

        if (category == "Models")
            emit modelClicked(name);*/
    }


//  void update(){
//         scaletag::frames::Project* pr =reinterpret_cast<scaletag::frames::Project*>(this->parent());
//         QStringList modelsNames = pr->getModelsNamesList();
//  }

};

#endif
