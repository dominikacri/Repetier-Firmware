#ifndef IO_TARGET
#error You need to set IO_TARGET before calling this file!
#endif

#undef IO_TMC_5160
#undef IO_TMC_2130

#include "tmc5160.h" 
#include "tmc2130.h" 

#if IO_TARGET == 1 // hardware init

#define IO_TMC_5160(name, csPin)\
    name.Init(); \
    name.ApplySettings();
#define IO_TMC_2130(name, csPin)\
    name.Init(); \
    name.ApplySettings();
#elif IO_TARGET == 4 // class
#define IO_TMC_5160(name, csPin) \
    class name##Class : public TMC5160 { \
    public:\
    name##Class() : TMC5160(csPin) { \
            SET_OUTPUT(csPin); \
            WRITE(csPin, 1); \
        } \
    }; \
    extern name##Class name;
#define IO_TMC_2130(name, csPin) \
    class name##Class : public TMC2130 { \
    public:\
    name##Class() : TMC2130(csPin) { \
            SET_OUTPUT(csPin); \
            WRITE(csPin, 1); \
        } \
    }; \
    extern name##Class name;

#elif IO_TARGET == 6 // variable
#define IO_TMC_5160(name, csPin) \
    name##Class name;
#define IO_TMC_2130(name, csPin) \
    name##Class name;
#else
#define IO_TMC_5160(name, csPin)
#define IO_TMC_2130(name, csPin)
#endif
