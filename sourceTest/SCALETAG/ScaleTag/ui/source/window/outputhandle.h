#ifndef OUTPUTHANDLE_H
#define OUTPUTHANDLE_H

#include<QTextEdit>
#include <QFile>
#include<qtextstream.h>
#include<QIODevice>
#include <iostream>
#include <streambuf>
#include <string>
#include<QObject>
namespace scaletag {
namespace window {
/*In this file we propose a way to redirect common stream (of std::basic_streambuf type)to QTextEdit
 * For the console class, use "Console* acout=new Console(&std::cout, scaleTagConsole);"
 *
 * For other streams, one can use the class OutputTo. ""OutputTo::getStream()" gives access
 * to the stream to be redirected in QTextEdit
 *

*/

//********************************** class Console ************
class Console : public QObject,public std::basic_streambuf<char>{
    Q_OBJECT

    std::ostream* customStream;
    std::streambuf* initialBuffer;
    std::string aString;
    QTextEdit* aTextEdit;

public:
    Console(/*QObject* _parent,*/ std::ostream* stream, QTextEdit* textEdit);
    ~Console();
    void release();

protected:
    int_type overflow(int_type v) override;
    std::streamsize xsputn(const char* p, std::streamsize n) override;

signals:
    void appendTextSafe(const QString &text);

private slots:
    void handleAppendText(const QString &text);


};



//********************************** class OutputDevice ************
class OutputDevice : public QIODevice {
    Q_OBJECT

public:
    OutputDevice(QTextEdit* textEdit);

    ~OutputDevice();

protected:
    qint64 readData(char *data, qint64 maxSize) override;

    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QTextEdit* textEdit;
};

//*********************************** class OutputTo *****************
class OutputTo : public QObject/*,public std::basic_streambuf<char> */{
    Q_OBJECT

public:
    OutputTo(QTextEdit* _textEdit);
    ~OutputTo();

    //void takeStandardOutput(QTextStream& _ss);
    QTextStream* getStream() ;

    //void init();

private:
    FILE* initialOut;
    QTextStream* tstream;
    OutputDevice* outputDevice;
    //QTextEdit& aTextEdit;
    //void (*aMessageHandler)(QtMsgType type, const QMessageLogContext&, const QString& msg);
    //std::function<void(QtMsgType type, const QMessageLogContext&, const QString& msg)> aMessageHandler;
};




} // namespace frames
} // namespace scaletag




#endif // MAINWINDOW_H
