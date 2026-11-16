/****************************************************************************
** Meta object code from reading C++ file 'mainwindowcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../source/controllers/mainwindowcontroller.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindowcontroller.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS = QtMocHelpers::stringData(
    "scaletag::controllers::MainWindowController"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS_t qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 43)   // "scaletag::controllers::MainWi..."
    },
    "scaletag::controllers::MainWindowController"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS[] = {

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

Q_CONSTINIT const QMetaObject scaletag::controllers::MainWindowController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindowController, std::true_type>
    >,
    nullptr
} };

void scaletag::controllers::MainWindowController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *scaletag::controllers::MainWindowController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *scaletag::controllers::MainWindowController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSscaletagSCOPEcontrollersSCOPEMainWindowControllerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int scaletag::controllers::MainWindowController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
