#include "tmc2130.h"

#include <TMCStepper.h>

TMC2130::TMC2130(uint8_t csPin)
{
    m_csPin = csPin;

    m_TMCDriver = new TMC2130Stepper(csPin, 0.075);
}
TMC2130::~TMC2130()
{
    if(m_TMCDriver)
    {
        delete m_TMCDriver;
        m_TMCDriver = nullptr;
    }
}

void TMC2130::PrintSettings()
{
    uint16_t rmsCurrent = m_TMCDriver->rms_current();
    uint16_t microSteps = m_TMCDriver->microsteps();
    bool en_pwm_mode = m_TMCDriver->en_pwm_mode();
    bool autoScale = m_TMCDriver->pwm_autoscale();
    uint8_t toff = m_TMCDriver->toff(); 
    
    int toffInt = toff;
      
    Serial.println(("RMS Current: ") + String(rmsCurrent));
    Serial.println(("Microsteps: ") + String(microSteps));

    Serial.println(("Toff: ") + String(toffInt));

    if (en_pwm_mode)
    {
        Serial.println(F("StealthChop: Enabled"));
    }
    else
    {
       Serial.println(F("StealthChop: Disabled"));
    }

    if (autoScale)
    {
        Serial.println(F("AutoScale: Enabled"));
    }
    else
    {
       Serial.println(F("AutoScale: Disabled"));
    }
}

bool TMC2130::Init()
{
    SPI.begin();

    uint8_t result = m_TMCDriver->test_connection();

    if (result)
    {
        Serial.println(F("failed!"));
        Serial.print(F("Likely cause: "));

        switch(result)
        {
            case 1: Serial.println(F("loose connection")); break;
            case 2: Serial.println(F("Likely cause: no power")); break;
        }
        Serial.println(F("Fix the problem and reset board."));
        return false;
    }
    
    Serial.println(F("Connect successfully to TMC2130"));

    this->PrintSettings();

    return true;
}

bool TMC2130::ApplySettings()
{
    uint16_t rmsCurrent = 800u;
    uint16_t microSteps = 16u;
    
    m_TMCDriver->push();
    m_TMCDriver->rms_current(rmsCurrent);
    m_TMCDriver->microsteps(microSteps);
    m_TMCDriver->intpol(true);

    Serial.println(F("TMC2130 - New base settings applied."));
    
    return true;
    
    m_TMCDriver->toff(5);
    m_TMCDriver->blank_time(24);
    m_TMCDriver->hysteresis_start(3);
    m_TMCDriver->hysteresis_end(2);
    m_TMCDriver->pwm_autoscale(true);
    //m_TMCDriver->pwm_autograd(1);
    m_TMCDriver->irun(15);
    m_TMCDriver->ihold(8);
    m_TMCDriver->chm(0); 
    m_TMCDriver->tbl(2);
    m_TMCDriver->TPOWERDOWN(10);
    m_TMCDriver->TPWMTHRS(5000);
       
    Serial.println(F("TMC2130 - New advanced settings applied."));

    this->PrintSettings();

    return false;
}