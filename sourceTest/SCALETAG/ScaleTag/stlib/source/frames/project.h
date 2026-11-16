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
#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include<QtQml/QQmlListProperty>
#include<QList>
#include<QString>
#include<stlib_global.h>
#include<frames/model.h>
#include<QStringList>
#include<QScopedPointer>
#include<QDebug>
#include<iostream>
#include<QStringList>

namespace scaletag {
namespace frames {


class STLIB_EXPORT Project : public QObject
{
    Q_OBJECT
   Q_PROPERTY(/*QQmlListProperty<QString>*/ QStringList ui_ModelsNames READ getModelsNamesList /*WRITE updateModelsNamesList*//*CONSTANT*/ NOTIFY modelListChanged)
   Q_PROPERTY(QString ui_currentModelName READ getCurrentModelName /*WRITE setCurrentModelName*//*CONSTANT*/ NOTIFY currentModelChanged)
   //Q_PROPERTY(qsizetype ui_currentModelIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
   Q_PROPERTY(qsizetype ui_currentModelIndex READ getCurrentIndex /*WRITE updateModelsNamesList*//*CONSTANT*/ /*NOTIFY currentModelChanged*/)

public:
    explicit Project(QObject *parent = nullptr);

    QList<Model*> getModelList() const;

    Model* getCurrentModel();

    Model* getModel(const QString& name);

    QString getCurrentModelName() const ;
    QString getCurrentModelName();

    QString setCurrentModelName(const QString& _name);

    QStringList getModelsNamesList() const;

    QStringList updateModelsNamesList();

    void setModelsNamesList(const QStringList& _stringList) ;

    qsizetype getCurrentIndex() const ;

    Q_INVOKABLE  void setCurrentIndex(qsizetype _index);

//  void setCurrentIndex(){
//        qsizetype currIndex=0;

//        for(size_t i=0; i<models->size(); i++){
//            auto* candMod = models->at(i);
//            if(candMod==currentModel) currIndex=i;
//        }
//        std::cout<<"setCurrentIndex() invoked"<<std::endl;
//  }


 //QStringList getModelsNamesList() const;


private :
 static int numModel; // By choice this variable is never decremented
 QScopedPointer<QList<Model*> > models;
 Model* currentModel;
 QStringList modelsNamesList;
 qsizetype currentModelIndex=0;

signals:
    void comboBoxIndexChanged(QString);
    void modelListChanged(/*QStringList*/);
    void currentModelChanged(Model*);
    void modelNameChanged(size_t pos);
    void currentModelModified(Model*);
    ///void deleteView(int _numViewPort);



public slots:
    void projectModify(const QString& filename);

    void projectCADModify(const QString& filename);

    void projectCADWithShapeModify(const QString& filename);

    void setCurrentModel(const QString& qsizetype /*_index*/);

    Model* createModel(const QString& _name);

    Model* createNoNameModel();

//    void renameModel(/*const QString& newName="newName"*/){
//        currentModel->setName();
//        emit currentModelChanged(/*_index*/);
//        emit modelListChanged();
//    }

    void renameModel(const QString& fromName, const QString& toName="New-Name");
    void renameMaterial(const QString& fromName, const QString& toName="New-Name");
    void assignMaterial(/*const QString& fromName*/);

    void update();

    void deleteModel(Model* _model);

    void deleteCurrentModel();

    void runPBCAnalysisLauncher();


};

} // namespace frames
} // namespace scaletag

#endif // PROJECT_H
