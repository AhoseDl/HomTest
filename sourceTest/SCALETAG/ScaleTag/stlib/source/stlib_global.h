#ifndef STLIB_GLOBAL_H
#define STLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STLIB_LIBRARY)
#  define STLIB_EXPORT Q_DECL_EXPORT
#else
#  define STLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // STLIB_GLOBAL_H
