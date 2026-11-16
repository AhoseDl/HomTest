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

#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QObject>
#include<stlib_global.h>
#include<window/mainwindow.h>
#include<QDockWidget>
#include<QWindow>
#include<QWindow>

namespace scaletag {
namespace controllers {

class STLIB_EXPORT MainWindowController : public QObject
{
    Q_OBJECT
public:
    explicit MainWindowController(QObject *parent = nullptr);

    window::MainWindow* createMainWindow();
    window::MainWindow* getWidgetWindow(){return widgetMainWindow.data();}
    //void show(){widgetMainWindow->show();}
    QWidget* getQmlMainWindowContainer(){return qmlMainWindowContainer.data();}
//    void hideQmlMainWindow(){qmlMainWindow->setVisible(false); qmlMainWindow->hide();
//        /*qmlMainWindow->close();*/ qDebug()<<"qml : "<<qmlMainWindow.data();
//        //qmlMainWindow->show();
//    }

    void setQmlMainWindow(QWindow* _qWindow){
    this->setParent(_qWindow);
        qmlMainWindow.reset(_qWindow);}
private:

    QSharedPointer<QWindow> qmlMainWindow;
    QSharedPointer<window::MainWindow> widgetMainWindow;
    QSharedPointer<QWidget> qmlMainWindowContainer;

signals:

};

} // namespace controllers
} // namespace scaletag

#endif // MAINWINDOWCONTROLLER_H
