/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/window/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::MainWindow",
    "importModelFired",
    "",
    "dir",
    "importCADFired",
    "importCADWithShapeFired",
    "runPBCAnalysisFired",
    "viewResultActionFired",
    "showMeshEdgesFired",
    "createViewPortFired",
    "createModelFired",
    "_name",
    "createNoNameModelFired",
    "deleteModelFired",
    "renameModelFired",
    "editingEndedWith",
    "from",
    "toName",
    "editingMaterialNameEndedWith",
    "mapSurfaceFired",
    "verifyPBCFired",
    "meshActionFired",
    "startSelectionFired",
    "endSelectionFired",
    "createMaterialFired",
    "Material",
    "_mat",
    "modifyMaterialFired",
    "updateMaterialFired",
    "_materialToModfyName",
    "deleteMaterialFired",
    "renameMaterialFired",
    "assignMaterialFired",
    "renameModel",
    "renameMaterial",
    "createMaterial",
    "modifyMaterial",
    "window::LinearElasticMaterialWidget*",
    "purposeTitle",
    "matName",
    "E",
    "nu"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS_t {
    uint offsetsAndSizes[84];
    char stringdata0[29];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[4];
    char stringdata4[15];
    char stringdata5[24];
    char stringdata6[20];
    char stringdata7[22];
    char stringdata8[19];
    char stringdata9[20];
    char stringdata10[17];
    char stringdata11[6];
    char stringdata12[23];
    char stringdata13[17];
    char stringdata14[17];
    char stringdata15[17];
    char stringdata16[5];
    char stringdata17[7];
    char stringdata18[29];
    char stringdata19[16];
    char stringdata20[15];
    char stringdata21[16];
    char stringdata22[20];
    char stringdata23[18];
    char stringdata24[20];
    char stringdata25[9];
    char stringdata26[5];
    char stringdata27[20];
    char stringdata28[20];
    char stringdata29[21];
    char stringdata30[20];
    char stringdata31[20];
    char stringdata32[20];
    char stringdata33[12];
    char stringdata34[15];
    char stringdata35[15];
    char stringdata36[15];
    char stringdata37[37];
    char stringdata38[13];
    char stringdata39[8];
    char stringdata40[2];
    char stringdata41[3];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 28),  // "scaletag::window::MainWindow"
        QT_MOC_LITERAL(29, 16),  // "importModelFired"
        QT_MOC_LITERAL(46, 0),  // ""
        QT_MOC_LITERAL(47, 3),  // "dir"
        QT_MOC_LITERAL(51, 14),  // "importCADFired"
        QT_MOC_LITERAL(66, 23),  // "importCADWithShapeFired"
        QT_MOC_LITERAL(90, 19),  // "runPBCAnalysisFired"
        QT_MOC_LITERAL(110, 21),  // "viewResultActionFired"
        QT_MOC_LITERAL(132, 18),  // "showMeshEdgesFired"
        QT_MOC_LITERAL(151, 19),  // "createViewPortFired"
        QT_MOC_LITERAL(171, 16),  // "createModelFired"
        QT_MOC_LITERAL(188, 5),  // "_name"
        QT_MOC_LITERAL(194, 22),  // "createNoNameModelFired"
        QT_MOC_LITERAL(217, 16),  // "deleteModelFired"
        QT_MOC_LITERAL(234, 16),  // "renameModelFired"
        QT_MOC_LITERAL(251, 16),  // "editingEndedWith"
        QT_MOC_LITERAL(268, 4),  // "from"
        QT_MOC_LITERAL(273, 6),  // "toName"
        QT_MOC_LITERAL(280, 28),  // "editingMaterialNameEndedWith"
        QT_MOC_LITERAL(309, 15),  // "mapSurfaceFired"
        QT_MOC_LITERAL(325, 14),  // "verifyPBCFired"
        QT_MOC_LITERAL(340, 15),  // "meshActionFired"
        QT_MOC_LITERAL(356, 19),  // "startSelectionFired"
        QT_MOC_LITERAL(376, 17),  // "endSelectionFired"
        QT_MOC_LITERAL(394, 19),  // "createMaterialFired"
        QT_MOC_LITERAL(414, 8),  // "Material"
        QT_MOC_LITERAL(423, 4),  // "_mat"
        QT_MOC_LITERAL(428, 19),  // "modifyMaterialFired"
        QT_MOC_LITERAL(448, 19),  // "updateMaterialFired"
        QT_MOC_LITERAL(468, 20),  // "_materialToModfyName"
        QT_MOC_LITERAL(489, 19),  // "deleteMaterialFired"
        QT_MOC_LITERAL(509, 19),  // "renameMaterialFired"
        QT_MOC_LITERAL(529, 19),  // "assignMaterialFired"
        QT_MOC_LITERAL(549, 11),  // "renameModel"
        QT_MOC_LITERAL(561, 14),  // "renameMaterial"
        QT_MOC_LITERAL(576, 14),  // "createMaterial"
        QT_MOC_LITERAL(591, 14),  // "modifyMaterial"
        QT_MOC_LITERAL(606, 36),  // "window::LinearElasticMaterial..."
        QT_MOC_LITERAL(643, 12),  // "purposeTitle"
        QT_MOC_LITERAL(656, 7),  // "matName"
        QT_MOC_LITERAL(664, 1),  // "E"
        QT_MOC_LITERAL(666, 2)   // "nu"
    },
    "scaletag::window::MainWindow",
    "importModelFired",
    "",
    "dir",
    "importCADFired",
    "importCADWithShapeFired",
    "runPBCAnalysisFired",
    "viewResultActionFired",
    "showMeshEdgesFired",
    "createViewPortFired",
    "createModelFired",
    "_name",
    "createNoNameModelFired",
    "deleteModelFired",
    "renameModelFired",
    "editingEndedWith",
    "from",
    "toName",
    "editingMaterialNameEndedWith",
    "mapSurfaceFired",
    "verifyPBCFired",
    "meshActionFired",
    "startSelectionFired",
    "endSelectionFired",
    "createMaterialFired",
    "Material",
    "_mat",
    "modifyMaterialFired",
    "updateMaterialFired",
    "_materialToModfyName",
    "deleteMaterialFired",
    "renameMaterialFired",
    "assignMaterialFired",
    "renameModel",
    "renameMaterial",
    "createMaterial",
    "modifyMaterial",
    "window::LinearElasticMaterialWidget*",
    "purposeTitle",
    "matName",
    "E",
    "nu"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      25,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  188,    2, 0x06,    1 /* Public */,
       4,    1,  191,    2, 0x06,    3 /* Public */,
       5,    1,  194,    2, 0x06,    5 /* Public */,
       6,    0,  197,    2, 0x06,    7 /* Public */,
       7,    0,  198,    2, 0x06,    8 /* Public */,
       8,    0,  199,    2, 0x06,    9 /* Public */,
       9,    0,  200,    2, 0x06,   10 /* Public */,
      10,    1,  201,    2, 0x06,   11 /* Public */,
      10,    0,  204,    2, 0x26,   13 /* Public | MethodCloned */,
      12,    0,  205,    2, 0x06,   14 /* Public */,
      13,    0,  206,    2, 0x06,   15 /* Public */,
      14,    0,  207,    2, 0x06,   16 /* Public */,
      15,    2,  208,    2, 0x06,   17 /* Public */,
      18,    2,  213,    2, 0x06,   20 /* Public */,
      19,    0,  218,    2, 0x06,   23 /* Public */,
      20,    0,  219,    2, 0x06,   24 /* Public */,
      21,    0,  220,    2, 0x06,   25 /* Public */,
      22,    0,  221,    2, 0x06,   26 /* Public */,
      23,    0,  222,    2, 0x06,   27 /* Public */,
      24,    1,  223,    2, 0x06,   28 /* Public */,
      27,    0,  226,    2, 0x06,   30 /* Public */,
      28,    2,  227,    2, 0x06,   31 /* Public */,
      30,    0,  232,    2, 0x06,   34 /* Public */,
      31,    0,  233,    2, 0x06,   35 /* Public */,
      32,    0,  234,    2, 0x06,   36 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      33,    0,  235,    2, 0x0a,   37 /* Public */,
      34,    0,  236,    2, 0x0a,   38 /* Public */,
      35,    0,  237,    2, 0x0a,   39 /* Public */,
      36,    4,  238,    2, 0x0a,   40 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25, QMetaType::QString,   26,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 37, QMetaType::QString, QMetaType::QString, QMetaType::Double, QMetaType::Double,   38,   39,   40,   41,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'importModelFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'importCADFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'importCADWithShapeFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'runPBCAnalysisFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'viewResultActionFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showMeshEdgesFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createViewPortFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createModelFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'createModelFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createNoNameModelFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteModelFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'renameModelFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'editingEndedWith'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'editingMaterialNameEndedWith'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'mapSurfaceFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'verifyPBCFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'meshActionFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startSelectionFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'endSelectionFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createMaterialFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Material, std::false_type>,
        // method 'modifyMaterialFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateMaterialFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Material, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'deleteMaterialFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'renameMaterialFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'assignMaterialFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'renameModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'renameMaterial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createMaterial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'modifyMaterial'
        QtPrivate::TypeAndForceComplete<window::LinearElasticMaterialWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

