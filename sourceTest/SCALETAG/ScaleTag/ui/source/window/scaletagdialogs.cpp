#include<scaletagdialogs.h>

namespace scaletag{
namespace window{

/**********  Start of RenameModelWidget ***************************/

RenameModelWidget::RenameModelWidget(const QString& fromName, QWidget* parent):
    QWidget(parent), oldName(fromName){

createContent();
QObject::connect(cancel, &QPushButton::clicked, this, &QWidget::close);
QObject::connect(ok, &QPushButton::clicked, editPlace, &QLineEdit::editingFinished);
QObject::connect(editPlace, &QLineEdit::editingFinished, this, &RenameModelWidget::onEditingFinished);

std::cout<<"Rename the model "<<fromName.toStdString()<<std::endl;
}

void RenameModelWidget::onEditingFinished(){

    QRegularExpression regex("\\w+");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, editPlace);
    editPlace->setValidator(validator);
    int pos=0;
    QString textToValidate = editPlace->text();
    QValidator::State state = validator->validate(textToValidate, pos);
    if(state != QValidator::Acceptable || textToValidate.size()>20) {std::cout<<"Enter a valid name "<<std::endl; editPlace->clear();}

    else{
    emit editingEndedWith(oldName, editPlace->text());
    std::cout<<" editingEndedWith emitted in RenameModelWidget"<<std::endl;
    close();}
}

void RenameModelWidget::createContent(){
    this->setWindowTitle(tr("Rename model"));
    //this->resize(100,100);
    contentbox = new QGroupBox( tr((QString("Rename %1  to").arg(oldName)).toUtf8()) );
    editPlace = new QLineEdit();
    //editPlace->setMinimumWidth(100);
    ok = new QPushButton("Ok");
    cancel = new QPushButton("Cancel");

    layout = new QGridLayout(this);

    QGridLayout* gLayout=new QGridLayout;
    gLayout->addWidget(editPlace, 0,0,1,0);
    gLayout->addWidget(ok, 1,0);
    gLayout->addWidget(cancel, 1,1);
    gLayout->setSizeConstraint(QLayout::SetFixedSize);

    contentbox->setLayout(gLayout);

    layout->addWidget(contentbox,0,0);
    setLayout(layout);
    layout->setRowMinimumHeight(0, contentbox->sizeHint().height());
    layout->setColumnMinimumWidth(0, contentbox->sizeHint().width());
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->show();
    std::cout<<"Window doesn't show "<<windowTitle().toStdString()<<std::endl;
}

//void RenameModelWidget::nameAccepted(const QString& name){}
//void RenameModelWidget::operationCancelled(){}

/**********  End of RenameModelWidget ***************************/



/******************  Start of RenameMaterialWidget **********************/

RenameMaterialWidget::RenameMaterialWidget(const QString& fromName, QWidget* parent):
    QWidget(parent), oldName(fromName){

    createContent();
    QObject::connect(cancel, &QPushButton::clicked, this, &QWidget::close);
    QObject::connect(ok, &QPushButton::clicked, editPlace, &QLineEdit::editingFinished);
    QObject::connect(editPlace, &QLineEdit::editingFinished, this, &RenameMaterialWidget::onEditingFinished);

    std::cout<<"Rename the model "<<fromName.toStdString()<<std::endl;
}

void RenameMaterialWidget::onEditingFinished(){

    QRegularExpression regex("\\w+");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, editPlace);
    editPlace->setValidator(validator);
    int pos=0;
    QString textToValidate = editPlace->text();
    QValidator::State state = validator->validate(textToValidate, pos);
    if(state != QValidator::Acceptable || textToValidate.size()>20) {std::cout<<"Enter a valid name "<<std::endl; editPlace->clear();}

    else{
    emit editingEndedWith(oldName, editPlace->text());
    std::cout<<" editingEndedWith emitted in RenameMaterialWidget"<<std::endl;
    close();}
}

void RenameMaterialWidget::createContent(){
    this->setWindowTitle(tr("Rename model"));
    //this->resize(100,100);
    contentbox = new QGroupBox( tr((QString("Rename %1  to").arg(oldName)).toUtf8()) );
    editPlace = new QLineEdit();
    //editPlace->setMinimumWidth(100);
    ok = new QPushButton("Ok");
    cancel = new QPushButton("Cancel");

    layout = new QGridLayout(this);

    QGridLayout* gLayout=new QGridLayout;
    gLayout->addWidget(editPlace, 0,0,1,0);
    gLayout->addWidget(ok, 1,0);
    gLayout->addWidget(cancel, 1,1);
    gLayout->setSizeConstraint(QLayout::SetFixedSize);

    contentbox->setLayout(gLayout);

    layout->addWidget(contentbox,0,0);
    setLayout(layout);
    layout->setRowMinimumHeight(0, contentbox->sizeHint().height());
    layout->setColumnMinimumWidth(0, contentbox->sizeHint().width());
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->show();
    std::cout<<"Window doesn't show "<<windowTitle().toStdString()<<std::endl;
}

