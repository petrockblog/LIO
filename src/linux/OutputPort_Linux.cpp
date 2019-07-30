#include "OutputPort_Linux.h"

OutputPort_Linux::OutputPort_Linux(uint32_t pinNo):port (pinNo){
    port.SetDirection(SysfsWrapper::Direction::Output);
}
OutputPort_Linux::OutputPort_Linux(uint32_t pinNo,bool DefaultValue):port(pinNo){
    port.SetDirection(DefaultValue?SysfsWrapper::Direction::High:SysfsWrapper::Direction::Low);
}

void OutputPort_Linux::Write(bool val){
    port.WriteVal(val);
}

uint32_t OutputPort_Linux::GetPinNo(){
    return port.GetPinNo();
}
