#pragma once
#include <gmock/gmock.h>
#include <IDebouncerStrategy.h>

class IDeboucerStrategy_mock:public IDebouncerStrategy{
public:
    MOCK_METHOD0(handleOffEvent,void ());
    MOCK_METHOD0(handleOnEvent,void());
};
