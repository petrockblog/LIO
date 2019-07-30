#ifndef IOUTPUTPORT_H
#define IOUTPUTPORT_H
#include <cstdint>
class IOutputPort
{
public:
    virtual~IOutputPort()=default;
    virtual void Write(bool val)=0;
    virtual uint32_t GetPinNo()=0;
};

#endif // IOUTPUTPORT_H
