/****************************************************************************
** Meta object code from reading C++ file 'masterpbc.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/models/masterpbc.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'masterpbc.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS = QtMocHelpers::stringData(
    "scaletag::models::MasterPBC",
    "setMappedFace",
    "",
    "IVtk_IdType",
    "faceId1",
    "faceId2",
    "getMappedFaces",
    "std::vector<std::pair<IVtk_IdType,IVtk_IdType>>",
    "clearAllMappedFaces",
    "setSurfaceId",
    "checkFaceId",
    "id",
    "initSelection",
    "IVtk_SelectionMode",
    "mode"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS_t {
    uint offsetsAndSizes[30];
    char stringdata0[28];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[8];
    char stringdata5[8];
    char stringdata6[15];
    char stringdata7[48];
    char stringdata8[20];
    char stringdata9[13];
    char stringdata10[12];
    char stringdata11[3];
    char stringdata12[14];
    char stringdata13[19];
    char stringdata14[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS = {
    {
        QT_MOC_LITERAL(0, 27),  // "scaletag::models::MasterPBC"
        QT_MOC_LITERAL(28, 13),  // "setMappedFace"
        QT_MOC_LITERAL(42, 0),  // ""
        QT_MOC_LITERAL(43, 11),  // "IVtk_IdType"
        QT_MOC_LITERAL(55, 7),  // "faceId1"
        QT_MOC_LITERAL(63, 7),  // "faceId2"
        QT_MOC_LITERAL(71, 14),  // "getMappedFaces"
        QT_MOC_LITERAL(86, 47),  // "std::vector<std::pair<IVtk_Id..."
        QT_MOC_LITERAL(134, 19),  // "clearAllMappedFaces"
        QT_MOC_LITERAL(154, 12),  // "setSurfaceId"
        QT_MOC_LITERAL(167, 11),  // "checkFaceId"
        QT_MOC_LITERAL(179, 2),  // "id"
        QT_MOC_LITERAL(182, 13),  // "initSelection"
        QT_MOC_LITERAL(196, 18),  // "IVtk_SelectionMode"
        QT_MOC_LITERAL(215, 4)   // "mode"
    },
    "scaletag::models::MasterPBC",
    "setMappedFace",
    "",
    "IVtk_IdType",
    "faceId1",
    "faceId2",
    "getMappedFaces",
    "std::vector<std::pair<IVtk_IdType,IVtk_IdType>>",
    "clearAllMappedFaces",
    "setSurfaceId",
    "checkFaceId",
    "id",
    "initSelection",
    "IVtk_SelectionMode",
    "mode"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   50,    2, 0x0a,    1 /* Public */,
       6,    0,   55,    2, 0x0a,    4 /* Public */,
       8,    0,   56,    2, 0x0a,    5 /* Public */,
       9,    0,   57,    2, 0x0a,    6 /* Public */,
      10,    1,   58,    2, 0x0a,    7 /* Public */,
      12,    1,   61,    2, 0x0a,    9 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    0x80000000 | 7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 3,   11,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::models::MasterPBC::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MasterPBC, std::true_type>,
        // method 'setMappedFace'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<IVtk_IdType, std::false_type>,
        QtPrivate::TypeAndForceComplete<IVtk_IdType, std::false_type>,
        // method 'getMappedFaces'
        QtPrivate::TypeAndForceComplete<std::vector<std::pair<IVtk_IdType,IVtk_IdType>>, std::false_type>,
        // method 'clearAllMappedFaces'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setSurfaceId'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkFaceId'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<IVtk_IdType, std::false_type>,
        // method 'initSelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<IVtk_SelectionMode, std::false_type>
    >,
    nullptr
} };

void scaletag::models::MasterPBC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MasterPBC *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setMappedFace((*reinterpret_cast< std::add_pointer_t<IVtk_IdType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<IVtk_IdType>>(_a[2]))); break;
        case 1: { std::vector<std::pair<IVtk_IdType,IVtk_IdType>> _r = _t->getMappedFaces();
            if (_a[0]) *reinterpret_cast< std::vector<std::pair<IVtk_IdType,IVtk_IdType>>*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->clearAllMappedFaces(); break;
        case 3: _t->setSurfaceId(); break;
        case 4: { bool _r = _t->checkFaceId((*reinterpret_cast< std::add_pointer_t<IVtk_IdType>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->initSelection((*reinterpret_cast< std::add_pointer_t<IVtk_SelectionMode>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *scaletag::models::MasterPBC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::models::MasterPBC::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEMasterPBCENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::models::MasterPBC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS = QtMocHelpers::stringData(
    "scaletag::models::PBCConstraintsManagerWidget",
    "aSurfaceIsSelected",
    "",
    "clearAllFired",
    "closeWidget"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS_t {
    uint offsetsAndSizes[10];
    char stringdata0[46];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 45),  // "scaletag::models::PBCConstrai..."
        QT_MOC_LITERAL(46, 18),  // "aSurfaceIsSelected"
        QT_MOC_LITERAL(65, 0),  // ""
        QT_MOC_LITERAL(66, 13),  // "clearAllFired"
        QT_MOC_LITERAL(80, 11)   // "closeWidget"
    },
    "scaletag::models::PBCConstraintsManagerWidget",
    "aSurfaceIsSelected",
    "",
    "clearAllFired",
    "closeWidget"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    1 /* Public */,
       3,    0,   33,    2, 0x06,    2 /* Public */,
       4,    0,   34,    2, 0x06,    3 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::models::PBCConstraintsManagerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PBCConstraintsManagerWidget, std::true_type>,
        // method 'aSurfaceIsSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearAllFired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'closeWidget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::models::PBCConstraintsManagerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PBCConstraintsManagerWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->aSurfaceIsSelected(); break;
        case 1: _t->clearAllFired(); break;
        case 2: _t->closeWidget(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PBCConstraintsManagerWidget::*)();
            if (_t _q_method = &PBCConstraintsManagerWidget::aSurfaceIsSelected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PBCConstraintsManagerWidget::*)();
            if (_t _q_method = &PBCConstraintsManagerWidget::clearAllFired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PBCConstraintsManagerWidget::*)();
            if (_t _q_method = &PBCConstraintsManagerWidget::closeWidget; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *scaletag::models::PBCConstraintsManagerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::models::PBCConstraintsManagerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEmodelsSCOPEPBCConstraintsManagerWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int scaletag::models::PBCConstraintsManagerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void scaletag::models::PBCConstraintsManagerWidget::aSurfaceIsSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void scaletag::models::PBCConstraintsManagerWidget::clearAllFired()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void scaletag::models::PBCConstraintsManagerWidget::closeWidget()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
