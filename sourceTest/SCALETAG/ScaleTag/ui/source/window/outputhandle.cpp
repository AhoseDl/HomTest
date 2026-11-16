#include<outputhandle.h>
#include<iostream>

#include <QScrollBar>
/* "aTextEdit->verticalScrollBar()->setValue(aTextEdit->verticalScrollBar()->maximum());"
 * The previous line can be used at the place of the function "scrollToEnd(aTextEdit);
 * This avoids messing with the QTextCursor entirely - otherwise a user reading earlier
 * log lines gets yanked to the end on every write."
*/
namespace scaletag {
namespace window {

void scrollToEnd(QTextEdit* te) {
    QTextCursor cursor(te->document());
    cursor.movePosition(QTextCursor::End);
    te->setTextCursor(cursor);
    te->ensureCursorVisible();
}


Console::Console(/*QObject* parent,*/ std::ostream* stream, QTextEdit* textEdit): QObject(textEdit),std::basic_streambuf<char>(){
   // release();
    customStream=stream;
    initialBuffer=stream->rdbuf();
    stream->rdbuf(this);
    aTextEdit=textEdit;

    connect(this, &Console::appendTextSafe, this, &Console::handleAppendText);
    }


    Console::~Console(){
    if(!aString.empty()){aTextEdit->append(aString.c_str());
    scrollToEnd(aTextEdit); }
    release();
    }


void Console::release(){
    if(initialBuffer!=nullptr && customStream!=nullptr){
        customStream->rdbuf(initialBuffer);
    }
}

void Console::handleAppendText(const QString &text){

//    if (v == '\n') {
        aTextEdit->append(text);
        aString.erase(aString.begin(), aString.end());
        scrollToEnd(aTextEdit);
 //   }
//    else {
//        aString += v;
//    }

}

std::basic_streambuf<char>::int_type Console::overflow(int_type v){

        if (v == EOF) {
        return EOF;
        }

    if (v == '\n') {
//        aTextEdit->append(aString.c_str());
//        aString.erase(aString.begin(), aString.end());
//        scrollToEnd(aTextEdit);
        emit appendTextSafe(QString::fromStdString(aString));
        aString.clear();
    }
    else {
        aString += static_cast<char>(v);
    }


    return v;
}

std::streamsize Console::xsputn(const char* p, std::streamsize n){
    //aString.append(p, p + n);
     if (n <= 0 || p == nullptr) return n;
    aString.append(p, static_cast<std::string::size_type>(n));

    std::string::size_type pos = 0;
    bool appended = false;
    while (pos != std::string::npos) {
        pos = aString.find('\n');
        if (pos != std::string::npos) {
            std::string tmp(aString.begin(), aString.begin() + pos);
            //aTextEdit->append(tmp.c_str());
            emit appendTextSafe(QString::fromStdString(tmp));
            aString.erase(aString.begin(), aString.begin() + pos + 1);
            appended = true;
        }
    }

    if (appended) scrollToEnd(aTextEdit);

    return n;
}



//***************************************************** OutputDevice class ***********************
    //**********************************
    OutputDevice::OutputDevice(QTextEdit* textEdit)
    :QIODevice(), textEdit(textEdit) {
            // Open the device in write-only mode
            open(QIODevice::WriteOnly | QIODevice::Text);
        }

        OutputDevice::~OutputDevice() {
            close();
        }

        qint64 OutputDevice::readData(char *data, qint64 maxSize)  {
            // This is not used, we are only writing to QTextEdit
            Q_UNUSED(data);
            Q_UNUSED(maxSize);
            return 0;
        }

        qint64 OutputDevice::writeData(const char *data, qint64 maxSize)  {
            // Write data to QTextEdit (convert it to QString)
            QString output = QString::fromUtf8(data, maxSize);
            textEdit->append(output); // Append to QTextEdit widget

            QTextCursor cursor(textEdit->document());
            cursor.movePosition(QTextCursor::End);
            textEdit->setTextCursor(cursor);
            textEdit->ensureCursorVisible();

            return maxSize; // Return the number of bytes written
        }


//***************************************************** OutputToOutputTo class ***********************
    OutputTo::OutputTo(QTextEdit* _textEdit): QObject()/*  :  aTextEdit(*_textEdit)*/{
            outputDevice=new OutputDevice(_textEdit);
            tstream = new QTextStream(outputDevice);//outputDevice

            initialOut=stdout;

//            aMessageHandler = [this](QtMsgType type, const QMessageLogContext&, const QString& msg)
//            {
//                switch (type) {
//                case QtInfoMsg:
//                    *tstream << QString("Debug: %1").arg(msg) << "\n";
//                    break;
//                case QtDebugMsg:
//                    *tstream << QString("Debug: %1").arg(msg) << "\n";
//                    break;
//                case QtWarningMsg:
//                    *tstream << QString("Warning: %1").arg(msg) <<"\n";
//                    break;
//                case QtCriticalMsg:
//                    *tstream << QString("Critical: %1").arg(msg) << "\n";
//                    break;
//                case QtFatalMsg:
//                    *tstream << QString("Fatal: %1").arg(msg) <<"\n";
//                    abort();
//                }
//            };
//            qInstallMessageHandler(aMessageHandler.target<void(QtMsgType,const QMessageLogContext&,const QString& msg)>());
//            //init();

        }

//    void OutputTo::init(){
//            Console acout(&std::cout, &aTextEdit);
//    //Console acerr(&std::cerr, &aTextEdit);
//    }
        OutputTo::~OutputTo() {
            // Go back to the initial output
            stdout = initialOut;
            delete tstream;
            delete outputDevice;
        }

        QTextStream* OutputTo::getStream() {
            return tstream;
        }
        // void OutputTo::takeStandardOutput(QTextStream& _ss) {
        // Create a new QTextStream to redirect to QTextEdit
        //  initialOut = stdout; // Save the original stdout
        //QIODevice iodev;
        //iodev.setCurrentReadChannel((stdin));
        //stdout = fdopen(qintptr(textEdit->getHandle()), "w");
        //        int ff=fdopen(stdout, "w");
        //textEdit->append(_ss.readAll());
        //cout.rdbuf(_ss.)
        //     _ss.setDevice(std::cout.rdbuf());
        //}




}}
