#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H

#include "GPIO_Linux.h"
#include "IOutputPort.h"
/**
 * @brief Represents an output port
 */
class OutputPort: public IOutputPort
{
private:
    GPIO_Linux port;
public:
    /**
     * @brief Creates and config optput pin. No default value
     * @param pinNo
     */
    OutputPort(uint32_t pinNo);
    /**
     * @brief Creates and config optput pin with the given default value
     * @param pinNo
     * @param DefaultValue
     */
    OutputPort(uint32_t pinNo,bool DefaultValue);
    virtual void Write(bool val) override;
    virtual uint32_t GetPinNo() override;
};

#endif // OUTPUTPORT_H
