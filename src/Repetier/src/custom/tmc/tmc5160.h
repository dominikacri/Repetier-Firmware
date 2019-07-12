#pragma once

#include <TMCStepper.h>
#include "tmcbase.h"

class TMC5160 : public TMCBase<TMC5160Stepper>
{
public:
    TMC5160(uint16_t csPin, 
        uint16_t rmsCurrent = 800, 
        uint16_t microSteps = 16, 
        bool interpolateMicrosteps = true, 
        ChopperMode chopperMode = ChopperMode::SpreadCycle, 
        bool enableStallguard = false,
        int8_t stallguardThreshold = 100
        ) :
        TMCBase(csPin, rmsCurrent, microSteps, interpolateMicrosteps, chopperMode, enableStallguard, stallguardThreshold)
    {

    };

    bool Init() override;
    bool ApplySettings() override;
    void PrintSettings() override;
};
