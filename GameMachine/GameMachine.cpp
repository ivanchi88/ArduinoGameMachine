#include "Arduino.h"
#include "GameMachine.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdlib.h>

// new
void * operator new(size_t size) {
	return malloc(size);
}
// placement new
void * operator new(size_t size, void * ptr) {
	return ptr;
}
// delete
void operator delete(void * ptr) {
	free(ptr);
}

Disparo::Disparo() {
	v = 2;
	existe = false;
}
void Disparo::setX(byte X) {
	x = X;
}
void Disparo::setY(byte Y) {
	y = Y;
}
void Disparo::setExiste(boolean e) {
	existe = e;
}
byte Disparo::getX() {
	return x;
}
byte Disparo::getY() {
	return y;
}
boolean Disparo::getExiste() {
	return existe;
}

void Disparo::moveUp() {
	y -= v;
	if (y < 3) {
		existe = false;
	}
}
void Disparo::draw(Adafruit_SSD1306 *display) {
	display->drawFastVLine(x, y, 3, WHITE);
}

/***************************************************************************************/

Nave::Nave() {
	lifes = -1;
	reset();
}

void Nave::reset() {
	x = 64;
	y = 60;
	w = 6;
	height = 6;

	if (lifes <= 0) {
		lifes = 3;
		points = 0;
	}
	for (byte i = 0; i < 3; i++) {
		disparos[i].setExiste(false);
	}
}
byte Nave::getX() {
	return x;
}
byte Nave::getY() {
	return y;
}
byte Nave::getHeight() {
	return height;
}
byte Nave::getWidth() {
	return width;
}
void Nave::moveLeft() {
	x += v;
}
void Nave::moveRight() {
	x -= v;
}
void Nave::moveUp() {
	y -= v;
}
void Nave::moveDown() {
	y += v;
}

void Nave::draw(int i, Adafruit_SSD1306 *display) {
	drawPlayer(i, display);
}

void Nave::drawPlayer(int dir, Adafruit_SSD1306 *display) {
	if (dir == 0) {
		drawPlayerIdle(display);
	} else if (dir == 1) {
		drawPlayerLeft(display);
	} else if (dir == 2) {
		drawPlayerRight(display);
	}

	for (byte i = 0; i < 3; i++) {
		if (disparos[i].getExiste()) {
			disparos[i].draw(display);
		}
	}
}

void Nave::drawPlayerIdle(Adafruit_SSD1306 *display) {
	display->drawTriangle(x, y, x + 3, y + -height, x + w, y, WHITE);
}

void Nave::drawPlayerLeft(Adafruit_SSD1306 *display) {
	display->drawTriangle(x, y, x + 2, y + -height, x + 4, y - 2, WHITE);
}

void Nave::drawPlayerRight(Adafruit_SSD1306 *display) {
	display->drawTriangle(x + 2, y - 1, x + 4, y + -height, x + w, y,
			WHITE);
}

void Nave::disparar() {
	for (byte i = 0; i < 3; i++) {
		if (!disparos[i].getExiste()) {
			disparos[i].setX(x + w / 2);
			disparos[i].setY(y - height);
			disparos[i].setExiste(true);
			break;

		}
	}
}
void Nave::moverDisparos() {
	for (byte i = 0; i < 3; i++) {
		if (disparos[i].getExiste()) {
			disparos[i].moveUp();
		}
	}

}
Disparo Nave::getDisparo(byte i) {
	return disparos[i];
}
void Nave::addPoints(int p) {
	points += p;
}
unsigned int Nave::getPoints() {
	return points;
}
byte Nave::getLifes() {
	return lifes;
}
void Nave::lostLife() {
	lifes--;
}

/**********************************************************************/

/**********************************************************************/

Asteroid::Asteroid() {
	randomSeed(analogRead(3));
	state = 0;
	reset();
}

byte Asteroid::getX() {
	return x;
}
byte Asteroid::getY() {
	return y;
}
byte Asteroid::getRad() {
	return rad;
}

byte Asteroid::getState() {
	return state;
}

void Asteroid::reset() {
	x = random(0, 125);
	rad = random(3, 8);
	v = random(1, 4);
	y = random(-50, -8);
	state = 0;
}

void Asteroid::fall() {
	if (state == 0) {
		y += v;
		if (y > 125) {
			reset();
		}
	}
}

