#include "part.h"
//#include<QDebug>

#ifndef PART_H
#define PART_H
namespace scaletag {
namespace frames {

Part::Part(QObject *parent)
    : QObject{parent}
{
    //CADReader=vtkSmartPointer<vtkOCCTReader>::New();
    //numPart++;

}

Part::Part(QObject *parent, const QString& _file)
    : QObject{parent}
{
    //numPart++;
    //CADReader=vtkSmartPointer<vtkOCCTReader>::New();
    //setData(_file);
    dataCADInitializerFile=_file;
}


void Part::setData(QString _file){
    dataCADInitializerFile=_file;
    CADReader->SetFileName(dataCADInitializerFile.toStdString().c_str());
    CADReader->SetFileFormat(vtkOCCTReader::Format::STEP);
    CADReader->RelativeDeflectionOn();
    CADReader->SetLinearDeflection(0.1);
    CADReader->SetAngularDeflection(0.5);
    CADReader->ReadWireOn();
    CADReader->Update();

    //set also the vtkocctreader* CADFile;
}



void Part::readStepFile(const QString& _file, TopoDS_Shape& shape){

  //  std::function<void()> readingFct = [_file, &shape, this](){
    std::string cadFile = this->getCADFile().toStdString();

    STEPControl_Reader stepReader;

    IFSelect_ReturnStatus status = stepReader.ReadFile(cadFile.c_str());
    if (status != IFSelect_RetDone) {
        std::cerr << "Failed to read STEP file" << std::endl;
        return;
    }

    stepReader.TransferRoots();
    shape = stepReader.OneShape();


    if (shape.IsNull()) std::cout<<" The underlying shape is null "<<std::endl;

    BRepCheck_Analyzer analyzer(shape);
    if (!analyzer.IsValid()){
        // bad TopoDS_Shape
        std::cout<<" The underlying shape is in bad state "<<std::endl;
    }

 //   };
   // heavyTaskLauncher(readingFct/*, _file, shape, this*/);
}



void Part::readIGESFile(const QString& _file, TopoDS_Shape& shape){

    std::string cadFile = this->getCADFile().toStdString();

    IGESControl_Reader igesReader;

    IFSelect_ReturnStatus status = igesReader.ReadFile(cadFile.c_str());
    if (status != IFSelect_RetDone) {
        std::cerr << "Failed to read IGES file" << std::endl;
        return;
    }

    igesReader.TransferRoots();
    shape = igesReader.OneShape();


    if (shape.IsNull()) std::cout<<" The underlying shape is null "<<std::endl;

    BRepCheck_Analyzer analyzer(shape);
    if (!analyzer.IsValid()){
        // bad TopoDS_Shape
        std::cout<<" The underlying shape is in bad state "<<std::endl;
    }

    Standard_Real linearDeflection = 0.05; // Only for IGES files
    BRepMesh_IncrementalMesh mesher(shape, linearDeflection); // Only for IGES files

}

void Part::readCADFile(const QString& _file, TopoDS_Shape& shape){

    /* This function combines the functions readIGESFile() and readCADFile() */
    int stepMode=1, igesMode=1; IFSelect_ReturnStatus status;

   // TopoDS_Shape cadShape ;

    STEPControl_Reader stepReader;
    IGESControl_Reader igesReader;

    status = stepReader.ReadFile(this->getCADFile().toStdString().c_str());
    if (status != IFSelect_RetDone) stepMode=0;

    if(stepMode==0){
        status = igesReader.ReadFile(this->getCADFile().toStdString().c_str());
        if (status != IFSelect_RetDone) igesMode=0;
    }

    if(stepMode==0 && igesMode==0){
        std::cout<<"Enter a valid CAD files. "<<std::endl; return;
    }

    if(stepMode==1) this->readStepFile(this->getCADFile(), shape );
    if(igesMode==1)  this->readIGESFile(this->getCADFile(), shape );
}
#endif

} // namespace frames
} // namespace cm
