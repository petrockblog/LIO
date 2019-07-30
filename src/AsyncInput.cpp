#include "AsyncInput.h"
#include <functional>
#include <iostream>
#include <syslog.h>

using namespace std;


AsyncInput::AsyncInput(IInputPort& inputPort):_inputPort(inputPort),exit(false){

}

void AsyncInput::Init(){
    _inputPort.SetTriggerEdge(IInputPort::TriggerEdge::Both);
    OffEvent();
    if(_inputPort.Read())
        OnEvent();
}

void AsyncInput::Listen(){
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

void AsyncInput::StartListening(){
    listenigThread=std::thread(bind(&AsyncInput::Listen,this));
}

void AsyncInput::StopListening(){
    unique_lock<mutex>l(mutExit);
    exit=true;
    condExit.wait(l);
    listenigThread.join();
}
