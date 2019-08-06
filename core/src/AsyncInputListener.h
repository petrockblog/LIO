#ifndef ASYNCINPUT_H
#define ASYNCINPUT_H
#include "IInputPort.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <functional>
/**
 * @brief Represents an input pin that can inform the outside world from its state change
 */
class AsyncInputListener
{
private:
    IInputPort& _inputPort;
    std::thread listenigThread;
    std::atomic<bool>exit;
    std::function<void(const char*)> _logFunction;
    std::function<void()>onCallback;
    std::function<void()>offCallback;
    void Listen();
    void OnEvent();
    void OffEvent();
public:
    AsyncInputListener(IInputPort& inputPort,std::function<void(const char*)>logFunction=[](const char*){});
    virtual ~AsyncInputListener();
    void StartListening();
    void StopListening();
    /**
     * @brief Set a callback that sould be called if input goes 0->1
     */
    virtual void SetOnCallback(std::function<void()>);
    /**
     * @brief Set a callback that sould be called if input goes 1->0
     */
    virtual void SetOffCallback(std::function<void()>);
};

#endif // ASYNCINPUT_H
