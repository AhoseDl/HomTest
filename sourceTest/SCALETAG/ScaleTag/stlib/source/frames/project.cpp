#include<frames/project.h>
#include<frames/heavytask.h>

#include<QDebug>
namespace scaletag {
namespace frames {
int Project::numModel=0;
Project::Project(QObject *parent)
    : QObject{parent}
{
    //QObject::connect(this, &Project::modelNameChanged, &Project::update);
    models.reset(new QList<Model*>());
    models->append(new Model(this));
    currentModel = models->at(0);
    //QObject::connect(this, &Project::currentIndexChanged, this, &Project::setCurrentIndex);
    //int frames::Part::numPart;
    numModel++;
    modelsNamesList<<models->at(0)->getName();

//    QObject::connect(this, &Project::currentModelChanged, [&]{setCurrentIndex();});


}


void Project::setCurrentModel(const QString& currentModelName){
    currentModel = models->at(0);
    QList<Model*>::Iterator it = models->begin();
    while( ((*it)->getName()!=currentModelName) && it!=models->end() ) {it ++; }

    if(it!=models->end())currentModel = *it;
    update();
}

QList<Model*> Project::getModelList() const {return *models.data();}

QStringList Project::getModelsNamesList() const{

    QStringList namesLists;

    //return QQmlListProperty<QString>(this, namesLists);

    QList<Model*>::iterator it;
    for(it=(*models).begin(); it != (*models).end(); it++) namesLists << ((*it)->getName());
    return  namesLists;
}

Model* Project::getCurrentModel(){return currentModel;}

Model* Project::getModel(const QString& name){

    for (int i=0; i<models->size(); i++)
        if(models->at(i)->getName() == name){
            return models->at(i);
        }
    return nullptr;
}

QString Project::getCurrentModelName() const {return currentModel->getName();}
QString Project::getCurrentModelName() {return currentModel->getName();}

QString Project::setCurrentModelName(const QString& _name) {QString oldName=currentModel->getName();
    currentModel->setName(_name);
    return oldName;}



void Project::setModelsNamesList(const QStringList& _stringList) {
    modelsNamesList=_stringList;
}

qsizetype Project::getCurrentIndex() const {return currentModelIndex;}

void Project::setCurrentIndex(qsizetype _index){

    // if(currentModelIndex != _index){
    QString prevName = models->at(currentModelIndex)->getName();
    QString currName = models->at(_index)->getName();

    std::cout<<"prevName : "<<prevName.toStdString()<<std::endl
              <<"currName : "<<currName.toStdString()
              <<std::endl;

    if(prevName != currName){

    currentModelIndex = _index;
    setCurrentModel(models->at(_index)->getName());
    emit currentModelChanged(/*_index*//*currentModel*/models->at(_index));
    }

}


/****** For Slots ***************/

void Project::projectModify(const QString& filename){
    std::cout<<"Project modified with "<<filename.toStdString()<<std::endl;
    currentModel->modify(filename);
    emit currentModelModified(currentModel);

    //emit currentModelChanged(currentModel);

}

void Project::projectCADModify(const QString& filename){
    //modify() must be reimplemented in model
    /*qDebug()*/std::cout<<"Project CAD modified with : " <<filename.toStdString()<<std::endl;
    currentModel->modifyCAD(filename);

    //emit currentModelModified(currentModel);
    //emit currentModelChanged(currentModel);
}

void Project::projectCADWithShapeModify(const QString& filename){
    std::cout<<"Project CAD modified with : " <<filename.toStdString()<<std::endl;
    currentModel->modifyCADWithShape(filename);
    emit currentModelChanged(currentModel);
}


Model* Project::createModel(const QString& _name){
    QString modelName =_name;
    if(modelName.isEmpty()) modelName=QString((std::string("Model-")+ std::to_string(/*models->size()+1*/++numModel)).c_str());
    Model* model=new Model(this);
    model->setName( modelName);
    models->append(model);
    setCurrentIndex(models->size()-1);
    currentModel=model;

    //QObject::connect(currentModel, &Model::needTreeViewUpdate, [](){std::cout<<" Quick check"<<std::endl;});

    emit currentModelChanged(/*_index*/currentModel);
    emit modelListChanged();
    currentModel->emitNeedTreeViewUpdateSignal();
    //QMetaObject::invokeMethod(getCurrentModel(), &scaletag::frames::Model::needTreeViewUpdate, Qt::QueuedConnection);
    return model;

}

Model* Project::createNoNameModel(){
    createModel("");
    return nullptr;
}

void Project::renameModel(const QString& fromName, const QString& toName){

    for (int i=0; i<models->size(); i++){
        QString modName=models->at(i)->getName();
        if(modName==toName){cout<<"The model"<<toName.toStdString()
                 <<" already exists. Please choose another name."<<std::endl; return;
        }
    }
    for (int i=0; i<models->size(); i++){
        QString modName=models->at(i)->getName();
        if(modName==fromName){
            models->at(i)->setName(toName);
            //emit currentModelChanged(/*_index*/);
            emit currentModelChanged(currentModel);
            emit modelNameChanged(i);
            emit modelListChanged();
        }
    }

}

void Project::renameMaterial(const QString& fromName, const QString& toName){
    std::cout<<"Renaming material "<<fromName.toStdString()<<std::endl;
    for (auto matName : getCurrentModel()->getMaterialsNamesList()){
                if(matName == toName){cout<<"The material"<<toName.toStdString()
                 <<" already exists. Please choose another name."<<std::endl; return;
        }
    }
    int i=0;
    for (auto& matName : getCurrentModel()->getMaterialsNamesList()){
        if(matName != fromName){
            i++;
            continue;
        }

        if( (qsizetype)i == getCurrentModel()->getMaterialsNamesList().size()){std::cout<<"The material "<<fromName.toStdString()
                      <<" does not exist"; return;}

        /*if(i !=0 )*/getCurrentModel()->getMaterials()[i].setName(toName.toStdString());
        //if(i==0) getCurrentModel()->getMaterials()[0].setName(toName.toStdString());

        //    emit currentModelChanged(currentModel);
        //    emit modelNameChanged(i);
        //    emit modelListChanged();
    }

}

void Project::assignMaterial(/*const QString& matToAssignName*/){

   // std::cout<<"Assigning material invoked."<<std::endl;
    QString matToAssignName = static_cast<QAction*>(sender())->text();
    auto* currentModel = getCurrentModel();
    int numberOfMat = currentModel->getMaterialsNamesList().size();
    if(numberOfMat==0){std::cout<<"No material was created in the model "<<currentModel->getName().toStdString()<<"."<<std::endl; return;}

    int currentId = currentModel->getMaterialsNamesList().size()-1;
    int fromNameId;
    int i=0;
    for (auto& matName : currentModel->getMaterialsNamesList()){
        if(matName != matToAssignName){
            i++;
            continue;
        }
        if( (qsizetype)i == currentModel->getMaterialsNamesList().size()){
            std::cout<<"The material "<<matToAssignName.toStdString()
                    <<" does not exist"; return;}
        fromNameId = i;
    }
    if(currentId != fromNameId) currentModel->exchangeMatPos(fromNameId, currentId);
    //QString fromName = static_cast<QAction*>(sender())->text();
    std::cout<<"The material "<<matToAssignName.toStdString()<<" is assigned to the model "<<currentModel->getName().toStdString()<<"."<<std::endl;
}


void Project::update(){
    emit currentModelChanged(currentModel);
    //emit modelNameChanged(i);
    emit modelListChanged();
        //modelsNamesList=getModelsNamesList();
    //qsizetype currentModelIndex=0;
}


void Project::deleteModel(Model* _model){
    /*
         * The first model cannot be deleted;
         * To remove the current model one just have to provide a nullptr _model
    */
    if(_model!=nullptr && models->size()==1/*_model->getName()==models->at(0)->getName()*/){
        std::cout<<"The project must have at least one model"<<std::endl;
        return;}

    if(_model){
        QString name= _model->getName();
        for (int i=0; i<models->size(); i++){
            if(models->at(i)->getName() == name){
                if(_model==currentModel) (i>0 ||i==models->size()-1) ? currentModel=models->at(i-1) : currentModel=models->at(i+1);
                delete _model, cout<<"model : "<<name.toStdString()<< " deleted"<<std::endl;

                models->removeAt(i);
                emit currentModelChanged(/*_index*/currentModel);
                emit modelListChanged();
                return;

            }
        }
    }

    else if(_model==nullptr && models->size()>1){
        QString name= currentModel->getName();
        for (int i=0; i<models->size(); i++){
            if(models->at(i)->getName() == name){
                delete currentModel;
                models->removeAt(i);
                    //   emit currentModelChanged();
                emit modelListChanged();

                return;
            }
        }
    }


}

void Project::deleteCurrentModel(){
    //        if(models->size()>1){
    //            QString name= currentModel->getName();
    //            for (int i=0; i<models->size(); i++){
    //                if(models->at(i)->getName() == name){
    //                    delete currentModel;
    //                    models->removeAt(i);
    //                    emit currentModelChanged(/*_index*/);
    //                    emit modelListChanged();
    //                    return;
    //                }
    //            }
    //        }
    deleteModel(currentModel);
}


void Project::runPBCAnalysisLauncher(){

    QThread *thr =new QThread;
    frames::HeavyWrapper* wrapper=new frames::HeavyWrapper()/*((Model*)this)*/;
    //wrapper->setModel((Model*)this);
    auto* currentModel= getCurrentModel();
    wrapper->setModel(currentModel);
    wrapper->moveToThread(thr);

    //QObject *parentThis = this->parent();
    //QObject::connect(this, &Model::headyWorkFinished, /*QApplication::instance()*/this,
    //  [this, parentThis, thr]{/*this->moveToThread(QApplication::instance()->thread());*/ this->setParent(parentThis); thr->quit(); /*thr->deleteLater();*/}  );

    //    QObject::connect(thr, &QThread::started, &wrapper, &Wrapper::runPBCAnalysisLauncher);
    //    QObject::connect(this, &Model::headyWorkFinished, thr, &QThread::quit);
    //    QObject::connect(thr, &QThread::finished, thr, &QThread::deleteLater);

    //this->setParent(nullptr);
    //this->moveToThread(thr);

    QObject::connect(thr, &QThread::started, wrapper, &HeavyWrapper::runPBCAnalysisLauncher);
    QObject::connect(wrapper, &HeavyWrapper::heavyComputationEnd, thr, &QThread::quit);
    QObject::connect(thr, &QThread::finished, thr, &QThread::deleteLater);
    QObject::connect(thr, &QThread::finished, wrapper, &HeavyWrapper::deleteLater);

    std::cout<<"Running PBC analysis with model "<<getCurrentModel()->getName().toStdString()<<std::endl;
    thr->start();
}





} // namespace frames
} // namespace scaletag