/****************** End of RenameMaterialWidget ****************/




/******************  Start of LinearElasticMaterialWidget **********************/

LinearElasticMaterialWidget::LinearElasticMaterialWidget(const QString& name, QWidget* parent ):materialName(name), QWidget(parent){
    createContent();
    forMaterialCreation = true;
    QObject::connect(cancel, &QPushButton::clicked, this, &QWidget::close);
    QObject::connect(ok, &QPushButton::clicked, this, &LinearElasticMaterialWidget::onEditingFinished);
    //The connection with MainWindow::CreateMaterialFired is done in MainWindow::createMaterial();
}

LinearElasticMaterialWidget::LinearElasticMaterialWidget(const QString& purposeTitle, const QString& name, double E, double nu, QWidget* parent ):materialName(name), QWidget(parent)
{
    createContent();
    forMaterialEditing = true;
    QObject::connect(cancel, &QPushButton::clicked, this, &QWidget::close);
    QObject::connect(ok, &QPushButton::clicked, this, &LinearElasticMaterialWidget::onEditingFinished);
    setPurposeTitle(purposeTitle);
    materialNameEdit->setText(name);
    youngModulusEdit->setText( QString::fromStdString(to_string(E)) );
    poissonCoefficientEdit->setText(QString::fromStdString(to_string(nu)));
}

void LinearElasticMaterialWidget::createContent(){
    std::cout<<"In content creation of LinearElasticMaterialWidget"<<std::endl;

    this->setWindowTitle(tr("Create a new material"));
    contentbox = new QGroupBox( tr("Elastic modulus and Poisson's ratio:") );
    materialNameEdit =  new QLineEdit();
    youngModulusEdit = new QLineEdit();
    poissonCoefficientEdit = new QLineEdit();
    ok = new QPushButton("Ok");
    cancel = new QPushButton("Cancel");

    layout = new QGridLayout(this);

    QFormLayout* fLayout=new QFormLayout;
    fLayout->addRow("Name: ", materialNameEdit);
    fLayout->addRow("Elastic modulus: ", youngModulusEdit);
    fLayout->addRow("Poisson's ratio: ", poissonCoefficientEdit);
    layout->addLayout(fLayout, 0,0,1,0);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(ok);
    hLay->addWidget(cancel);

    layout->addLayout(hLay, 1,0, 1,0);

    contentbox->setLayout(layout);

    QGridLayout* widgetLay=new QGridLayout(this);


    widgetLay->addWidget(contentbox,0,0);
    widgetLay->setRowMinimumHeight(0, contentbox->sizeHint().height());
    widgetLay->setColumnMinimumWidth(0, contentbox->sizeHint().width());
    widgetLay->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(widgetLay);
    this->show();
}

