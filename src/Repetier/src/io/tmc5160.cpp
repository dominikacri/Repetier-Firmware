#include "tmc5160.h"

#include <TMCStepper.h>

TMC5160::TMC5160(uint8_t csPin)
{
    m_csPin = csPin;

    m_TMCDriver = new TMC5160Stepper(csPin, 0.075);
}
TMC5160::~TMC5160()
{
    if(m_TMCDriver)
    {
        delete m_TMCDriver;
        m_TMCDriver = nullptr;
    }
}

void TMC5160::PrintSettings()
{
    uint16_t rmsCurrent = m_TMCDriver->rms_current();
    uint16_t microSteps = m_TMCDriver->microsteps();
    bool en_pwm_mode = m_TMCDriver->en_pwm_mode();
    bool autoScale = m_TMCDriver->pwm_autoscale();
    uint8_t toff = m_TMCDriver->toff(); 
    
    int toffInt = toff;

    Serial.println(("TMC5160: Settings for Pin:  ") + String(m_csPin));  

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

bool TMC5160::Init()
{
    SPI.begin();

    uint8_t result = m_TMCDriver->test_connection();

    if (result)
    {
        Serial.println("TMC5160: SPI communication failed. Pin: " + String(m_csPin));
        Serial.print(F("TMC5160: Likely cause: "));

        switch(result)
        {
            case 1: Serial.println(F("Loose connection")); break;
            case 2: Serial.println(F("No power")); break;
        }
        Serial.println(F("TMC5160: Fix the problem and reset board."));
        return false;
    }
    
    Serial.println("TMC5160: Connect successfully to Pin: " + String(m_csPin));

    this->PrintSettings();

    return true;
}

bool TMC5160::ApplySettings()
{
    uint16_t rmsCurrent = 1000u;
    uint16_t microSteps = 16u;
    
    m_TMCDriver->rms_current(rmsCurrent);
    m_TMCDriver->microsteps(microSteps);
    m_TMCDriver->intpol(true);
    m_TMCDriver->toff(5);

    Serial.println(F("TMC5160 - New base settings applied."));
    this->PrintSettings();

    return true;
    
    m_TMCDriver->blank_time(24);
    m_TMCDriver->hysteresis_start(3);
    m_TMCDriver->hysteresis_end(2);
    m_TMCDriver->pwm_autoscale(true);
    m_TMCDriver->pwm_autograd(1);
    m_TMCDriver->irun(15);
    m_TMCDriver->ihold(8);
    m_TMCDriver->chm(0); 
    m_TMCDriver->tbl(2);
    m_TMCDriver->TPOWERDOWN(10);
    m_TMCDriver->TPWMTHRS(5000);
       
    Serial.println(F("TMC5160 - New advanced settings applied."));
    this->PrintSettings();

    return true;
}