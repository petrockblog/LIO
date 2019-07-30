#ifndef INPUTPORT_H
#define INPUTPORT_H

#include "GPIO_Linux.h"
#include "IInputPort.h"
#include <mutex>
#include <condition_variable>
/**
 * @brief Represents an input port
 */
class InputPort:public IInputPort
{
private:
    int fd;
    GPIO_Linux port;
public:
    /**
     * @brief Create and config an input port
     * @param GPIO No.
     */
    InputPort(uint32_t pinNo);
    virtual ~InputPort();
    virtual void SetTriggerEdge(TriggerEdge edge) override;
    /**
     * @brief This function blocks the execution maximum for the given time or until event happens on the input pin
     * @param timeout_ms maximum wait time. -1 means wait until event happens
     * @return true if event happened, false otherwise
     */
    virtual bool WaitForValidEvent(int timeout_ms=1000) override;
    virtual bool Read() override;
    virtual uint32_t GetPinNo() override;
};

#endif // INPUTPORT_H
