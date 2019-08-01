#ifndef SYSFSWRAPPER_H
#define SYSFSWRAPPER_H

#include <string>

#include <fstream>
#include <memory>

/**
 * @brief Base class of Input Output pins. Wraps sysfs kernel interface
 */
class SysfsWrapper
{
private:
    uint32_t _pinNo;
    std::string const basePath;
    std::string pinBasePath;

public:
    enum class Direction{Input, Output, High, Low};
    void SetDirection(Direction dir);
    void SetActiveLow(bool isActiveLow);
    SysfsWrapper(uint32_t pinNo);
    virtual ~SysfsWrapper();
    bool ReadVal();
    void WriteVal(bool val);
    std::string GetPinBasePath();
    uint32_t GetPinNo();

};

#endif // SYSFSWRAPPER_H
