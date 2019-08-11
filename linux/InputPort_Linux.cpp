#include "InputPort_Linux.h"
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <syslog.h>
#include <functional>

using namespace std;

void sigactionHandler(int sig, siginfo_t * info, void *)
{
    cout<<"signal: "<<sig<<", fd: "<<info->si_fd<<endl;
}

InputPort_Linux::InputPort_Linux(uint32_t pinNo):port(pinNo){
    port.SetDirection(SysfsWrapper::Direction::Input);
    fd=open(string(port.GetPinBasePath()+"value").c_str(),O_RDONLY | O_NONBLOCK);
    struct sigaction sa;

    sa.sa_sigaction=sigactionHandler;
    sigaction(POLL_IN,&sa,NULL);
    if(fd<0){
        syslog(LOG_ERR,"Unable to open input for polling: %d - %ud",static_cast<int>(fd),port.GetPinNo());
    }
    else{
        //a newly opened file consider changed, hence the seek and the read
        Read();
    }
}

InputPort_Linux::~InputPort_Linux(){
    if(fd>=0)
        close(fd);
    struct sigaction sa;
    sa.sa_handler=SIG_DFL;
    sigaction(POLL_IN,&sa,NULL);
}

void InputPort_Linux::SetTriggerEdge(InputPort_Linux::TriggerEdge edge){
    auto triggerEdge=make_unique<fstream>(port.GetPinBasePath()+"edge");
    if(!triggerEdge->good()){
        syslog(LOG_CRIT,"Unable to open edge descriptor. - %ud",port.GetPinNo());
        throw runtime_error("Unable to open edge descriptor.");
    }
    switch(edge){
        case TriggerEdge::None:
            *triggerEdge<<"none";
            break;
        case TriggerEdge::Rising:
            *triggerEdge<<"rising";
            break;
        case TriggerEdge::Falling:
            *triggerEdge<<"falling";
            break;
        case TriggerEdge::Both:
            *triggerEdge<<"both";
            break;
    }
}

IInputPort::WaitResult InputPort_Linux::WaitForEvent(std::chrono::milliseconds timeout){
    WaitResult ret=WaitResult::Timeout;
    if(fd>=0)
    {
        pollfd pollDescr;
        pollDescr.fd=fd;
        pollDescr.events=POLLPRI;
        int pret=poll(&pollDescr,1,timeout.count());
        if(pret<0){
            cerr<<"Error during poll: "<<pret<<endl;
        }
        else if(pret==0)//timeout
            ;
        else
            ret=WaitResult::EventOccurred;
    }
    return ret;
}

void InputPort_Linux::StopWaitingForEvent(){
    //TODO: implement me
}

bool InputPort_Linux::Read(){
    if(fd>=0){
        lseek(fd, 0, SEEK_SET);
        char b[2];
        read(fd,b,2);
        return b[0]=='1';
    }
    else
        return port.ReadVal();
}

uint32_t InputPort_Linux::GetPinNo(){
    return port.GetPinNo();
}

void InputPort_Linux::SetPullUpDown(IInputPort::PullUpDown pud){
    //TODO: implement me
}
