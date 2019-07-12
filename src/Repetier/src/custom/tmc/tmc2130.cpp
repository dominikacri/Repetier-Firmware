#include "tmc2130.h"

void TMC2130::PrintSettings()
{
   TMCBase::PrintSettings();
}

bool TMC2130::Init()
{
    Serial.println(F("TMC2130: Try to Init"));

    if (!TMCBase::Init())
    {
        return false;
    }

    Serial.println(F("TMC2130: Init was successfull"));

    this->PrintSettings();

    this->ApplySettings();

    return true;
}

bool TMC2130::ApplySettings()
{
    TMCBase::ApplySettings();

    m_TMCDriver.I_scale_analog(true);
       
    Serial.println(F("TMC2130 - New settings applied."));

    this->PrintSettings();

    /* 
    Serial.println(F("TMC2130 - Drive Motor Y Motor."));

    pinMode(56, OUTPUT);
    pinMode(60, OUTPUT);
    pinMode(61, OUTPUT);
    digitalWrite(56, LOW);      // Enable driver in hardware

    for (uint16_t i = 15000; i>0; i--) 
    {
        digitalWrite(60, HIGH);
        delayMicroseconds(160);
        digitalWrite(60, LOW);
        delayMicroseconds(160);
    }
    */

    return true;
}
