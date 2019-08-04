#pragma once

#include <TMCStepper.h>
#include "tmcbase.h"

class TMC2130 : public TMCBase<TMC2130Stepper>
{
public:
    TMC2130(uint16_t csPin, 
        uint16_t rmsCurrent = 600, 
        uint16_t microSteps = 16, 
        bool interpolateMicrosteps = true, 
        ChopperMode chopperMode = ChopperMode::SpreadCycle, 
        bool stallguard = false, 
        int8_t stallguardThreshold = 100) :

        TMCBase(csPin, rmsCurrent, microSteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold)
        {

        };
        
    bool Init() override;
    bool ApplySettings() override;
    void PrintSettings() override;
};