void Asteroid::draw(Adafruit_SSD1306 *display) {
	if (state == 0) {
		display->drawCircle(x, y, rad, WHITE);
	} else if (state == 1) {
		display->drawCircle(x, y, rad / 2, WHITE);
		state++;
	} else if (state == 2) {
		display->drawCircle(x, y, rad / 3, WHITE);
		reset();
	}
}

void Asteroid::getDestroyed() {
	state++;
}

/************************************************************************/

Game::Game(Adafruit_SSD1306 &display) {
	_display = &display;
}

/**********************************************************************/

SpaceGame::SpaceGame(Adafruit_SSD1306 &display) :
		Game(display) {
	level = 5;
}

boolean SpaceGame::jugar(byte btn_pin, byte x_pin, byte y_pin) {
	boolean replay = true;
	while (replay) {
		moveAll(btn_pin, x_pin, y_pin);
		drawUI();
		if (checkCollitions()) {
			nave.lostLife();
			if (nave.getLifes() <= 0) {
				_display->clearDisplay();
				_display->setTextSize(2);
				_display->setCursor(10, 20);
				_display->print("GAME OVER");
				_display->setCursor(52, 35);
				_display->print(nave.getPoints());
				_display->display();
				_display->clearDisplay();
				level = MIN_LEVEL;
				delay(3000);
				replay = goToMenuScreen(btn_pin, x_pin);
			}
			resetGame();
		}
	}
	return replay;
}

boolean Game::goToMenuScreen(byte btn_pin, byte x_pin)
{
	boolean replay;
	_display->clearDisplay();
					byte exit = 1;
					byte position = 0;
					while (exit != 0) {
						int xDir = analogRead(x_pin);
						if ((xDir > 520) && (position == 1)) {
							position--; //mover izq
						} else if ((xDir < 480) && (position == 0)) {
							position++; //mover dch
						}
						if (digitalRead(btn_pin) == 0) {
							exit = 0;
							replay = position != 1; //si la posicion = 1 (NO) exit to menu
						}

						if (exit != 0) {
							_display->setCursor(10, 20);
							_display->print("CONTINUE?");
							_display->setCursor(10 + position * 60, 40);
							_display->print(">");
							_display->setCursor(25, 40);
							_display->print("YES");
							_display->setCursor(85, 40);
							_display->print("NO");
							_display->display();
							_display->clearDisplay();
						}
					}
		_display->setTextSize(1);
		return replay;
}

void SpaceGame::moverPlayer(byte btn_pin, byte x_pin, byte y_pin) {
	int xDir = 0;
	int yDir = 0;
	byte dir = 0;

	xDir = analogRead(x_pin);
	yDir = analogRead(y_pin);
	if (digitalRead(btn_pin) == 0) {
		nave.disparar();
	}

	if (xDir > 520) {
		//izq
		dir = 1;
		if (nave.getX() > 5) {
			nave.moveRight();
		}
	} else if (xDir < 480) {
		dir = 2;
		if (nave.getX() < 120) {
			nave.moveLeft();
		}
	} else {
		dir = 0;
	}

	if ((yDir > 520) && (nave.getY() < 60)) {
		nave.moveDown();
	} else if ((yDir < 480) && (nave.getY() > 6)) {
		nave.moveUp();
	}

	nave.moverDisparos();
	nave.draw(dir, _display);

	_display->display();
	_display->clearDisplay();
}

void SpaceGame::moveAllAsteroids() {
	for (byte i = 0; i < level; i++) {
		asteroids[i].fall();
		asteroids[i].draw(_display);
	}
}

void SpaceGame::moveAll(byte btn_pin, byte x_pin, byte y_pin) {
	moverPlayer(btn_pin, x_pin, y_pin);
	moveAllAsteroids();
}

boolean SpaceGame::colision(Nave nave, Asteroid asteroid) {
	byte aX, aY, aRad;
	boolean hasCollide = false; // hasCollide = si hay colision true si no permanece false
	aX = asteroid.getX();
	aY = asteroid.getY();
	aRad = asteroid.getRad();
	if (collitionPoint(aX, aY, aRad, nave.getX(), nave.getY())) // punto (0,0) del triangulo
			{
		hasCollide = true;
	} else if (collitionPoint(aX, aY, aRad, nave.getX() + nave.getWidth(),
			nave.getY())) // punto (width,0) del triangulo
			{
		hasCollide = true;
	} else if (collitionPoint(aX, aY, aRad, nave.getX() + nave.getWidth() / 2,
			nave.getY() - nave.getHeight())) //punto (whith/2, heigth);
					{
		hasCollide = true;
	}

	return hasCollide;
}

