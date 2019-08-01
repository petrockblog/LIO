#include "SysfsWrapper.h"
#include <sstream>
#include <stdexcept>
#include <syslog.h>
using namespace std;


SysfsWrapper::SysfsWrapper(uint32_t pinNo):_pinNo(pinNo),basePath("/sys/class/gpio"),pinBasePath(""){
    stringstream ss;
    ss<<basePath<<"/gpio"<<_pinNo<<"/";
    pinBasePath=ss.str();

    fstream exp(basePath+"/export");
    if(!exp.good()){
        string err_msg="unable to open IO port - ";
        err_msg.append(to_string(pinNo));
        syslog(LOG_CRIT,"%s",err_msg.c_str());
        throw runtime_error(err_msg);
    }
    exp<<to_string(_pinNo);
}

SysfsWrapper::~SysfsWrapper(){
    fstream unExp(basePath+"/unexport");
    unExp<<to_string(_pinNo);
}

void SysfsWrapper::SetDirection(SysfsWrapper::Direction dir){
    auto direction=make_unique<fstream>(pinBasePath+"direction");
    if(!direction->good()){
        syslog(LOG_CRIT,"Unable to open direction file - %ud",_pinNo);
        throw runtime_error("Unable to open direction");
    }
    switch(dir){
        case Direction::Input:
            *direction<<"in";
            break;
        case Direction::Output:
            *direction<<"out";
            break;
        case Direction::High:
            *direction<<"high";
            break;
        case Direction::Low:
            *direction<<"low";
            break;
    }
}

void SysfsWrapper::SetActiveLow(bool isActiveLow){
    auto activeLow=make_unique<fstream>(pinBasePath+"active_low");
    if(!activeLow->good()){
        syslog(LOG_CRIT,"Unable to open active_low file - %ud",_pinNo);
        throw runtime_error("Unable to open active_low");
    }
    *activeLow<<(isActiveLow?"1":"0");
}

bool SysfsWrapper::ReadVal(){
    auto value=make_unique<fstream>(pinBasePath+"value");
    if(!value->good()){
        syslog(LOG_CRIT,"Unable to open value for read - %ud",_pinNo);
        throw runtime_error("Unable to open value for read");
    }
    bool ret;
    *value>>ret;
    return ret;
}

void SysfsWrapper::WriteVal(bool val){
    auto value=make_unique<fstream>(pinBasePath+"value");
    if(!value->good()){
        syslog(LOG_CRIT,"Unable to open value for write - %ud",_pinNo);
        throw runtime_error("Unable to open value for write");
    }
    *value<<val;
}

uint32_t SysfsWrapper::GetPinNo(){
    return _pinNo;
}

string SysfsWrapper::GetPinBasePath(){
    return pinBasePath;
}
