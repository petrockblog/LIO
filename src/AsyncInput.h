#ifndef ASYNCINPUT_H
#define ASYNCINPUT_H
#include "IInputPort.h"
#include "EventHandledInput.h"
#include <thread>
#include <mutex>
#include <condition_variable>
/**
 * @brief Represents an input pin that can inform the outside world from its state change
 */
class AsyncInput: public EventHandledInput
{
private:
    IInputPort& _inputPort;
    std::thread listenigThread;
    std::condition_variable condExit;
    std::mutex mutExit;
    bool exit;
    void Listen();
public:
    AsyncInput(IInputPort& inputPort);
    void Init();
    void StartListening();
    void StopListening();
};

#endif // ASYNCINPUT_H
