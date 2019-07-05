#pragma once

#include <stdint.h>
#include <TMCStepper.h>

template<class T> class TMCBase
{
public:
    TMCBase<T>(uint8_t csPin) : m_TMCDriver(csPin), m_csPin(csPin) { };

    bool Init() 
    {
        SPI.begin();

        uint8_t result = m_TMCDriver.test_connection();

        if (result)
        {
            Serial.println("SPI communication failed. Pin: " + String(m_csPin));
            Serial.print(F("Likely cause: "));

            switch(result)
            {
                case 1: Serial.println(F("Loose connection")); break;
                case 2: Serial.println(F("No power")); break;
            }
            Serial.println(F("Fix the problem and reset board."));
            return false;
        }
    
        Serial.println("Connected to Pin: " + String(m_csPin));

        return true;
    };

    virtual void PrintSettings() 
    {
        uint16_t rmsCurrent = m_TMCDriver.rms_current();
        uint16_t microSteps = m_TMCDriver.microsteps();

        bool en_pwm_mode = m_TMCDriver.en_pwm_mode();
        uint8_t chopperMode = m_TMCDriver.chm();

        uint8_t toff = m_TMCDriver.toff();
        uint8_t blankTime = m_TMCDriver.blank_time(); 

        bool autoScale = m_TMCDriver.pwm_autoscale();

        uint8_t irun = m_TMCDriver.irun();
        uint8_t ihold = m_TMCDriver.ihold();

        uint8_t hystStart = m_TMCDriver.hysteresis_start();
        int8_t hystEnd = m_TMCDriver.hysteresis_end();

        uint8_t stallguardThreshold = m_TMCDriver.sgt();

        // TODO: extend  string for using uint8
        int toffInt = toff;
        int blankTimeInt = blankTime;
        int irunInt = irun;
        int iholdInt = ihold;

        int hystStartInt = hystStart;
        int hystEndInt = hystEnd;

        Serial.println(("TMCBase: Settings for Pin:  ") + String(m_csPin));  
            
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
    };


protected:
    T m_TMCDriver;
    uint16_t m_csPin;
};