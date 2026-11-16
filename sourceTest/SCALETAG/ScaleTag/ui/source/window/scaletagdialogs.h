#ifndef RENAMEMODEL_H
#define RENAMEMODEL_H

#include<iostream>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QFormLayout>

#include<QCheckBox>

#include<QSlider>
#include<QComboBox>
#include<QDoubleSpinBox>


#include<physics.h>
#include<frames/model.h>


namespace scaletag{
namespace window{

class RenameModelWidget : public QWidget{

    Q_OBJECT

public:

    explicit RenameModelWidget(const QString& fromName, QWidget* parent=nullptr );
    void setOldName(const QString& _oldName){oldName=_oldName;}


private:

    void createContent();

    QGridLayout* layout;
    QGroupBox* contentbox;
   // QLabel* titleLabel;
   /* QLabel**/ QPushButton* ok;
    /*QLabel**/QPushButton* cancel;
    QLineEdit* editPlace;
    //QComboBox*
    //QCheckBox*
    //QGroupBox*
    QString oldName;


private slots:
 //   void nameAccepted(const QString& name);
    //void operationCancelled();
    void onEditingFinished();

signals:
    void editingEndedWith(const QString& fromName, const QString& toName);

};


/*  For Linear elastic widget */


class LinearElasticMaterialWidget : public QWidget{
    Q_OBJECT

public:

    explicit LinearElasticMaterialWidget(const QString& name, QWidget* parent=nullptr );
     explicit LinearElasticMaterialWidget(const QString& purposeTitle, const QString& name, double E, double nu, QWidget* parent=nullptr );
    void setPurposeTitle(const QString& title);


private:

    void createContent();

    QGridLayout* layout;
    QGroupBox* contentbox;
    QPushButton* ok;
    QPushButton* cancel;
    QLineEdit* youngModulusEdit;
    QLineEdit* poissonCoefficientEdit;
    QLineEdit* materialNameEdit;

    QString materialName;
    double youngModuls;
    double poissonCoefficient;

    bool forMaterialCreation = false;
    bool forMaterialEditing = false;





private slots:
    void operationCancelled(){}

    void onEditingFinished();

signals:
    //void editingEndedWith(const QString& materialName, double E, double nu);
    void editingEndedWith( Material _mat); //Intended for material creation
    void editingMaterialEndedWith(Material _mat, QString _materialToModifyName); //Intended for material editing

};



class RenameMaterialWidget : public QWidget{

    Q_OBJECT

public:

    explicit RenameMaterialWidget(const QString& fromName, QWidget* parent=nullptr );
    void setOldName(const QString& _oldName){oldName=_oldName;}


private:

    void createContent();

    QGridLayout* layout;
    QGroupBox* contentbox;
    QPushButton* ok;
    QPushButton* cancel;
    QLineEdit* editPlace;
    QString oldName;


private slots:

    void onEditingFinished();

signals:
    void editingEndedWith(const QString& fromName, const QString& toName);

};

//class PBCConstraintsManagerWidget{
//    // to help for periodic surfaces mapping

//private:
//    void createContend();
//    QGridLayout* layout;
//    QLineEdit firstSurf;
//    QLineEdit* secondSurf;
//    QPushButton* validate;
//    QPushButton*  clear;
//    QPushButton* quit;
//public slots:


//signals:
//};

/******************************************* Widget for view pannel ***********************************/

class ViewPannel : public QWidget{

    Q_OBJECT

private:
    QSlider* scaleFactorSlider;
    QSlider* timeSlider;
    QComboBox* fieldCombo;
    QComboBox* timeCombo;
    QComboBox* compCombo;

    QLineEdit* fieldEdit;
 //   QLineEdit* factorEdit;
//    QLineEdit* modelName;
//    QLineEdit* scaleFactorEdit;
    QDoubleSpinBox* scaleFactorEdit;

    QCheckBox* deformShapeCheck;

    QPushButton* ok;
    QPushButton* cancel;

    QLabel* modelLabel;
    QLabel* deformedShapeLabel;
    QLabel* fieldLabel;
    QLabel* scaleFactorLabel;
    QLabel* timeLabel;

     //QGroupBox* contentBox;
    QVBoxLayout* mainVLayout;
     QHBoxLayout* validatingLineLayout;
    QFormLayout* layFormWidgets;



    int timeRank=0;
    double scaleFactor=1.;
    double scaleMin, scaleMax;
    double currentTime;
QStringList timeSteps;

    int nTimeStep;
    QString fieldName;
    QString modelName;
    QStringList fieldNames;

    QStringList comps;

    bool showDeformed=false;



    frames::Model* model;
   // QStringList fieldNames;
    //QStringList timeSteps;

    void createContent();  //For creating child widgets
    void initialize(); // For initializing parameters based on model

public:
    explicit ViewPannel(QObject* _model, QWidget* parent = nullptr);
    void setModel(frames::Model* _model);


signals:
    void viewResultsWith(const QString& fieldName, int comp, int timeRank, double scaleFactor, bool showDeformed);

public slots:
    //void parametrizeTheView();
    void requestView(/*const QString& fieldName, int timeRank, double scaleFactor*/);
    void updateCompCombo(/*int fieldIndex*/);




};

}}
#endif
