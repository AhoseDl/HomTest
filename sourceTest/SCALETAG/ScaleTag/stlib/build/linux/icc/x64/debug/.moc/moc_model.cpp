/****************************************************************************
** Meta object code from reading C++ file 'model.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/frames/model.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'model.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS = QtMocHelpers::stringData(
    "scaletag::frames::Model",
    "modelModified",
    "",
    "Model*",
    "_model",
    "modelCADModified",
    "Part*",
    "_part",
    "modelCADWithShapeModified",
    "headyWorkFinished",
    "askViewResults",
    "materialModified",
    "_name",
    "needTreeViewUpdate",
    "runPBCAnalysis",
    "runPBCAnalysisLauncher",
    "viewResults",
    "addMaterial",
    "Material",
    "_mat",
    "updateMaterial",
    "_currentMat",
    "_oldMaterialName",
    "getMaterials",
    "getMaterial",
    "std::string",
    "matName",
    "deleteMaterial",
    "_matName",
    "renameMaterial",
    "oldName",
    "newName"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS_t {
    uint offsetsAndSizes[64];
    char stringdata0[24];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[7];
    char stringdata5[17];
    char stringdata6[6];
    char stringdata7[6];
    char stringdata8[26];
    char stringdata9[18];
    char stringdata10[15];
    char stringdata11[17];
    char stringdata12[6];
    char stringdata13[19];
    char stringdata14[15];
    char stringdata15[23];
    char stringdata16[12];
    char stringdata17[12];
    char stringdata18[9];
    char stringdata19[5];
    char stringdata20[15];
    char stringdata21[12];
    char stringdata22[17];
    char stringdata23[13];
    char stringdata24[12];
    char stringdata25[12];
    char stringdata26[8];
    char stringdata27[15];
    char stringdata28[9];
    char stringdata29[15];
    char stringdata30[8];
    char stringdata31[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 23),  // "scaletag::frames::Model"
        QT_MOC_LITERAL(24, 13),  // "modelModified"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 6),  // "Model*"
        QT_MOC_LITERAL(46, 6),  // "_model"
        QT_MOC_LITERAL(53, 16),  // "modelCADModified"
        QT_MOC_LITERAL(70, 5),  // "Part*"
        QT_MOC_LITERAL(76, 5),  // "_part"
        QT_MOC_LITERAL(82, 25),  // "modelCADWithShapeModified"
        QT_MOC_LITERAL(108, 17),  // "headyWorkFinished"
        QT_MOC_LITERAL(126, 14),  // "askViewResults"
        QT_MOC_LITERAL(141, 16),  // "materialModified"
        QT_MOC_LITERAL(158, 5),  // "_name"
        QT_MOC_LITERAL(164, 18),  // "needTreeViewUpdate"
        QT_MOC_LITERAL(183, 14),  // "runPBCAnalysis"
        QT_MOC_LITERAL(198, 22),  // "runPBCAnalysisLauncher"
        QT_MOC_LITERAL(221, 11),  // "viewResults"
        QT_MOC_LITERAL(233, 11),  // "addMaterial"
        QT_MOC_LITERAL(245, 8),  // "Material"
        QT_MOC_LITERAL(254, 4),  // "_mat"
        QT_MOC_LITERAL(259, 14),  // "updateMaterial"
        QT_MOC_LITERAL(274, 11),  // "_currentMat"
        QT_MOC_LITERAL(286, 16),  // "_oldMaterialName"
        QT_MOC_LITERAL(303, 12),  // "getMaterials"
        QT_MOC_LITERAL(316, 11),  // "getMaterial"
        QT_MOC_LITERAL(328, 11),  // "std::string"
        QT_MOC_LITERAL(340, 7),  // "matName"
        QT_MOC_LITERAL(348, 14),  // "deleteMaterial"
        QT_MOC_LITERAL(363, 8),  // "_matName"
        QT_MOC_LITERAL(372, 14),  // "renameMaterial"
        QT_MOC_LITERAL(387, 7),  // "oldName"
        QT_MOC_LITERAL(395, 7)   // "newName"
    },
    "scaletag::frames::Model",
    "modelModified",
    "",
    "Model*",
    "_model",
    "modelCADModified",
    "Part*",
    "_part",
    "modelCADWithShapeModified",
    "headyWorkFinished",
    "askViewResults",
    "materialModified",
    "_name",
    "needTreeViewUpdate",
    "runPBCAnalysis",
    "runPBCAnalysisLauncher",
    "viewResults",
    "addMaterial",
    "Material",
    "_mat",
    "updateMaterial",
    "_currentMat",
    "_oldMaterialName",
    "getMaterials",
    "getMaterial",
    "std::string",
    "matName",
    "deleteMaterial",
    "_matName",
    "renameMaterial",
    "oldName",
    "newName"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEframesSCOPEModelENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  116,    2, 0x06,    1 /* Public */,
       5,    1,  119,    2, 0x06,    3 /* Public */,
       8,    1,  122,    2, 0x06,    5 /* Public */,
       9,    0,  125,    2, 0x06,    7 /* Public */,
      10,    0,  126,    2, 0x06,    8 /* Public */,
      11,    1,  127,    2, 0x06,    9 /* Public */,
      11,    0,  130,    2, 0x26,   11 /* Public | MethodCloned */,
      13,    0,  131,    2, 0x06,   12 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    0,  132,    2, 0x0a,   13 /* Public */,
      15,    0,  133,    2, 0x0a,   14 /* Public */,
      16,    0,  134,    2, 0x0a,   15 /* Public */,
      17,    1,  135,    2, 0x0a,   16 /* Public */,
      20,    2,  138,    2, 0x0a,   18 /* Public */,
      23,    0,  143,    2, 0x0a,   21 /* Public */,
      24,    1,  144,    2, 0x10a,   22 /* Public | MethodIsConst  */,
      27,    1,  147,    2, 0x0a,   24 /* Public */,
      29,    2,  150,    2, 0x0a,   26 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, 0x80000000 | 18,   19,
    QMetaType::QString, 0x80000000 | 18, QMetaType::QString,   21,   22,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::QString, QMetaType::QString,   28,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   30,   31,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::frames::Model::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEframesSCOPEModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Model, std::true_type>,
        // method 'modelModified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Model *, std::false_type>,
        // method 'modelCADModified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Part *, std::false_type>,
        // method 'modelCADWithShapeModified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Part *, std::false_type>,
        // method 'headyWorkFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'askViewResults'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'materialModified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'materialModified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'needTreeViewUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runPBCAnalysis'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runPBCAnalysisLauncher'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'viewResults'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addMaterial'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Material &, std::false_type>,
        // method 'updateMaterial'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Material &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getMaterials'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getMaterial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::string &, std::false_type>,
        // method 'deleteMaterial'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'renameMaterial'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void scaletag::frames::Model::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Model *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->modelModified((*reinterpret_cast< std::add_pointer_t<Model*>>(_a[1]))); break;
        case 1: _t->modelCADModified((*reinterpret_cast< std::add_pointer_t<Part*>>(_a[1]))); break;
        case 2: _t->modelCADWithShapeModified((*reinterpret_cast< std::add_pointer_t<Part*>>(_a[1]))); break;
        case 3: _t->headyWorkFinished(); break;
        case 4: _t->askViewResults(); break;
        case 5: _t->materialModified((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->materialModified(); break;
        case 7: _t->needTreeViewUpdate(); break;
        case 8: _t->runPBCAnalysis(); break;
        case 9: _t->runPBCAnalysisLauncher(); break;
        case 10: _t->viewResults(); break;
        case 11: { QString _r = _t->addMaterial((*reinterpret_cast< std::add_pointer_t<Material>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 12: { QString _r = _t->updateMaterial((*reinterpret_cast< std::add_pointer_t<Material>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->getMaterials(); break;
        case 14: _t->getMaterial((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 15: { QString _r = _t->deleteMaterial((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 16: { QString _r = _t->renameMaterial((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Model* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Part* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Part* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Model::*)(Model * );
            if (_t _q_method = &Model::modelModified; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Model::*)(Part * );
            if (_t _q_method = &Model::modelCADModified; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Model::*)(Part * );
            if (_t _q_method = &Model::modelCADWithShapeModified; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Model::*)();
            if (_t _q_method = &Model::headyWorkFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Model::*)();
            if (_t _q_method = &Model::askViewResults; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Model::*)(const QString & );
            if (_t _q_method = &Model::materialModified; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Model::*)();
            if (_t _q_method = &Model::needTreeViewUpdate; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::frames::Model::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::frames::Model::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::frames::Model::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void scaletag::frames::Model::modelModified(Model * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void scaletag::frames::Model::modelCADModified(Part * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void scaletag::frames::Model::modelCADWithShapeModified(Part * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void scaletag::frames::Model::headyWorkFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void scaletag::frames::Model::askViewResults()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void scaletag::frames::Model::materialModified(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 7
void scaletag::frames::Model::needTreeViewUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS = QtMocHelpers::stringData(
    "scaletag::frames::HeavyWrapper",
    "heavyComputationEnd",
    "",
    "runPBCAnalysisLauncher"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS_t {
    uint offsetsAndSizes[8];
    char stringdata0[31];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS = {
    {
        QT_MOC_LITERAL(0, 30),  // "scaletag::frames::HeavyWrapper"
        QT_MOC_LITERAL(31, 19),  // "heavyComputationEnd"
        QT_MOC_LITERAL(51, 0),  // ""
        QT_MOC_LITERAL(52, 22)   // "runPBCAnalysisLauncher"
    },
    "scaletag::frames::HeavyWrapper",
    "heavyComputationEnd",
    "",
    "runPBCAnalysisLauncher"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   27,    2, 0x0a,    2 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::frames::HeavyWrapper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<HeavyWrapper, std::true_type>,
        // method 'heavyComputationEnd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runPBCAnalysisLauncher'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::frames::HeavyWrapper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HeavyWrapper *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->heavyComputationEnd(); break;
        case 1: _t->runPBCAnalysisLauncher(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HeavyWrapper::*)();
            if (_t _q_method = &HeavyWrapper::heavyComputationEnd; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *scaletag::frames::HeavyWrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::frames::HeavyWrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyWrapperENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::frames::HeavyWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void scaletag::frames::HeavyWrapper::heavyComputationEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
