/****************************************************************************
** Meta object code from reading C++ file 'objectFactory.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/frames/objectFactory.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'objectFactory.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS = QtMocHelpers::stringData(
    "scaletag::frames::ObjectFactory",
    "qmlWelcomeViewPassed",
    "",
    "modelsNamesListChanged",
    "showMainWindow",
    "renameModel",
    "fromName",
    "toName",
    "renameMaterial",
    "modifyMaterial",
    "assignMaterial",
    "createActionsForRenamingInWindow",
    "createActionsForMaterialDeletingInWindow",
    "createActionsForMaterialRenamingInWindow",
    "createActionsForMaterialAssigning",
    "updateModelView",
    "frames::Model*",
    "_model",
    "updateModelCADView",
    "frames::Part*",
    "_part",
    "updateModelCADWithShapeView",
    "showCurrentModel",
    "visualization::ScaleTagViewPort*",
    "viewResult",
    "showMeshEdges",
    "createViewport",
    "showCurrentModelInCurrentViewPort",
    "Model*",
    "getTreeView",
    "ScaleTagTreeView*",
    "updateItemInTreeView",
    "category",
    "itemName",
    "updateTreeView",
    "StartSelection",
    "EndSelection",
    "mapPBCSurfaces",
    "verifyPBCSurfaces",
    "generatePBCMesh",
    "initializeView",
    "getVisualization",
    "scaletag::visualization::Visualization*",
    "getModelsNamesList",
    "nameAt",
    "qsizetype",
    "i",
    "getMainWindowController",
    "scaletag::controllers::MainWindowController*",
    "ui_Visualization",
    "ui_ModelsNames",
    "ui_workDirectory",
    "scaletag::frames::WorkDirectory*",
    "ui_mainWindowController",
    "ui_project",
    "scaletag::frames::Project*"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS_t {
    uint offsetsAndSizes[112];
    char stringdata0[32];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[15];
    char stringdata5[12];
    char stringdata6[9];
    char stringdata7[7];
    char stringdata8[15];
    char stringdata9[15];
    char stringdata10[15];
    char stringdata11[33];
    char stringdata12[41];
    char stringdata13[41];
    char stringdata14[34];
    char stringdata15[16];
    char stringdata16[15];
    char stringdata17[7];
    char stringdata18[19];
    char stringdata19[14];
    char stringdata20[6];
    char stringdata21[28];
    char stringdata22[17];
    char stringdata23[33];
    char stringdata24[11];
    char stringdata25[14];
    char stringdata26[15];
    char stringdata27[34];
    char stringdata28[7];
    char stringdata29[12];
    char stringdata30[18];
    char stringdata31[21];
    char stringdata32[9];
    char stringdata33[9];
    char stringdata34[15];
    char stringdata35[15];
    char stringdata36[13];
    char stringdata37[15];
    char stringdata38[18];
    char stringdata39[16];
    char stringdata40[15];
    char stringdata41[17];
    char stringdata42[40];
    char stringdata43[19];
    char stringdata44[7];
    char stringdata45[10];
    char stringdata46[2];
    char stringdata47[24];
    char stringdata48[45];
    char stringdata49[17];
    char stringdata50[15];
    char stringdata51[17];
    char stringdata52[33];
    char stringdata53[24];
    char stringdata54[11];
    char stringdata55[27];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS = {
    {
        QT_MOC_LITERAL(0, 31),  // "scaletag::frames::ObjectFactory"
        QT_MOC_LITERAL(32, 20),  // "qmlWelcomeViewPassed"
        QT_MOC_LITERAL(53, 0),  // ""
        QT_MOC_LITERAL(54, 22),  // "modelsNamesListChanged"
        QT_MOC_LITERAL(77, 14),  // "showMainWindow"
        QT_MOC_LITERAL(92, 11),  // "renameModel"
        QT_MOC_LITERAL(104, 8),  // "fromName"
        QT_MOC_LITERAL(113, 6),  // "toName"
        QT_MOC_LITERAL(120, 14),  // "renameMaterial"
        QT_MOC_LITERAL(135, 14),  // "modifyMaterial"
        QT_MOC_LITERAL(150, 14),  // "assignMaterial"
        QT_MOC_LITERAL(165, 32),  // "createActionsForRenamingInWindow"
        QT_MOC_LITERAL(198, 40),  // "createActionsForMaterialDelet..."
        QT_MOC_LITERAL(239, 40),  // "createActionsForMaterialRenam..."
        QT_MOC_LITERAL(280, 33),  // "createActionsForMaterialAssig..."
        QT_MOC_LITERAL(314, 15),  // "updateModelView"
        QT_MOC_LITERAL(330, 14),  // "frames::Model*"
        QT_MOC_LITERAL(345, 6),  // "_model"
        QT_MOC_LITERAL(352, 18),  // "updateModelCADView"
        QT_MOC_LITERAL(371, 13),  // "frames::Part*"
        QT_MOC_LITERAL(385, 5),  // "_part"
        QT_MOC_LITERAL(391, 27),  // "updateModelCADWithShapeView"
        QT_MOC_LITERAL(419, 16),  // "showCurrentModel"
        QT_MOC_LITERAL(436, 32),  // "visualization::ScaleTagViewPort*"
        QT_MOC_LITERAL(469, 10),  // "viewResult"
        QT_MOC_LITERAL(480, 13),  // "showMeshEdges"
        QT_MOC_LITERAL(494, 14),  // "createViewport"
        QT_MOC_LITERAL(509, 33),  // "showCurrentModelInCurrentView..."
        QT_MOC_LITERAL(543, 6),  // "Model*"
        QT_MOC_LITERAL(550, 11),  // "getTreeView"
        QT_MOC_LITERAL(562, 17),  // "ScaleTagTreeView*"
        QT_MOC_LITERAL(580, 20),  // "updateItemInTreeView"
        QT_MOC_LITERAL(601, 8),  // "category"
        QT_MOC_LITERAL(610, 8),  // "itemName"
        QT_MOC_LITERAL(619, 14),  // "updateTreeView"
        QT_MOC_LITERAL(634, 14),  // "StartSelection"
        QT_MOC_LITERAL(649, 12),  // "EndSelection"
        QT_MOC_LITERAL(662, 14),  // "mapPBCSurfaces"
        QT_MOC_LITERAL(677, 17),  // "verifyPBCSurfaces"
        QT_MOC_LITERAL(695, 15),  // "generatePBCMesh"
        QT_MOC_LITERAL(711, 14),  // "initializeView"
        QT_MOC_LITERAL(726, 16),  // "getVisualization"
        QT_MOC_LITERAL(743, 39),  // "scaletag::visualization::Visu..."
        QT_MOC_LITERAL(783, 18),  // "getModelsNamesList"
        QT_MOC_LITERAL(802, 6),  // "nameAt"
        QT_MOC_LITERAL(809, 9),  // "qsizetype"
        QT_MOC_LITERAL(819, 1),  // "i"
        QT_MOC_LITERAL(821, 23),  // "getMainWindowController"
        QT_MOC_LITERAL(845, 44),  // "scaletag::controllers::MainWi..."
        QT_MOC_LITERAL(890, 16),  // "ui_Visualization"
        QT_MOC_LITERAL(907, 14),  // "ui_ModelsNames"
        QT_MOC_LITERAL(922, 16),  // "ui_workDirectory"
        QT_MOC_LITERAL(939, 32),  // "scaletag::frames::WorkDirectory*"
        QT_MOC_LITERAL(972, 23),  // "ui_mainWindowController"
        QT_MOC_LITERAL(996, 10),  // "ui_project"
        QT_MOC_LITERAL(1007, 26)   // "scaletag::frames::Project*"
    },
    "scaletag::frames::ObjectFactory",
    "qmlWelcomeViewPassed",
    "",
    "modelsNamesListChanged",
    "showMainWindow",
    "renameModel",
    "fromName",
    "toName",
    "renameMaterial",
    "modifyMaterial",
    "assignMaterial",
    "createActionsForRenamingInWindow",
    "createActionsForMaterialDeletingInWindow",
    "createActionsForMaterialRenamingInWindow",
    "createActionsForMaterialAssigning",
    "updateModelView",
    "frames::Model*",
    "_model",
    "updateModelCADView",
    "frames::Part*",
    "_part",
    "updateModelCADWithShapeView",
    "showCurrentModel",
    "visualization::ScaleTagViewPort*",
    "viewResult",
    "showMeshEdges",
    "createViewport",
    "showCurrentModelInCurrentViewPort",
    "Model*",
    "getTreeView",
    "ScaleTagTreeView*",
    "updateItemInTreeView",
    "category",
    "itemName",
    "updateTreeView",
    "StartSelection",
    "EndSelection",
    "mapPBCSurfaces",
    "verifyPBCSurfaces",
    "generatePBCMesh",
    "initializeView",
    "getVisualization",
    "scaletag::visualization::Visualization*",
    "getModelsNamesList",
    "nameAt",
    "qsizetype",
    "i",
    "getMainWindowController",
    "scaletag::controllers::MainWindowController*",
    "ui_Visualization",
    "ui_ModelsNames",
    "ui_workDirectory",
    "scaletag::frames::WorkDirectory*",
    "ui_mainWindowController",
    "ui_project",
    "scaletag::frames::Project*"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       5,  262, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  206,    2, 0x06,    6 /* Public */,
       3,    0,  207,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,  208,    2, 0x0a,    8 /* Public */,
       5,    2,  209,    2, 0x0a,    9 /* Public */,
       8,    2,  214,    2, 0x0a,   12 /* Public */,
       9,    0,  219,    2, 0x0a,   15 /* Public */,
      10,    0,  220,    2, 0x0a,   16 /* Public */,
      11,    0,  221,    2, 0x0a,   17 /* Public */,
      12,    0,  222,    2, 0x0a,   18 /* Public */,
      13,    0,  223,    2, 0x0a,   19 /* Public */,
      14,    0,  224,    2, 0x0a,   20 /* Public */,
      15,    1,  225,    2, 0x0a,   21 /* Public */,
      18,    1,  228,    2, 0x0a,   23 /* Public */,
      21,    1,  231,    2, 0x0a,   25 /* Public */,
      22,    1,  234,    2, 0x0a,   27 /* Public */,
      24,    0,  237,    2, 0x0a,   29 /* Public */,
      25,    0,  238,    2, 0x0a,   30 /* Public */,
      26,    0,  239,    2, 0x0a,   31 /* Public */,
      27,    1,  240,    2, 0x0a,   32 /* Public */,
      29,    0,  243,    2, 0x0a,   34 /* Public */,
      31,    2,  244,    2, 0x0a,   35 /* Public */,
      34,    0,  249,    2, 0x0a,   38 /* Public */,
      35,    0,  250,    2, 0x0a,   39 /* Public */,
      36,    0,  251,    2, 0x0a,   40 /* Public */,
      37,    0,  252,    2, 0x0a,   41 /* Public */,
      38,    0,  253,    2, 0x0a,   42 /* Public */,
      39,    0,  254,    2, 0x0a,   43 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      40,    0,  255,    2, 0x02,   44 /* Public */,
      41,    0,  256,    2, 0x02,   45 /* Public */,
      43,    0,  257,    2, 0x102,   46 /* Public | MethodIsConst  */,
      44,    1,  258,    2, 0x102,   47 /* Public | MethodIsConst  */,
      47,    0,  261,    2, 0x02,   49 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 23,    2,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 23,
    QMetaType::Void, 0x80000000 | 28,   17,
    0x80000000 | 30,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   32,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    0x80000000 | 42,
    QMetaType::QStringList,
    QMetaType::QString, 0x80000000 | 45,   46,
    0x80000000 | 48,

 // properties: name, type, flags
      49, 0x80000000 | 42, 0x00015409, uint(-1), 0,
      50, QMetaType::QStringList, 0x00015001, uint(1), 0,
      51, 0x80000000 | 52, 0x00015409, uint(-1), 0,
      53, 0x80000000 | 48, 0x00015409, uint(-1), 0,
      54, 0x80000000 | 55, 0x00015409, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::frames::ObjectFactory::staticMetaObject = { {
    QMetaObject::SuperData::link<IObjectFactory::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS_t,
        // property 'ui_Visualization'
        QtPrivate::TypeAndForceComplete<scaletag::visualization::Visualization*, std::true_type>,
        // property 'ui_ModelsNames'
        QtPrivate::TypeAndForceComplete<QStringList, std::true_type>,
        // property 'ui_workDirectory'
        QtPrivate::TypeAndForceComplete<scaletag::frames::WorkDirectory*, std::true_type>,
        // property 'ui_mainWindowController'
        QtPrivate::TypeAndForceComplete<scaletag::controllers::MainWindowController*, std::true_type>,
        // property 'ui_project'
        QtPrivate::TypeAndForceComplete<scaletag::frames::Project*, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ObjectFactory, std::true_type>,
        // method 'qmlWelcomeViewPassed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'modelsNamesListChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showMainWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'renameModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'renameMaterial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'modifyMaterial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'assignMaterial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createActionsForRenamingInWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createActionsForMaterialDeletingInWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createActionsForMaterialRenamingInWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createActionsForMaterialAssigning'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateModelView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<frames::Model *, std::false_type>,
        // method 'updateModelCADView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<frames::Part *, std::false_type>,
        // method 'updateModelCADWithShapeView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<frames::Part *, std::false_type>,
        // method 'showCurrentModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<visualization::ScaleTagViewPort *, std::false_type>,
        // method 'viewResult'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showMeshEdges'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createViewport'
        QtPrivate::TypeAndForceComplete<visualization::ScaleTagViewPort *, std::false_type>,
        // method 'showCurrentModelInCurrentViewPort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Model *, std::false_type>,
        // method 'getTreeView'
        QtPrivate::TypeAndForceComplete<ScaleTagTreeView *, std::false_type>,
        // method 'updateItemInTreeView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateTreeView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'StartSelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'EndSelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'mapPBCSurfaces'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'verifyPBCSurfaces'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'generatePBCMesh'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'initializeView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getVisualization'
        QtPrivate::TypeAndForceComplete<scaletag::visualization::Visualization *, std::false_type>,
        // method 'getModelsNamesList'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        // method 'nameAt'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<qsizetype, std::false_type>,
        // method 'getMainWindowController'
        QtPrivate::TypeAndForceComplete<scaletag::controllers::MainWindowController *, std::false_type>
    >,
    nullptr
} };