boolean SpaceGame::collitionPoint(byte aX, byte aY, byte aRad, byte nX,
		byte nY) {
	if ((aX + aRad >= nX) && (aX - aRad <= nX) && (aY + aRad >= nY)
			&& (aY - aRad <= nY)) // punto (0,0) del triangulo
			{
		return true;
	} else
		return false;
}

boolean SpaceGame::checkCollitions() {
	boolean col = false;
	for (byte i = 0; i < level; i++) {
		if (colision(nave, asteroids[i])) {
			col = true;
			break;
		}
		for (byte j = 0; j < 3; j++) {
			if (nave.getDisparo(j).getExiste()) {
				if (collitionPoint(asteroids[i].getX(), asteroids[i].getY(),
						asteroids[i].getRad(), nave.getDisparo(j).getX(),
						nave.getDisparo(j).getY() - 3)) {

					if (asteroids[i].getState() == 0) {
						asteroids[i].getDestroyed();
						nave.addPoints(12 - asteroids[i].getRad());
						nave.getDisparo(j).setExiste(false);
						if (level < N) {
							level = MIN_LEVEL + (nave.getPoints() / 30);
						}
						break;
					}

				}
			}
		}
	}

	return col;
}

void SpaceGame::resetGame() {
	nave.reset();
	for (byte i = 0; i < N; i++) {
		asteroids[i].reset();
	}
}

void SpaceGame::drawUI() {
	_display->setCursor(0, 0);
	_display->print("Pts: ");
	_display->print(nave.getPoints());
	_display->setCursor(80, 0);
	_display->print("Lif: ");
	_display->print(nave.getLifes());

	//_display->print("       ");
	//_display->print(sizeof(_display));
}

/**********************************************************************/
PongBall::PongBall()
{
	randomSeed(analogRead(3));
	reset();
}

byte PongBall::getX()
{
	return x;
}
byte PongBall::getY()
{
	return y;
}
char PongBall:: getXSpeed()
{
	return xSpeed;
}
char PongBall::getYSpeed()
{
	return ySpeed;
}

byte PongBall::getV()
{
	return v;
}
byte PongBall::getRadius()
{
	return radius;
}
void PongBall::setXSpeed(char xs)
{
	xSpeed = xs;
}
void PongBall::setYSpeed(char ys){
	ySpeed = ys;
}

void PongBall::reset()
{
	x = 64;
	y = 32;
	v = 1;
	xSpeed = -1;
	ySpeed = -1;

}

void PongBall::move()
{
	if (xSpeed < 0)
	{
		x+= xSpeed - v;
	}
	else
	{
		x+= xSpeed + v;
	}

	if (ySpeed < 0)
	{
		y+= ySpeed - v;
	}
	else
	{
		y+= ySpeed + v;
	}
}

void PongBall::draw(Adafruit_SSD1306 *display)
{
	//display->drawPixel(x, y, WHITE);
	display->fillRect(x, y, radius, radius, WHITE);
	//display->fillCircle(x, y, radius, WHITE);
	//display->drawCircle(x, y, radius, WHITE);
}

/**********************************************************************/
JugadorPong::JugadorPong(byte r)
{
	rol = r;
	reset();
}

byte JugadorPong::getX()
{
	return x;
}
byte JugadorPong::getY()
{
	return y;
}
byte JugadorPong::getHeight()
{
	return height;
}
byte JugadorPong::getWidth()
{
	return width;
}

void JugadorPong::move (byte yPin)
{
	if (!rol) //maquina
	{
		if ((yPin == 1) && (y > 1))
		{
			y-= speed;
		} else if ((yPin == 2) && (y < 63 - height))
		{
			y+= speed;
		}
	} else //jugador
	{
		int yDir;
		yDir = analogRead(yPin);
		if ((yDir < 480) && (y > 1))
		{
			y-= speed;
		}
		else if ((yDir > 520) && (y < 63 - height))
		{
			y+= speed;
		}
	}
}

