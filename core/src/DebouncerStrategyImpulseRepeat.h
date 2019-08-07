#ifndef DEBOUNCESSTRATEGYIMPULSEREPEAT_H
#define DEBOUNCESSTRATEGYIMPULSEREPEAT_H
#include "IDebouncerStrategy.h"
#include <chrono>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>

class DebouncerStrategyImpulseRepeat : public IDebouncerStrategy
{

public:
    struct RepeatInfo{
        std::chrono::milliseconds interval;
        uint32_t multiplier;
    };
    DebouncerStrategyImpulseRepeat();
    virtual ~DebouncerStrategyImpulseRepeat()override;
    virtual void handleOnEvent() override;
    virtual void handleOffEvent() override;
    void setRepeatStrategy(std::list<RepeatInfo>strategy);

private:
    std::thread* repeatThread;
    std::list<RepeatInfo> repeatStrategy;
    std::list<RepeatInfo>::iterator actualStrategy;
    std::condition_variable repeatStrategyCnd;
    void doRepeat();
    void stopRepeatThread();
    void resetActualStrategy();
};

#endif // DEBOUNCESSTRATEGYIMPULSEREPEAT_H
