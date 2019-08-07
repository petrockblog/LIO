#ifndef DEBOUNCESSTRATEGYSIMPLE_H
#define DEBOUNCESSTRATEGYSIMPLE_H
#include "IDebouncerStrategy.h"

class DebouncerStrategySimple:public IDebouncerStrategy
{
public:
    DebouncerStrategySimple()=default;
    virtual void handleOnEvent() override;
    virtual void handleOffEvent() override;
};



#endif // DEBOUNCESSTRATEGYSIMPLE_H
