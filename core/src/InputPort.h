#ifndef INPUTPORT_H
#define INPUTPORT_H
#include <cstdint>
#include <condition_variable>
#include <chrono>
#include <functional>

class InputPort
{
public:
    virtual ~InputPort()=default;
    enum class TriggerEdge{None, Rising, Falling, Both};
    enum class PullUpDown{HiZ, PullUp, PullDown};
    virtual void SetTriggerEdge(TriggerEdge edge)=0;
    virtual void SetPullUpDown(PullUpDown pud)=0;
    virtual void SignalStartListening()=0;
    virtual void SignalStopListening()=0;
    virtual bool Read()=0;
    virtual uint32_t GetPinNo()=0;
    void SetEventCallback(std::function<void(bool)>cb);
    void RemoveEventCallback();
private:
    std::function<void(bool)> eventCb;
    std::mutex eventCbMutex;
    void dummyCb();
protected:
    void callEventCallback();
    InputPort();
};



#endif
