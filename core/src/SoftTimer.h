#ifndef SOFTTIMER_H
#define SOFTTIMER_H
#include <future>
#include <atomic>
#include <condition_variable>

#include "IBasicTimer.h"
#include "Signal.h"

class SoftTimer:public IBasicTimer{
    // IBasicTimer interface
public:
    SoftTimer();
    virtual ~SoftTimer() override;
    virtual void SetTimeout(std::chrono::milliseconds) override;
    virtual void SetTimeoutCallback(std::function<void ()>) override;
    virtual void Start() override;
    virtual void StopAndReset() override;

private:
    enum class state{idle,running};
    std::atomic<state> myState;
    std::function<void()> timeoutCallback;
    std::chrono::milliseconds timeout;
    std::atomic<bool> timerThreadExitRequest;
    std::future<void> timerThreadFuture;
    void timerTask(std::promise<void>&& exitPromise);
    Signal s;
};


#endif