void JugadorPong::reset()
{
	if (rol == 0)
	{
		x = 18;
	}
	else
	{
		x = 110;
	}
	y = 30;
	height = 10;
}

void JugadorPong:: draw(Adafruit_SSD1306 *display)
{
	display->fillRect(x, y, width, height, WHITE);
}


/**********************************************************************/


PongGame::PongGame(Adafruit_SSD1306 &display) :
		Game(display) {
	jugadores[0] = new JugadorPong(0);
	jugadores[1] = new JugadorPong(1);
	puntosJugador = 0;
	puntosMaquina = 0;
}

boolean PongGame::jugar (byte btn_pin, byte x_pin, byte y_pin)
{
	boolean replay = true;
	while (replay)
	{
		while (!checkCollitions())
		{
		draw();
		moveAll(y_pin);
		//delay(50);
		}
		_display->clearDisplay();
		if (puntosJugador == maxPoints)
		{
			_display->setTextSize(2);
			_display->setCursor(25, 20);
			_display->print("YOU WON");
			_display->setCursor(45, 35);
			_display->print(puntosMaquina);
			_display->print("  ");
			_display->print(puntosJugador);
			_display->display();
			_display->clearDisplay();
		}
		else if (puntosMaquina == maxPoints)
		{
			_display->setTextSize(2);
			_display->setCursor(25, 20);
			_display->print("YOU LOST");
			_display->setCursor(45, 35);
			_display->print(puntosMaquina);
			_display->print("  ");
			_display->print(puntosJugador);
			_display->display();
			_display->clearDisplay();
		}
		if ((puntosMaquina == maxPoints) || (puntosJugador == maxPoints))
		{
			delay(3000);
			replay = goToMenuScreen(btn_pin, x_pin);
			puntosMaquina = 0;
			puntosJugador = 0;
		}

		reset();
	}
	return replay;
}


void PongGame:: drawPlayers()
{
	jugadores[0]->draw(_display);
	jugadores[1]->draw(_display);
}

void PongGame:: drawGUI()
{
	_display->drawRect(0, 0, 128, 64, WHITE);
	_display->setTextSize(1);
	_display->setCursor(44, 5);
	_display->print(puntosMaquina);
	_display->setCursor(84, 5);
	_display->print(puntosJugador);
}

void PongGame:: draw()
{
	_display->clearDisplay();
	drawPlayers();
	ball.draw(_display);
	drawGUI();
	_display->display();
}

void PongGame::moveAll(byte y_pin)
{
	ball.move();
	movePlayers(y_pin);
}
void PongGame::movePlayers(byte y_pin)
{
	byte dir = 0;
	if (ball.getX() < 80)
	{
		if ((ball.getY() + ball.getRadius() - 5) < jugadores[0]->getY())
		{
			dir = 1; //mover arriba
		}
		else if ((ball.getY() + ball.getRadius() + 5) > (jugadores[0]->getY() + (jugadores[0]->getHeight() >> 1)))
		{
			dir = 2; //mover abajo
		}
	}
	jugadores[0]->move(dir);
	jugadores[1]->move(y_pin);
}
void PongGame::moveBall()
{
	ball.move();
}

boolean PongGame::checkCollitions()  //devuelve true si hay un gol y ya hace lo de las colisiones
{
	boolean colision = false;
	//colisiones con la pared de arriba y abajo
	if ((ball.getYSpeed() < 0) && (ball.getY() < 2))
	{
		ball.setYSpeed(-ball.getYSpeed());
	}
	else if ((ball.getYSpeed() > 0) && (ball.getY() + ball.getRadius() > 62))
	{
		ball.setYSpeed(-ball.getYSpeed());
	}

	// Colisiones de la maquina
	if (ball.getXSpeed() < 0) //Esta yendo a la izquierda
	{
		if ( (ball.getX() <= (jugadores[0]->getX() + jugadores[0]->getWidth() )) && (ball.getX() >= jugadores[0]->getX())) //esta entre el ancho de la pala
		{
			if ((ball.getY() <= jugadores[0]->getY() + jugadores[0]->getHeight()) && (ball.getY() >= jugadores[0]->getY()))
			{
				changeBallDirection(ball.getY(), jugadores[0]->getY(), jugadores[0]->getHeight());
			}
		}
	}
	//colisiones del jugador
	if (ball.getXSpeed() > 0)
	{
		if ( ((ball.getX() + ball.getRadius()) <= (jugadores[1]->getX() + jugadores[1]->getWidth() )) && ((ball.getX() + ball.getRadius()) >= jugadores[1]->getX()))
		{
			if ((ball.getY() <= jugadores[1]->getY() + jugadores[1]->getHeight()) && (ball.getY() >= jugadores[1]->getY()))
						{
							changeBallDirection(ball.getY(), jugadores[1]->getY(), jugadores[1]->getHeight());
						}
		}
	}
	// colisiones de gol
	if (ball.getX() < 5)
	{
		colision = true;
		puntosJugador += 1;
	} else if (ball.getX() > 123)
	{
		colision = true;
		puntosMaquina +=1;
	}
	return colision;
}

