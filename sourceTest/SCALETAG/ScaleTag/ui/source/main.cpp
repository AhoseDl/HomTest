#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include<QObject>
#include<QQmlContext>
#include<QDebug>
#include<QWindow>
#include<QList>
#include<vector>
#include<visualization/visualization.h>

#include<QMainWindow>
#include<QApplication>
#include<QWindow>
#include <QGraphicsAnchorLayout>
#include <QDockWidget>

//#include<QVTKOpenGLNativeWidget.h>

#include <controllers/solaris.h>


#include<vtkConeSource.h>
#include<QQuickView>
#include<QIcon>
#include<QQuickStyle>
#include<QStringList>
#include<controllers/mainwindowcontroller.h>

//#include<fem.h>
#include<femPbc.h>
#include<pbcanalysis.h>

//#include<solver.h>   // To be decommented to take in account mumps utilities

#include<petscutilities.h>
//#include<petscutilities2.h>
#include<chrono>
//#include"/home/ahose/Desktop/ThinkCell/kuu.h"

#include<QQuickVTKItem.h>

using namespace std;

int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QCoreApplication::setApplicationName("Scaletag");
    QGuiApplication::setApplicationDisplayName("Scaletag");
    QCoreApplication::setOrganizationName("Scaletag");
    QCoreApplication::setOrganizationDomain("scaletag.com");

    int ws, wr;
    chrono::time_point<std::chrono::high_resolution_clock> ti, tf;
        scaletag::fem::BCs bc;
    std::valarray<double> disp;
    std::valarray<double> force;

    double E11=0, nu12=0, nu13=0;
    MeshPbc<double, 3> me;


    MPI_Comm COMM_OBAR; int commOBarSize;

  //  MPI_Init(&argc, &argv);
    PetscFunctionBeginUser;
    //PetscInitialize(&argc, &argv, NULL, NULL);
    SlepcInitialize(&argc,&argv,NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ws);
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);

    int commColor = (wr==0 ? MPI_UNDEFINED : 0);
    MPI_Comm_split(MPI_COMM_WORLD, commColor, wr, &COMM_OBAR);
        MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_size(COMM_OBAR, &commOBarSize);
    MPI_Barrier(MPI_COMM_WORLD);

  if (wr==1) cout<<"COMM_OBAR creation completed with a size of : "<<commOBarSize<<std::endl;
  MPI_Comm_free(&COMM_OBAR);
MPI_Barrier(MPI_COMM_WORLD);

//        if(wr==0){
   // me.marcInitialize("/home/ahose/Documents/ForScaleTagFem/OctetTruss.dat");
  //  me.marcInitialize("/home/ahose/Documents/ForScaleTagFem/KelvinSeul.dat");
//me.marcInitialize("/home/ahose/Desktop/OctetSeul.dat");
//me.marcInitialize("/home/ahose/Desktop/NouveauOctetSeul2.dat");
//    me.marcInitialize("/media/ahose/4133-5044/octetseul_TractionX.dat");
// me.marcInitialize("/home/ahose/Desktop/KelvinSeul.dat");
//me.marcInitialize("/home/ahose/Desktop/octetseul_final.dat");

    MPI_Barrier(MPI_COMM_WORLD);
   // cout<<"number of nodes ... : "<< me.getNumberOfNodes()<<" wr : "<<wr<<" ws : "<<ws<<std::endl;
    //scaletag::fem::matrix<double> mat;
    //std::valarray<size_t> numPbc;
    //scaletag::models::PbcUtilities pbc;
    //vector<pbcEquation> pbcLinks;

    ti=chrono::high_resolution_clock::now();
//}


    //To be continued after visualization
    MPI_Barrier(MPI_COMM_WORLD);
//    if(/*wr!=0*/1){
//        /*PetscCall(*/scaletag::fem::ElasXAnalysis(/*&argc, &argv, */me, 3, bc, scaletag::fem::matrixValueTetra,
//                                               disp, force, E11, nu12, nu13 )/*)*/;
//    }
        MPI_Barrier(MPI_COMM_WORLD);
//End to be continued after visualization
    //To be contiued after visualization
    tf=chrono::high_resolution_clock::now();
    cout<<"computation duration : "<<chrono::duration_cast<chrono::milliseconds>(tf-ti).count() <<endl;

