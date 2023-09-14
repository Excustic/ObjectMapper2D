# ObjectMapper2D
An arduino project that shows on a LED matrix the position of on a object placed on a surface, using a gesture and touch sensor.

As it was my first time touching Arduino and working with hardware, it took me a couple of hours to figure out the wiring and configuring everything, but it really should take you half an hour if you have some basic knowledge of how arduino works.

https://github.com/Excustic/ObjectMapper2D/assets/47672175/c75cfe56-3f76-4f0b-8e95-0aacf7f10daf

## Prerequisites
- Arduino UNO
- Sparkfun SEN-12780 (RETIRED)
- RobotDyn Touch sensor (No documentation available online)
- CJMCU 8x8 LED Matrix
- Breadboard to attach the sensors
- External power source

## QuickStart guide
### Connect the ZX axis sensor
<img width="561" alt="image" src="https://github.com/Excustic/ObjectMapper2D/assets/47672175/e9fa9f8e-8d40-4931-b524-fff80bd4dbb8">

### Connect the Touch sensor
I couldn't find any documentation on this part but generally it should look like this, just connect the voltage and ground accordinglt while connecting the sig pin to one of UNO's non-PWM digital ports (they're denoted with ~ sign).
<img width="278" alt="image" src="https://github.com/Excustic/ObjectMapper2D/assets/47672175/8cd649a9-c14e-4215-b71e-50fbce7cdb68">

### Connect the LED Matrix
This requires an external power source as the led matrix does require 5 volts and 2 amps which you can tweak in the program to lower values.
- Connect the Voltage and Ground pins to the external power source.
- Connect an additional ground cable from the uno to the external power source so they could share the same ground.
- Connect the DOUT pin to one of the PWM digital pins in the UNO. (Other sources on the internet tell that the DOUT and DIN are inverted but you should check for yourself to see which pin is working).
![image](https://github.com/Excustic/ObjectMapper2D/assets/47672175/e7115ac7-39b7-4052-ad73-4f789ff86d70)

### Start the program on Arduino and you're ready to go!

Additional images

![image](https://github.com/Excustic/ObjectMapper2D/assets/47672175/787ea06b-9313-4f7c-8e6e-976d4064ac87)
![image](https://github.com/Excustic/ObjectMapper2D/assets/47672175/633062d2-1eaa-4db8-b196-6f51efe78d0e)

## Suggestions for improvement
As this was just a quick demo I tried to build and didn't really care about clarity and reusability too much this code is plain and not very useful for other cases.
This is why I would recommend:
- Generalising and building an interface for a distance sensor having ReadX and ReadY methods which returns floats between 0 and 100.
- Generalising and uilding an interface for a touch sensor having TouchPress, TouchRelease and onTouchListener method with callback.
- Adding funcionality which stacking distance sensors in an array and enlarging the scanning radius.
- Adding functionality so that the program can recognize multiple objects at once.
