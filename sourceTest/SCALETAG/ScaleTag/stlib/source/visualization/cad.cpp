#include "cad.h"

//cad::cad()
//{

//}

//class HighLighter{

    //One may use                 vtkNew<IVtkTools_SubPolyDataFilter> subFilter;
    //subFilter->SetInputConnection(myDataSource->GetOutputPort());
    //subFilter->SetData(shapeId, subIds);

//
HighLighter::HighLighter(vtkRenderer* ren, vtkRenderWindow* renWin){hren=ren; hwin=renWin;}

vtkSmartPointer<vtkActor> HighLighter:: highLight(const TopoDS_Shape* _theShape, std::string vtkColorString) const//IVtkOCC_Shape*
    {

        //IVtkOCC_Shape* hVtkShape = new IVtkOCC_Shape(*_theShape);
        vtkNew<IVtkTools_ShapeDataSource> hSource;
        hSource->SetShape(new IVtkOCC_Shape(*_theShape));

        vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
        vtkSmartPointer<vtkActor> hActor = vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkPolyDataMapper> hMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

        hActor->SetMapper(hMapper);
        hren->AddActor(hActor);

        //hActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
        hActor->GetProperty()->SetColor(colors->GetColor3d(vtkColorString.c_str()).GetData());
        hActor->GetProperty()->SetLineWidth(2.0);
        hActor->GetProperty()->SetPointSize(6.0);
        // Slight offset so the highlight doesn't z-fight with the base surface
        hActor->GetProperty()->SetAmbient(1.0);
        hActor->GetProperty()->SetDiffuse(0.0);
        //hActor->GetProperty()->SetEdgeVisibility(true);


        hMapper->SetInputConnection(hSource->GetOutputPort());
//        IVtkTools::InitShapeMapper(hMapper);
//        hMapper->ScalarVisibilityOff();
        hMapper->SetResolveCoincidentTopologyToPolygonOffset();
        //hMapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(0, -2.0, -2.0); //Just for newer version of VTK(above 9.3 - mine)
        hMapper->SetRelativeCoincidentTopologyPolygonOffsetParameters(-2, -2.);
       // hMapper->SetScalarMo

        //hren->Render();
        //hwin->Render();

        return hActor;


    }

    //void setHilightMode(int _mode);

//private:

//    vtkRenderer* hren;
//    vtkRenderWindow* hwin;

//};


void SelectionTool::OnShapePicked(const TopoDS_Shape& shape, IVtk_SelectionMode mode)
{
    // e.g. highlight the face, emit a Qt signal to QML, etc.
   // emit faceSelected(/* whatever id/geom info you expose to QML */);

   // std::cout<<"A shape is selected."<<std::endl;
    HighLighter hlight(renderer, renderer->GetRenderWindow());
    //addSelectedActor( hlight.highLight(&shape));
    select(hlight.highLight(&shape)); //hlight.highLight(&shape) returns an actor
    //this->SetTopoIndex(); Getting selected index from TopoDS_Shape shape
}

void SelectionTool::OnPickCleared()
{
    //emit selectionCleared();

    //std::cout<<"All selections are cleared."<<std::endl;
    for(auto aActor:selectedActors) {
        renderer->RemoveActor(aActor);
    }
    selectedActors.clear();

}


SelectionTool::~SelectionTool()
{
    if (pickObserver) {
        pickObserver->RemoveListener(this);
        if (renderWindowInteractor) {
            renderWindowInteractor->RemoveObserver(pickObserver);
        }
    }
}


void ShapePickObserver::Initialize(vtkRenderWindowInteractor* interactor,
                                   vtkRenderer* renderer,
                                   const vtkSmartPointer<IVtkTools_ShapePicker>& picker,
  IVtk_SelectionMode _mode                                 )
{
    myRenderer = renderer;
    myPicker   = picker;
    renderWindowInteractor = interactor;

    //myPicker->SetSelectionMode(myMode);

    SetSelectionMode(_mode);

    renderWindowInteractor->AddObserver(vtkCommand::LeftButtonPressEvent, this, 1.0);
}

void ShapePickObserver::SetSelectionMode(IVtk_SelectionMode mode)
{
    myMode = mode;
    if (myPicker) {
        myPicker->SetSelectionMode(myMode);
    }
}

void ShapePickObserver::AddListener(IShapePickListener* listener)
{
    myListeners.push_back(listener);
}

void ShapePickObserver::RemoveListener(IShapePickListener* listener)
{
    myListeners.erase(
        std::remove(myListeners.begin(), myListeners.end(), listener),
        myListeners.end());
}

void ShapePickObserver::Execute(vtkObject* caller, unsigned long eventId, void*)
{
    if (eventId != vtkCommand::LeftButtonPressEvent) {
        return;
    }
    auto* interactor = static_cast<vtkRenderWindowInteractor*>(caller);

    HandleLeftButtonPress(interactor);  //HandleLeftButtonPress() return the vtkActor which wrap the subshape selected
}

vtkActor* ShapePickObserver::HandleLeftButtonPress(vtkRenderWindowInteractor* interactor)
{
    if (!myPicker || !myRenderer) {
        return nullptr;
    }

    int x, y;
    interactor->GetEventPosition(x, y);

    // For performing the actual VTK pick against the renderer
    myPicker->Pick(static_cast<double>(x), static_cast<double>(y), 0.0, myRenderer);

    IVtk_ShapeIdList pickedIds = myPicker->GetPickedShapesIds();

        if (pickedIds.IsEmpty()) {
            for (auto* listener : myListeners) {
            if(listener) {listener->OnPickCleared(); /*listener->SetCurrentIndex(0);*/}
                //std::cout<<"listener "<<listener<<" Picked ids is empty "<<std::endl;
                //std::cout<<"x : "<<x<<" y : "<<y<<std::endl;
            }
            //return;
        }

    vtkSmartPointer<vtkActorCollection> pickedActors = myPicker->GetPickedActors();
    if (!pickedActors || pickedActors->GetNumberOfItems() == 0) {
        //std::cout<<"Any actor is picked"<<std::endl;
        return nullptr; // nothing hit — already cleared above
    }

    pickedActors->InitTraversal();
    vtkActor* actor = pickedActors->GetNextActor();
    if (!actor) return nullptr;

    //std::cout<<"Actor picked "<<std::endl;
    // Recover the OCC shape wrapper attached to this actor's data source
    IVtkTools_ShapeDataSource* dataSource = IVtkTools_ShapeObject::GetShapeSource(actor);
    if (!dataSource) return nullptr;

    Handle(IVtkOCC_Shape) occShape = dataSource->GetShape();
    if (occShape.IsNull()) return nullptr;

    // Now resolve which sub-shape (face, in SM_Face mode) was actually picked
    IVtk_IdType shapeId = occShape->GetId();
    IVtk_ShapeIdList subIds = myPicker->GetPickedSubShapesIds(shapeId);




    TopoDS_Shape resultShape;
    if (!subIds.IsEmpty()) {
        IVtk_IdType subId = subIds.First();

        std::cout<<"subId2 : "<<subId<<std::endl;

        resultShape = occShape->GetSubShape(subId);
    } else {
        resultShape = occShape->GetShape();
    }


    for (auto* listener : myListeners) {
        {listener->OnShapePicked(resultShape, myMode); /*listener->SetCurrentIndex(subId);*/}
    }

//    HighLighter hlight(myRenderer, myRenderer->GetRenderWindow());
//    return hlight.highLight(&resultShape);
}

void SelectionTool::SetSelectionMode(IVtk_SelectionMode _mode){
   myMode=_mode;
    pickObserver->SetSelectionMode(_mode);
}

