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


#ifndef SCALETAGVIEWPORT_H
#define SCALETAGVIEWPORT_H

#include<QObject>
#include<QWindow>
#include<stlib_global.h>
#include<QQmlApplicationEngine>
#include<QQuickVTKRenderWindow.h>
#include<QQuickVTKRenderItem.h>
//#include<QQuickWindow>
#include<vtkNamedColors.h>
#include<vtkActor.h>
#include<vtkAxesActor.h>
#include<vtkRenderer.h>
#include<vtkProperty.h>
#include<vtkPoints.h>
#include<vtkUnstructuredGrid.h>
#include<vtkPolyData.h>
#include<vtkPolyDataMapper.h>
#include<vtkDataSetMapper.h>
#include<vtkRenderWindow.h>
#include<QQuickVTKInteractiveWidget.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<MeshPbc.h>
#include<vector>
#include<node.h>
#include<element.h>
//#include<visualization/visualization.h>

#include <QSharedPointer>

#include<vtkAxesActor.h>
#include<vtkOrientationMarkerWidget.h>
#include<vtkTextActor.h>
#include<vtkCaptionActor2D.h>

#include<QWeakPointer>
#include<QScopedPointer>
#include<vtkSmartPointer.h>
#include<QString>
#include<QQmlApplicationEngine>
#include<frames/project.h>
#include<QList>
#include<frames/model.h>
#include<visualization/generatedataforvis.h>
#include<QWidget>
 #include <QGuiApplication>

namespace scaletag {
namespace visualization {

/* When a viewport is created, the current model is immediately displayed in it
 * When a subwindow in mainWindow Mdi area (coming from a viewport) ::aboutToShow(), the viewport is made current.
*/
class GenerateDataForVis;

class STLIB_EXPORT ScaleTagViewPort : public QObject
{
    Q_OBJECT
    //QSharedPointer<QQmlApplicationEngine> engine;
    QQmlApplicationEngine* engine;
   QString viewUrl = "qrc:/views/Vtk.qml" ;
    QScopedPointer<QWidget> viewPortWidget;   //

    /*QWidget::createWindowContainer() +
Know how to get container from widget and widget
from container
subwindow = mdiArea->addSubWidget(viewPortWidget);
subwindow::shubWindowActivated() -> scaleTagViewPort is current viewport
*/
   QString name = "ViewPort-ScaleTag";


   vtkSmartPointer<vtkRenderer> ren;
   QScopedPointer<QWindow> win;
   vtkSmartPointer<vtkOrientationMarkerWidget> orientationMarker;
   vtkSmartPointer<vtkAxesActor> axesActor;
   QSharedPointer<frames::Model> model;
   //vtkSmartPointer<visualization::GenerateDataForVis> dataForVis;
   //QSharedPointer<visualization::GenerateDataForVis> dataForVis;
   visualization::GenerateDataForVis* dataForVis;

   int numViewport;
   //static int numberOfViewPort;

   QQuickVTKRenderItem *qquickvtkItem;
   vtkNew<vtkDataSetMapper> meshMapper;
   vtkNew<vtkCompositePolyDataMapper> shapeMapper;
   vtkNew<vtkActor> meshActor;
   vtkNew<vtkActor> shapeActor;


public:
    explicit ScaleTagViewPort(QObject *parent = nullptr, QQmlApplicationEngine* engine=nullptr);
    ~ScaleTagViewPort();
    vtkRenderer* getRenderer();
    QWindow* getWindow();

    void addOrientationGizmo();

    void setNumViewPort(int _num);
    int getNumViewPort();


   /* QSharedPointer<*/visualization::GenerateDataForVis*/*>*/ getDataForVis();
    void setDataForVis(visualization::GenerateDataForVis* _dataForVis);

signals:
    void selected(ScaleTagViewPort* _scaleTagViewPort);
    void viewPortDeleted(int _numViewPort);

public slots:
    void setCurrentModel(frames::Model* _model);

    void onWindowDeleted(QObject* _window);

    //void deleteViewPort(int viewportNum);

    void view();


};

} // namespace visualization
} // namespace scaletag

#endif
