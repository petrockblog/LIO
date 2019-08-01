#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H

#include "SysfsWrapper.h"
#include "IOutputPort.h"
/**
 * @brief Represents an output port
 */
class OutputPort_Linux: public IOutputPort
{
private:
    SysfsWrapper port;
public:
    /**
     * @brief Creates and config optput pin. No default value
     * @param pinNo
     */
    OutputPort_Linux(uint32_t pinNo);
    /**
     * @brief Creates and config optput pin with the given default value
     * @param pinNo
     * @param DefaultValue
     */
    OutputPort_Linux(uint32_t pinNo,bool DefaultValue);
    virtual void Write(bool val) override;
    virtual uint32_t GetPinNo() override;
};

#endif // OUTPUTPORT_H
