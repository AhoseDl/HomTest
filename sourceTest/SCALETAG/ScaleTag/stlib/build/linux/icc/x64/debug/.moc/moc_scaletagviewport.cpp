/****************************************************************************
** Meta object code from reading C++ file 'scaletagviewport.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/visualization/scaletagviewport.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scaletagviewport.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS = QtMocHelpers::stringData(
    "scaletag::visualization::ScaleTagViewPort",
    "selected",
    "",
    "ScaleTagViewPort*",
    "_scaleTagViewPort",
    "viewPortDeleted",
    "_numViewPort",
    "setCurrentModel",
    "frames::Model*",
    "_model",
    "onWindowDeleted",
    "_window",
    "view"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS_t {
    uint offsetsAndSizes[26];
    char stringdata0[42];
    char stringdata1[9];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[18];
    char stringdata5[16];
    char stringdata6[13];
    char stringdata7[16];
    char stringdata8[15];
    char stringdata9[7];
    char stringdata10[16];
    char stringdata11[8];
    char stringdata12[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS = {
    {
        QT_MOC_LITERAL(0, 41),  // "scaletag::visualization::Scal..."
        QT_MOC_LITERAL(42, 8),  // "selected"
        QT_MOC_LITERAL(51, 0),  // ""
        QT_MOC_LITERAL(52, 17),  // "ScaleTagViewPort*"
        QT_MOC_LITERAL(70, 17),  // "_scaleTagViewPort"
        QT_MOC_LITERAL(88, 15),  // "viewPortDeleted"
        QT_MOC_LITERAL(104, 12),  // "_numViewPort"
        QT_MOC_LITERAL(117, 15),  // "setCurrentModel"
        QT_MOC_LITERAL(133, 14),  // "frames::Model*"
        QT_MOC_LITERAL(148, 6),  // "_model"
        QT_MOC_LITERAL(155, 15),  // "onWindowDeleted"
        QT_MOC_LITERAL(171, 7),  // "_window"
        QT_MOC_LITERAL(179, 4)   // "view"
    },
    "scaletag::visualization::ScaleTagViewPort",
    "selected",
    "",
    "ScaleTagViewPort*",
    "_scaleTagViewPort",
    "viewPortDeleted",
    "_numViewPort",
    "setCurrentModel",
    "frames::Model*",
    "_model",
    "onWindowDeleted",
    "_window",
    "view"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    1,   50,    2, 0x0a,    5 /* Public */,
      10,    1,   53,    2, 0x0a,    7 /* Public */,
      12,    0,   56,    2, 0x0a,    9 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QObjectStar,   11,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::visualization::ScaleTagViewPort::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ScaleTagViewPort, std::true_type>,
        // method 'selected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ScaleTagViewPort *, std::false_type>,
        // method 'viewPortDeleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setCurrentModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<frames::Model *, std::false_type>,
        // method 'onWindowDeleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QObject *, std::false_type>,
        // method 'view'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::visualization::ScaleTagViewPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ScaleTagViewPort *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->selected((*reinterpret_cast< std::add_pointer_t<ScaleTagViewPort*>>(_a[1]))); break;
        case 1: _t->viewPortDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->setCurrentModel((*reinterpret_cast< std::add_pointer_t<frames::Model*>>(_a[1]))); break;
        case 3: _t->onWindowDeleted((*reinterpret_cast< std::add_pointer_t<QObject*>>(_a[1]))); break;
        case 4: _t->view(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< ScaleTagViewPort* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ScaleTagViewPort::*)(ScaleTagViewPort * );
            if (_t _q_method = &ScaleTagViewPort::selected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ScaleTagViewPort::*)(int );
            if (_t _q_method = &ScaleTagViewPort::viewPortDeleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::visualization::ScaleTagViewPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::visualization::ScaleTagViewPort::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEvisualizationSCOPEScaleTagViewPortENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::visualization::ScaleTagViewPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void scaletag::visualization::ScaleTagViewPort::selected(ScaleTagViewPort * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void scaletag::visualization::ScaleTagViewPort::viewPortDeleted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
