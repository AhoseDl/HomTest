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

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include<stlib_global.h>
#include<models/MeshPbc.h>
//#include<frames/project.h>
#include<frames/part.h>
#include<pbcanalysis.h>
//#include<chrono>
 //#include<femPbc.h>

#include<map>
//#include<visualization/generatedataforvis.h>

#include<physics.h>

#include<QThread>
#include<QApplication> //for QApplication::instance()
#include<scaletagviewport.h>


#include<masterpbc.h>

namespace scaletag {
 namespace visualization { class GenerateDataForVis; }
namespace frames {

class STLIB_EXPORT Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    //static Model* createModel(QObject *parent = nullptr);
    ~Model();
    QString getName() const;
    void setName(const QString& _name="NewName");

    Part* getCurrentPart();
    void setMesh(const QString& fileName); //clear mesh and set another

    void setMesh(const MeshPbc<double,3>&  _mesh);

    void setMesh(MeshPbc<double,3>* _mesh);

    QStringList getMaterialsNamesList();
    QStringList getFieldsNamesList();
    int getFieldCompSize();
    int getFieldCompSize(const QString& fieldName, int tps);

    void exchangeMatPos(const int& matId1, const int& matId2);

    MeshPbc<double, 3>* getMesh();

    visualization::GenerateDataForVis* getDataForVis();

    void setDataForVis(visualization::GenerateDataForVis* _dataForVis);

    void setMeshFile(const std::string& meshFile);
    std::string getMeshFile();
    void modify(const QString& fileName);

    void modifyCAD(const QString& fileName);

    void modifyCADWithShape(const QString& fileName);


    void addField(const std::string& name, std::vector<std::valarray<double> >& value);

    void getField(const std::string& name, std::vector<std::valarray<double> >& value);


    std::map<std::string, std::vector<std::valarray<double>> > getFields();

    int getNumberOfTimeStep(){
        int nstep = 0;
        if (fields.empty()) return 1; //For the undeformed state
        auto it = std::begin(fields);

        nstep = /*(static_cast<std::vector<std::valarray<double>>>*/(it->second)/*)*/.size()+1;
        return nstep;

    }


    void emitNeedTreeViewUpdateSignal();

    models::MasterPBC* getMasterPBC();
private:
    QString name;
    MeshPbc<double, 3>* mesh=nullptr;
    std::string meshFile;
    QList<Part*> parts;
    Part* currentPart{nullptr};

    std::map<std::string, std::vector<std::valarray<double>> > fields;

    std::vector<Material> materials;

    //QScopedPointer<scaletag::visualization::GenerateDataForVis> dataForVis;
    scaletag::visualization::GenerateDataForVis* dataForVis;
    //Material currentMaterial;

    QScopedPointer<scaletag::models::MasterPBC> masterPBC;


signals:
    void modelModified(Model* _model/*=nullptr*/);
    void modelCADModified(Part* _part);
    void modelCADWithShapeModified(Part* _part);

    void headyWorkFinished();
    void askViewResults();

    //void materialDeleted(const QString& _name="");
    //void materialAdded(const QString& _name="");
    void materialModified(const QString& _name="");

        void needTreeViewUpdate();

public slots:
    void runPBCAnalysis();
    void runPBCAnalysisLauncher();
    void viewResults();
    //void onViewPortDeleted(int numViewPort){}

    QString addMaterial(const Material& _mat/*const QString& _matType, const QString& _matName, double E, double nu*/);

     QString updateMaterial(const Material& _currentMat, const QString& _oldMaterialName);

     std::vector<Material>& getMaterials();

     const Material& getMaterial(const std::string& matName) const ;

    QString deleteMaterial(const QString & _matName);

    QString renameMaterial(const QString& oldName, const QString& newName);

};


class STLIB_EXPORT HeavyWrapper : public QObject{
    Q_OBJECT

public :
    explicit HeavyWrapper(QObject* parent =nullptr);
    frames::Model* getModel();
    void setModel(Model* _model);

    ~HeavyWrapper();

private:
    frames::Model* model;

signals:
    void heavyComputationEnd();

public slots:
    void runPBCAnalysisLauncher();
};

} // namespace frames
} // namespace scaletag

#endif // MODEL_H
