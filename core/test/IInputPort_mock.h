#pragma once
#include <gmock/gmock.h>
#include "IInputPort.h"

class IInputPort_mock : public IInputPort{
public:
    MOCK_METHOD1(SetTriggerEdge,void(TriggerEdge edge));
    MOCK_METHOD1(SetPullUpDown, void(PullUpDown pud));
    MOCK_METHOD1(WaitForEvent,WaitResult(std::chrono::milliseconds));
    MOCK_METHOD0(Read,bool());
    MOCK_METHOD0(GetPinNo,uint32_t());
    MOCK_METHOD0(StopWaitingForEvent,void());

public:
    MOCK_METHOD1(loggerFunction, void(const char*));
    MOCK_METHOD0(onCallback, void());
    MOCK_METHOD0(offCallback, void());

};
