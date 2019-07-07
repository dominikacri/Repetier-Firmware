#ifndef IO_TARGET
#error You need to set IO_TARGET before calling this file!
#endif

#undef IO_TMC_5160
#undef IO_TMC_2130

#undef IO_TMC_ENABLE_WARNINGS
#define IO_TMC_ENABLE_WARNINGS(driverName)

#include "tmc/tmc5160.h" 
#include "tmc/tmc2130.h" 

#if IO_TARGET == 1 // hardware init
#define IO_TMC_5160(name5160, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold)\
    name5160.Init();
#define IO_TMC_2130(name2130, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold)\
    name2130.Init();
#elif IO_TARGET == 3 // 100ms
#undef IO_TMC_ENABLE_WARNINGS
#define IO_TMC_ENABLE_WARNINGS(driverName)\
    driverName.ReadErrors();
#define IO_TMC_5160(name5160, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold)
#define IO_TMC_2130(name2130, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold)
#elif IO_TARGET == 4 // class
#define IO_TMC_5160(name5160, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold) \
    class name5160##Class : public TMC5160 { \
    public:\
    name5160##Class() : TMC5160(csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold) { \
            SET_OUTPUT(csPin); \
            WRITE(csPin, 1); \
        } \
    }; \
    extern name5160##Class name5160;
#define IO_TMC_2130(name2130, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold) \
    class name2130##Class : public TMC2130 { \
    public:\
    name2130##Class() : TMC2130(csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold) { \
            SET_OUTPUT(csPin); \
            WRITE(csPin, 1); \
        } \
    }; \
    extern name2130##Class name2130;
#elif IO_TARGET == 6 // variable
#define IO_TMC_5160(name5160, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold) \
    name5160##Class name5160;
#define IO_TMC_2130(name2130, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold) \
    name2130##Class name2130;
#else
#define IO_TMC_5160(name5160, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold)
#define IO_TMC_2130(name2130, csPin, rmsCurrent, microsteps, interpolateMicrosteps, chopperMode, stallguard, stallguardThreshold)
#endif
