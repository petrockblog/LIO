#include "DebouncedListener.h"

using namespace std;

DebouncedListener::DebouncedListener(IInputPort &port,
                                     std::function<void ()> onCallback,
                                     std::function<void ()> offCallback,
                                     std::chrono::milliseconds debounceDuration):
    listener(port)
{
    debouncer.setStrategy(&simpleStrategy);
    debouncer.setDebounceInterval(debounceDuration);
    listener.SetOnCallback(bind(&Debouncer::on,&debouncer));
    listener.SetOffCallback(bind(&Debouncer::off,&debouncer));
    setOnCallback(onCallback);
    setOffCallback(offCallback);
    listener.StartListening();
}

DebouncedListener::~DebouncedListener(){
    listener.StopListening();
}

void DebouncedListener::setOnCallback(std::function<void ()> onCallback){
    simpleStrategy.setOnCallback(onCallback);
}

void DebouncedListener::setOffCallback(std::function<void ()> offCallback){
    simpleStrategy.setOffCallback(offCallback);
}
