# RAMP
Arduino Interpolation Library

## New objects

The library add a class template to manage interpolation beetween values of various data types.  

* **ramp** default ramp object for "byte" interpolation
* **rampByte** ramp object for "byte" interpolation
* **rampUnsignedChar** ramp object for "unsigned char" interpolation
* **rampChar** ramp object for "char" interpolation
* **rampInt** ramp object for "int" interpolation
* **rampUnsignedInt** ramp object for "unsigned int" interpolation
* **rampLong** ramp object for "long" interpolation
* **rampUnsignedLong** ramp object for "unsigned long" interpolation
* **rampFloat** ramp object for "float" interpolation
* **rampDouble** ramp object for "doubl" interpolation

## Methods

* **go()** go to a new value
* **pause()** pause the interpolation
* **resume()** resume from pause
* **update()** update value of the interpolation according to its parameters

* **getValue()** get actual value of the interpolation
* **getOrigin()** get last origin value of the interpolation
* **getTarget()** get last target value of the interpolation
* **getCompletion()** get the completion percentage of the interpolation
* **getDuration()** get last duration of the interpolation

* **setGrain()** set interpolation grain
* **setAutomation()** set automation mode

* **isPaused()** pause state
* **isRunning()** running state
* **isFinished()** finish state

## Examples

First you need to instanciate the object as a global variable :

	ramp myRamp;

Then you need to a new value to reach : 

	myRamp.go(new_value, (ramp_duration), (ramp_mode), (loop_mode));

At least you'll need to update to get the actual interpolation value :

	myInterpolationValue = myRamp.update();

## Interpolation mode

* **NONE**
* **LINEAR**
* **QUADRATIC_IN**
* **QUADRATIC_OUT**
* **QUADRATIC_INOUT**
* **CUBIC_IN**
* **CUBIC_OUT**
* **CUBIC_INOUT**
* **QUARTIC_IN**
* **QUARTIC_OUT**
* **QUARTIC_INOUT**
* **QUINTIC_IN**
* **QUINTIC_OUT**
* **QUINTIC_INOUT**
* **SINUSOIDAL_IN**
* **SINUSOIDAL_OUT**
* **SINUSOIDAL_INOUT**
* **EXPONENTIAL_IN**
* **EXPONENTIAL_OUT**
* **EXPONENTIAL_INOUT**
* **CIRCULAR_IN**
* **CIRCULAR_OUT**
* **CIRCULAR_INOUT**
* **ELASTIC_IN**
* **ELASTIC_OUT**
* **ELASTIC_INOUT**
* **BACK_IN**
* **BACK_OUT**
* **BACK_INOUT**
* **BOUNCE_IN**
* **BOUNCE_OUT**
* **BOUNCE_INOUT**

## Loop mode

* **ONCEFORWARD**
* **LOOPFORWARD**
* **FORTHANDBACK**
* **ONCEBACKWARD**
* **LOOPBACKWARD**
* **BACKANDFORTH**
