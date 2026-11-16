#include<frames/heavytask.h>

namespace scaletag{namespace frames{
HeavyTaskWrapper::HeavyTaskWrapper(std::function<void()> _heavyTask) : heavyTask(std::move(_heavyTask)) {}

void HeavyTaskWrapper::process(){
heavyTask();
emit heavyTaskFinished();
}
/*
template<typename T, typename... Args>
QThread* heavyTaskLauncher(T* obj, void (T::*fct)(Args...), Args... args){

QThread* thr = new QThread;
HeavyTaskWrapper* task = new HeavyTaskWrapper([obj, fct, args...](){(obj->*fct)(args...);});
task->moveToThread(thr);

QObject::connect(thr, &QThread::started, task, &HeavyTaskWrapper::process);
QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, thr, &QThread::quit);
    QObject::connect(task, &HeavyTaskWrapper::heavyTaskFinished, task, &HeavyTaskWrapper::deleteLater);
    QObject::connect(thr, &QThread::finished, thr, &QThread::deleteLater);

thr->start();
return thr;

}*/

}}
