#include "AsyncInputListener.h"
#include <functional>
#include <iostream>
#include <syslog.h>

using namespace std;


AsyncInputListener::AsyncInputListener(IInputPort& inputPort):_inputPort(inputPort),exit(false){

}

void AsyncInputListener::Init(){
    _inputPort.SetTriggerEdge(IInputPort::TriggerEdge::Both);
    OffEvent();
    if(_inputPort.Read())
        OnEvent();
}

void AsyncInputListener::Listen(){
    syslog(LOG_INFO,"Thread started for input %u",_inputPort.GetPinNo());
    while(1){
        bool validEvent=_inputPort.WaitForValidEvent();
        {
            volatile unique_lock<mutex>l(mutExit);
            if(exit)
                break;
        }
        if(validEvent){
            if(_inputPort.Read())
                OnEvent();
            else
                OffEvent();
        }
    }
    syslog(LOG_INFO,"Thread end for input %u",_inputPort.GetPinNo());
    condExit.notify_all();
}

void AsyncInputListener::StartListening(){
    listenigThread=std::thread(bind(&AsyncInputListener::Listen,this));
}

void AsyncInputListener::StopListening(){
    unique_lock<mutex>l(mutExit);
    exit=true;
    condExit.wait(l);
    listenigThread.join();
}
