#include "tmc2130.h"

TMC2130::TMC2130(uint16_t csPin) :
    TMCBase(csPin)
{

}

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
    
    this->PrintSettings();

    Serial.println(F("TMC2130: Init was successfull"));

    return true;
}

bool TMC2130::ApplySettings()
{
    uint16_t rmsCurrent = 600;
    uint16_t microSteps = 16;
    bool interpolate = true;
    
    m_TMCDriver.push();
    m_TMCDriver.I_scale_analog(true);

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
 
    m_TMCDriver.TPOWERDOWN(128);
      
    //m_TMCDriver->hysteresis_start(3);
    //m_TMCDriver->hysteresis_end(2);

    //m_TMCDriver->irun(15);
    //m_TMCDriver->ihold(8);
    
    //m_TMCDriver->TPWMTHRS(5000);
       
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