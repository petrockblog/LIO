#ifndef IBASICTIMER_H
#define IBASICTIMER_H
#include <chrono>
#include <functional>

class IBasicTimer{
public:
    virtual ~IBasicTimer()=default;
    virtual void SetTimeout(std::chrono::milliseconds)=0;
    virtual void SetTimeoutCallback(std::function<void()>)=0;
    virtual void Start()=0;
    virtual void StopAndReset()=0;
};
#endif
