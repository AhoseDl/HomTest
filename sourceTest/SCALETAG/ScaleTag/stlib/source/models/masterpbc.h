#ifndef MASTERPBC_H
#define MASTERPBC_H

#include <pbcmeshtools.h>
#include <cad.h> //include also SelectionTool

#include<QHBoxLayout>
#include<QGridLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QGroupBox>
//#include<frames/objectFactory.h>
//#include<visualization.h>

#include<unordered_map>
#include<functional>

namespace scaletag {

namespace frames{
class ObjectFactory;}

//namespace visualization {
//class Visualization;

//}

namespace models{



class MasterPBC : public QObject{

    Q_OBJECT
public:
    MasterPBC();
    MasterPBC(frames::ObjectFactory* objectFactory, QObject* parent=nullptr);
    ~MasterPBC();

    void initialize(frames::ObjectFactory* objectFactory, QObject* parent=nullptr);

       void highLightAllMappedFaces();





private:
    SelectionTool* selectionTool;
    fem::PBCMeshTool* pbcMeshTool;


    //std::map<IVtk_IdType, IVtk_IdType> mappedFaces;
    //std::unordered_map<IVtk_IdType, IVtk_IdType> mappedFaces;
    std::vector<std::pair<IVtk_IdType, IVtk_IdType>> mappedFaces;

    scaletag::frames::ObjectFactory* objectFactory;

    std::pair<IVtk_IdType, IVtk_IdType> facesPair {0,0}; //used internally for populating mappedFaces
    int idx=0;
//    auto putFirstId = [&idx, &facesPair](IVtk_IdType faceId){ if (idx==0) {facesPair.first=faceId; idx=1;} };
//    auto putSecondId = [&idx, &facesPair](IVtk_IdType faceId){ if (idx==1) {
//            facesPair.second=faceId;
//            mappedFaces.insert(facesPair);
//            facesPair={0,0};
//            idx=0;}
//    };





public slots:
    void setMappedFace(IVtk_IdType faceId1, IVtk_IdType faceId2);

//    std::unordered_map<IVtk_IdType, IVtk_IdType> getMappedFaces();
    std::vector<std::pair<IVtk_IdType, IVtk_IdType>> getMappedFaces();

    void clearAllMappedFaces();

    void setSurfaceId();

//    bool checkFaceId(IVtk_IdType id){
//        for(auto p: mappedFaces){
//            if (id==p.first ||id==p.second) return false;
//        }
//        return true;
//    }

    bool checkFaceId(IVtk_IdType id) {
        if (this->idx == 1 && id == this->facesPair.first) {
            return false;
        }

        for (auto p : this->mappedFaces) {
            if (id == p.first || id == p.second) {
                return false;
            }
        }

        // Face is unique and safe to select
        return true;
    }

    void initSelection(IVtk_SelectionMode mode);





};




class PBCConstraintsManagerWidget :public QWidget{
    // to help for periodic surfaces mapping

    Q_OBJECT

public:

    PBCConstraintsManagerWidget(QWidget* parent=nullptr);

private:
    void createContent();

    QHBoxLayout* hlay;
    QGridLayout* layout;
    QGroupBox* contentBox;
    QLineEdit firstSurf;
    QLineEdit* secondSurf;
    QPushButton* validate;
    QPushButton*  clearAll;
    QPushButton* quit;

public slots:


signals:
    void aSurfaceIsSelected();
    void clearAllFired();
    void closeWidget();
};


}}

#endif
