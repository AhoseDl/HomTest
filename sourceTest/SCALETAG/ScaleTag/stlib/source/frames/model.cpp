#include<visualization/generatedataforvis.h>
#include "model.h"

namespace scaletag {
namespace frames {

Model::Model(QObject *parent)
    : QObject{parent}, parts()
{
    name=QString("Model-1");
    dataForVis=/*.reset*/( new scaletag::visualization::GenerateDataForVis(this) );
    masterPBC.reset(new models::MasterPBC());

}

Model::~Model(){
    if(mesh) delete mesh;
}

//Model* Model::createModel(QObject *_parent){

//    return new Model(_parent);

//}

QString Model::getName() const {return name;}

void Model::setName(const QString& _name){name=_name;}

Part* Model::getCurrentPart(){return currentPart;}

void Model::setMesh(const QString& fileName){
    mesh->marcInitialize(fileName.toStdString());
//    mesh->reorder();
}
void Model::setMeshFile(const std::string& _meshFile){
    meshFile=_meshFile;
}

std::string Model::getMeshFile(){return meshFile;}
void Model::setMesh(const MeshPbc<double,3>&  _mesh){*mesh=_mesh;}
void Model::setMesh(MeshPbc<double,3>* _mesh){mesh=_mesh;}

MeshPbc<double, 3>* Model::getMesh() {return mesh;}

scaletag::visualization::GenerateDataForVis* Model::getDataForVis(){ return dataForVis/*.data()*/ ;}

void Model::setDataForVis(visualization::GenerateDataForVis* _dataForVis){
/*dataForVis.reset(_dataForVis);*/
    dataForVis=_dataForVis;}


void Model::modify(const QString& fileName){
    //to be reimplemented
    if(fileName.isEmpty()) return;
    mesh=new MeshPbc<double,3>();
    mesh->marcInitialize(fileName.toStdString());
    //        mesh->reorder();
    setMeshFile(fileName.toStdString());
    //emit modelModified(this);

}

void Model::modifyCAD(const QString& fileName){
    //to be reimplemente
    //        mesh=new MeshPbc<double,3>();
    //        mesh->marcInitialize(fileName.toStdString());
    //        mesh->reorder();

    Part* part= new Part(this, fileName);
    part->setModel(this);
    parts.append(part);
    currentPart=part;
    part->setCADFile(fileName);
    emit modelCADModified(currentPart);

}

void Model::modifyCADWithShape(const QString& fileName){
    qDebug()<< "modifyCADWithShape running on thread:" << QThread::currentThread()
                                                              << "main thread is:" << QApplication::instance()->thread();
    Part* part= new Part(this, fileName);
    part->setModel(this);
    parts.append(part);
    currentPart=part;
    part->setCADFile(fileName);

    emit modelCADWithShapeModified(currentPart);

}


void Model::addField(const std::string& name, std::vector<std::valarray<double> >& value){
    fields[name] = value;
}
void Model::getField(const std::string& name, std::vector<std::valarray<double> >& value){
    value = fields.at(name);
}


std::map<std::string, std::vector<std::valarray<double> > > Model::getFields(){ return  fields;}


void Model::emitNeedTreeViewUpdateSignal(){emit needTreeViewUpdate();}

models::MasterPBC* Model::getMasterPBC(){return masterPBC.data();}






QStringList Model::getMaterialsNamesList(){
    QStringList theNames;
    for(auto& mat:materials){
        QString matName= QString::fromStdString(mat.getName());
        theNames.append(matName);
    }
    return theNames;
}

QStringList Model::getFieldsNamesList(){
    QStringList theNames;
    QString fieldName;
    for(auto& field : fields){
        fieldName = QString::fromStdString( (field.first) );
        theNames.append(fieldName);
    }
    return theNames;
}

int Model::getFieldCompSize(){

    //For now, this function returns the number of components of the first field;

    int nComp; int nField = getFieldsNamesList().size();
    if(nField==0) return 0; //No field yet

    if(nField != 0) {
        auto it = std::begin(fields);
        nComp = (it->second)[0].size()/(getMesh()->getNumberOfNodes());
        return nComp;
    }
    return 0;
}

int Model::getFieldCompSize(const QString& fieldName, int tps){
   // if(fieldName.toStdString()=="") return 0;

    int nComp = (fields.at(fieldName.toStdString())[tps].size())/(getMesh()->getNumberOfNodes());
    return nComp;

}

void Model::exchangeMatPos(const int& matId1, const int& matId2){
    int matsSize = materials.size();
    if(matId1>=matsSize || matId2>=matsSize || matId1==matId2) return;

    Material mat1 = materials[matId1];
    Material mat2 = materials[matId2];

    materials[matId1] = mat2;
    materials[matId2] = mat1;
}


QString Model::addMaterial(const Material& _mat/*const QString& _matType, const QString& _matName, double E, double nu*/){

    std::string  theMatName = _mat.getName();
    std::string comparisonName;
    for(const auto& _aMat:materials){
        comparisonName = _aMat.getName();
        if(theMatName==comparisonName){
            std::cout<<"A material of name "<<theMatName<<
                " already exists. Please choose another name";
            return"";}
    }
    materials.push_back(std::move(_mat));

    std::cout<<"Material "<<_mat.getName()<<" is added "<<std::endl;

    emit materialModified(QString::fromStdString( _mat.getName()) );
    emit needTreeViewUpdate();

    return QString::fromStdString( _mat.getName());
}

QString Model::updateMaterial(const Material& _currentMat, const QString& _oldMaterialName){
    std::cout<<"Updating a material..."<<std::endl;
    int i =0;
    auto materialNameList = getMaterialsNamesList();
    for(const auto& matName : materialNameList){
        if(matName != _oldMaterialName) {i++; continue;}
        if(i == materialNameList.size()){std::cout<<"The material \" "<<_oldMaterialName.toStdString()<<" \" does not exist."; return "";}
        materials[i] = _currentMat;
        std::cout<<"material new name : "<<_currentMat.getName()<<std::endl;
        //        emit materialModified(QString::fromStdString(_currentMat.getName()));
        emit needTreeViewUpdate();
        return _oldMaterialName;
    }
}

std::vector<Material>& Model::getMaterials(){return materials;}

const Material& Model::getMaterial(const std::string& matName) const {
    int i=0;
    for(const auto& mat : materials){
        if(mat.getName() != matName) {i++; continue;}

    }
    if(i == materials.size()){std::cout<<"The material "<<matName<<" does not exist."<<std::endl;}

    if(i !=  materials.size() ){return materials[i]; }

}

QString Model::deleteMaterial(const QString & _matName){
    std::vector<Material>::iterator it;
    for(it=materials.begin(); it !=materials.end(); it++){
        QString theMaterialName = QString::fromStdString( it->getName() );
        if(theMaterialName != _matName)  continue;

        if(it!=materials.end()) {materials.erase(it);
            emit needTreeViewUpdate();
            return QString::fromStdString(it->getName());
        }
    }
    return "";
}

QString Model::renameMaterial(const QString& oldName, const QString& newName){

    std::vector<Material>::iterator it;
    for(it=materials.begin(); it !=materials.end(); it++){
        QString theMaterialName = QString::fromStdString( it->getName() );
        if(theMaterialName != oldName)  continue;
    }
    if(it!=materials.end()) {it->setName(newName.toStdString());
    }

    emit needTreeViewUpdate();

    return oldName;
}




void Model::runPBCAnalysis(){

    int ws, wr;
    chrono::time_point<std::chrono::high_resolution_clock> ti, tf;
    scaletag::fem::BCs bc;
    std::valarray<double> disp;
    std::valarray<double> force;

    double E11=0, nu12=0, nu13=0;

    MeshPbc<double, 3> *me = getMesh();

    Material material("Hooke", "plasgray");
    material["E"] = 608. ;
    material["nu"] = 0.4 ;
    std::vector<Material> mats;
    mats.push_back(material);
    me->assignMat(0);

    LinearElasticity3D phys;



    ElementalMatrixLinearElasticity3D elementalMatrix(phys, mats, *me);


    ti=chrono::high_resolution_clock::now();
    MPI_Barrier(MPI_COMM_WORLD);
    PetscFunctionBeginUser;
    std::cout<<"Running PBC analysis ..."<<std::endl;
    //    if(/*wr!=0*/1){
            /*PetscCall(*/scaletag::fem::ElasXAnalysis(*me, 3, bc,/* scaletag::fem::matrixValueTetra,*/
   elementalMatrix,
                                                   disp, force, E11, nu12, nu13 )/*)*/;
    //    }
    MPI_Barrier(MPI_COMM_WORLD);
    tf=chrono::high_resolution_clock::now();
    std::cout<<"Job terminated."<<std::endl;
    cout<<"computation duration : "<<chrono::duration_cast<chrono::milliseconds>(tf-ti).count() <<endl;
    PetscFunctionReturn();
}

void Model::runPBCAnalysisLauncher(){

    QThread *thr =new QThread;
    frames::HeavyWrapper* wrapper=new frames::HeavyWrapper()/*((Model*)this)*/;
    //wrapper->setModel((Model*)this);
    //wrapper->setModel(reinterpret_cast<Model*>(sender()));
    wrapper->setModel(this);
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


    thr->start();

}

void Model::viewResults(){

    std::cout<<"Results viewed"<<std::endl;
    emit askViewResults();
}






/************ Definition of HeavyWrapper method *********************/

HeavyWrapper::HeavyWrapper(QObject* parent) : QObject(parent){
}

frames::Model* HeavyWrapper::getModel(){return model;}

void HeavyWrapper::setModel(Model* _model){model=_model;}

HeavyWrapper::~HeavyWrapper(){}







void HeavyWrapper::runPBCAnalysisLauncher(){
//PetscVoidFunction();
    if(model){    int ws, wr;
        chrono::time_point<std::chrono::high_resolution_clock> ti, tf;
        scaletag::fem::BCs bc;
        std::valarray<double> disp0;
        std::valarray<double> force0;

        std::vector<std::valarray<double>> disps, forces;

        double E11 = 0, nu12 = 0, nu13 = 0;
        double E22 = 0., E33 = 0., nu23 = 0., G12 = 0., G13 = 0., G23 = 0. ;

        MeshPbc<double, 3> *me = model->getMesh();

        //The assigned material is the last one in the materials list of the model
        Material material;
        std::vector<Material> mats = model->getMaterials();
        int matsSize = mats.size();
        if(matsSize==0){
        //Material material("Hooke", "plasgray");
            double E = 608., nu=0.4;
        material = Material("Hooke", "plasgray");
        material["E"] = E ;
        material["nu"] = nu ;
//        std::vector<Material> mats;
            mats.push_back(material);
        me->assignMat(matsSize);
            std::cout<<"Using the default material \"plasgray\" for the analysis."<<std::endl;
            std::cout<<"Young modulus : "<<E
                      <<" Poisson ration : "<<nu<<std::endl;
        }
        else {
        //material = model->getMaterials()[matsSize-1];
            std::cout<<"Using material "<<mats[matsSize-1].getName()<<" for the analysis."<<std::endl;
            mats[matsSize-1].setType("Hooke");
            me->assignMat(matsSize-1);
        }

//        std::vector<Material> mats;
//        mats.push_back(material);
//        std::vector<Material> mats = model->getMaterials();
 //       me->assignMat(matsSize-1);

        LinearElasticity3D phys;

        ElementalMatrixLinearElasticity3D elementalMatrix(phys, mats, *me);

        ti=chrono::high_resolution_clock::now();
        MPI_Barrier(MPI_COMM_WORLD);
        //PetscFunctionBeginUser;

//        std::cout<<"Model "<<this->model->getName().toStdString()<<" : "<</*"Running uniaxial traction along X direction ..."*/"Running PBC loadings ..."<<std::endl;
//        disps.resize(1); forces.resize(1);
//        PetscErrorCode ierr = scaletag::fem::ElasXAnalysis(*me, 3, bc,elementalMatrix, disps[0], forces[0], E11, nu12, nu13 );

        std::cout<<"Model "<<this->model->getName().toStdString()<<" : "<<"Running PBC loadings ..."<<std::endl;
       PetscErrorCode ierr = ElasFullAnalysisGen(*me, 3, bc, elementalMatrix, disps, forces, E11, E22, E33, nu12, nu13, nu23, G12, G13, G23);

        MPI_Barrier(MPI_COMM_WORLD);
        tf=chrono::high_resolution_clock::now();
        std::cout<<"Job terminated."<<std::endl;
        cout<<"computation duration : "<<chrono::duration_cast<chrono::milliseconds>(tf-ti).count() <<endl;

       // emit heavyComputationEnd();
        //   time         comp
        std::vector<std::valarray<double>> timeDisp(1);
        std::vector<std::valarray<double>> timeForce(1);

        for(int i=0; i<disps.size(); i++){
        timeDisp[0] = disps[i];
        timeForce[0] = forces[i];

        std::string disp="disp", force="force";
        model->addField(disp+std::to_string(i), timeDisp);
        model->addField(force+std::to_string(i), timeForce);
        }

        //model->addField("stress", stress);
        //this->moveToThread(QApplication::instance()->thread());
        // emit headyWorkFinished();
        //PetscFunctionReturn();
    }

    emit heavyComputationEnd();
}

/************** End of definition of HeavyWrapper methods ****************/

} // namespace frames
} // namespace scaletag
