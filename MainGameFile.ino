#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <GameMachine.h>
#include <stdlib.h>

const byte btn = 2; // digital pin connected to switch output
const byte xPin = 1; // analog pin connected to X output
const byte yPin = 0; // analog pin connected to Y output
// new
/*void * operator new (size_t size) { return malloc (size); }
// placement new
void * operator new (size_t size, void * ptr) { return ptr; }
// delete
void operator delete (void * ptr) { free (ptr); }*/

//Adafruit_SSD1306 display(-1);


void setup()
{

	//display.begin(0x3C);
	//*Adafruit_SSD1306 display(-1);
	//display.begin(0x3C);
	/*display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);

	display.print("MENU");
	display.display();*/
	/*
	 *
	 * prueba github
	 *
	 */

	GameMachine* gameMachine;
	gameMachine = new GameMachine(btn, xPin, yPin);
 
	gameMachine->begin();
	gameMachine->menu();

}

void loop() {

//Serial.print(gameMachine.i);


}
