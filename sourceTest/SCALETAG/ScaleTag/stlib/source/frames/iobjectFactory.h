 /*=========================================================================
    2
    3   Program:   SCALETAG
    4   Module:    visualization.h
    5
    6   Copyright (c) Dela Ahose
    7   All rights reserved.
    8   See Copyright.txt or http://www.scaletag.com/Copyright.htm for details.
    9
   10      This software is distributed WITHOUT ANY WARRANTY; without even
   11      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
   12      PURPOSE.  See the above copyright notice for more information.
   13
   14 =========================================================================*/

#ifndef IOBJECTFACTORY_H
#define IOBJECTFACTORY_H

#include <QObject>
#include<stlib_global.h>

namespace scaletag {
    namespace frames {


class STLIB_EXPORT IObjectFactory : public QObject
{
    Q_OBJECT
public:
    explicit IObjectFactory(QObject *parent = nullptr);

signals:

};
    }//namespace frames

} // namespace scaletag

#endif // IOBJECTFACTORY_H
