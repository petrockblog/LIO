#include "AsyncInputListener.h"
#include <functional>
#include <iostream>
#include <sstream>

using namespace std;


AsyncInputListener::AsyncInputListener(IInputPort& inputPort,std::function<void(const char*)>logFunction)
    :_inputPort(inputPort),exit(false),_logFunction(logFunction){

}

AsyncInputListener::~AsyncInputListener(){
    StopListening();
}

void AsyncInputListener::Listen(){
    stringstream log;

    log<<"Thread started for input "<<_inputPort.GetPinNo()<<endl;
    _logFunction(log.str().c_str());
    log.str("");
    while(1){
        IInputPort::WaitResult waitResult=_inputPort.WaitForEvent();
        if(exit)
            break;
        if(waitResult==IInputPort::WaitResult::EventOccurred){
            if(_inputPort.Read())
                OnEvent();
            else
                OffEvent();
        }
    }
    log<<"Thread ended for input "<<_inputPort.GetPinNo()<<endl;
    _logFunction(log.str().c_str());
}

void AsyncInputListener::StartListening(){
    listenigThread=std::thread(bind(&AsyncInputListener::Listen,this));
}

void AsyncInputListener::StopListening(){
    exit=true;
    _inputPort.StopWaitingForEvent();

    if(listenigThread.joinable())
        listenigThread.join();
}

