// RAMP library for Arduino
// initial release by Sylvain GARNAVAULT - 2016/08/07

#include "Ramp.h"

/*-----------------------------
 CLASS CONSTRUCTORS
 -----------------------------*/

template <class T>
_ramp<T>::_ramp() {
    init((T)0);
}

template <class T>
_ramp<T>::_ramp(T _val) {
    init(_val);
}

template <class T>
void _ramp<T>::init(T _val) {
    A         = _val;
    B         = _val;
    val       = _val;
    mode      = NONE;
    t         = millis();
    dur       = 0;
    pos       = 0;
    grain     = 10;
    loop      = ONCEFORWARD;
    speed     = FORWARD;
    paused    = false;
    automated = true;
}

/*-----------------------------
 RAMP UPDATE
 -----------------------------*/

template <class T>
T _ramp<T>::update() {
    bool doUpdate = true;
    unsigned long newTime = 0;
    unsigned long delta = grain;
    
    if (automated) {
        newTime = millis();
        delta = newTime - t;
        doUpdate = delta >= grain;
    }
    
    if (mode != NONE && doUpdate) {

        t = newTime;
        if (!paused) {
            
            // update ramp position within limits
            switch (speed) {
                case FORWARD:
                    if ((long long)pos + delta < dur) {
                        pos += delta;
                    }
                    else pos = dur;
                    break;
                case BACKWARD:
                    if ((long long)pos - delta > 0) {
                        pos -= delta;
                    }
                    else pos = 0;
                    break;
            }
        
            // update value according to the new position
            if (mode != NONE and dur > 0 and A != B) {
                float k = (float)pos/(float)dur;
				val = B >= A ? (A + (B-A)*ramp_calc(k,mode)) : (A - (A-B)*ramp_calc(k,mode));
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
    }
    
    return val;
}

/*-----------------------------
 RAMP ACTIONS
 -----------------------------*/

template <class T>
T _ramp<T>::go(T _val) {
    return go(_val,0,NONE);
}

template <class T>
T _ramp<T>::go(T _val, unsigned long _dur) {
    return go(_val,_dur,LINEAR);
}

template <class T>
T _ramp<T>::go(T _val, unsigned long _dur, ramp_mode _mode) {
    return go(_val,_dur,_mode,ONCEFORWARD);
}

template <class T>
T _ramp<T>::go(T _val, unsigned long _dur, ramp_mode _mode, loop_mode _loop) {
    A      = val;
    B      = _val;
    mode   = _mode;
    dur    = _dur;
    t      = millis();
    
    if (_dur == 0) val  = B;
    
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
void _ramp<T>::pause() {
    paused = true;
}

template <class T>
void _ramp<T>::resume() {
    paused = false;
}

/*-----------------------------
 RAMP STATES
 -----------------------------*/

template <class T>
bool _ramp<T>::isFinished() {
    if (speed==FORWARD)
        return (pos==dur);
    if (speed==BACKWARD)
        return (pos==0);
    return false;
}

template <class T>
bool _ramp<T>::isRunning() {
    return (!isFinished() && !paused);
}

template <class T>
bool _ramp<T>::isPaused() {
    return (!paused);
}

/*-----------------------------
 RAMP SETTERS
 -----------------------------*/

template <class T>
void _ramp<T>::setGrain(unsigned long _grain) {
    grain = _grain;
}

template <class T>
void _ramp<T>::setAutomation(bool _automated) {
    automated = _automated;
}

/*-----------------------------
 RAMP GETTERS
 -----------------------------*/

template <class T>
float _ramp<T>::getCompletion() {
    float val = pos*10000/dur;
    val /= 100.;
    return (float) val;
}

template <class T>
unsigned long _ramp<T>::getDuration() {
    return dur;
}

template <class T>
unsigned long _ramp<T>::getPosition() {
    return pos;
}

template <class T>
T _ramp<T>::getValue() {
    return val;
}

template <class T>
T _ramp<T>::getOrigin() {
    return A;
}

template <class T>
T _ramp<T>::getTarget() {
    return B;
}


/*-----------------------------
 EXPLICIT DECLARATION
 -----------------------------*/

template class _ramp<char>;
template class _ramp<unsigned char>;
template class _ramp<int>;
template class _ramp<unsigned int>;
template class _ramp<long>;
template class _ramp<unsigned long>;
template class _ramp<float>;
template class _ramp<double>;


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

float ramp_calc(float k, ramp_mode m) {

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
            
        case ELASTIC_OUT:
            a = 1;
            p = 0.3;
            s = p*asin(1/a) / (2*M_PI);
            return (a*pow(2,-10*k)*sin((k-s)*(2*M_PI)/p)+1);
            
            
        case ELASTIC_INOUT:
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
