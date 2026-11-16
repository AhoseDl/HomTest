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

#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include<QObject>
#include<frames/heavytask.h>
#include<stlib_global.h>
#include<QQmlApplicationEngine>
#include<QQuickVTKRenderWindow.h>
#include<QQuickVTKRenderItem.h>
#include<QWindow>
#include<vtkNamedColors.h>
#include<vtkActor.h>
#include<vtkRenderer.h>
#include<vtkProperty.h>
#include<vtkPoints.h>
#include<vtkUnstructuredGrid.h>
#include<vtkPolyData.h>
#include<vtkPolyDataMapper.h>
#include<vtkDataSetMapper.h>
#include<vtkCompositePolyDataMapper.h>
#include<vtkRenderWindow.h>
#include<QQuickVTKInteractiveWidget.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<MeshPbc.h>
#include<vector>
#include<node.h>
#include<element.h>

#include<vtkScalarBarActor.h>
#include<vtkAxesActor.h>
#include<vtkOrientationMarkerWidget.h>
#include<vtkTextActor.h>
#include<vtkCaptionActor2D.h>

#include<visualization/scaletagviewport.h>

#include<QWeakPointer>
#include<QScopedPointer>
#include<vtkSmartPointer.h>
#include<QString>
#include<QQmlApplicationEngine>
#include<frames/project.h>
#include<QList>




#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkOCC_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <STEPControl_Reader.hxx>
#include<IGESControl_Reader.hxx>
#include<IVtkTools.hxx>
#include <IFSelect_ReturnStatus.hxx>  //For checking if file read succeeds
#include <IVtk_Types.hxx>
#include<IVtkTools_ShapeObject.hxx>
#include<BRepCheck_Analyzer.hxx>
#include<BRepMesh_IncrementalMesh.hxx>

#include<cad.h>
#include<memory.h>



namespace scaletag {

 namespace frames{ class Project;}

namespace visualization {

class ScaleTagViewPort;
class GenerateDataForVis;



class STLIB_EXPORT Visualization : public QObject
{
    Q_OBJECT
   QSharedPointer<QQmlApplicationEngine> engine;
    QSharedPointer<QObject> viewParent;
   QSharedPointer<scaletag::frames::Project> project;
    QList<visualization::ScaleTagViewPort*> viewPorts;
    //QSharedPointer<visualization::ScaleTagViewPort> currentViewPort;
    visualization::ScaleTagViewPort* currentViewPort;
    QSharedPointer<QQuickVTKRenderItem> qmlMainWindowRenderItem;

    int numberOfViewPorts;
     vtkNew<vtkNamedColors> scaleTagColors;

    vtkSmartPointer<vtkAxesActor> qmlAxes;
    vtkNew<vtkOrientationMarkerWidget> qmlOrientationMarker;
   vtkNew<vtkDataSetMapper> qmlMapper;
    vtkNew<vtkCompositePolyDataMapper> qmlCompMapper;
    vtkNew<vtkActor> qmlActor;
    vtkNew<vtkInteractorStyleTrackballCamera> qmlStyle;

    visualization::GenerateDataForVis* currentDataForVis;

    vtkNew<vtkScalarBarActor> act;


    std::unique_ptr<SelectionTool> selection;



public:
    explicit Visualization(QObject *parent = nullptr, QQmlApplicationEngine* engine=nullptr);

    ~Visualization();

    ScaleTagViewPort* createViewport(QObject* parent=nullptr);

    QList<visualization::ScaleTagViewPort*>& getViewPorts();

    /*QSharedPointer<*/visualization::ScaleTagViewPort*/*>*/ getCurrentViewPort();

    void setProject(frames::Project& _project);

    void setPipelineEnd(int mapperType);

    void update();



    void InitScaleTagColors();


    Q_INVOKABLE void init(const QString& filepath="");
    Q_INVOKABLE void initialize();
    Q_INVOKABLE void endSelection();

    Q_INVOKABLE void initSelection(IVtk_SelectionMode mode);

    vtkMapper* getQmlMapper(int mapperType=0);
    vtkRenderer* getRenderer();

    void renderView(vtkAlgorithm* algo, int mapperType=0);
    void renderView( frames::Part* _part);

    void renderCADWithShapeView(frames::Part* _part);
    void viewShapeActor(GenerateDataForVis* dataForVis);

    void setDataForVis(GenerateDataForVis* _dataForVis);

    GenerateDataForVis* getDataForVis();

    void viewResults();
    void viewFieldResults(const QString& fieldName, int comp, int timeRank/*=1*/, double scaleFactor/*=1.*/, bool showDeformed);

    SelectionTool* getSelection(){return selection.get();}


signals:
    void openFileClicked();
    void viewPortCreated(ScaleTagViewPort* viewPort);
    void currentViewPortChanged(ScaleTagViewPort* viewPort); //emitted when current viewport is changed

public slots:
    void setCurrentViewPort(visualization::ScaleTagViewPort* viewport);
    void deleteViewPort(int _numViewPort);

};


} // namespace visualization
} // namespace scaletag

#endif
