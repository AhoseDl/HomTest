/****************************************************************************
** Meta object code from reading C++ file 'solaris.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/controllers/solaris.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'solaris.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS = QtMocHelpers::stringData(
    "scaletag::controllers::Solaris",
    "getObjectFactory",
    "frames::IObjectFactory*",
    "",
    "initObjectFactory",
    "ui_ObjectFactory",
    "scaletag::frames::IObjectFactory*"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[31];
    char stringdata1[17];
    char stringdata2[24];
    char stringdata3[1];
    char stringdata4[18];
    char stringdata5[17];
    char stringdata6[34];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS = {
    {
        QT_MOC_LITERAL(0, 30),  // "scaletag::controllers::Solaris"
        QT_MOC_LITERAL(31, 16),  // "getObjectFactory"
        QT_MOC_LITERAL(48, 23),  // "frames::IObjectFactory*"
        QT_MOC_LITERAL(72, 0),  // ""
        QT_MOC_LITERAL(73, 17),  // "initObjectFactory"
        QT_MOC_LITERAL(91, 16),  // "ui_ObjectFactory"
        QT_MOC_LITERAL(108, 33)   // "scaletag::frames::IObjectFact..."
    },
    "scaletag::controllers::Solaris",
    "getObjectFactory",
    "frames::IObjectFactory*",
    "",
    "initObjectFactory",
    "ui_ObjectFactory",
    "scaletag::frames::IObjectFactory*"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       1,   28, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    3, 0x102,    2 /* Public | MethodIsConst  */,
       4,    0,   27,    3, 0x02,    3 /* Public */,

 // methods: parameters
    0x80000000 | 2,
    QMetaType::Void,

 // properties: name, type, flags
       5, 0x80000000 | 6, 0x00015409, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::controllers::Solaris::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS_t,
        // property 'ui_ObjectFactory'
        QtPrivate::TypeAndForceComplete<scaletag::frames::IObjectFactory*, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Solaris, std::true_type>,
        // method 'getObjectFactory'
        QtPrivate::TypeAndForceComplete<frames::IObjectFactory *, std::false_type>,
        // method 'initObjectFactory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::controllers::Solaris::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Solaris *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { frames::IObjectFactory* _r = _t->getObjectFactory();
            if (_a[0]) *reinterpret_cast< frames::IObjectFactory**>(_a[0]) = std::move(_r); }  break;
        case 1: _t->initObjectFactory(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< scaletag::frames::IObjectFactory* >(); break;
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<Solaris *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< scaletag::frames::IObjectFactory**>(_v) = _t->getObjectFactory(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *scaletag::controllers::Solaris::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::controllers::Solaris::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPESolarisENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::controllers::Solaris::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
