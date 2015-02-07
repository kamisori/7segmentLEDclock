# 7segmentLEDclock
I needed a clock i could read in the dark, so i put this together from an arduino i had lying around and a 7seg LED i got today from Adafruit


Using a Arduino Duemilanova, said 7segment LED Display with backpack from Adafruit, and four buttons i took from an old project i put this together. Might make a nicer enclosure some day.

Using the outer buttons you can scroll through the following "screens": "HH:MM", "MM:ss", "dd.mm.", "YYYY" and finally one screen with all LEDs turned off.
Using the inner two buttons you can select either the left or the right digit group. Then you can use the outer buttons to increment/decrement the digit group which is then highlighted using the decimal dots.

I was lazy and used 30 days for incrementing the month value which makes setting the date kinda awkward.

Also the buttons use the internal pull up resistors of the arduino.
