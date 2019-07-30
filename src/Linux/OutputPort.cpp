#include "OutputPort.h"

OutputPort::OutputPort(uint32_t pinNo):port (pinNo){
    port.SetDirection(GPIO_Linux::Direction::Output);
}
OutputPort::OutputPort(uint32_t pinNo,bool DefaultValue):port(pinNo){
    port.SetDirection(DefaultValue?GPIO_Linux::Direction::High:GPIO_Linux::Direction::Low);
}

void OutputPort::Write(bool val){
    port.WriteVal(val);
}

uint32_t OutputPort::GetPinNo(){
    return port.GetPinNo();
}
