#ifndef DEBOUNCEDLISTENER_H
#define DEBOUNCEDLISTENER_H

#include <functional>
#include <chrono>
#include <cstdint>

#include "AsyncInputListener.h"
#include "Debouncer.h"
#include "DebouncerStrategySimple.h"


class DebouncedListener
{
private:
    AsyncInputListener listener;
    Debouncer debouncer;
    DebouncerStrategySimple simpleStrategy;

public:
    DebouncedListener(IInputPort& port,
                      std::function<void()>onCallback,
                      std::function<void()>offCallback,
                      std::chrono::milliseconds debounceDuration=std::chrono::milliseconds(100));
    ~DebouncedListener();
    void setOnCallback(std::function<void()>onCallback);
    void setOffCallback(std::function<void()>onCallback);
};

#endif // DEBOUNCEDLISTENER_H