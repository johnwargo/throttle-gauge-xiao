# Throttle Gauge (XAIO ESP32-C6)

A simple throttle guage for my friend's Porsche Boxter race car.

The guage is a strip of 20 Adafruit NeoPixels arranged in a strip. The first 15 represent the throttle position (from 1 to 15). Illuminating the NeoPixels in Green until the throttle hits 80%, then all NeoPixels turn White. The last 5 NeoPixels indicate whether the brake is on or off; the final 5 NeoPixels illuminate Red when the driver presses the brake pedal.

![NeoPixel strip](images/neopixel-strip.jpg)

## Hardware Components

+ [Seeed Studio XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html)
+ [Breadboard-friendly RGB Smart NeoPixel - Sheet of 25](https://www.adafruit.com/product/1558)
+ [12V to 5V DC Buck Converter](https://www.amazon.com/Converter-Voltage-Waterproof-Regulator-Step-Down/dp/B07Y2V1F8V)

## Gauge PCB

To make the gauge easier to assemble, I created a circuit board for it using [Fritzing](https://fritzing.org/). I always wanted to learn how to design my own circuit boards, so this was my very first. You can find the design for the board in the repository's [Fritzing](Fritzing) folder.

![Fritzing PCB View](images/fritzing-pcb.png)



Reads voltage (0 to 15V) on pin `A0`

Drives NeoPixels on pin `D1`

There's a todo in the code to scale the gauge values so we can achieve full throttle. The voltage divider calls for 1k ohm and 500 ohm resistors but I could only get a 530 ohm for R2. This means the calculated `illuminatedPixels` may never max out. So, once we get this thing running, we'll have to tweak that calculation a little.

## Reference

[Voltage Divider](https://ohmslawcalculator.com/voltage-divider-calculator)

![Voltage Divider Calculator](images/voltage-divider-calculator.png)

