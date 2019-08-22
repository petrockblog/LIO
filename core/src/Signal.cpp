#include "Signal.h"
using namespace std;

void Signal::wait(){
    unique_lock<mutex>lck(m);
    c.wait(lck,[&]{return indicator;});
    indicator=false;
}

void Signal::signal(){
    {
        unique_lock<mutex> lck(m);
        indicator=true;
    }
    c.notify_one();
}

