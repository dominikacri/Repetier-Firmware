#pragma once

#include <stdint.h>

class TMC5160Stepper;

class TMC5160
{
public:
    TMC5160(uint8_t csPin);
    ~TMC5160();
    bool Init();
    bool ApplySettings();
    void PrintSettings();
   
private:
    uint8_t m_csPin = 0;
    TMC5160Stepper* m_TMCDriver = nullptr;
};
