#pragma once
#include <chrono>
#include <cstdint>
#include <mutex>
#include <thread>
#include <thread>
#include <condition_variable>
#include <atomic>
#include "IDebouncerStrategy.h"
/**
 *  @brief  a class that filters out the debounce from an input.
*/
class Debouncer{
public:
    enum class operationMode{singelEvent,repeatingEvent};

private:
    std::chrono::milliseconds interval;
    std::condition_variable* cond;
    std::atomic<operationMode> opMode;
    std::atomic<bool> exit;
    std::mutex zeroMutex;
    std::mutex oneMutex;
    std::mutex notifierMutex;
    std::mutex strategyMutex;
    std::condition_variable oneCondition;
    std::condition_variable zeroCondition;
    std::thread thr_debounce;
    IDebouncerStrategy* debouncerStrategy;
    void debouncer();
public:
    Debouncer();
    ~Debouncer();
    void setDebounceInterval(std::chrono::milliseconds);
    void setStrategy(IDebouncerStrategy*);
    void off();
    void on();

};
