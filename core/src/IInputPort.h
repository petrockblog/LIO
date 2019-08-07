#ifndef IINPUTPORT_H
#define IINPUTPORT_H
#include <cstdint>
#include <condition_variable>
#include <chrono>

class IInputPort
{
public:
    virtual ~IInputPort()=default;
    enum class TriggerEdge{None, Rising, Falling, Both};
    enum class PullUpDown{HiZ, PullUp, PullDown};
    enum class WaitResult{Timeout, EventOccurred, Exit};
    virtual void SetTriggerEdge(TriggerEdge edge)=0;
    virtual void SetPullUpDown(PullUpDown pud)=0;
    /**
     * @brief This function blocks the execution maximum for the given time or until event happens on the input pin
     * @param timeout maximum wait time. 0 means wait until event happens
     * @return processResult
     */
    virtual WaitResult WaitForEvent(std::chrono::milliseconds timeout=std::chrono::milliseconds(0))=0;
    virtual void StopWaitingForEvent()=0;
    virtual bool Read()=0;
    virtual uint32_t GetPinNo()=0;
};

#endif
