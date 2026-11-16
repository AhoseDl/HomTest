#ifndef CM_FRAMES_PART_H
#define CM_FRAMES_PART_H

#include <QObject>
#include<stlib_global.h>
//#include<frames/model.h>

#include<vtkOCCTReader.h>
#include<vtkSmartPointer.h>
#include<vtkNew.h>
#include<vtkCompositeDataSet.h>

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

#include<frames/heavytask.h>
//#include<QtConcurrent>

namespace scaletag {
namespace frames {

class Model;

class STLIB_EXPORT Part : public QObject
{
    Q_OBJECT
public:
    explicit Part(QObject *parent = nullptr);
    explicit Part(QObject *parent , const QString& _file="");

    void setData(QString _file);
    void setName(const QString& _name){    name=_name;}
    QString getName(){return name;}
    void setModel(frames::Model* _model){model=_model;}
    frames::Model* getModel(){return model;}
    vtkOCCTReader* getCADReader(){return CADReader;}

    void setCADFile(const QString& _file){dataCADInitializerFile=_file;}
    QString getCADFile(){return dataCADInitializerFile;}

    void initializeOCCTReader(){
        CADReader=vtkSmartPointer<vtkOCCTReader>::New();
        setData(dataCADInitializerFile);
    }


    void readStepFile(const QString& _file, /*opencascade::handle<*/TopoDS_Shape/*>*/&);
    void readIGESFile(const QString& _file, /*opencascade::handle<*/TopoDS_Shape/*>*/&);
    void readCADFile(const QString& _file, /*opencascade::handle<*/TopoDS_Shape/*>*/&);

    void createVISSource(){partSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();}
    IVtkTools_ShapeDataSource* getVISSource(){return partSource;}



private:
    //static int numPart;
    frames::Model* model;
    vtkCompositeDataSet* dataCAD{nullptr};
    QString dataCADInitializerFile{};
    //QString name=QString::fromStdString("Part-"+std::to_string(numPart));
    QString name="Part-1";
    vtkSmartPointer<vtkOCCTReader> CADReader;
    vtkSmartPointer<IVtkTools_ShapeDataSource> partSource;  //For VIS


signals:

};

} // namespace frames
} // namespace cm

#endif // CM_FRAMES_PART_H
