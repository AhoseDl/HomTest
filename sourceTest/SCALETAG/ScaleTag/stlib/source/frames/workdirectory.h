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
#ifndef WORKDIRECTORY_H
#define WORKDIRECTORY_H

#include <QObject>
#include<QDir>
#include<stlib_global.h>
#include<QFileDialog>
#include<QDebug>
#include<QDir>
#include<QString>
#include<iostream>

namespace scaletag {
namespace frames {
using namespace std;
class STLIB_EXPORT WorkDirectory : public QObject
{
    Q_OBJECT
   // std::string workDirectory;
public:
    explicit WorkDirectory(QObject *parent = nullptr);

    Q_INVOKABLE QString getDirectory();


public slots:
//    void setWorkDirectory(QString directory);
    void setWorkDirectory();
signals:

};

} // namespace window
} // namespace scaletag

#endif // WORKDIRECTORY_H
