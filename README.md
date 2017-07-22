# ArduinoGameMachine
Its a simple software that can play small games on an Arduino (I've only tested the uno).

I've used an Arduino Uno compatible board to test it so it should run well on any other similar arduino board.
As the display I use a 128x64 px Oled SSD1306 display, the I2C model.
I also use an analog joystick and a button (I use the build-in button but others should work too)

If you use the same model of display as I do, you can download here its specific library: https://github.com/adafruit/Adafruit_SSD1306
If not, you have to find the right one.
And regardless of the screen model you have to download the Adafruit gfx library: https://learn.adafruit.com/adafruit-gfx-graphics-library/overview and in github https://github.com/adafruit/Adafruit-GFX-Library

The library I wrote consists on a bunch of classes. The main class is the GameMachine that controls the "main menu" and the selection of the game. Its constructor GameMachine (byte btn_pin, byte x_pin, byte y_pin, byte resetPin = resetPi) needs the button pin, the x pin the y pin (from the joystick) and an optional reset pin for the display. 
After creating the object, the begin (byte oled_i2c_dir = i2c_dir) function must be called with the direction of the display.
If you add a new game you must add an element in the gameTable array, increase the LIST_SIZE define and a new case in the menu function. Use the others as a model.

All game classes you make must inherit form the Game class as it has the display and initialises it in its constructor.




