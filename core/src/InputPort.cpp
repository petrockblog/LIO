#include "InputPort.h"

using namespace std;

InputPort::InputPort():eventCb(bind(&InputPort::dummyCb,this)){

}
void InputPort::SetEventCallback(std::function<void (bool)> cb){
    unique_lock<mutex>lck(eventCbMutex);
    eventCb=cb;
}

void InputPort::RemoveEventCallback(){
    unique_lock<mutex>lck(eventCbMutex);
    eventCb=bind(&InputPort::dummyCb,this);
}
void InputPort::dummyCb(){

}

void InputPort::callEventCallback(){
    unique_lock<mutex>lck(eventCbMutex);
    eventCb(Read());
}
