#ifndef INPUTPORT_H
#define INPUTPORT_H

#include "SysfsWrapper.h"
#include "IInputPort.h"
#include <mutex>
#include <condition_variable>
#include <atomic>
/**
 * @brief Represents an input port
 */
class InputPort_Linux:public IInputPort
{
private:
    std::atomic<int> fd;
    SysfsWrapper port;
public:
    /**
     * @brief Create and config an input port
     * @param GPIO No.
     */
    InputPort_Linux(uint32_t pinNo);
    virtual ~InputPort_Linux()override;
    virtual void SetTriggerEdge(TriggerEdge edge) override;
    /**
     * @brief This function blocks the execution maximum for the given time or until event happens on the input pin
     * @param timeout_ms maximum wait time. -1 means wait until event happens
     * @return true if event happened, false otherwise
     */
    virtual WaitResult WaitForEvent(std::chrono::duration<uint64_t,std::milli> timeout=std::chrono::duration<uint64_t,std::milli>(0)) override;
    virtual void StopWaitingForEvent() override;
    virtual bool Read() override;
    virtual uint32_t GetPinNo() override;
    virtual void SetPullUpDown(PullUpDown pud) override;
};

#endif // INPUTPORT_H
