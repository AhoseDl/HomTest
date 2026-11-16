#include <frames/workdirectory.h>
//using namespace std;


namespace scaletag {
namespace frames{


WorkDirectory::WorkDirectory(QObject *parent)
    : QObject{parent}
{
//workDirectory=QDir::homePath().toStdString();
//workDirectory = QDir::currentPath().toStdString();
//QDir::setCurrent(QDir::homePath());
}


//void WorkDirectory::setWorkDirectory(QString directory){
//    workDirectory=directory.toStdString();
//    QDir::setCurrent(directory);
//}

void WorkDirectory:: setWorkDirectory(){
QString dir = QFileDialog::getExistingDirectory(nullptr, tr("Open Directory"),QDir::homePath(),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
QDir::setCurrent(dir);
/*qDebug()*/std::cout<<"Work directory set to : "<<QDir::currentPath().toStdString()<<std::endl;
}


QString WorkDirectory::getDirectory(){
return QDir::currentPath();

}

} // namespace window
} // namespace scaletag
