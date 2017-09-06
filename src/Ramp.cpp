// RAMP library for Arduino
// initial release by Sylvain GARNAVAULT - 2016/08/07

#include "Ramp.h"

/*-----------------------------
 CLASS METHODS
 -----------------------------*/

template <class T>
ramp<T>::ramp() {
    ramp(0);
}

template <class T>
ramp<T>::ramp(T _val) {
    A      = _val;
    B      = _val;
    val    = _val;
    mode   = NONE;
    dur    = 0;
    pos    = 0;
    loop   = ONCEFORWARD;
    speed  = FORWARD;
    paused = false;
}

template <class T>
T ramp<T>::go(T _val) {
    val  = _val;
    go(_val,0,NONE);
}

template <class T>
T ramp<T>::go(T _val, long _dur, byte _mode) {
    return go(_val,_dur,(ramp_mode)_mode);
}

template <class T>
T ramp<T>::go(T _val, long _dur, ramp_mode _mode) {
    A      = val;
    B      = _val;
    mode   = _mode;
    dur    = _dur;
    pos    = 0;
    loop   = ONCEFORWARD;
    speed  = FORWARD;
    paused = false;
    return val;
}

template <class T>
T ramp<T>::go(T _val, long _dur, byte _mode, byte _loop) {
    return go(_val,_dur,(ramp_mode)_mode, (loop_mode)_loop);
}

template <class T>
T ramp<T>::go(T _val, long _dur, ramp_mode _mode, byte _loop) {
    return go(_val,_dur, _mode, (loop_mode)_loop);
}

template <class T>
T ramp<T>::go(T _val, long _dur, byte _mode, loop_mode _loop) {
    return go(_val,_dur,(ramp_mode)_mode, _loop);
}


template <class T>
T ramp<T>::go(T _val, long _dur, ramp_mode _mode, loop_mode _loop) {
    A      = val;
    B      = _val;
    mode   = _mode;
    dur    = _dur;
    if (_loop < ONCEBACKWARD) {
        pos   = 0;
        speed = FORWARD;
    }
    else {
        pos   = dur;
        speed = BACKWARD;
    }
    loop   = _loop;
    paused = false;
    return val;
}

template <class T>
T ramp<T>::value() {
    return val;
}

template <class T>
T ramp<T>::update() {
    if (!paused) {
        switch (speed) {
            case FORWARD:
                if (pos < dur) pos++;
                break;
            case BACKWARD:
                if (pos > 0) pos--;
                break;
        }
        
        if (mode) {
            //float k = (float)pos/(float)dur;
            long  p = (pos*100)/dur;
            float k = p/100.;
            //Serial.println(k);
            val = A + (B-A)*ramp_calc(k,mode); // todo
            constrain(val,A,B);
        }
        else {
            val = B;
        }
    }
    
    if (isFinished()) {
        switch (loop) {
            case LOOPFORWARD:
                pos = 0;
                break;
            case LOOPBACKWARD:
                pos = dur;
                break;
            case FORTHANDBACK:
            case BACKANDFORTH:
                switch (speed) {
                    case FORWARD:
                        speed = BACKWARD;
                        break;
                    case BACKWARD:
                        speed = FORWARD;
                        break;
                }
                break;
                
            case ONCEBACKWARD:
            case ONCEFORWARD:
            default:
                break;
        }
    }
    
    return val;
}

/*-----------------------------
 RAMP STATES & ACTIONS
 -----------------------------*/

template <class T>
bool ramp<T>::isFinished() {
    if (speed==FORWARD)
        return (pos==dur);
    if (speed==BACKWARD)
        return (pos==0);
}

template <class T>
bool ramp<T>::isRunning() {
    return (!isFinished() && !paused);
}

template <class T>
bool ramp<T>::isPaused() {
    return ((bool)!paused);
}

template <class T>
void ramp<T>::pause() {
    paused = true;
}

template <class T>
void ramp<T>::resume() {
    paused = false;
}



/*-----------------------------
 EXPLICIT DECLARATION
 -----------------------------*/

template class ramp<char>;
template class ramp<unsigned char>;
template class ramp<int>;
template class ramp<unsigned int>;
template class ramp<long>;
template class ramp<unsigned long>;
template class ramp<float>;
template class ramp<double>;


/*-----------------------------
 GENERIC FUNCTIONS
 -----------------------------*/

float powin(float k, uint8_t p) {
    return pow(k,p);
}

float powout(float k, uint8_t p) {
    return 1-pow(1-k,p);
}

