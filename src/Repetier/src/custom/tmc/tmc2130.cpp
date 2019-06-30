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
    uint8_t chopperMode = m_TMCDriver->chm();

    uint8_t toff = m_TMCDriver->toff();
    uint8_t blankTime = m_TMCDriver->blank_time(); 

    bool autoScale = m_TMCDriver->pwm_autoscale();
    
    uint8_t irun = m_TMCDriver->irun();
    uint8_t ihold = m_TMCDriver->ihold();

    uint8_t hystStart = m_TMCDriver->hysteresis_start();
    int8_t hystEnd = m_TMCDriver->hysteresis_end();
    
    uint8_t stallguardThreshold = m_TMCDriver->sgt();
    
    // TODO: extend  string for using uint8
    int toffInt = toff;
    int blankTimeInt = blankTime;
    int irunInt = irun;
    int iholdInt = ihold;

    int hystStartInt = hystStart;
    int hystEndInt = hystEnd;

    Serial.println(("TMC2130: Settings for Pin:  ") + String(m_csPin));  
      
    Serial.println(("RMS Current: ") + String(rmsCurrent));
    Serial.println(("Microsteps: ") + String(microSteps));
    Serial.println(("Toff: ") + String(toffInt));
    Serial.println(("BlankTime: ") + String(blankTimeInt));
    Serial.println(("IRun: ") + String(irunInt));
    Serial.println(("IHold: ") + String(iholdInt));

    Serial.println(("HysteresisStart: ") + String(hystStartInt));
    Serial.println(("HysteresisEnd: ") + String(hystEndInt));

    Serial.println(("StallguardThreshold: ") + String(stallguardThreshold));

    if (en_pwm_mode)
    {
        Serial.println(F("StealthChop: Enabled"));
    }
    else
    {
       Serial.println(F("StealthChop: Disabled"));
    }

    if (!chopperMode) 
    {
        Serial.println(F("ChopperMode: SpreadCycle"));
    }
    else
    {
        Serial.println(F("ChopperMode: Constant Toff"));
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
        Serial.println("TMC2130: SPI communication failed. Pin: " + String(m_csPin));
        Serial.print(F("TMC2130: Likely cause: "));

        switch(result)
        {
            case 1: Serial.println(F("loose connection")); break;
            case 2: Serial.println(F("Likely cause: no power")); break;
        }
        Serial.println(F("TMC2130: Fix the problem and reset board."));
        return false;
    }
    
    Serial.println("TMC2130 - Connect successfully to Pin: " + m_csPin);

    this->PrintSettings();

    return true;
}

bool TMC2130::ApplySettings()
{
    uint16_t rmsCurrent = 600;
    uint16_t microSteps = 16;
    bool interpolate = true;
    
    m_TMCDriver->push();
    m_TMCDriver->I_scale_analog(true);

    m_TMCDriver->rms_current(rmsCurrent);
    m_TMCDriver->microsteps(microSteps);
    m_TMCDriver->intpol(interpolate);
    
    m_TMCDriver->toff(3);
    m_TMCDriver->blank_time(24);
    
    m_TMCDriver->en_pwm_mode(false);
    m_TMCDriver->chm(0); // chopermode spreadcyle instead of consttoff
   
    m_TMCDriver->pwm_freq(2);
    m_TMCDriver->pwm_ampl(255);
    m_TMCDriver->pwm_grad(1);

    m_TMCDriver->pwm_autoscale(true); 
 
    m_TMCDriver->TPOWERDOWN(128);
      
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