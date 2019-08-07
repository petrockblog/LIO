#ifndef DEBOUNCERSTRATEGY_H
#define DEBOUNCERSTRATEGY_H

#include <functional>

class IDebouncerStrategy
{
protected:
    std::function<void()>onCb;
    std::function<void()>offCb;
public:
    IDebouncerStrategy()=default;
    virtual ~IDebouncerStrategy()=default;
    virtual void handleOnEvent()=0;
    virtual void handleOffEvent()=0;
    void setOnCallback(std::function<void()>);
    void setOffCallback(std::function<void()>);
};

#endif // DEBOUNCERSTRATEGY_H