void LinearElasticMaterialWidget::setPurposeTitle(const QString& title){
    this->setWindowTitle(tr(title.toStdString().c_str()));
}
void LinearElasticMaterialWidget::onEditingFinished(){

    QRegularExpression regexName("\\w+");
    QRegularExpressionValidator* nameValidator = new QRegularExpressionValidator(regexName, materialNameEdit);
    materialNameEdit->setValidator(nameValidator);
    int pos=0;
    QString textToValidateMatName = materialNameEdit->text();
    QValidator::State stateMaterialName = nameValidator->validate(textToValidateMatName, pos);

    //QRegularExpression regexValues(R"(^(?:[1-9]\d*(?:\.\d+)?|0\.\d*[1-9]\d*)$)"); // for real numbers greater than zero

    QDoubleValidator* youngValidator = new QDoubleValidator(/*0.0, 10000.0, 10,*/ this);
    youngValidator->setNotation(QDoubleValidator::ScientificNotation);
    youngValidator->setLocale(QLocale::C);
    youngModulusEdit->setValidator(youngValidator);
    pos=0;
    QString textToValidateYoung = youngModulusEdit->text();
    QValidator::State stateYoung = youngValidator->validate(textToValidateYoung, pos);

    QDoubleValidator* poissonValidator = new QDoubleValidator(/*0.0, 10000.0, 10,*/ this);
    poissonValidator->setNotation(QDoubleValidator::ScientificNotation);
    poissonValidator->setLocale(QLocale::C);
    poissonCoefficientEdit->setValidator(poissonValidator);
    pos=0;
    QString textToValidatePoisson = poissonCoefficientEdit->text();
    QValidator::State statePoisson = poissonValidator->validate(textToValidatePoisson, pos);


    bool okStateMat=1, okStateYoung=1, okStatePoisson=1, okYoung=1, okPoisson=1, okMaterial=0;

    if(stateMaterialName != QValidator::Acceptable || textToValidateMatName.size()>20)
    {std::cout<<"Enter a valid name "<<std::endl; materialNameEdit->clear(); okStateMat=0;}

    if(stateYoung != QValidator::Acceptable )
    {std::cout<<"Enter a positive decimal value "<<std::endl; youngModulusEdit->clear(); okStateYoung=0;}

    if(statePoisson != QValidator::Acceptable )
    {std::cout<<"Enter a valid Poisson coefficient "<<std::endl; poissonCoefficientEdit->clear(); okStatePoisson=0;}


    double E = QLocale().toDouble(textToValidateYoung, &okYoung);
    double nu = QLocale().toDouble(textToValidatePoisson, &okPoisson);

    if( !( E>0) ) {std::cout<<"The young modulus must be greater than 0"<<std::endl; youngModulusEdit->clear(); okYoung=0;}
    if( !(nu>0 && nu<=0.5) ) {std::cout<<"The Poisson's coefficient must be less than or equal to 0.5"<<std::endl; poissonCoefficientEdit->clear(); okPoisson=0;}

    okMaterial = okStateMat && okStateYoung && okStatePoisson && okYoung && okPoisson ;

    if(okMaterial){


    //emit editingEndedWith(materialNameEdit->text(), E, nu);
    QString _matName = materialNameEdit->text();
    Material _mat("Hooke", _matName.toStdString());
    _mat["E"]=E; _mat["nu"]=nu;
//    const Material _matToSend=_mat;
    if(forMaterialCreation) emit editingEndedWith(_mat);
    if(forMaterialEditing) emit editingMaterialEndedWith(_mat, materialName);
    std::cout<<" editingEndedWith emitted in CreateMaterialWidget"<<std::endl;
    close();}
}

/******************  End of LinearElasticMaterialWidget **********************/


/******************************************* Widget for view pannel ***********************************/

ViewPannel::ViewPannel(QObject* _model, QWidget* parent):QWidget(parent){
    //auto* mainWindow;
    setModel((frames::Model*)_model);
    createContent();


    QObject::connect(ok, &QPushButton::clicked, this, &ViewPannel::requestView, Qt::QueuedConnection);
    QObject::connect(cancel, &QPushButton::clicked, this, &ViewPannel::close, Qt::QueuedConnection);
}


void ViewPannel::createContent(){
    //this->resize(350, 350);
    this->setFixedSize(350, 350);
    this->setWindowTitle(tr("Model visualization"));
    if(!model) return;
    initialize();

    fieldCombo = new QComboBox(this);
    fieldCombo->addItems(fieldNames);
    timeCombo = new QComboBox(this);
    timeCombo->addItems(timeSteps);

    compCombo = new QComboBox(this);
    compCombo->addItems(comps);
    //compCombo->setEditable(false);

    deformShapeCheck = new QCheckBox(this); deformShapeCheck->setChecked(true);
    modelLabel = new QLabel(this);
    modelLabel->setText(modelName);

    //scaleFactorEdit = new QLineEdit( QString::number( scaleFactor ));
    scaleFactorEdit = new QDoubleSpinBox(this);
    scaleFactorEdit->setRange(0.0, 10000.);
    scaleFactorEdit->setDecimals(2);
    scaleFactorEdit->setSingleStep(0.1);
    scaleFactorEdit->setLocale(QLocale(QLocale::C));
    scaleFactorEdit->setValue(1.0);

    scaleFactorSlider = new QSlider(this);
    scaleFactorSlider->setOrientation(Qt::Horizontal);

    timeSlider = new QSlider(this);
    timeSlider->setOrientation(Qt::Horizontal);

    ok  = new QPushButton("ok", this);
    cancel = new QPushButton("cancel", this);

   // contentBox = new QGroupBox( tr("Model view") );
    mainVLayout = new QVBoxLayout;
    validatingLineLayout = new QHBoxLayout;
    layFormWidgets = new QFormLayout;  layFormWidgets->setSpacing(10);
    modelName = model->getName();
    layFormWidgets->addRow("Model : ", /*new QLabel(modelName)*/ modelLabel);
    layFormWidgets->addRow("Deformed shape : ", deformShapeCheck);
    layFormWidgets->addRow("Field : ", fieldCombo);
    layFormWidgets->addRow("Choose the field's component : ", compCombo);

    QSpacerItem *verticalSpacer = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);
    layFormWidgets->addItem(verticalSpacer);

    layFormWidgets->addRow("Scale factor : ", scaleFactorEdit);
    layFormWidgets->addRow(scaleFactorSlider);

    QSpacerItem *verticalSpacer2 = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);
    layFormWidgets->addItem(verticalSpacer2);

    layFormWidgets->addRow("Time : ", timeCombo);
    layFormWidgets->addRow(timeSlider);



    this->setLayout(mainVLayout);
    //mainVLayout->addWidget(contentBox);

    validatingLineLayout->addWidget(ok);
    validatingLineLayout->addWidget(cancel);
    validatingLineLayout->addStretch(1); // Les boutons sont repoussés à gauche

    mainVLayout->addLayout(layFormWidgets);
    mainVLayout->addLayout(validatingLineLayout);

    updateCompCombo(/*fieldCombo->currentIndex()*/);
    //QObject::connect(fieldCombo, &QComboBox::currentIndexChanged , this, &ViewPannel::updateCompCombo, Qt::QueuedConnection);
    QObject::connect(fieldCombo, &QComboBox::currentIndexChanged, this, [this](){updateCompCombo();});

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();



}


