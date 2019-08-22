#include "Debouncer.h"
#include <algorithm>
#include <iostream>
using namespace std;
Debouncer::Debouncer():
    cond(nullptr),
    exit(false),
    thr_debounce(&Debouncer::debouncer,this){

}
Debouncer::~Debouncer(){
    exit=true;
    unique_lock<mutex>lckZero(zeroMutex);
    unique_lock<mutex>lckOne(oneMutex);
    oneCondition.notify_one();
    zeroCondition.notify_one();
    thr_debounce.join();
}
void Debouncer::off(){
    zeroCondition.notify_one();
}
void Debouncer::on(){
    oneCondition.notify_one();
}
void Debouncer::setDebounceInterval(std::chrono::milliseconds interv){
    volatile unique_lock<mutex> lck(zeroMutex);
    interval=interv;
}

void Debouncer::setStrategy(IDebouncerStrategy * strategy){
    volatile unique_lock<mutex>lck(strategyMutex);
    debouncerStrategy=strategy;
}

void Debouncer::debouncer(){
    enum state{on,onToOff,off,offToOn};
    state currentState=state::off;
    while(!exit){
        switch(currentState){
            case off:{
                unique_lock<mutex> onelck(oneMutex);
                oneCondition.wait(onelck);
                currentState=offToOn;
                break;
            }
            case offToOn:{
                unique_lock<mutex> zerolck(zeroMutex);
                if(zeroCondition.wait_for(zerolck,interval)==cv_status::timeout){
                    volatile unique_lock<mutex>lck(strategyMutex);
                    if(debouncerStrategy)
                        debouncerStrategy->handleOnEvent();
                    currentState=on;
                }
                else
                    currentState=off;
                break;
            }
            case on:{
                unique_lock<mutex> zerolck(zeroMutex);
                zeroCondition.wait(zerolck);
                currentState=onToOff;
                break;
            }
            case onToOff:{
                unique_lock<mutex> onelck(oneMutex);
                if(oneCondition.wait_for(onelck,interval)==cv_status::timeout){
                    volatile unique_lock<mutex>lck(strategyMutex);
                    if(debouncerStrategy)
                        debouncerStrategy->handleOffEvent();
                    currentState=off;
                }
                else
                    currentState=on;
                break;
            }
        }
    }
}



















