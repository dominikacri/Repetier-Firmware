#pragma once

#include "tmcbase.h"

class TMC2130 : public TMCBase<TMC2130Stepper>
{
public:
    TMC2130(uint16_t csPin);
    bool Init();
    bool ApplySettings();
    void PrintSettings();
};
