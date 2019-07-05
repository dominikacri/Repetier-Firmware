#include "tmc5160.h"

TMC5160::TMC5160(uint16_t csPin) : TMCBase(csPin)
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
    uint16_t rmsCurrent = 600;
    uint16_t microSteps = 16;
    bool interpolate = true;
    
    m_TMCDriver.push();

    m_TMCDriver.rms_current(rmsCurrent);
    m_TMCDriver.microsteps(microSteps);
    m_TMCDriver.intpol(interpolate);

    m_TMCDriver.toff(3);
    m_TMCDriver.blank_time(24);
     
    m_TMCDriver.en_pwm_mode(false);
    m_TMCDriver.chm(0); // chopermode spreadcyle instead of consttoff 

    m_TMCDriver.pwm_freq(2);
    m_TMCDriver.pwm_ampl(255);
    m_TMCDriver.pwm_grad(1);
    
    m_TMCDriver.pwm_autoscale(true);
    m_TMCDriver.pwm_autograd(true);

    m_TMCDriver.TPOWERDOWN(128);
   
    //m_TMCDriver->hysteresis_start(3);
    //m_TMCDriver->hysteresis_end(2);
    
    //m_TMCDriver->irun(15);
    //m_TMCDriver->ihold(8);
    
    //m_TMCDriver->TPWMTHRS(5000);
       
    Serial.println(F("TMC5160 - New  settings applied."));
    this->PrintSettings();

    return true;
}