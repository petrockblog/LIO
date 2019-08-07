#include "IDebouncerStrategy.h"
void IDebouncerStrategy::setOnCallback(std::function<void()>cb){
    onCb=cb;
}
void IDebouncerStrategy::setOffCallback(std::function<void()>cb){
    offCb=cb;
}