float powinout(float k, uint8_t p) {
    k *= 2;
    if (k<1)
        return 0.5*pow(k,p);
    return 1-0.5*fabs(pow(2-k,p));
}

float ramp_calc(float k, enum ramp_mode m) {

    if (k == 0 || k == 1)
        return k;
    
    float a, p, s;
    
    switch (m) {
        case QUADRATIC_IN:
            return powin(k,2);
            
        case QUADRATIC_OUT:
            return powout(k,2);
            
        case QUADRATIC_INOUT:
            return powinout(k,2);
            
        case CUBIC_IN:
            return powin(k,3);
            
        case CUBIC_OUT:
            return powout(k,3);
            
        case CUBIC_INOUT:
            return powinout(k,3);
            
        case QUARTIC_IN:
            return powin(k,4);
            
        case QUARTIC_OUT:
            return powout(k,4);
            
        case QUARTIC_INOUT:
            return powinout(k,4);
            
        case QUINTIC_IN:
            return powin(k,5);
            
        case QUINTIC_OUT:
            return powout(k,5);
            
        case QUINTIC_INOUT:
            return powinout(k,5);
            
        case SINUSOIDAL_IN:
            return 1-cos(k*(M_PI/2));
            
        case SINUSOIDAL_OUT:
            return sin(k*(M_PI/2));
            
        case SINUSOIDAL_INOUT:
            return -0.5*(cos(M_PI*k)-1);
            
        case EXPONENTIAL_IN:
            return pow(2,10*(k-1));
            
        case EXPONENTIAL_OUT:
            return (1-pow(2,-10*k));
            
        case EXPONENTIAL_INOUT:
            k *= 2.;
            if (k<1)
                return 0.5*pow(2,10*(k-1));
            k--;
            return 0.5*(2-pow(2,-10*k));
            
        case CIRCULAR_IN:
            return -(sqrt(1-k*k)-1);
            
        case CIRCULAR_OUT:
            k--;
            return sqrt(1-k*k);
            
        case CIRCULAR_INOUT:
            k *= 2;
            if (k<1)
                return -0.5*(sqrt(1-k*k)-1);
            k -= 2;
            return 0.5*(sqrt(1-k*k)+1);
            
        case ELASTIC_IN:
            k -= 1;
            a = 1;
            p = 0.3*1.5;
            s = p*asin(1/a) / (2*M_PI);
            return -a*pow(2,10*k)*sin((k-s)*(2*M_PI)/p);
            
        case ELASTIC_OUT:       //BUG???
            a = 1;
            p = 0.3;
            s = p*asin(1/a) / (2*M_PI);
            return (a*pow(2,-10*k)*sin((k-s)*(2*M_PI)/p)+1);
            
            
        case ELASTIC_INOUT:     //BUG???
            k = k*2 - 1;
            a = 1;
            p = 0.3*1.5;
            s = p*asin(1/a) / (2*M_PI);
            if ((k + 1) < 1)
                return -0.5*a*pow(2,10*k)*sin((k-s)*(2*M_PI)/p);
            return a*pow(2,-10*k)*sin((k-s)*(2*M_PI)/p)*0.5+1;
            
        case BACK_IN:
            s = 1.70158;
            return k*k*((s+1)*k-s);
            
        case BACK_OUT:
            k--;
            s = 1.70158;
            return k*k*((s+1)*k+s)+1;
            
        case BACK_INOUT:
            k *= 2;
            s = 1.70158;
            s *= 1.525;
            if (k < 1)
                return 0.5*k*k*((s+1)*k-s);
            k -= 2;
            return 0.5*k*k*((s+1)*k+s)+1;
            
        case BOUNCE_IN:
            return 1-ramp_calc(1-k,BOUNCE_OUT);
            
        case BOUNCE_OUT:
            if (k < (1/2.75))
                return 7.5625*k*k;
            if (k < (2/2.75)) {
                k -= 1.5/2.75;
                return 7.5625*k*k+0.75;
            }
            if (k < (2.5/2.75)) {
                k -= (2.25/2.75);
                return 7.5625*k*k+0.9375;
            }
            k -= (2.625/2.75);
            return 7.5625*k*k+0.984375;
            
        case BOUNCE_INOUT:
            if (k < 0.5) {
                return ramp_calc(k*2,BOUNCE_IN)*0.5;
            }
            return ramp_calc(k*2-1,BOUNCE_OUT)*0.5+0.5;
            break;
            
        case LINEAR:
        default:
            return k;
    }
}