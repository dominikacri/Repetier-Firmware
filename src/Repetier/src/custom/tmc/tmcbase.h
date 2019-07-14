#pragma once

#include <cstdint>
#include <TMCStepper.h>
#include "tmcdefines.h"

// only for debug
//#define MOSI_PIN    75
//#define MISO_PIN    74
//#define SCK_PIN     76
// only for debug

template<class T> class TMCBase
{
public:
    TMCBase<T>(uint16_t csPin, uint16_t rmsCurrent, uint16_t microSteps, bool interpolateMicrosteps, ChopperMode chopperMode, bool stallGuard, int8_t stallGuardThreshold) 
        : m_TMCDriver(csPin),
          m_csPin(csPin),
          m_rmsCurrent(rmsCurrent),
          m_microSteps(microSteps),
          m_interpolateMicroSteps(interpolateMicrosteps),
          m_ChopperMode(chopperMode),
          m_stallGuard(stallGuard),
          m_stallGuardThreshold(stallGuardThreshold),
          m_lastStallGuardResult(0),
          m_lastStallGuardTriggered(false),
          m_lastFullstepActive(false),
          m_lastCurrentScaling(0),
          m_Initialized(false)
    { 

    }


    virtual bool Init() 
    {
        if (m_Initialized)
        {
            return true;
        }
        
        // TODO: BAD HACK, SET THE MAX31xxx SPI Devices to high 
        //pinMode(32, OUTPUT);
        digitalWrite(32, HIGH);
        //pinMode(47, OUTPUT);
        digitalWrite(47, HIGH);
        
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

        m_Initialized = true;
        return true;
    }

    virtual void ReadErrors()
    {
         // TODO: make this faster. each call seems to read the wohle status again and parse only the bits
         
        bool overTemperaturPreWarn = m_TMCDriver.otpw();
        bool overTemperatur = m_TMCDriver.ot();
        bool shortPhaseA = m_TMCDriver.s2ga();
        bool shortPhaseB = m_TMCDriver.s2gb();

        if (overTemperaturPreWarn)
        {
            Serial.println(("Overtemp Prewarn !!! Pin: ") + String(m_csPin));     
        }

        if (overTemperatur)
        {
            Serial.println(("Over temperatur !!! Pin: ") + String(m_csPin));     
        }

        if (shortPhaseA)
        {
            Serial.println(("Short Phase A !!! Pin: ") + String(m_csPin));
        }
        
        if (shortPhaseB)
        {
            Serial.println(("Short Phase B !!! Pin: ") + String(m_csPin));
        }
    }

    virtual void PrintStatusOnChange()
    {
        // TODO: make this faster. each call seems to read the wohle status again and parse only the bits

        uint16_t stallGuardResult = m_TMCDriver.sg_result();
        bool stallGuardTriggered = m_TMCDriver.stallguard();
        bool fullstepActive = m_TMCDriver.fsactive();
        uint8_t currentScaling = m_TMCDriver.cs_actual();

        uint16_t diffStallGuardVal = abs(stallGuardResult - m_lastStallGuardResult);
        if (diffStallGuardVal > 60)
        {
            Serial.println(("Stallguard Result: ") + String(stallGuardResult));
            m_lastStallGuardResult = stallGuardResult;
        }

        if (stallGuardTriggered != m_lastStallGuardTriggered)
        {
            Serial.println(("StallGuard Triggered: ") + String(stallGuardTriggered));
            m_lastStallGuardTriggered = stallGuardTriggered;
        }

        if (fullstepActive != m_lastFullstepActive)
        {
            Serial.println(("Fullstep Active: ") + String(fullstepActive));
            m_lastFullstepActive = fullstepActive;
        }
        
        if (currentScaling != m_lastCurrentScaling)
        {
            Serial.println(("Current Scaling: ") + String(currentScaling));
            m_lastCurrentScaling = currentScaling;
        }
    }

    virtual bool ApplySettings()
    {
        m_TMCDriver.push();

        m_TMCDriver.rms_current(m_rmsCurrent);
        m_TMCDriver.microsteps(m_microSteps);
        m_TMCDriver.intpol(m_interpolateMicroSteps);

        m_TMCDriver.toff(3);
        m_TMCDriver.blank_time(24);

        m_TMCDriver.pwm_freq(2);
        m_TMCDriver.pwm_ampl(255);
        m_TMCDriver.pwm_grad(1);

        if (m_ChopperMode==ChopperMode::SpreadCycle)
        {
            m_TMCDriver.en_pwm_mode(false);
        }

        if (m_ChopperMode==ChopperMode::StealthChop)
        {
            m_TMCDriver.en_pwm_mode(true);
        }

        m_TMCDriver.chm(0); // chopermode spreadcyle instead of consttoff, only if  PWM mode is false
        m_TMCDriver.pwm_autoscale(true);

        m_TMCDriver.TPOWERDOWN(128);


        m_TMCDriver.sgt(m_stallGuardThreshold);

        // stallguard is only available in SpreadCycle
        if (m_ChopperMode == ChopperMode::SpreadCycle && m_stallGuard)
        {
            m_TMCDriver.TCOOLTHRS(1024); // seems that set this before setting diagstall is needed
            m_TMCDriver.diag1_stall(true); 

            //TODO: //#if MOTHERBOARD != 310 // Rambo Einsy has diag0 and diag1 bound together so this could cause a defect
            m_TMCDriver.diag1_pushpull(true);

        }

        // TODO: set TPWMTHRS settings for switching from stealth to spread

        //m_TMCDriver->hysteresis_start(3);
        //m_TMCDriver->hysteresis_end(2);
    
        //m_TMCDriver->irun(15);
        //m_TMCDriver->ihold(8);
    
        //m_TMCDriver->TPWMTHRS(5000);
        return true;
    }

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
    }

    bool WaitForStandStill()
    {
        while(!m_TMCDriver->stst());

        return true;
    }

protected:
    T m_TMCDriver;
    uint16_t m_csPin;
    uint16_t m_rmsCurrent;
    uint16_t m_microSteps;
    bool m_interpolateMicroSteps;
    ChopperMode m_ChopperMode;
    bool m_stallGuard;
    int8_t m_stallGuardThreshold;


    uint16_t m_lastStallGuardResult;
    bool m_lastStallGuardTriggered;
    bool m_lastFullstepActive;
    uint8_t m_lastCurrentScaling;

    bool m_Initialized;

};