if(wr==0){

//#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif


    QQuickVTKRenderWindow::setupGraphicsBackend();
//QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

//QSurfaceFormat format;
//format.setVersion(3, 2);
//format.setProfile(QSurfaceFormat::CoreProfile);
//format.setDepthBufferSize(24);
//format.setStencilBufferSize(8);
//QSurfaceFormat::setDefaultFormat(format);
        //QGuiApplication qmlapp(argc, argv);
        QApplication widgetapp(argc, argv);

    qmlRegisterUncreatableType<scaletag::frames::IObjectFactory>("SCT", 1, 0, "IObjectFactory", "Interface");
    qmlRegisterType<scaletag::visualization::Visualization>("SCT", 1,0,  "Visualisation");
    // qmlRegisterType<scaletag::frames::WorkDirectory>("SCT", 1,0,  "WorkDirectory");
    qmlRegisterType<scaletag::controllers::MainWindowController>("SCT", 1,0,  "MainWindowController");

    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Fusion");
    //   QQuickStyle::setStyle("Imagine");
    //   QQuickStyle::setStyle("Material");
    //QQuickStyle::setStyle("Universal");

    engine.addImportPath("qrc:/");
    engine.addImportPath("/usr/local/lib/qml");
    const QUrl url(QStringLiteral("qrc:/views/MainView.qml"));

    //auto solaris=new scaletag::controllers::Solaris(&engine);
    // engine.rootContext()->setContextProperty("solaris",solaris);
    //auto objectFactory=new scaletag::frames::ObjectFactory(/*(QWindow*)qmlMainWindow,*/ &engine);
    auto solaris=new scaletag::controllers::Solaris(&engine);
    engine.rootContext()->setContextProperty("solaris",solaris);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &widgetapp, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    //The above connection so that i can use only one event loop
    engine.load(url);

   // engine.load(url);




    {
    //auto qmlMainWindow= dynamic_cast<QWindow*>(engine.rootObjects().value(0));

    //engine.rootContext()->setContextProperty("solaris",solaris);
    //engine.load(QUrl(QStringLiteral("qrc:/views/Vtk.qml")));

    //For VTK Rendering

    //   objectFactory->setQmlMainWindow(qmlMainWindow2);
    //   solaris->setObjectFactory(objectFactory);
    //   scaletag::controllers::Solaris* solaris= new scaletag::controllers::Solaris(qmlMainWindow,
    //(new scaletag::frames::ObjectFactory(/*(scaletag::window::MainWindow*)widgetMainWindow,*/(QWindow*)qmlMainWindow, &engine)),&engine );
    //   engine.rootContext()->setContextProperty("solaris",solaris);
    }
    widgetapp.setQuitOnLastWindowClosed(true);
        //  qmlapp.setQuitOnLastWindowClosed(true);
    //   widgetapp.setWindowIcon(QPixmap(":/logos/marc.png"));
    widgetapp.setWindowIcon(QPixmap(":/logos/scaletaglogo.png"));
    //qmlapp.setQuitOnLastWindowClosed(false);
    widgetapp.setOrganizationName("ScaleTag");
    widgetapp.setOrganizationDomain("ScaleTag");



//end if(wr==0)
    //scaletag::models::testMain(argc, argv);

   {

//scaletag::visualization::Visualization view(qmlMainWindow);
//engine.rootContext()->setContextProperty("view",&view);




//scaletag::window::WorkDirectory workDirectory(qmlMainWindow);
//engine.rootContext()->setContextProperty("workDirectory", &workDirectory);


//scaletag::controllers::Solaris solaris(&win, new scaletag::frames::ObjectFactory(&win));

//scaletag::window::MainWindow* widgetMainWindow = new scaletag::window::MainWindow();//a++
//widgetMainWindow->setWindowTitle("ScaleTag");//a++
//widgetMainWindow->setWindowIcon(QPixmap(":/logos/marc.png"));//a++
//widgetapp.setWindowIcon(QPixmap(":/logos/marc.png"));//a++

//QQuickWidget* wid=new QQuickWidget(QUrl("qrc:/views/Vtk.qml"), &rwin);
//widgetMainWindow.show();


//QWidget* container = QWidget::createWindowContainer( (QWindow*)qmlMainWindow); //a++

//container->resize(800,800);//a++
//QDockWidget *dock = new QDockWidget(("Visualization"),widgetMainWindow); //a++
//dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);



//dock->setWidget(container);//a++
//widgetMainWindow->addDockWidget(/*Qt::LeftDockWidgetArea*/Qt::TopDockWidgetArea, dock); //a++
//widgetMainWindow->show(); //a++
//widgetMainWindow.setCentralWidget(container);
//widgetMainWindow.show();

//QQuickWidget* wid=new QQuickWidget;
//wid->setSource(QUrl("qrc:/views/Vtk.qml"));
//wid->show();
//QQuickVTKRenderItem* item = (wid)->findChild<QQuickVTKRenderItem*>("visual");

//cout<<"1:"<<"item : "<<item;
   }



   //int loop= (qmlapp.exec() &&  widgetapp.exec());

   return widgetapp.exec();
  // return loop;
}

MPI_Barrier(MPI_COMM_WORLD);
//PetscFinalize();
SlepcFinalize();
PetscFunctionReturn(PETSC_SUCCESS);
//MPI_Finalize();
return 0;
}
