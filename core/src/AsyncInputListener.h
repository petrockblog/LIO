#ifndef ASYNCINPUT_H
#define ASYNCINPUT_H
#include "IInputPort.h"
#include "EventHandledInput.h"
#include <thread>
#include <atomic>
#include <condition_variable>
/**
 * @brief Represents an input pin that can inform the outside world from its state change
 */
class AsyncInputListener: public EventHandledInput
{
private:
    IInputPort& _inputPort;
    std::thread listenigThread;
    std::atomic<bool>exit;
    std::function<void(const char*)> _logFunction;
    void Listen();
public:
    AsyncInputListener(IInputPort& inputPort,std::function<void(const char*)>logFunction=[](const char*){});
    virtual ~AsyncInputListener();
    void StartListening();
    void StopListening();
};

#endif // ASYNCINPUT_H
