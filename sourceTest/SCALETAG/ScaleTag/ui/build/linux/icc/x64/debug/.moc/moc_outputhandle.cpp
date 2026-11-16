/****************************************************************************
** Meta object code from reading C++ file 'outputhandle.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/window/outputhandle.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'outputhandle.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::Console",
    "appendTextSafe",
    "",
    "text",
    "handleAppendText"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS_t {
    uint offsetsAndSizes[10];
    char stringdata0[26];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS = {
    {
        QT_MOC_LITERAL(0, 25),  // "scaletag::window::Console"
        QT_MOC_LITERAL(26, 14),  // "appendTextSafe"
        QT_MOC_LITERAL(41, 0),  // ""
        QT_MOC_LITERAL(42, 4),  // "text"
        QT_MOC_LITERAL(47, 16)   // "handleAppendText"
    },
    "scaletag::window::Console",
    "appendTextSafe",
    "",
    "text",
    "handleAppendText"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS[] = {

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
       1,    1,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   29,    2, 0x08,    3 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::Console::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Console, std::true_type>,
        // method 'appendTextSafe'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleAppendText'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void scaletag::window::Console::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Console *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->appendTextSafe((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->handleAppendText((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Console::*)(const QString & );
            if (_t _q_method = &Console::appendTextSafe; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *scaletag::window::Console::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::Console::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEConsoleENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "std::basic_streambuf<char>"))
        return static_cast< std::basic_streambuf<char>*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::window::Console::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void scaletag::window::Console::appendTextSafe(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::OutputDevice"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS = {
    {
        QT_MOC_LITERAL(0, 30)   // "scaletag::window::OutputDevice"
    },
    "scaletag::window::OutputDevice"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::OutputDevice::staticMetaObject = { {
    QMetaObject::SuperData::link<QIODevice::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OutputDevice, std::true_type>
    >,
    nullptr
} };

void scaletag::window::OutputDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *scaletag::window::OutputDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::OutputDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputDeviceENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QIODevice::qt_metacast(_clname);
}

int scaletag::window::OutputDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QIODevice::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS = QtMocHelpers::stringData(
    "scaletag::window::OutputTo"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS = {
    {
        QT_MOC_LITERAL(0, 26)   // "scaletag::window::OutputTo"
    },
    "scaletag::window::OutputTo"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject scaletag::window::OutputTo::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OutputTo, std::true_type>
    >,
    nullptr
} };

void scaletag::window::OutputTo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *scaletag::window::OutputTo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::window::OutputTo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEwindowSCOPEOutputToENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::window::OutputTo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
