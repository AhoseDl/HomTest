#ifndef HEAVYTASK_H
#define HEAVYTASK_H

#include<QObject>
#include<functional>
#include<QThread>

namespace scaletag {namespace frames{

class HeavyTaskWrapper : public QObject{
    Q_OBJECT

public:
explicit HeavyTaskWrapper(std::function<void()> _heavyTask);

public slots:
void process();


private:
std::function<void()> heavyTask;

signals :
void heavyTaskFinished();



};


//template<typename T, typename... Args>
//QThread* heavyTaskLauncher(T* obj, void (T::*fct)(Args...), Args... args){

//QThread* thr = new QThread;
//HeavyTaskWrapper* task = new HeavyTaskWrapper([obj, fct, args...](){(obj->*fct)(args...);});
//task->moveToThread(thr);

//QObject::connect(thr, &QThread::started, task, &HeavyTaskWrapper::process);
//QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, thr, &QThread::quit);
//    QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, task, &HeavyTaskWrapper::deleteLater);
//    QObject::connect(thr, &QThread::finished, thr, &QThread::deleteLater);

//thr->start();
//return thr;

//}
template<typename T, typename... FctArgs, typename... CallArgs>
QThread* heavyTaskLauncher(T* obj, void (T::*fct)(FctArgs...), CallArgs&&... args)
{
QThread* thr = new QThread;
HeavyTaskWrapper* task = new HeavyTaskWrapper(
    [obj, fct, tup = std::make_tuple(std::forward<CallArgs>(args)...)]() mutable {
        std::apply([obj, fct](auto&&... a) {
            (obj->*fct)(std::forward<decltype(a)>(a)...);
        }, std::move(tup));
    }
    );
task->moveToThread(thr);

QObject::connect(thr, &QThread::started, task, &HeavyTaskWrapper::process);
QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, thr, &QThread::quit);
QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, task, &HeavyTaskWrapper::deleteLater);
QObject::connect(thr, &QThread::finished, thr, &QThread::deleteLater);

thr->start();
return thr;
}


template<typename... FctArgs, typename... CallArgs>
QThread* heavyTaskLauncher(std::function<void()> fct)
{
QThread* thr = new QThread;
HeavyTaskWrapper* task = new HeavyTaskWrapper(fct
    );
task->moveToThread(thr);

QObject::connect(thr, &QThread::started, task, &HeavyTaskWrapper::process);
QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, thr, &QThread::quit);
QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, task, &HeavyTaskWrapper::deleteLater);
QObject::connect(thr, &QThread::finished, thr, &QThread::deleteLater);

thr->start();
return thr;
}

}}


#endif
