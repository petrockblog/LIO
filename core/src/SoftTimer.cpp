#include "SoftTimer.h"
#include <thread>
#include <functional>
#include <chrono>

using namespace std;
SoftTimer::SoftTimer():myState(state::idle),timeoutCallback([]{}),
timerThreadExitRequest(false){
    promise<void> exitPromise;
    timerThreadFuture=exitPromise.get_future();
    thread(bind(&SoftTimer::timerTask,this,placeholders::_1),move(exitPromise)).detach();//start thread
    StopAndReset();
}

SoftTimer::~SoftTimer(){
    timerThreadExitRequest=true;
    s.signal();
    while(timerThreadFuture.wait_for(100ms)!=future_status::ready){
        s.signal();
    }
}

void SoftTimer::SetTimeout(std::chrono::milliseconds timeout){
    this->timeout=timeout;
}

void SoftTimer::SetTimeoutCallback(std::function<void ()>cb){
    timeoutCallback=cb;
}

void SoftTimer::Start(){
    myState=state::running;
    s.signal();
}

void SoftTimer::StopAndReset(){
    myState=state::idle;
    s.signal();
}

void SoftTimer::timerTask(std::promise<void> &&exitPromise){
    while(!timerThreadExitRequest){
        switch(myState){
            case state::idle:{
                s.wait();
                break;
            }
            case state::running:{
                if(s.wait_for(timeout)==Signal::sig_status::timeout){
                    timeoutCallback();
                    myState=state::idle;
                }
                break;
            }
        }
    }
    exitPromise.set_value_at_thread_exit();
}
