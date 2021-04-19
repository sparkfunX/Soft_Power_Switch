SparkX Soft Power Switch
===================================================

[![SparkX Soft Power Switch (SPX-17870)](https://cdn.sparkfun.com//assets/parts/1/7/0/5/9/17870-Soft_Power_Switch-01.jpg)](https://www.sparkfun.com/products/17870)

[*SparkX Soft Power Switch (SPX-17870)*](https://www.sparkfun.com/products/17870)

[![SparkX Soft Power Switch (SPX-17870)](https://cdn.sparkfun.com//assets/parts/1/7/0/5/9/Soft_Power_Switch_Demo.gif)](https://www.sparkfun.com/products/17870)



The Soft Power Switch is a passive, hard on/off switch with software feedback and control. In other words, it's like the on/off switch on a laptop. A simple press will turn the system on. Another press can (with MCU intervention) turn off the system. And if things go really wrong, pressing and holding the button will force a power-down. If you're building something with an enclosed battery and need a good power button, this is the board you need.

The Sense/Control pin can act as an output to a microcontroller indicating the state of the power button (high = not pressed, low = user in pressing power button). This can be used as an input to your firmware to begin to shut down before power is lost. Alternatively, the Sense/Control pin can be driven low by the system forcing power off via software. Additionally, the Fast Off pin can be used to immediately power down a system.

The most common use case is something like this:

* Microcontroller begins running code and checks to see if power button is still being pressed for X milliseconds.
* If not, it's just a tap, go back to sleep.
* If yes, power up and run normal program.

Later...

* User presses power button in normal program
* If power button is pressed for <X milliseconds, it's just a tap, ignore.
* If power button is pressed for >X milliseconds, it's time to power down.

The wake-up and power-down times can be tweaked for your own user experience but we found 500ms for power up and 2s for power down work really well.

If needed, pads are provided for a larger, external button. A measurement jumper is available to measure passive and active current so you can verify the quiescent current of your system.

The Soft Power Switch supports battery charging. If the target device has onboard LiPo charging, current can be passed back through the cut-off circuit and charge the battery.

The Soft Power Switch works up to 12V. There is a 3.3V clamping diode to prevent over-voltage exposure.

The current design is good but has a few problems. The 22uF cap we used requires an override time of 13 seconds. Under normal operation, the system will power down as quickly as you define in the code (1-2s) so users will rarely experience this. But if the system locks up, 13s is a long time to wait. We've also seen a few edge cases (low load or no load) where the override cut-off occurs only after the button is held for >13s and then released. This is caused by shorting between the finger on the button and the near by capacitor. Using an external button allows the unit to power down correctly every time.

We'll continue to refine the design but it's really handy as it is so we decided to release it!

Pros:

* Very low <50nA quiescent current
* Hard override power down in case your microcontroller loses its mind
* Sense/Control pin can be connected to a microcontroller 

Cons:

* Limited to 4A
* No AC loads

Repository Contents
-------------------
* **/Firmware** - Example sketches demonstrating how to read and control the switch
* **/Hardware** - Eagle design files (.brd, .sch)
* **LICENSE.md** - license information

Product Versions
----------------
* [SPX-17870](https://www.sparkfun.com/products/17870) - Original SparkX Release.

License Information
-------------------

This product is _**open source**_!

Please review the LICENSE.md file for license information.

If you have any questions or concerns on licensing, please contact technical support on our [SparkFun forums](https://forum.sparkfun.com/viewforum.php?f=123).

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
