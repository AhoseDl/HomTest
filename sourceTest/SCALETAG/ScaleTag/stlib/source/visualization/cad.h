#ifndef CAD_H
#define CAD_H

#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkOCC_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <STEPControl_Reader.hxx>
#include<IVtkTools.hxx>
#include <IFSelect_ReturnStatus.hxx>  //For checking if file read succeeds
#include <IVtk_Types.hxx>


#include<vtkCommand.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkActorCollection.h>
#include<IVtkTools_ShapeObject.hxx>

#include<QObject>



#include<vtkPolyDataMapper.h>
#include<vtkActor.h>
#include<vtkProperty.h>
#include<vtkNamedColors.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>

#include<QDebug>

class HighLighter{

    //One may use                 vtkNew<IVtkTools_SubPolyDataFilter> subFilter;
    //subFilter->SetInputConnection(myDataSource->GetOutputPort());
    //subFilter->SetData(shapeId, subIds);

public:

    HighLighter(vtkRenderer* ren, vtkRenderWindow* renwin);
    HighLighter(){}
    void initialize(vtkRenderer* ren, vtkRenderWindow* renwin){hren=ren; hwin=renwin;}
    vtkSmartPointer<vtkActor> highLight(const TopoDS_Shape* _theShape, std::string vtkColorString="Red") const ;//IVtkOCC_Shape*;

    //void setHilightMode(int _mode);

private:

    vtkRenderer* hren;
    vtkRenderWindow* hwin;

};

class cad
{
public:
    cad();
};

class IShapePickListener
{
    /*Objects which respond to pick event might child of this class*/
public:

    virtual void OnShapePicked(const TopoDS_Shape& shape,
                               IVtk_SelectionMode mode) = 0;
    virtual void OnPickCleared() = 0;

    virtual ~IShapePickListener() = default;

//    void SetCurrentIndex(IVtk_IdType _currentIndex){currentIndex=_currentIndex ;}
//    IVtk_IdType getCurrentIndex(){ return currentIndex ;}

private:
   // IVtk_IdType currentIndex = 0; // 1-based index; 0 -> noting selected

};




class ShapePickObserver : public vtkCommand
{
public:
    static ShapePickObserver* New() { return new ShapePickObserver(); }
    vtkTypeMacro(ShapePickObserver, vtkCommand);

    void Initialize(vtkRenderWindowInteractor* interactor,
                    vtkRenderer* renderer,
                    const vtkSmartPointer<IVtkTools_ShapePicker>& picker,
                    IVtk_SelectionMode mode);

    void AddListener(IShapePickListener* listener);
    void RemoveListener(IShapePickListener* listener);

    void SetSelectionMode(IVtk_SelectionMode mode);

    void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;

private:
    ShapePickObserver() = default;

    vtkActor* HandleLeftButtonPress(vtkRenderWindowInteractor* interactor);

    vtkSmartPointer<IVtkTools_ShapePicker> myPicker;
    vtkRenderer*                           myRenderer = nullptr;
    IVtk_SelectionMode                     myMode = SM_Face;
    std::vector<IShapePickListener*>       myListeners;
    vtkRenderWindowInteractor* renderWindowInteractor;
};



class SelectionTool : public IShapePickListener{

public:

   // SelectionTool() = default;
    SelectionTool(){}
    ~SelectionTool();


    void OnShapePicked(const TopoDS_Shape& shape, IVtk_SelectionMode mode);
    void OnPickCleared();
    void SetSelectionMode(IVtk_SelectionMode mode);

    void Initialize(vtkRenderWindowInteractor* _interactor, vtkRenderer* _renderer,
                    const vtkSmartPointer<IVtkTools_ShapePicker>& _picker,
IVtk_SelectionMode _mode=SM_Face){
      //  vtkSmartPointer<IVtkTools_ShapePicker> picker = vtkSmartPointer<IVtkTools_ShapePicker>::New();

        renderer = _renderer;
        renderWindowInteractor = _interactor;
        picker = _picker;
        myMode = _mode;

        picker->SetRenderer(renderer);  // your existing vtkRenderer

        pickObserver = vtkSmartPointer<ShapePickObserver>::New();
        //SetSelectionMode(_mode);
        pickObserver->SetSelectionMode(_mode);   // surface picking
        pickObserver->AddListener(this);
        pickObserver->Initialize(renderWindowInteractor, renderer, picker, _mode);
        std::cout<<"Under addListener(this) "<<std::endl;

        highlight.initialize(renderer, renderer->GetRenderWindow());
    }