void PongGame::changeBallDirection(byte yBall, byte yPlayer, byte heightPlayer)
{
	byte newSpeed = abs(yPlayer + (heightPlayer /2) - yBall);
	if (ball.getXSpeed() < 0)
	{
		ball.setXSpeed( newSpeed/2 + ball.getV());
	}
	else
	{
		ball.setXSpeed(- (newSpeed/2 + ball.getV()));
	}
	if (ball.getYSpeed() < 0)
	{
		ball.setYSpeed(-(newSpeed/2 + ball.getV()));
	}
	else
	{
		ball.setYSpeed(newSpeed/2 + ball.getV());
	}

}

void PongGame::reset()
{
	ball.reset();
	jugadores[0]->reset();
	jugadores[1]->reset();
}
/**********************************************************************/




















/**********************************************************************/
GameMachine::GameMachine(byte btn_pin, byte x_pin, byte y_pin, byte resetPin =
		resetPi) :
		_display(resetPin) {
	btn = btn_pin;
	y_x_Pin = x_pin + (y_pin << 3);
}

void GameMachine::begin(byte oled_i2c_dir = i2c_dir) {

	_display.begin(oled_i2c_dir);

	_display.clearDisplay();
	_display.display();

	pinMode(btn, INPUT);
	digitalWrite(btn, HIGH);

	_display.setTextSize(1);
	_display.setTextColor(WHITE);
	_display.setCursor(0, 0);

}

void GameMachine::menu() {
	byte position = 0;
	byte xPin = (y_x_Pin << 5) >> 5;
	byte yPin = y_x_Pin >> 3;

	while (true) {
		int yDir = analogRead(y_x_Pin >> 3);
		if ((yDir > 520) && (position < LIST_SIZE - 1)) {
			position++;
		} else if ((yDir < 480) && (position > 0)) {
			position--;
		}
		if (digitalRead(btn) == 0) {
			switch (position) {
			case 0:
				SpaceGame* spaceGame;
				spaceGame = new SpaceGame(_display);
				while (spaceGame->jugar(btn, xPin, yPin)) {

				}
				delete (spaceGame);
				break;
			case 1:
				PongGame* pongGame;
				pongGame = new PongGame(_display);
				while (pongGame->jugar(btn, xPin, yPin))
				{

				}
				delete(pongGame);
				break;
			case 2:
				_display.clearDisplay();
				_display.setCursor(20, 20);
				_display.print(gameTable[position]);
				_display.display();
				delay(2000);
				_display.clearDisplay();
				break;
			case 3:
				_display.clearDisplay();
				_display.setCursor(20, 20);
				_display.print(gameTable[position]);
				_display.display();
				delay(2000);
				_display.clearDisplay();
				break;

			}

		}
		if ((position < LIST_SIZE) && (position >= 0)) {
			_display.setCursor(0, position * 10);
			_display.print(">");
		}
		for (byte i = 0; i < LIST_SIZE; i++) {
			_display.setCursor(10, i * 10);
			_display.print(gameTable[i]);
		}
		_display.display();
		_display.clearDisplay();
		delay(100);
	}
}

// zona de los nombres de los juegos

// tabla a la que hay que añadir los nuevos juegos

// buffer de los caracteres

// xPin: (y_x_Pin << 5) >> 5
// yPin: y_x_Pin >> 3

/*const char game_0 [] PROGMEM = "Space Game";
 const char game_1 [] PROGMEM = "game 2";

 const char* const game_table [] PROGMEM = {game_0, game_1};*/

