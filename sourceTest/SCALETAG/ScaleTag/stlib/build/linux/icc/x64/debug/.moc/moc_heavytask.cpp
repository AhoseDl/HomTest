/****************************************************************************
** Meta object code from reading C++ file 'heavytask.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/frames/heavytask.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'heavytask.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS = QtMocHelpers::stringData(
    "scaletag::frames::HeavyTaskWrapper",
    "heavyTaskFinished",
    "",
    "process"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS_t {
    uint offsetsAndSizes[8];
    char stringdata0[35];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS = {
    {
        QT_MOC_LITERAL(0, 34),  // "scaletag::frames::HeavyTaskWr..."
        QT_MOC_LITERAL(35, 17),  // "heavyTaskFinished"
        QT_MOC_LITERAL(53, 0),  // ""
        QT_MOC_LITERAL(54, 7)   // "process"
    },
    "scaletag::frames::HeavyTaskWrapper",
    "heavyTaskFinished",
    "",
    "process"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS[] = {

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

Q_CONSTINIT const QMetaObject scaletag::frames::HeavyTaskWrapper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<HeavyTaskWrapper, std::true_type>,
        // method 'heavyTaskFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'process'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void scaletag::frames::HeavyTaskWrapper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HeavyTaskWrapper *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->heavyTaskFinished(); break;
        case 1: _t->process(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HeavyTaskWrapper::*)();
            if (_t _q_method = &HeavyTaskWrapper::heavyTaskFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *scaletag::frames::HeavyTaskWrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::frames::HeavyTaskWrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEframesSCOPEHeavyTaskWrapperENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::frames::HeavyTaskWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void scaletag::frames::HeavyTaskWrapper::heavyTaskFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
