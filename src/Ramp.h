// RAMP library for Arduino
// initial release by Sylvain GARNAVAULT - 2016/08/07

#ifndef Ramp_h
#define Ramp_h

#include "Arduino.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*-----------------------------
 MODES ENUM
 -----------------------------*/

enum ramp_mode {
    NONE                = 0x00,
    LINEAR              = 0x01,
    QUADRATIC_IN        = 0x02,
    QUADRATIC_OUT       = 0x03,
    QUADRATIC_INOUT     = 0x04,
    CUBIC_IN            = 0x05,
    CUBIC_OUT           = 0x06,
    CUBIC_INOUT         = 0x07,
    QUARTIC_IN          = 0x08,
    QUARTIC_OUT         = 0x09,
    QUARTIC_INOUT       = 0x0A,
    QUINTIC_IN          = 0x0B,
    QUINTIC_OUT         = 0x0C,
    QUINTIC_INOUT       = 0x0D,
    SINUSOIDAL_IN       = 0x0E,
    SINUSOIDAL_OUT      = 0x0F,
    SINUSOIDAL_INOUT    = 0x10,
    EXPONENTIAL_IN      = 0x11,
    EXPONENTIAL_OUT     = 0x12,
    EXPONENTIAL_INOUT   = 0x13,
    CIRCULAR_IN         = 0x14,
    CIRCULAR_OUT        = 0x15,
    CIRCULAR_INOUT      = 0x16,
    ELASTIC_IN          = 0x17,
    ELASTIC_OUT         = 0x18,
    ELASTIC_INOUT       = 0x19,
    BACK_IN             = 0x1A,
    BACK_OUT            = 0x1B,
    BACK_INOUT          = 0x1C,
    BOUNCE_IN           = 0x1D,
    BOUNCE_OUT          = 0x1E,
    BOUNCE_INOUT        = 0x1F
};

enum speed_mode {
    FORWARD             = 0x00,
    BACKWARD            = 0x01
};

enum loop_mode {
    ONCEFORWARD         = 0x00,
    LOOPFORWARD         = 0x01,
    FORTHANDBACK        = 0x02,
    ONCEBACKWARD        = 0x03,
    LOOPBACKWARD        = 0x04,
    BACKANDFORTH        = 0x05
};

/*-----------------------------
 GENERIC FUNCTIONS
 -----------------------------*/

float powin(float, unsigned char);
float powout(float, unsigned char);
float powinout(float, unsigned char);
float ramp_calc(float, ramp_mode);

/*-----------------------------
 CLASS TEMPLATE
 -----------------------------*/

template <class T = unsigned char>
class ramp {
private:
    unsigned long pos, dur;                 		// ramp position and duration
	unsigned int  grain;							// grain 
    T val, A, B;                            		// actual, origin and destination values
    bool paused;                            		// paused or not
    enum loop_mode loop;                    		// loop mode
    enum ramp_mode mode;                    		// ramp mode
    enum speed_mode speed;                  		// speed mode
    
public:
    ramp();                                 		// default constructor
    ramp(T);                                		// constructor with initial value
    T value();                              		// get value
    T update();                             		// update values
    T go(T);                                		// go directly to a new value
    T go(T, unsigned long, ramp_mode);              // ramp to _val, within _dur with _mode
    T go(T, unsigned long, ramp_mode, loop_mode);   // ramp to _val, within _dur with _mode
    
    bool isFinished();                      		// is the ramp finished
    bool isRunning();                       		// is the ramp running
    bool isPaused();                        		// is the ramp paused
    void pause();                           		// pause the ramp
    void resume();                          		// resume the ramp
};

/*-----------------------------
 CLASS ALIAS
 -----------------------------*/

typedef ramp<char> rampChar;
typedef ramp<unsigned char> rampUnsignedchar;
typedef ramp<int> rampInt;
typedef ramp<unsigned int> rampUnsignedInt;
typedef ramp<long> rampLong;
typedef ramp<unsigned long> rampUnsignedLong;
typedef ramp<float> rampFloat;
typedef ramp<double> rampDouble;

#endif /* Ramp_h */