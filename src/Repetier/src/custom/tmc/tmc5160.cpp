#include "tmc5160.h"
#include "Repetier.h"

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

    Serial.println(F("TMC5160: Init was successfull"));

    this->PrintSettings();

    this->ApplySettings();

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