    void select(vtkActor* actor, int _selectionType=0){
        setSelectionType(selectionType);
        if(selectionType==0){
            //Select single entities and keep them as actors in selectedActors;
            for(auto aActor:selectedActors) {renderer->RemoveActor(aActor);}
            selectedActors.clear();
            addSelectedActor(actor);
        } else
//            if(selectionType==1)
//        {
//            //Use to color mapped faces. Keep entities actors in mappedFacesActors;
//            for(auto aActor : mappedFacesActors){renderer->RemoveActor(aActor);}
//            mappedFacesActors.clear();
//            addMappedActor(actor);
//        }
//        else
        {
        addSelectedActor(actor);
        }

    }

    void addSelectedActor(vtkActor* actor){selectedActors.push_back(actor);}

    void addMappedActor(vtkActor* actor){mappedFacesActors.push_back(actor);}

    void removeMappedActors(){
        for(auto anActor : mappedFacesActors)
        renderer->RemoveActor(anActor);
//        for(auto anActor : selectedActors)
//        renderer->RemoveActor(anActor);
        //renderWindowInteractor->GetRenderWindow()->Render();
    }
    void setSelectionType(int _selectionType=0){selectionType=_selectionType;}

    IVtk_IdType getCurrentSubId(){

        IVtk_IdType pId=0, subId=0;

//        if (picker){

//        if(picker->GetPickedShapesIds().IsEmpty()) {std::cout<<"Select a Face."<<std::endl; return 0;}
//        if(!(picker->GetPickedActors())){std::cout<<"Issue with actor"<<std::endl; return 0;}
//         //pId = picker->GetPickedShapesIds().First();  //


//        vtkSmartPointer<vtkActorCollection> pickedActors = picker->GetPickedActors();
//        if (!pickedActors || pickedActors->GetNumberOfItems() == 0) {
//            std::cout<<"No actor exists."<<std::endl;  return 0;
//        }
//        picker->GetPickedActors()->InitTraversal();
//        std::cout<<"actors size : "<<pickedActors->GetNumberOfItems()<<std::endl;
//        vtkActor* actor = pickedActors->GetNextActor();

//        if (!actor) {std::cout<<"actor is null."<<std::endl; return 0; }
//        IVtkTools_ShapeDataSource* dataSource = IVtkTools_ShapeObject::GetShapeSource(actor);
//        if (!dataSource) {std::cout<<"dataSource is null"<<std::endl; return 0;}


        IVtkTools_ShapeDataSource* dataSource = getShapeDataSource();
        Handle(IVtkOCC_Shape) occShape = dataSource->GetShape();
        if (occShape.IsNull()){std::cout<<"Shape is not defined."<<std::endl; return 0;}

        pId = occShape->GetId();
         IVtk_ShapeIdList subIds = picker->GetPickedSubShapesIds(pId);

        if (!subIds.IsEmpty()) {
            subId = subIds.First();
        } else {
            subId = pId;
        }
    //}
        return subId;
    }



    void setShapeDataSource(IVtkTools_ShapeDataSource* _shapeDataSource){
        shapeDatasource=_shapeDataSource;
    }

    IVtkTools_ShapeDataSource* getShapeDataSource(){return shapeDatasource;}

    vtkActor* highLight(const TopoDS_Shape* _theShape, std::string vtkColorString)  {
        mappedFacesActors.push_back( highlight.highLight(_theShape,vtkColorString) );
    }

private:
    vtkSmartPointer<ShapePickObserver> pickObserver;
    vtkSmartPointer<IVtkTools_ShapePicker> picker;
    vtkRenderer*                           renderer = nullptr;
    IVtk_SelectionMode                     myMode = SM_Face;
    int selectionType                             = 0; //1->multi selection; 0->uni selection

    vtkRenderWindowInteractor* renderWindowInteractor;

    std::vector<vtkActor*> selectedActors;
    std::vector<vtkActor*> mappedFacesActors;

    IVtkTools_ShapeDataSource* shapeDatasource;



    //signals:
    //   void faceSelected(/* ... */);
    //    void selectionCleared();

    HighLighter highlight;


};





#endif // CAD_H
