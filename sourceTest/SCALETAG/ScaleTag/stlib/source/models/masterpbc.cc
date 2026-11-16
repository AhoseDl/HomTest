#include<masterpbc.h>
#include<frames/objectFactory.h>

namespace scaletag { namespace models {

MasterPBC::MasterPBC(){}

MasterPBC::MasterPBC(frames::ObjectFactory* objectFactory, QObject* parent) : QObject(parent), objectFactory(objectFactory){
    this->objectFactory = objectFactory;
    selectionTool = /*new SelectionTool();*/ objectFactory->getVisualization()->getSelection();
   // pbcMeshTool = new fem::PBCMeshTool();

//    selectionTool->initS (SM_Face);

}

void MasterPBC::initialize(frames::ObjectFactory* objectFactory, QObject* parent){
    this->objectFactory = objectFactory;
    selectionTool = objectFactory->getVisualization()->getSelection();
}

MasterPBC::~MasterPBC(){
//    if(selectionTool)     delete selectionTool;
//    if(pbcMeshTool) delete pbcMeshTool;
}

void MasterPBC::highLightAllMappedFaces(){
    //selectionTool->setSelectionType(1); //Will help colorize mapped faces(in MaterPBC)

    IVtkOCC_Shape::Handle theShape = selectionTool->getShapeDataSource()->GetShape();
    for(auto& aMappedFace : mappedFaces){

        //selectionTool->Initialize();

        selectionTool->highLight(&theShape->GetSubShape(aMappedFace.first), "Blue");
        selectionTool->highLight(&theShape->GetSubShape(aMappedFace.second), "Green");
        //objectFactory->getVisualization()->getRenderer()->Render();

    }


}


void MasterPBC::setMappedFace(IVtk_IdType faceId1, IVtk_IdType faceId2){}

/*std::unordered_map<IVtk_IdType, IVtk_IdType>*/std::vector<std::pair<IVtk_IdType, IVtk_IdType>> MasterPBC::getMappedFaces(){return mappedFaces;}

void MasterPBC::clearAllMappedFaces(){
    mappedFaces.clear();
    selectionTool->removeMappedActors();
    std::cout<<"All the faces mapped are cleared."<<std::endl;
}



//void MasterPBC::setSurfaceId(){

//    /* Some helpful functions*/
//    auto putFacesPair = [this](IVtk_IdType faceId){

//        auto putFirstId = [this](IVtk_IdType faceId){ if (this->checkFaceId(faceId)) {this->facesPair.first=faceId;} };

//        auto putSecondId = [this](IVtk_IdType faceId){ if (this->checkFaceId(faceId)) {
//                this->facesPair.second=faceId;
////                this->mappedFaces.insert(facesPair);
////                std::cout<<"Surfaces pair : <"<<facesPair.first<<" "<<facesPair.second<<"> is added."<<std::endl;
////                this->facesPair={0,0};
////                this->idx=0;
//            }};


//        if(this->idx==0) {std::cout<</*"Face id : "<<faceId<<" is added."<<"\n"
//                      <<*/"Select the second face."<<std::endl;}

//        if(!( this->checkFaceId(faceId)) ){std::cout<<"Face is already selected"<<std::endl; return;}

//        if(this->idx==0){
//            putFirstId(faceId);

//            if (this->facesPair.first == faceId) {
//                this->idx = 1;
//                std::cout << "Face id : " << this->facesPair.first << " is added.\n"
//                          << "Select the second face." << std::endl;
//            } else {
//                std::cout << "Failed to add Face id: " << faceId << std::endl;
//            }
//            return;}

//        if(this->idx==1){
//            if(faceId == facesPair.first) {std::cout<<"A face cannot be mapped to itself."<<std::endl; return;}
//            putSecondId(faceId);
//            //this->mappedFaces.insert(facesPair);
//            this->mappedFaces.push_back(facesPair);
//            std::cout<<"Surfaces pair : <"<<facesPair.first<<" "<<facesPair.second<<"> is added."<<std::endl;
//            this->idx=0; this->facesPair={0,0};
//            highLightAllMappedFaces();

//            std::cout<<"Select the first face."<<std::endl;

//        }
//    };
//    /* End of helpful functions */

//    scaletag::visualization::Visualization* visualization = objectFactory->getVisualization();
//    IVtkTools_ShapeDataSource* shapeSource =  objectFactory->getProject()->getCurrentModel()->getCurrentPart()->getVISSource();
//    selectionTool->setShapeDataSource(shapeSource);

//    if(shapeSource==nullptr){std::cout<<"The shape is not defined."<<std::endl; return;}
//    if(shapeSource->GetShape().IsNull()){std::cout<<"The shape's CAD is not defined."<<std::endl; return;}

//    IVtk_IdType id = shapeSource->GetShape()->GetId();
////    IVtk_IdType subId = shapeSource->GetShape()->GetSubIds(id).First();
//   // IVtk_ShapeIdList subIds = shapeSource->GetShape()->GetSubIds(id);
////    if(subIds.IsEmpty()) {std::cout<<" No sub-shape selected."<<std::endl; return ;}
////   std::cout<<"Shape selected id : "<<id<<std::endl;

//   //To get the id selected, one may use selectionTool in MasterPBC.


//    if(selectionTool){IVtk_IdType subId =  selectionTool->getCurrentSubId();
//    std::cout<<" subId : "<<subId<<std::endl;
//        putFacesPair(subId);

//     //   Put subId in " std::map<IVtk_IdType, IVtk_IdType> mappedFaces;" logic;
//    // A valid subId to add was not previously added

//    }

//}



void MasterPBC::initSelection(IVtk_SelectionMode mode){

    //selectionTool = std::make_unique<SelectionTool>().data();
    auto selection = selectionTool;
    //        selection->In
    auto* vis = objectFactory->getVisualization();
    auto* ren = vis->getRenderer();
    auto* iren = vis->getRenderer()->GetRenderWindow()->GetInteractor();
    vtkSmartPointer<IVtkTools_ShapePicker> picker = vtkSmartPointer<IVtkTools_ShapePicker>::New();

    if(ren==nullptr || iren==nullptr ||picker==nullptr) {
        std::cout<<"Problem in the view configuration "<<std::endl;
        return;
    }
    selection->Initialize(iren, ren, picker, mode);
    std::cout<<"Selectiion mode has been initialized"<<std::endl;

}

void MasterPBC::setSurfaceId(){

    /* Some helpful functions*/
    auto putFacesPair = [this](IVtk_IdType faceId){

        auto putFirstId = [this](IVtk_IdType faceId){ if (this->checkFaceId(faceId)) {this->facesPair.first=faceId;} };

        auto putSecondId = [this](IVtk_IdType faceId){ if (this->checkFaceId(faceId)) {
                this->facesPair.second=faceId;
//                this->mappedFaces.insert(facesPair);
//                std::cout<<"Surfaces pair : <"<<facesPair.first<<" "<<facesPair.second<<"> is added."<<std::endl;
//                this->facesPair={0,0};
//                this->idx=0;
            }};


        if(this->idx==0) {
            std::cout<<"Face id : "<<faceId<<" is added."<<std::endl;
            std::cout<<"Select the second face."<<std::endl;
        }

        if(!( this->checkFaceId(faceId)) ){std::cout<<"Face is already selected"<<std::endl; return;}

        if(this->idx==0){
            putFirstId(faceId); this->idx=1;
            //            std::cout<<"Face id : "<<this->facesPair.first<<" is added."<<"\n"
            //                      <<"Select the second face."<<std::endl;
            return;}

        if(this->idx==1){
            if(faceId == facesPair.first) {std::cout<<"A face cannot be mapped to itself."<<std::endl; return;}
            putSecondId(faceId);
            //this->mappedFaces.insert(facesPair);
            this->mappedFaces.push_back(facesPair);
            std::cout<<"Surfaces pair : <"<<facesPair.first<<" "<<facesPair.second<<"> is added."<<std::endl;
            this->idx=0; this->facesPair={0,0};
            highLightAllMappedFaces();

            std::cout<<"Select the first face."<<std::endl;

        }
    };
    /* End of helpful functions */

    scaletag::visualization::Visualization* visualization = objectFactory->getVisualization();
    IVtkTools_ShapeDataSource* shapeSource =  objectFactory->getProject()->getCurrentModel()->getCurrentPart()->getVISSource();
    selectionTool->setShapeDataSource(shapeSource);

    if(shapeSource==nullptr){std::cout<<"The shape is not defined."<<std::endl; return;}
    if(shapeSource->GetShape().IsNull()){std::cout<<"The shape's CAD is not defined."<<std::endl; return;}

    IVtk_IdType id = shapeSource->GetShape()->GetId();
    //    IVtk_IdType subId = shapeSource->GetShape()->GetSubIds(id).First();
    // IVtk_ShapeIdList subIds = shapeSource->GetShape()->GetSubIds(id);
    //    if(subIds.IsEmpty()) {std::cout<<" No sub-shape selected."<<std::endl; return ;}
    //   std::cout<<"Shape selected id : "<<id<<std::endl;

    //To get the id selected, one may use selectionTool in MasterPBC.


    if(selectionTool){IVtk_IdType subId =  selectionTool->getCurrentSubId();
        std::cout<<" subId : "<<subId<<std::endl;
        putFacesPair(subId);

        //   Put subId in " std::map<IVtk_IdType, IVtk_IdType> mappedFaces;" logic;
        // A valid subId to add was not previously added

    }

}


/**** For the widget ****/
PBCConstraintsManagerWidget::PBCConstraintsManagerWidget(QWidget *parent) : QWidget(parent, Qt::Tool){
    createContent();
}

void PBCConstraintsManagerWidget::createContent(){
    validate = new QPushButton("Ok");
    quit = new QPushButton("Quit");
    clearAll = new QPushButton("Clear all");
    hlay = new QHBoxLayout();
    contentBox = new QGroupBox( tr("Please select the faces to be constrained:") );

    hlay->addWidget(validate);
    hlay->addWidget(quit);
    hlay->addWidget(clearAll);
    this->setLayout(hlay);
    this->setWindowTitle("Select surfaces for PBC constraints.");
    this->show();
    this->raise();
    this->activateWindow();

    QObject::connect(validate, &QPushButton::clicked, this, &PBCConstraintsManagerWidget::aSurfaceIsSelected);
    QObject::connect(clearAll, &QPushButton::clicked, this, &PBCConstraintsManagerWidget::clearAllFired);
    QObject::connect(quit, &QPushButton::clicked, this, [this]{
        emit closeWidget(); this->close();});
    //ObjectFactory responds to closeWidget by ending Selection


}



}}
