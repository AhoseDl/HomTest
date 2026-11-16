#include "mainwindowcontroller.h"
#include<QLayout>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QQuickVTKRenderItem.h>
#include <QQuickWidget>
#include<QQmlEngine>


namespace scaletag {
namespace controllers {

MainWindowController::MainWindowController(QObject *parent)
    : QObject{parent}
{
    qmlMainWindow.reset(qobject_cast<QWindow*>(parent));

    widgetMainWindow.reset(qobject_cast<window::MainWindow*>(new scaletag::window::MainWindow()));
}

window::MainWindow* MainWindowController::createMainWindow()
{
    widgetMainWindow->setWindowTitle("ScaleTag");
    widgetMainWindow->setWindowIcon(QIcon(QPixmap(":/logos/scaletaglogo.png")));

    auto* mdiArea=widgetMainWindow->getMdiArea();
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setActivationOrder(QMdiArea::StackingOrder/*QMdiArea::CreationOrder*//*QMdiArea::ActivationHistoryOrder*/);
    mdiArea->setTabPosition(QTabWidget::South);
    mdiArea->setTabsMovable(true);
    mdiArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetMainWindow->setCentralWidget(mdiArea);

    QWidget* container = QWidget::createWindowContainer(qmlMainWindow.data(), widgetMainWindow.data()/*->centralWidget()*/,/*Qt::Window*/Qt::Window);
    //QWidget* container = QWidget::createWindowContainer(qmlMainWindow.data());
    container->setVisible(true);
    //container->resize(900,850);
    //container->setMinimumHeight(1);
    //container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::QSizePolicy::Preferred);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);//************
    container->setFocusPolicy(Qt::TabFocus);
    //container->setWindowFlags(Qt::SubWindow|Qt::CustomizeWindowHint);
   container->setWindowFlags((Qt::Window|Qt::CustomizeWindowHint)&(~Qt::WindowCloseButtonHint));

    auto* subWindow = mdiArea->addSubWindow(container);
    //subWindow->setWindowFlags(subWindow->windowFlags()|Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::SubWindow);
   // subWindow->setResizeMode(QQuickView::SizeRootObjectToView);

//    widgetMainWindow->setCentralWidget(container);
    //widgetMainWindow->initializeOutput();

//    container->setAttribute(Qt::WA_OpaquePaintEvent); // Tentative to make disappear the black slash of vtk window
//    container->setAutoFillBackground(true);
//    container->setPalette(QPalette(Qt::blue));

    widgetMainWindow->show();
    return widgetMainWindow.data();


    {
    //qmlMainWindow->destroy();


//QDockWidget* dock=new QDockWidget(widgetMainWindow.data(),Qt::Widget);
//    dock->setWindowFlags(Qt::CustomizeWindowHint |
//                        Qt::Widget|
//                        Qt::WindowCloseButtonHint |
//                        Qt::WindowMinimizeButtonHint |
//                        Qt::WindowMaximizeButtonHint
//                        );
//    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea |Qt::TopDockWidgetArea);
//    dock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
//    widgetMainWindow->addDockWidget(Qt::TopDockWidgetArea, dock);
//    dock->setWidget(container);
//    dock->setParent(widgetMainWindow->centralWidget());
//    //dock->showMaximized();
//    dock->show();
//widgetMainWindow->setCentralWidget(dock);


//    container->mapToParent(QPoint(100,100));
//    QVBoxLayout* layout=new QVBoxLayout(widgetMainWindow->centralWidget());
//    layout->addWidget(container);
//    qWidget->setLayout(layout);


//    widgetMainWindow->setCentralWidget(container);
//    container->setLayout(new QGridLayout(widgetMainWindow->centralWidget()));
//    widgetMainWindow->centralWidget()->setLayout(container->layout());
   // container->hide();
    //container->show();
    //container->stackUnder(widgetMainWindow->centralWidget());
//     QVBoxLayout* layout=new QVBoxLayout(widgetMainWindow->centralWidget());
//     layout->addWidget(container,1,Qt::AlignVCenter | Qt::AlignHCenter);
//     widgetMainWindow->centralWidget()->setLayout(layout);



    //qmlMainWindow->destroy();
//    container->setMinimumSize(400,400);
//    container->setMaximumSize(800,800);



    //container->setVisible(false);

    //scaletag::window::MainWindow*
   // widgetMainWindow.reset(new scaletag::window::MainWindow());


//    QDockWidget *dock = new QDockWidget(("Visualization"),widgetMainWindow.data());
//   dock->setVisible(false);  // comment this line and a band appears
//    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea |Qt::TopDockWidgetArea);
//   dock->setWidget(container);


  //widgetMainWindow->addDockWidget(/*Qt::LeftDockWidgetArea*/Qt::TopDockWidgetArea, dock);
//widgetMainWindow->centralWidget()->layout()->addWidget(container);

//   QVBoxLayout* layout= new QVBoxLayout(container);
//   layout->addWidget(container);
//   container->setFocusProxy(widgetMainWindow->centralWidget());



   //container->setWindowFlags(Qt::WindowMinimizeButtonHint);

 //widgetMainWindow->menuBar()->setCornerWidget(dock, Qt::TopLeftCorner);
   //dock->setFloating(false);
   //dock->setWindowFlag(Qt::SubWindow);



//   QHBoxLayout* layout =new QHBoxLayout;
//   layout->addWidget(container);
//   layout->addWidget(widgetMainWindow->centralWidget(),1, Qt::AlignVCenter);
//   layout->addWidget(widgetMainWindow->statusBar(),1,Qt::AlignVCenter);

   //widgetMainWindow->statusBar()->setStyleSheet("QStatusBar{border-top: 1px outset grey;}");
    }



}

} // namespace controllers
} // namespace scaletag
