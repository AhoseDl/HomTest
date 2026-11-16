/****************************************************************************
** Meta object code from reading C++ file 'scaletagdialogs.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/window/scaletagdialogs.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scaletagdialogs.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::RenameModelWidget",
    "editingEndedWith",
    "",
    "fromName",
    "toName",
    "onEditingFinished"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[36];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[7];
    char stringdata5[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 35),  // "scaletag::window::RenameModel..."
        QT_MOC_LITERAL(36, 16),  // "editingEndedWith"
        QT_MOC_LITERAL(53, 0),  // ""
        QT_MOC_LITERAL(54, 8),  // "fromName"
        QT_MOC_LITERAL(63, 6),  // "toName"
        QT_MOC_LITERAL(70, 17)   // "onEditingFinished"
    },
    "scaletag::window::RenameModelWidget",
    "editingEndedWith",
    "",
    "fromName",
    "toName",
    "onEditingFinished"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS[] = {

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
       1,    2,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   31,    2, 0x08,    4 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::RenameModelWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RenameModelWidget, std::true_type>,
        // method 'editingEndedWith'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onEditingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::window::RenameModelWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenameModelWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->editingEndedWith((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->onEditingFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RenameModelWidget::*)(const QString & , const QString & );
            if (_t _q_method = &RenameModelWidget::editingEndedWith; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::window::RenameModelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::RenameModelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameModelWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int scaletag::window::RenameModelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void scaletag::window::RenameModelWidget::editingEndedWith(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::LinearElasticMaterialWidget",
    "editingEndedWith",
    "",
    "Material",
    "_mat",
    "editingMaterialEndedWith",
    "_materialToModifyName",
    "operationCancelled",
    "onEditingFinished"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[46];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[5];
    char stringdata5[25];
    char stringdata6[22];
    char stringdata7[19];
    char stringdata8[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 45),  // "scaletag::window::LinearElast..."
        QT_MOC_LITERAL(46, 16),  // "editingEndedWith"
        QT_MOC_LITERAL(63, 0),  // ""
        QT_MOC_LITERAL(64, 8),  // "Material"
        QT_MOC_LITERAL(73, 4),  // "_mat"
        QT_MOC_LITERAL(78, 24),  // "editingMaterialEndedWith"
        QT_MOC_LITERAL(103, 21),  // "_materialToModifyName"
        QT_MOC_LITERAL(125, 18),  // "operationCancelled"
        QT_MOC_LITERAL(144, 17)   // "onEditingFinished"
    },
    "scaletag::window::LinearElasticMaterialWidget",
    "editingEndedWith",
    "",
    "Material",
    "_mat",
    "editingMaterialEndedWith",
    "_materialToModifyName",
    "operationCancelled",
    "onEditingFinished"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,
       5,    2,   41,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   46,    2, 0x08,    6 /* Private */,
       8,    0,   47,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::LinearElasticMaterialWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LinearElasticMaterialWidget, std::true_type>,
        // method 'editingEndedWith'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Material, std::false_type>,
        // method 'editingMaterialEndedWith'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Material, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'operationCancelled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::window::LinearElasticMaterialWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LinearElasticMaterialWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->editingEndedWith((*reinterpret_cast< std::add_pointer_t<Material>>(_a[1]))); break;
        case 1: _t->editingMaterialEndedWith((*reinterpret_cast< std::add_pointer_t<Material>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->operationCancelled(); break;
        case 3: _t->onEditingFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LinearElasticMaterialWidget::*)(Material );
            if (_t _q_method = &LinearElasticMaterialWidget::editingEndedWith; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LinearElasticMaterialWidget::*)(Material , QString );
            if (_t _q_method = &LinearElasticMaterialWidget::editingMaterialEndedWith; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::window::LinearElasticMaterialWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::LinearElasticMaterialWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPELinearElasticMaterialWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int scaletag::window::LinearElasticMaterialWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void scaletag::window::LinearElasticMaterialWidget::editingEndedWith(Material _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void scaletag::window::LinearElasticMaterialWidget::editingMaterialEndedWith(Material _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::RenameMaterialWidget",
    "editingEndedWith",
    "",
    "fromName",
    "toName",
    "onEditingFinished"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[39];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[7];
    char stringdata5[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 38),  // "scaletag::window::RenameMater..."
        QT_MOC_LITERAL(39, 16),  // "editingEndedWith"
        QT_MOC_LITERAL(56, 0),  // ""
        QT_MOC_LITERAL(57, 8),  // "fromName"
        QT_MOC_LITERAL(66, 6),  // "toName"
        QT_MOC_LITERAL(73, 17)   // "onEditingFinished"
    },
    "scaletag::window::RenameMaterialWidget",
    "editingEndedWith",
    "",
    "fromName",
    "toName",
    "onEditingFinished"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS[] = {

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
       1,    2,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   31,    2, 0x08,    4 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::RenameMaterialWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RenameMaterialWidget, std::true_type>,
        // method 'editingEndedWith'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onEditingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::window::RenameMaterialWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenameMaterialWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->editingEndedWith((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->onEditingFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RenameMaterialWidget::*)(const QString & , const QString & );
            if (_t _q_method = &RenameMaterialWidget::editingEndedWith; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::window::RenameMaterialWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::RenameMaterialWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPERenameMaterialWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int scaletag::window::RenameMaterialWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void scaletag::window::RenameMaterialWidget::editingEndedWith(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::ViewPannel",
    "viewResultsWith",
    "",
    "fieldName",
    "comp",
    "timeRank",
    "scaleFactor",
    "showDeformed",
    "requestView",
    "updateCompCombo"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[29];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[5];
    char stringdata5[9];
    char stringdata6[12];
    char stringdata7[13];
    char stringdata8[12];
    char stringdata9[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 28),  // "scaletag::window::ViewPannel"
        QT_MOC_LITERAL(29, 15),  // "viewResultsWith"
        QT_MOC_LITERAL(45, 0),  // ""
        QT_MOC_LITERAL(46, 9),  // "fieldName"
        QT_MOC_LITERAL(56, 4),  // "comp"
        QT_MOC_LITERAL(61, 8),  // "timeRank"
        QT_MOC_LITERAL(70, 11),  // "scaleFactor"
        QT_MOC_LITERAL(82, 12),  // "showDeformed"
        QT_MOC_LITERAL(95, 11),  // "requestView"
        QT_MOC_LITERAL(107, 15)   // "updateCompCombo"
    },
    "scaletag::window::ViewPannel",
    "viewResultsWith",
    "",
    "fieldName",
    "comp",
    "timeRank",
    "scaleFactor",
    "showDeformed",
    "requestView",
    "updateCompCombo"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    5,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   43,    2, 0x0a,    7 /* Public */,
       9,    0,   44,    2, 0x0a,    8 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Bool,    3,    4,    5,    6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::ViewPannel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ViewPannel, std::true_type>,
        // method 'viewResultsWith'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'requestView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateCompCombo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::window::ViewPannel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ViewPannel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->viewResultsWith((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[5]))); break;
        case 1: _t->requestView(); break;
        case 2: _t->updateCompCombo(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ViewPannel::*)(const QString & , int , int , double , bool );
            if (_t _q_method = &ViewPannel::viewResultsWith; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::window::ViewPannel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::ViewPannel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEViewPannelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int scaletag::window::ViewPannel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void scaletag::window::ViewPannel::viewResultsWith(const QString & _t1, int _t2, int _t3, double _t4, bool _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
