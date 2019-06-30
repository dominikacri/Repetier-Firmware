#pragma once

#include <stdint.h>

class TMC2130Stepper;

class TMC2130
{
public:
    TMC2130(uint8_t csPin);
    ~TMC2130();
    bool Init();
    bool ApplySettings();
    void PrintSettings();
   
private:
    uint8_t m_csPin = 0;
    TMC2130Stepper* m_TMCDriver = nullptr;
};
