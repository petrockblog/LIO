#ifndef IINPUTPORT_H
#define IINPUTPORT_H
#include <cstdint>
class IInputPort
{
public:
    virtual ~IInputPort()=default;
    enum class TriggerEdge{None, Rising, Falling, Both};
    virtual void SetTriggerEdge(TriggerEdge edge)=0;
    /**
     * @brief This function blocks the execution maximum for the given time or until event happens on the input pin
     * @param timeout_ms maximum wait time. -1 means wait until event happens
     * @return true if event happened, false otherwise
     */
    virtual bool WaitForValidEvent(int timeout_ms=1000)=0;
    virtual bool Read()=0;
    virtual uint32_t GetPinNo()=0;
};

#endif
