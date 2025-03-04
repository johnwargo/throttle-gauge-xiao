# Throttle Gauge (XAIO)

Built using a Seeed Studio XIAO C6 board and a bunch of NeoPixels.

Reads voltage (0 to 15V) on pin `A0`

Drives NeoPixels on pin `D1`

There's a todo in the code to scale the gauge values so we can achieve full throttle. The voltage divider calls for 1k ohm and 500 ohm resistors but I could only get a 530 ohm for R2. This means the calculated `illuminatedPixels` may never max out. So, once we get this thing running, we'll have to tweak that calculation a little.

## Reference

[Voltage Divider](https://ohmslawcalculator.com/voltage-divider-calculator)

![Voltage Divider Calculator](images/voltage-divider-calculator.png)

