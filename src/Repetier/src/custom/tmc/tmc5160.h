#pragma once


#include "tmcbase.h"

class TMC5160 : public TMCBase<TMC5160Stepper>
{
public:
    TMC5160(uint16_t csPin);
    bool Init();
    bool ApplySettings();
    void PrintSettings() override;
   

};
