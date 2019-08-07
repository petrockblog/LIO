#include "DebouncerStrategyImpulseRepeat.h"
#include <condition_variable>

using namespace std;

DebouncerStrategyImpulseRepeat::DebouncerStrategyImpulseRepeat(){
    resetActualStrategy();
}

DebouncerStrategyImpulseRepeat::~DebouncerStrategyImpulseRepeat(){
    stopRepeatThread();

}
void DebouncerStrategyImpulseRepeat::handleOnEvent(){
    stopRepeatThread();
    repeatThread=new thread(bind(&DebouncerStrategyImpulseRepeat::doRepeat,this));
}

void DebouncerStrategyImpulseRepeat::handleOffEvent(){
    resetActualStrategy();
    offCb();
}

void DebouncerStrategyImpulseRepeat::setRepeatStrategy(std::list<DebouncerStrategyImpulseRepeat::RepeatInfo> strategy){
    repeatStrategy=strategy;
    if(repeatStrategy.begin()!=repeatStrategy.end())
        --repeatStrategy.begin()->multiplier;//first execution happens immediately
    resetActualStrategy();
}

void DebouncerStrategyImpulseRepeat::doRepeat(){
    mutex m;
    unique_lock<mutex> cnd(m);
    bool exit=false;
    if(repeatStrategy.begin()!=repeatStrategy.end())//first execution should be executed immediatley
        onCb();

    while(actualStrategy!=repeatStrategy.end() && !exit){
        for(uint32_t i=0;i<actualStrategy->multiplier;++i){
            if(repeatStrategyCnd.wait_for(cnd,actualStrategy->interval)==cv_status::no_timeout){
                exit=true;
                break;
            }
            onCb();
        }
        ++actualStrategy;
    }
}

void DebouncerStrategyImpulseRepeat::stopRepeatThread(){
    if(repeatThread){
        if(repeatThread->joinable()){
            repeatStrategyCnd.notify_one();
            repeatThread->join();
        }
        delete repeatThread;
        repeatThread=nullptr;
    }
}

void DebouncerStrategyImpulseRepeat::resetActualStrategy(){
    actualStrategy=repeatStrategy.begin();
}
