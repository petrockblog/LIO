#include "Debouncer.h"
#include <algorithm>
#include <iostream>
using namespace std;
Debouncer::Debouncer():cond(nullptr),exit(false){
    t=new thread(&Debouncer::doDebounce,this);
}
Debouncer::~Debouncer(){
    exit=true;
    oneCondition.notify_one();
    zeroCondition.notify_one();
    t->join();
    delete t;
}
void Debouncer::off(){
    zeroCondition.notify_one();
}
void Debouncer::on(){
    oneCondition.notify_one();
}
void Debouncer::setSampleInterval(std::chrono::duration<uint64_t,std::milli> interv){
    unique_lock<mutex> lck(zeroMutex);
    interval=interv;
}
void Debouncer::setNotifyer(std::condition_variable* cnd){
    cond=cnd;
}
void Debouncer::doDebounce(){
    std::mutex oneMutex;
    unique_lock<mutex> onelck(oneMutex);
    while(!exit){
        oneCondition.wait(onelck);
        if(exit)
            break;
        unique_lock<mutex> zerolck(zeroMutex);
        if(zeroCondition.wait_for(zerolck,interval)==cv_status::timeout){
            if(cond)
                cond->notify_all();
        }
        else if(exit)
            break;
    }
}



