void scaletag::window::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->importModelFired((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->importCADFired((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->importCADWithShapeFired((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->runPBCAnalysisFired(); break;
        case 4: _t->viewResultActionFired(); break;
        case 5: _t->showMeshEdgesFired(); break;
        case 6: _t->createViewPortFired(); break;
        case 7: _t->createModelFired((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->createModelFired(); break;
        case 9: _t->createNoNameModelFired(); break;
        case 10: _t->deleteModelFired(); break;
        case 11: _t->renameModelFired(); break;
        case 12: _t->editingEndedWith((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->editingMaterialNameEndedWith((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->mapSurfaceFired(); break;
        case 15: _t->verifyPBCFired(); break;
        case 16: _t->meshActionFired(); break;
        case 17: _t->startSelectionFired(); break;
        case 18: _t->endSelectionFired(); break;
        case 19: _t->createMaterialFired((*reinterpret_cast< std::add_pointer_t<Material>>(_a[1]))); break;
        case 20: _t->modifyMaterialFired(); break;
        case 21: _t->updateMaterialFired((*reinterpret_cast< std::add_pointer_t<Material>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 22: _t->deleteMaterialFired(); break;
        case 23: _t->renameMaterialFired(); break;
        case 24: _t->assignMaterialFired(); break;
        case 25: _t->renameModel(); break;
        case 26: _t->renameMaterial(); break;
        case 27: _t->createMaterial(); break;
        case 28: { window::LinearElasticMaterialWidget* _r = _t->modifyMaterial((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< window::LinearElasticMaterialWidget**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(const QString & );
            if (_t _q_method = &MainWindow::importModelFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & );
            if (_t _q_method = &MainWindow::importCADFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & );
            if (_t _q_method = &MainWindow::importCADWithShapeFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::runPBCAnalysisFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::viewResultActionFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::showMeshEdgesFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::createViewPortFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & );
            if (_t _q_method = &MainWindow::createModelFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::createNoNameModelFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::deleteModelFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::renameModelFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & , const QString & );
            if (_t _q_method = &MainWindow::editingEndedWith; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & , const QString & );
            if (_t _q_method = &MainWindow::editingMaterialNameEndedWith; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::mapSurfaceFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::verifyPBCFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::meshActionFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::startSelectionFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::endSelectionFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(Material );
            if (_t _q_method = &MainWindow::createMaterialFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::modifyMaterialFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(Material , QString );
            if (_t _q_method = &MainWindow::updateMaterialFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::deleteMaterialFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::renameMaterialFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::assignMaterialFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 24;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::window::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int scaletag::window::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void scaletag::window::MainWindow::importModelFired(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void scaletag::window::MainWindow::importCADFired(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void scaletag::window::MainWindow::importCADWithShapeFired(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void scaletag::window::MainWindow::runPBCAnalysisFired()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void scaletag::window::MainWindow::viewResultActionFired()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void scaletag::window::MainWindow::showMeshEdgesFired()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void scaletag::window::MainWindow::createViewPortFired()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void scaletag::window::MainWindow::createModelFired(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 9
void scaletag::window::MainWindow::createNoNameModelFired()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void scaletag::window::MainWindow::deleteModelFired()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void scaletag::window::MainWindow::renameModelFired()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void scaletag::window::MainWindow::editingEndedWith(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void scaletag::window::MainWindow::editingMaterialNameEndedWith(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void scaletag::window::MainWindow::mapSurfaceFired()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void scaletag::window::MainWindow::verifyPBCFired()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void scaletag::window::MainWindow::meshActionFired()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void scaletag::window::MainWindow::startSelectionFired()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void scaletag::window::MainWindow::endSelectionFired()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void scaletag::window::MainWindow::createMaterialFired(Material _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void scaletag::window::MainWindow::modifyMaterialFired()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void scaletag::window::MainWindow::updateMaterialFired(Material _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void scaletag::window::MainWindow::deleteMaterialFired()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void scaletag::window::MainWindow::renameMaterialFired()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void scaletag::window::MainWindow::assignMaterialFired()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}
QT_WARNING_POP