void ViewPannel::initialize(){
    if(!model) return ;
    modelName = model->getName();
    fieldNames = model->getFieldsNamesList();

    nTimeStep = model ->getNumberOfTimeStep();

    for(int i=0; i<nTimeStep; i++)
    timeSteps.append( QString::number(i) );

//    int nComp = model->getFieldCompSize();
//    if(nComp != 0)
//    for(int i=0; i<nComp ; i++) {comps.append(QString::number(i+1)); comps.append("module");}






}

void ViewPannel::setModel(frames::Model* _model){model=_model;}


void ViewPannel::requestView(/*const QString& fieldName, int timeRank, double scaleFactor*/){
    std::cout<<"Executing requestView"<<std::endl;
    fieldName = fieldCombo->currentText();

//    QString scaleFactorText = scaleFactorEdit->text();
//    bool okScaleFactor=true;
//    scaleFactor = QLocale().toDouble(scaleFactorText, &okScaleFactor);
    scaleFactor = scaleFactorEdit->value();

    //timeRank = /*timeCombo->currentIndex()*/1-1;
    timeRank = timeCombo->currentIndex();


    showDeformed = deformShapeCheck->isChecked();

    bool okInt; int comp;
    //int comp = -1 ; //For magnitude display
    string currCompText = compCombo->currentText().toStdString();
    if(currCompText == "module") comp=4 ; else comp = compCombo->currentText().toInt(&okInt);
    std::cout<<"comp : "<<comp<<std::endl;
    emit viewResultsWith(fieldName, comp, timeRank, scaleFactor, showDeformed);
}

//void ViewPannel::updateCompCombo(int fieldIndex){
//    std::cout<<"updateCompComp called."<<std::endl;
//    comps.clear();
//    compCombo->clear();

//    if(!model) return;
//    QString fieldName = fieldCombo->itemText(fieldIndex);
//    int nComp = model->getFieldCompSize(fieldName, timeCombo->currentIndex());
//    if(nComp != 0){
//    for(int i=0; i<nComp ; i++) comps.append(QString::number(i+1));
//    comps.append("module");
//    }

//    compCombo->blockSignals(true);
//    compCombo->addItems(comps);
//    compCombo->blockSignals(false);
//}
void ViewPannel::updateCompCombo(/*int fieldIndex*/){
    const QString fieldName = fieldCombo->currentText();
    compCombo->blockSignals(true);
    compCombo->clear();
    comps.clear();

    if (model && !fieldName.isEmpty()) {
    int nComp = model->getFieldCompSize(fieldName, /*time-independent or step 1*/ 0);
    std::cout<<"getFieldCompSize() returns nComp : "<<nComp<<std::endl;
   // if (nComp <= 0) nComp = 3;   // temporary fallback, to check whether this is the cause
    for (int i = 0; i < nComp; i++) comps.append(QString::number(i+1));
    comps.append("module");
    }
    compCombo->addItems(comps);
    compCombo->setCurrentIndex(0);
    compCombo->blockSignals(false);
}


}}
