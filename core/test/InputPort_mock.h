#pragma once
#include <gmock/gmock.h>
#include "InputPort.h"

class InputPort_mock : public InputPort{
public:
    MOCK_METHOD1(SetTriggerEdge,void(TriggerEdge edge));
    MOCK_METHOD1(SetPullUpDown, void(PullUpDown pud));
    MOCK_METHOD0(Read,bool());
    MOCK_METHOD0(SignalStartListening,void());
    MOCK_METHOD0(SignalStopListening,void());
    MOCK_METHOD0(GetPinNo,uint32_t());
    void explicitEventCallback(){
        callEventCallback();
    }
public:
//    MOCK_METHOD1(loggerFunction, void(const char*));
//    MOCK_METHOD0(onCallback, void());
//    MOCK_METHOD0(offCallback, void());

};