void scaletag::frames::ObjectFactory::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ObjectFactory *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->qmlWelcomeViewPassed(); break;
        case 1: _t->modelsNamesListChanged(); break;
        case 2: _t->showMainWindow(); break;
        case 3: _t->renameModel((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->renameMaterial((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->modifyMaterial(); break;
        case 6: _t->assignMaterial(); break;
        case 7: _t->createActionsForRenamingInWindow(); break;
        case 8: _t->createActionsForMaterialDeletingInWindow(); break;
        case 9: _t->createActionsForMaterialRenamingInWindow(); break;
        case 10: _t->createActionsForMaterialAssigning(); break;
        case 11: _t->updateModelView((*reinterpret_cast< std::add_pointer_t<frames::Model*>>(_a[1]))); break;
        case 12: _t->updateModelCADView((*reinterpret_cast< std::add_pointer_t<frames::Part*>>(_a[1]))); break;
        case 13: _t->updateModelCADWithShapeView((*reinterpret_cast< std::add_pointer_t<frames::Part*>>(_a[1]))); break;
        case 14: _t->showCurrentModel((*reinterpret_cast< std::add_pointer_t<visualization::ScaleTagViewPort*>>(_a[1]))); break;
        case 15: _t->viewResult(); break;
        case 16: _t->showMeshEdges(); break;
        case 17: { visualization::ScaleTagViewPort* _r = _t->createViewport();
            if (_a[0]) *reinterpret_cast< visualization::ScaleTagViewPort**>(_a[0]) = std::move(_r); }  break;
        case 18: _t->showCurrentModelInCurrentViewPort((*reinterpret_cast< std::add_pointer_t<Model*>>(_a[1]))); break;
        case 19: { ScaleTagTreeView* _r = _t->getTreeView();
            if (_a[0]) *reinterpret_cast< ScaleTagTreeView**>(_a[0]) = std::move(_r); }  break;
        case 20: _t->updateItemInTreeView((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->updateTreeView(); break;
        case 22: _t->StartSelection(); break;
        case 23: _t->EndSelection(); break;
        case 24: _t->mapPBCSurfaces(); break;
        case 25: _t->verifyPBCSurfaces(); break;
        case 26: _t->generatePBCMesh(); break;
        case 27: _t->initializeView(); break;
        case 28: { scaletag::visualization::Visualization* _r = _t->getVisualization();
            if (_a[0]) *reinterpret_cast< scaletag::visualization::Visualization**>(_a[0]) = std::move(_r); }  break;
        case 29: { QStringList _r = _t->getModelsNamesList();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 30: { QString _r = _t->nameAt((*reinterpret_cast< std::add_pointer_t<qsizetype>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 31: { scaletag::controllers::MainWindowController* _r = _t->getMainWindowController();
            if (_a[0]) *reinterpret_cast< scaletag::controllers::MainWindowController**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Model* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ObjectFactory::*)();
            if (_t _q_method = &ObjectFactory::qmlWelcomeViewPassed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ObjectFactory::*)();
            if (_t _q_method = &ObjectFactory::modelsNamesListChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< scaletag::controllers::MainWindowController* >(); break;
        case 4:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< scaletag::frames::Project* >(); break;
        case 2:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< scaletag::frames::WorkDirectory* >(); break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< scaletag::visualization::Visualization* >(); break;
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ObjectFactory *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< scaletag::visualization::Visualization**>(_v) = _t->getVisualization(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->getModelsNamesList(); break;
        case 2: *reinterpret_cast< scaletag::frames::WorkDirectory**>(_v) = _t->getWorkDirectory(); break;
        case 3: *reinterpret_cast< scaletag::controllers::MainWindowController**>(_v) = _t->getMainWindowController(); break;
        case 4: *reinterpret_cast< scaletag::frames::Project**>(_v) = _t->getProject(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *scaletag::frames::ObjectFactory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::frames::ObjectFactory::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEObjectFactoryENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return IObjectFactory::qt_metacast(_clname);
}

int scaletag::frames::ObjectFactory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IObjectFactory::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void scaletag::frames::ObjectFactory::qmlWelcomeViewPassed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void scaletag::frames::ObjectFactory::modelsNamesListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
