#ifndef IO_TARGET
#error You need to set IO_TARGET before calling this file!
#endif

#undef IO_TMC_5160
#undef IO_TMC_2130

#include "tmc5160.h" 
#include "tmc2130.h" 

#if IO_TARGET == 1 // hardware init

#define IO_TMC_5160(name5160, csPin5160)\
    if(name5160.Init()) {name5160.ApplySettings();};
#define IO_TMC_2130(name2130, csPin2130)\
    if(name2130.Init()) {name2130.ApplySettings();};
#elif IO_TARGET == 4 // class
#define IO_TMC_5160(name5160, csPin5160) \
    class name5160##Class : public TMC5160 { \
    public:\
    name5160##Class() : TMC5160(csPin5160) { \
            SET_OUTPUT(csPin5160); \
            WRITE(csPin5160, 1); \
        } \
    }; \
    extern name5160##Class name5160;
#define IO_TMC_2130(name2130, csPin2130) \
    class name2130##Class : public TMC2130 { \
    public:\
    name2130##Class() : TMC2130(csPin2130) { \
            SET_OUTPUT(csPin2130); \
            WRITE(csPin2130, 1); \
        } \
    }; \
    extern name2130##Class name2130;

#elif IO_TARGET == 6 // variable
#define IO_TMC_5160(name5160, csPin5160) \
    name5160##Class name5160;
#define IO_TMC_2130(name2130, csPin2130) \
    name2130##Class name2130;
#else
#define IO_TMC_5160(name5160, csPin5160)
#define IO_TMC_2130(name2130, csPin2130)
#endif
