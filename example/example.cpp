#include <iostream>
#include <thread>
#include <signal.h>
#include <memory>


#include "OutputPort_Linux.h"
#include "InputPort_Linux.h"
#include "AsyncInputListener.h"


using namespace std;

static shared_ptr<IOutputPort>out_port;
static shared_ptr<IInputPort>in_sync;
static shared_ptr<AsyncInputListener>in_async;


static bool stop=false;
/**
 * @brief shutDown - stop async input listening
 */
void shutDown(){
    cout<<"Shutdown in progress"<<endl;
    if(in_async)
        in_async->StopListening();
    stop=true;
    cout<<"Shutdown finished"<<endl;
}
/**
 * @brief handleSignals-in case of termination, free resources
 */
void handleSignals(int){
    static bool shutdownInProgress=false;
    cout<<"Termination event"<<endl;
    if(!shutdownInProgress){
        shutdownInProgress=true;
        shutDown();
    }
    else{
        cout<<"app is shutting down"<<endl;
    }
}

void OnEvent(){
    cout<<endl<<"---IN(Async): ON event!---"<<endl;
}
void OffEvent(){
    cout<<endl<<"---IN(Async): OFF event!---"<<endl;
}

int main(){
    cout<<"Program started"<<endl;
    try{
        InputPort_Linux inputPort(2);
        out_port=make_shared<OutputPort_Linux>(14);
        in_sync=make_shared<InputPort_Linux>(3);
        in_async=make_shared<AsyncInputListener>(inputPort);
        in_async->SetOnCallback(OnEvent);
        in_async->SetOffCallback(OffEvent);

        //signal hangling (ctrl+c, or service termination)
        struct sigaction sigHandler;
        sigHandler.sa_handler=handleSignals;
        sigemptyset(&sigHandler.sa_mask);
        sigHandler.sa_flags=0;
        sigaction(SIGTERM,&sigHandler,NULL);
        sigaction(SIGINT,&sigHandler,NULL);

        in_sync->SetTriggerEdge(IInputPort::TriggerEdge::Both);
        in_async->StartListening();
        cout<<"Running"<<endl;
        int cnt=0;
        while (!stop && cnt++<10) {
            cout<<"Set output to ";
            if(cnt%2){
                cout<<"1"<<endl;
                out_port->Write(true);
            }
            else{
                cout<<"0"<<endl;
                out_port->Write(false);
            }
                  cout<<"Waiting for sync input event..." <<flush;
            if(in_sync->WaitForEvent(4000ms)==IInputPort::WaitResult::EventOccurred){
                cout<<"Yes: "<<in_sync->Read()<<endl;
            }
            else{
                cout<<"No sync input event."<<endl;

            }
            this_thread::sleep_for(chrono::seconds(4));
            cout<<"-----------------------------"<<endl;
        }
        if(!stop)
            shutDown();
    }
    catch(...){
        cout<<"Fatal error."<<endl;
        shutDown();
    }
    cout<<"Program terminated"<<endl;
}
