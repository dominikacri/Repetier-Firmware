#include "tmc5160.h"

TMC5160::TMC5160(uint16_t csPin, 
        uint16_t rmsCurrent , 
        uint16_t microSteps , 
        bool interpolateMicrosteps , 
        ChopperMode chopperMode , 
        bool enableStallguard ,
        int8_t stallguardThreshold) : 
        
        TMCBase(csPin, rmsCurrent, microSteps, interpolateMicrosteps, chopperMode, enableStallguard, stallguardThreshold)
{

}

void TMC5160::PrintSettings()
{
    TMCBase::PrintSettings();
    
    bool autoGrade = m_TMCDriver.pwm_autograd(); // Stealthcop
  
    if (autoGrade)
    {
        Serial.println(F("AutoGrade: Enabled"));
    }
    else
    {
       Serial.println(F("AutoGrade: Disabled"));
    }
}

bool TMC5160::Init()
{
    Serial.println(F("TMC5160: Try to Init"));

    if (!TMCBase::Init())
    {
        return false;
    }
    
    this->PrintSettings();

    Serial.println(F("TMC5160: Init was successfull"));

    return true;
}

bool TMC5160::ApplySettings()
{
    TMCBase::ApplySettings();

    m_TMCDriver.pwm_autograd(true);
       
    Serial.println(F("TMC5160 - New  settings applied."));

    this->PrintSettings();

    return true;
}