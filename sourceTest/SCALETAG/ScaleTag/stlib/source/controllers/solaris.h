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
/**
 * This class is intended to orchestrate any object injection
 * in engine and for the qml and widget(ui) initialization
 */

#ifndef SOLARIS_H
#define SOLARIS_H

#include <QObject>
#include<QScopedPointer>

#include<frames/objectFactory.h>
#include<frames/iobjectFactory.h>
#include<QQmlApplicationEngine>
#include<QScopedPointer>

/*
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkOCC_Shape.hxx>
#include <TopoDS_Face.hxx>
 #include <STEPControl_Reader.hxx>
#include<IVtkTools.hxx>
 #include <IFSelect_ReturnStatus.hxx>  //For checking if file read succeeds
 #include <IVtk_Types.hxx>
#include<IVtkTools_ShapeObject.hxx>
#include <BRepBuilderAPI_Copy.hxx>
 #include <BRepPrimAPI_MakeBox.hxx>
#include<BRepCheck_Analyzer.hxx>

#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Dir.hxx>
#include <gp_Trsf.hxx>

#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>

#include <TopExp_Explorer.hxx>
#include <TopTools_ListOfShape.hxx>
#include <BRepTools.hxx>
 #include <Geom_Plane.hxx>
#include<Geom2d_Line.hxx>
#include<Geom2d_TrimmedCurve.hxx>
#include<Geom2d_TrimmedCurve.hxx>
#include<Geom_CylindricalSurface.hxx>*/

namespace scaletag {
namespace controllers {

class Solaris : public QObject
{
    Q_OBJECT
    Q_PROPERTY(scaletag::frames::IObjectFactory* ui_ObjectFactory READ getObjectFactory CONSTANT)
public:
    explicit Solaris(/*frames::ObjectFactory* objectFactory=nullptr, */QQmlApplicationEngine* engine=nullptr);


//        void setObjectFactory(frames::IObjectFactory* _objectFactory) {objectFactory.reset(_objectFactory);
//        }

      Q_INVOKABLE frames::IObjectFactory* getObjectFactory() const;
    //void initialize(); //initiate user interface
      Q_INVOKABLE void initObjectFactory(){
          //QWindow* win=reinterpret_cast<QWindow*>(engine->rootObjects()[0]);
        objectFactory.reset(new frames::ObjectFactory(engine.data())); }



      void testOcc(){

        STEPControl_Reader reader;
       // IFSelect_ReturnStatus status = reader.ReadFile("/home/ahose/Desktop/GMSH/Diabolo/build/diabolo.step");
        //IFSelect_ReturnStatus status = reader.ReadFile("/home/ahose/Desktop/GMSH/Haltere/build/HalterePlein.step");
        IFSelect_ReturnStatus status = reader.ReadFile("/home/ahose/Desktop/GMSH/KelvinCellGmsh/hollow/build/kelvin.step");
        if (status != IFSelect_RetDone) {
            std::cerr << "Failed to read STEP file" << std::endl;
            //return 1;
            return;
        }
        reader.TransferRoots();
        TopoDS_Shape cadShape = reader.OneShape();


        if (cadShape.IsNull())
            return;

        BRepCheck_Analyzer analyzer(cadShape);
        if (!analyzer.IsValid())
        {
            // bad TopoDS_Shape
            return;
        }

        // 2. Wrap into a VIS source that remembers CAD topology
        vtkNew<IVtkTools_ShapeDataSource> dataSource;
        dataSource->SetShape(new IVtkOCC_Shape(cadShape));

        vtkNew<vtkPolyDataMapper> mapper;

       mapper->SetInputConnection(dataSource->GetOutputPort());

        IVtkTools::InitShapeMapper(mapper); //Initializes the mapper using standard/default OCCT colors

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);
        IVtkTools_ShapeObject::SetShapeSource(dataSource, actor); //This ensures that the Open CASCADE shape source is properly stored inside the VTK actor or dataset's internal information object using a unique key, allowing selection tools like IVtkTools_ShapePicker to find it later


        vtkNew<vtkNamedColors> colors;
        vtkNew<vtkRenderer> renderer;
        renderer->AddActor(actor);
        renderer->SetBackground(colors->GetColor3d("MidnightBlue"/*"MistyRose"*//*"Salmon*"*/).GetData());
        vtkNew<vtkRenderWindow> renwin;
        renwin->AddRenderer(renderer);
        renwin->SetSize(800,800);
        vtkNew<vtkRenderWindowInteractor> iren;
        vtkNew<vtkInteractorStyleTrackballCamera> style;
        iren->SetInteractorStyle(style);
        iren->SetRenderWindow(renwin);

        renwin->Render();
        iren->Start();

      }

  private:
    QSharedPointer<scaletag::frames::IObjectFactory> objectFactory;
    QSharedPointer<QQmlApplicationEngine> engine;



signals:

};

} // namespace controllers
} // namespace scaletag

#endif // SOLARIS_H
