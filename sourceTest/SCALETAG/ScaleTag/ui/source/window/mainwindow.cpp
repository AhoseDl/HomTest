#include <window/mainwindow.h>

#include <QQuickView>
#include<QQmlEngine>
#include <QQuickWidget>
#include <QFontDatabase>
#include <QQmlComponent>


namespace scaletag {
namespace window {

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{qobject_cast<QWidget*>(parent)}
{
    init();
}

void MainWindow::init(){
//    setWindowIcon(QPixmap(":/logos/marc.png"));
    //setWindowIcon(QPixmap(":/logos/scaletaglogo.png"));
    mdiArea.reset(new QMdiArea(this));
    workdir.reset(new frames::WorkDirectory());

    /*QQuickView*/ QQuickWidget view;
    view.engine()->addImportPath("qrc:/");
    view.engine()->addImportPath("qrc:/components/");
    view.setSource( QUrl(QStringLiteral("qrc:/buttons/ImportModel.qml")) );
    //view.setResizeMode(QQuickWidget::SizeRootObjectToView); //QQuickWidget::SizeViewToRootObject
    //view.setResizeMode(QQuickWidget::SizeViewToRootObject);
    //view.resize(40,40);
    //view.show();
    QDockWidget *dockRight = new QDockWidget((""),this, Qt::Widget/*Qt::SubWindow*/);
    dockRight->setAllowedAreas(Qt::RightDockWidgetArea);
    //this->resizeDocks({dockRight}, {20}, Qt::Horizontal);
        //dockRight->setFixedWidth(100);
    QWidget* allWid = new QWidget();
    QVBoxLayout* lay = new QVBoxLayout();
//    QWidget* viewContainer = QWidget::createWindowContainer(qobject_cast<QWindow*>(&view), this, Qt::Window);
//    viewContainer->resize(100,100);
//    viewContainer->setWindowFlags((Qt::Window|Qt::CustomizeWindowHint)&(~Qt::WindowCloseButtonHint));
//    viewContainer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    viewContainer->setVisible(true);

    int fontId = QFontDatabase::addApplicationFont(":assets/fontawesome");
    auto ftfamily = QFontDatabase::applicationFontFamilies(fontId)[0];
    QFont font;
    font.setFamily(ftfamily);
    font.setPixelSize(50);

    QPushButton* importModelFileButton=new QPushButton(/*"\uf04b", this*/);
    importModelFileButton->setFont(font);
    importModelFileButton->setText("\uf15b");
    importModelFileButton->setFlat(true);

    importModelFileButton->setStyleSheet(QString(R"(
        QPushButton {
            /*background-color: %1;   Background color set to the darkened green */
            color: %1;          /* White text */
            border-radius: 10px;   /* Rounded corners */
            padding: 10px 20px;    /* Padding around text */
        }
        QPushButton:hover {
            color: %2;  /* Darker color on hover */
        }

    )").arg( QColor("#B7950B").name(), QColor("#B7950B").lighter().name() ));
    importModelFileButton->setToolTip("Import Model");

    QPushButton* runPbcAnalysisButton=new QPushButton("\uf04b", this);
    runPbcAnalysisButton->setFont(font);
    runPbcAnalysisButton->setStyleSheet(QString(R"(
        QPushButton{
            /*background-color: %1;   Background color set to the darkened green */
            color: %1;          /* White text */
            border-radius: 10px;   /* Rounded corners */
            padding: 10px 20px;    /* Padding around text */
        }
        QPushButton:hover {
            color: %2;  /* Darker color on hover */
        }
    )").arg( QColor("#1e8449").name(), QColor("#1e8449").lighter().name() ));
    runPbcAnalysisButton->setToolTip("Run PBC analysis");
    runPbcAnalysisButton->setFlat(true);

    QPushButton* showLinkButton=new QPushButton(this);
    showLinkButton->setFont(font);
    showLinkButton->setStyleSheet(QString(R"(
        QPushButton{
            qproperty-icon: url("");
           qproperty-iconSize: 45px 45px;
        /*background-image : url(%1);*/
/*background-repeat: no-repeat;*/
border-image : url(%1);
         border:none;
/*background-position: center ;*/
        width: 45px ;
        height : 45px;
            }
        QPushButton:hover{
            /* padding-left: 2px; */
            /*padding-right: 2px; */
/* background-image: url(%2);*/
border-image : url(%2);
             border:none;
/*background-position:  center ;*/
 width: 45px ;
  height : 45px;
        }
    )").arg( QString(":/icons/link"), QString(":/icons/link-hover")) );

    showLinkButton->setToolTip("View links");
    showLinkButton->setFlat(true);
    showLinkButton->setFixedWidth(45);


    QPushButton* viewResultButton=new QPushButton(/*"", this*/);
    viewResultButton->setFont(font);
    viewResultButton->setText("\uf06e");
    viewResultButton->setFlat(true);
    viewResultButton->setStyleSheet(QString(R"(
        QPushButton{
            /*background-color: %1;   Background color set to the darkened green */
            color: %1;          /* White text */
            border-radius: 10px;   /* Rounded corners */
            padding: 10px 20px;    /* Padding around text */
        }
        QPushButton:hover {
            color: %2;  /* Darker color on hover */
        }
    )").arg( QColor(/*"#1e8449"*/"#0047AB").name(), QColor(/*"#1e8449"*/"#0047AB").lighter().name() ));
    /*Pure Blue: #0000FF Baby Blue: #89CFF0 Cobalt Blue: #0047AB Dark Blue: #00008B Navy Blue: #000080
     * Sky Blue: #87CEEB Electric Blue: #1F51FF*/
    viewResultButton->setToolTip("View job results");
    viewResultButton->setFlat(true);


    QPushButton* showMeshEdgesButton=new QPushButton(this);
    showMeshEdgesButton->setFont(font);
    showMeshEdgesButton->setStyleSheet(QString(R"(
        QPushButton{
            qproperty-icon: url("");
           qproperty-iconSize: 45px 45px;
        /*background-image : url(%1);*/
/*background-repeat: no-repeat;*/
border-image : url(%1);
         border:none;
/*background-position: center ;*/
        width: 45px ;
        height : 45px;
            }
        QPushButton:hover{
            /* padding-left: 2px; */
            /*padding-right: 2px; */
/* background-image: url(%2);*/
border-image : url(%2);
             border:none;
/*background-position:  center ;*/
 width: 45px ;
  height : 45px;
background-color: rgba(0, 0, 0, 100);
        }
    )").arg( QString(":icons/showMeshEdges2"), QString(":icons/showMeshEdges2")) );

    showMeshEdgesButton->setToolTip("Show mesh edges.");
    showMeshEdgesButton->setFlat(true);
    showMeshEdgesButton->setFixedWidth(45);


    lay->addWidget(importModelFileButton);
    lay->addWidget(&view/*viewContainer*/);
    lay->addWidget(showLinkButton, 0, Qt::AlignHCenter);
    lay->addWidget(runPbcAnalysisButton);
    lay->addWidget(viewResultButton);
    lay->addWidget(showMeshEdgesButton, 0, Qt::AlignHCenter);


    allWid->setLayout(lay);
    dockRight->setWidget(allWid);
    dockRight->setFixedWidth(120);

    addDockWidget(Qt::RightDockWidgetArea, dockRight);
    dockRight->setVisible(true);  // comment this line and a band appears
    dockRight->show();
    view.show();
    dockRight->setFloating(false);
    dockRight->setFeatures(QDockWidget::NoDockWidgetFeatures);


    this->resize(900, 850);
    // setup menubar
    fileMenu = menuBar()->addMenu("&File");
    viewMenu = menuBar()->addMenu("&View");
    editMenu = menuBar()->addMenu("&Edit");
    selectionMenu = menuBar()->addMenu("&Selection");
    modelMenu = menuBar()->addMenu("&Model");
    pbcMeshMenu = menuBar()->addMenu("&PBC Mesh");

    // setup toolbar
    fileToolBar = addToolBar("File");
    viewToolBar = addToolBar("View");
    modelToolBar=addToolBar("Model");

    // main area for image display

    // setup status bar
    mainStatusBar = statusBar();
    mainStatusBar->setMinimumHeight(10);

    /*QTextEdit**/ scaleTagConsole = new QTextEdit(mainStatusBar);//mainStatusBar);
     /*QTextEdit**/ scaleTagInfo=new QTextEdit(mainStatusBar);
    Console* acout=new Console(&std::cout, scaleTagConsole);
    Console* acerr=new Console(&std::cerr, scaleTagInfo);
     //scaleTagConsole->setFixedHeight(200);
    //mainStatusBar->layout()->setContentsMargins(11, 11, 11, 11);
    QFont* scaleTagFont=new QFont("helvetica", 9, QFont::Bold, 0);
    scaleTagFont->setStyleHint(QFont::Monospace);
    scaleTagConsole->setCurrentFont(*scaleTagFont);
    //scaleTagConsole->currentFont().setStyleHint(QFont::Monospace);
    scaleTagConsole->setMinimumHeight(100);
    scaleTagConsole->setMinimumWidth(850);
    //scaleTagConsole->setTextColor("#ffffff");
    scaleTagConsole->setTextColor(QColor(0xff,0xff, 0xff));
    scaleTagConsole->setReadOnly(true);
    //scaleTagConsole->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    scaleTagConsole->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::QSizePolicy::Expanding);
    //scaleTagConsole->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    scaleTagConsole->setAlignment(Qt::AlignLeft);
    //mainStatusBar->adjustSize();
    scaleTagConsole->setReadOnly(true);
//    mainStatusBar->setFixedHeight(scaleTagConsole->height());
//    mainStatusBar->setFixedWidth(scaleTagConsole->width());


    scaleTagInfo->setCurrentFont(*scaleTagFont);
    scaleTagInfo->setMinimumHeight(100);
    scaleTagInfo->setMinimumWidth(850);
    scaleTagInfo->setTextColor(QColor(0xff,0xff, 0xff));
    //scaleTagInfo->setTextBackgroundColor(QColor(0x00,0x00, 0x00));
    //scaleTagInfo->setStyleSheet("background-color:#2c001e/*#333333 */;");
    scaleTagInfo->setStyleSheet(/*"color: #efefef;"*/
                    "background-color: black;"
                    "selection-color: green;"
                    /*"selection-background-color: blue;"*/);
    scaleTagInfo->setReadOnly(true);
    scaleTagInfo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::QSizePolicy::Expanding);
    scaleTagInfo->setAlignment(Qt::AlignLeft);

    //QTextStream ss(stdout);
   OutputTo  outputto(scaleTagInfo);
   // outputto.init();



     QTextStream* stream = outputto.getStream();
     *stream<<"Though your sins be as scarlet\n"
               "They shall be as white as snow\n"
               "Though your sins be as scarlet\n"
               "They shall be as white as snow\n"
               "Though they be red like crimson\n"
               "They shall be as wool!\n"
               "Though your sins be as scarlet\n"
               "Though your sins be as scarlet\n"
               "They shall be as white as snow\n"
               "They shall be as white as snow\n"
               "Hear the voice that entreats you\n"
               " Oh, return ye unto God!\n"
               "Hear the voice that entreats you\n"
               "Oh, return ye unto God!\n"
               "He is of great compassion\n"
               "And of wondrous love\n"
               "Hear the voice that entreats you\n"
               " Hear the voice that entreats you\n"
               " Oh, return ye unto God!\n"
               "Oh, return ye unto God!";
     //stream->device()->setCurrentWriteChannel(fileno(stdout));
    //outputto.takeStandardOutput(ss);
    //QTextStream ss(stdout);
    //cout<<"info widget initialized";
    //ss<<"info widget initialized";
    //scaleTagInfo->append(ss.readAll());

    QTabWidget* tabWidget=new QTabWidget(mainStatusBar);
    tabWidget->addTab(scaleTagConsole, "Term");
    tabWidget->addTab(scaleTagInfo, "Infos");


    mainStatusBar->setWindowTitle("Information");
    mainStatusBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::QSizePolicy::Expanding);
    //mainStatusBar->addPermanentWidget(scaleTagConsole,1);
    mainStatusBar->addPermanentWidget(tabWidget,1);

    QDockWidget *dock = new QDockWidget(("Information / Console"),this, Qt::Widget/*Qt::SubWindow*/);
    dock->setVisible(true);  // comment this line and a band appears
    dock->setAllowedAreas(Qt::BottomDockWidgetArea /*| Qt::RightDockWidgetArea*/);
    dock->setWidget(mainStatusBar);
    //dock->setFloating(false);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures |QDockWidget::DockWidgetVerticalTitleBar);
    //dock->setFeatures(dock->features()|QDockWidget::DockWidgetVerticalTitleBar);
    this->addDockWidget(Qt::BottomDockWidgetArea,dock);


    //QSplitter* splitter=new QSplitter(Qt::Vertical);
    //splitter->addWidget(centralWidget());
    //splitter->addWidget(mainStatusBar);
    //splitter->setSizes(QList<int>({100,200}));

   // mainStatusBar->setLayout(scaleTagConsole->layout());
    //scaleTagConsole->setLayout(mainStatusBar->layout());
     //this->layout()->addWidget(mainStatusBar);
    //this->layout()->addWidget(scaleTagConsole);
     //this->centralWidget()->layout()->addWidget(mainStatusBar);
    //mainStatusBar->setLayout(this->centralWidget()->layout());

//    QVBoxLayout* layout=new QVBoxLayout;
//    mainStatusBar->setLayout(layout);
//    ((QVBoxLayout*)(mainStatusBar->layout()))->addWidget(scaleTagConsole,1);
//    QVBoxLayout* layout=new QVBoxLayout;
//    layout->addWidget(centralWidget(),0);
//    layout->addWidget(mainStatusBar,0);


    //mainStatusBar->setFixedHeight(400);
    //mainStatusBar->setMaximumHeight(200);


    std::ostringstream sout("Hello0000000");
    sout<<"Welcome to the information area!"<<endl;
    sout<<"Quand le ciel bas et lourd pèse comme un couvercle\nsur l'esprit gémissant en proie aux longs ennuis\net que de l'horizon embrassant tout le cercle\nil nous verse un jour noir plus triste que les nuits"<<endl;
    sout<<endl<<"Catch a falling star and put it in your pocket\nNever let it fade away\nCatch a falling star and put  it in your pocket\n"
            "Save it for a rainy day\n For love may come and tap you on the shoulder\nSome starless night\n Just in case you feel you wanna hold her\n"
            "You'll have a pocket full of starlight";
    scaleTagConsole->/*setText*/append(QString::fromStdString(sout.str()));
    scaleTagConsole->setStyleSheet("background-color:#2c001e/*#333333 */;"); //#604b62 #75507B #673147 #333333 #dd4814 #77216f #aea79f
    mainStatusBar->setSizeGripEnabled(true);

    QAction* setWorkDirAction = new QAction("&Set Work Directory");
    fileMenu->addAction(setWorkDirAction);
    //fileToolBar->addAction(setWorkDirAction);
    QObject::connect(setWorkDirAction, &QAction::triggered, workdir.data(), &frames::WorkDirectory::setWorkDirectory);

    QAction* importModelAction =new QAction("&Import Model");
   // modelMenu->addAction(importModelAction);
    fileToolBar->addAction(importModelAction);
    fileMenu->addAction(importModelAction);
    QObject::connect(importModelAction, &QAction::triggered, this, [this](){
  //QString filename = QFileDialog::getExistingFile(nullptr, tr("Open Directory"),QDir::currentPath(),QFileDialog::DontResolveSymlinks);
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QDir::currentPath(), tr("Models (*.inp *.dat)"));
  /*qDebug()*/std::cout<<"filename imported : "<< fileName.toStdString()<<std::endl;
    emit importModelFired(fileName);});


    QObject::connect(importModelFileButton, &QPushButton::clicked, importModelAction, &QAction::triggered);

    QAction* runPBCAnalysisAction =new QAction("&Run PBC Analysis");
    modelMenu->addAction(runPBCAnalysisAction);
    modelToolBar->addAction(runPBCAnalysisAction);
    modelMenu->addAction(runPBCAnalysisAction);
    QObject::connect(runPbcAnalysisButton, &QPushButton::clicked, runPBCAnalysisAction, &QAction::triggered);
    QObject::connect(runPBCAnalysisAction, &QAction::triggered, this, &MainWindow::runPBCAnalysisFired);

    QAction* viewResultAction =new QAction("&View results");
    viewMenu->addAction(viewResultAction);
    viewToolBar->addAction(viewResultAction);
    QObject::connect(viewResultButton, &QPushButton::clicked, viewResultAction, &QAction::triggered);
    QObject::connect(viewResultAction, &QAction::triggered, this, &MainWindow::viewResultActionFired);


    QAction* showMeshEdgesAction = new QAction("&Show mesh edges");
    pbcMeshMenu->addAction(showMeshEdgesAction);
    QObject::connect(showMeshEdgesButton, &QPushButton::clicked, showMeshEdgesAction, &QAction::triggered);
    QObject::connect(showMeshEdgesAction, &QAction::triggered, this, &MainWindow::showMeshEdgesFired);

    QAction* importCADAction = new QAction("&Import CAD");
    fileMenu->addAction(importCADAction);
    modelToolBar->addAction(importCADAction);
    QObject::connect(importCADAction, &QAction::triggered, this, [this](){
        QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QDir::currentPath(), tr("CAD (*.stl *.iges *.igs *.step *.stp *.STEP)"));
        /*qDebug()*/std::cout<<"filename imported : "<< fileName.toStdString()<<std::endl;
        emit importCADFired(fileName);
    });


    QAction* importCADWithShapeAction = new QAction("&Import CAD with shape");
    fileMenu->addAction(importCADWithShapeAction);
    modelToolBar->addAction(importCADWithShapeAction);
    QObject::connect(importCADWithShapeAction, &QAction::triggered, this, [this](){
        QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QDir::currentPath(), tr("CAD (*.iges *.igs *.step *.stp *.STEP)"));
        std::cout<<"filename imported : "<< fileName.toStdString()<<std::endl;
        emit importCADWithShapeFired(fileName);
    });


    QAction* mapSurfaceAction = new QAction("&Map periodic surfaces");

    QAction* verifyPBCAction = new QAction("&Verify PBC constraints");

    QAction* meshAction = new QAction("&Generate the periodic mesh");

    pbcMeshMenu->addAction(mapSurfaceAction);
    pbcMeshMenu->addAction(verifyPBCAction);
    pbcMeshMenu->addAction(meshAction);

    QObject::connect(mapSurfaceAction, &QAction::triggered, this, &MainWindow::mapSurfaceFired);
    QObject::connect(verifyPBCAction, &QAction::triggered, this, &MainWindow::verifyPBCFired);
    QObject::connect(meshAction, &QAction::triggered, this, &MainWindow::meshActionFired);



    QAction* createViewPortAction = new QAction("&Create viewport");
    viewMenu->addAction(createViewPortAction);
    QObject::connect(createViewPortAction, &QAction::triggered, this, &MainWindow::createViewPortFired);

    QAction* createModelAction = new QAction("&Create Model");
    modelMenu->addAction(createModelAction);
    QObject::connect(createModelAction, &QAction::triggered, this, &MainWindow::createNoNameModelFired);

    QAction* deleteModelAction = new QAction("&Delete Model");
    modelMenu->addAction(deleteModelAction);
    QObject::connect(deleteModelAction, &QAction::triggered, this, &MainWindow::deleteModelFired);


    renameModelMenu= modelMenu->addMenu("&Rename Model");
    //actions( here models names) are added through objectfactory
    QObject::connect(renameModelMenu, &QMenu::aboutToShow, this, &MainWindow::renameModelFired);
    //QObject::connect(RenameMode)

    QAction *createMaterialAction, *modifyMaterialAction, *deleteMaterialAction, *renameMaterialAction;
    materialMenu = menuBar()->addMenu("Ma&terial");
    createMaterialAction = new QAction("&Create a new material");
    materialMenu->addAction(createMaterialAction);

    modifyMaterialMenu = materialMenu->addMenu("&Modify a material");
    renameMaterialMenu = materialMenu->addMenu("&Rename a material");
    deleteMaterialMenu = materialMenu->addMenu("&Delete a material");
    assignMaterialMenu = materialMenu->addMenu("&Assign a material");

    QObject::connect(modifyMaterialMenu, &QMenu::aboutToShow, this, &MainWindow::modifyMaterialFired/*&MainWindow::modifyMaterial*/);//modifyMaterial to be put in ObjectFactory class
    QObject::connect(renameMaterialMenu, &QMenu::aboutToShow, this, &MainWindow::renameMaterialFired);
    QObject::connect(deleteMaterialMenu, &QMenu::aboutToShow, this, &MainWindow::deleteMaterialFired);
    QObject::connect(assignMaterialMenu, &QMenu::aboutToShow, this, &MainWindow::assignMaterialFired);




    deleteMaterialAction = new QAction("&Delete a material");
    renameMaterialAction = new QAction("&Rename a material");

//    deleteMaterialMenu->addAction(deleteMaterialAction);
//    renameMaterialMenu->addAction(renameMaterialAction);

    QObject::connect(createMaterialAction, &QAction::triggered, this, &MainWindow::createMaterial);







    QAction* startSelectionAction = new QAction("&Select Faces");
    selectionMenu->addAction(startSelectionAction);
    QObject::connect(startSelectionAction, &QAction::triggered, this, &MainWindow::startSelectionFired);

    QAction* endSelectionAction = new QAction("&End Faces selection");
    selectionMenu->addAction(endSelectionAction);
    QObject::connect(endSelectionAction, &QAction::triggered, this, &MainWindow::endSelectionFired);

    //implementing the treeView
    //treeView.reset( new QTreeView);
    //treeModel.reset(new QStandardItemModel);
    //treeView->setModel(treeModel.data());


}


} // namespace frames
} // namespace scaletag
