#pragma once
#include <chrono>
#include <cstdint>
#include <mutex>
#include <thread>
#include <list>
#include <thread>
#include <condition_variable>
#include <atomic>
/**
 *  @brief  a class that filters out the debounce from an input.
*/
class Debouncer{
private:
    std::chrono::duration<uint64_t,std::milli> interval;
    std::condition_variable* cond;
    std::mutex zeroMutex;
    std::atomic<bool> exit;
    std::thread* t;
    std::condition_variable oneCondition;
    std::condition_variable zeroCondition;
    void doDebounce();
public:
    Debouncer();
    ~Debouncer();
    void setSampleInterval(std::chrono::duration<uint64_t,std::milli>);
    void off();
    void on();
    void setNotifyer(std::condition_variable*);
};
