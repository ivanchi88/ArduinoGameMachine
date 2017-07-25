#ifndef SpaceGame_h
#define SpaceGame_h
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Disparo {
private:
	byte x, y, v;
	boolean existe;
public:
	Disparo(); //inicia un disparo con la v y existe = false
	void setX(byte X);
	void setY(byte Y);
	void setExiste(boolean e);
	byte getX();
	byte getY();
	boolean getExiste();
	void moveUp();	//actualiza la posicion de la bala
	void draw(Adafruit_SSD1306 *display); //dibuja la bala de tamaño  1x3 px
};

/*****************************************************************************************/

class Nave {
private:
	byte x, y, height, w;
	byte v = 2;
	char lifes;
	unsigned int points;
	Disparo disparos[3]; //cargador con 3 balas
public:
	Nave(); //inicializa el jugador con lifes = -1 y reset()
	void reset(); //restaura la posicion inicial, las vidas si hiciera falta y los disparos
	byte getX();
	byte getY();
	byte getHeight();
	byte getWidth();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void draw(int i, Adafruit_SSD1306 *display); //para que fuera un metodo comun a todas las clases
	void drawPlayer(int dir, Adafruit_SSD1306 *display); //dibuja al personaje en funcion de la direccion
	void drawPlayerIdle(Adafruit_SSD1306 *display); //nave quieta
	void drawPlayerLeft(Adafruit_SSD1306 *display); //nave a la izq
	void drawPlayerRight(Adafruit_SSD1306 *display); //nave a la dch
	void disparar(); // activa el primer disparo disponible desde la punta de la nave
	void moverDisparos(); //mueve todos los disparos activos
	Disparo getDisparo(byte i); //devuelve el disparo seleccionado
	void addPoints(int p); //suma puntuacion
	unsigned int getPoints();
	byte getLifes();
	void lostLife(); //resta una vida
};

/**********************************************************************/

/**********************************************************************/

class Asteroid {
private:
	byte x, rad, v, state;
	char y;
public:
	Asteroid(); //crea una semilla aleatoria. estado = 0 y reset()
	byte getX();
	byte getY();
	byte getRad();
	byte getState();
	void reset(); //determina la posicion aleatoria
	void fall(); // desplaza el asteroide  si el estado == 0
	void draw(Adafruit_SSD1306 *display);
	void getDestroyed(); // aumenta el estado para que pase a "destruido".

};

/************************************************************************/

class Game {
protected:
	Adafruit_SSD1306 *_display; // puntero a la instancia del display
public:
	Game(Adafruit_SSD1306 &display);
	boolean goToMenuScreen(byte btn_pin, byte x_pin);

};

/**********************************************************************/

class SpaceGame: public Game {
private:
#define N 30 //Numero de asteroides que habra (tambien el nivel)
#define MIN_LEVEL 5 //nivel minimo
	byte level;
	Nave nave;
	Asteroid asteroids[N];

public:
	SpaceGame(Adafruit_SSD1306 &display);
	boolean jugar(byte btn_pin, byte x_pin, byte y_pin);
	void moverPlayer(byte btn_pin, byte x_pin, byte y_pin);
	void moveAllAsteroids();
	void moveAll(byte btn_pin, byte x_pin, byte y_pin);
	boolean colision(Nave nave, Asteroid asteroid);
	boolean collitionPoint(byte aX, byte aY, byte aRad, byte nX, byte nY);
	boolean checkCollitions();
	void resetGame();
	void drawUI();
};

/**************************************************************************/

class PongBall {
	private:
		byte x, y, v; //x, y, velocity,
		char  xSpeed, ySpeed; //horizontal speed & vertical speed
		#define radius 2
	public:
		PongBall();
		byte getX();
		byte getY();
		char getXSpeed();
		char getYSpeed();
		byte getV();
		byte getRadius();

		void setXSpeed(char xs);
		void setYSpeed(char ys);

		void reset();
		void move();
		void draw(Adafruit_SSD1306 *display);

};
/**************************************************************************/
class JugadorPong {
private:
	byte y, x, height, rol, speed; //rol es 0 maquina y 1 jugador
#define width 2
#define speed 2

public:
	JugadorPong (byte r);
	byte getX();
	byte getY();
	byte getHeight();
	byte getWidth();
	void draw(Adafruit_SSD1306 *display);
	void move (byte yPin);
	void reset();
};

/**************************************************************************/

class PongGame : public Game{
private:
	JugadorPong *jugadores [2];
	PongBall ball;
	byte puntosJugador, puntosMaquina;
#define maxPoints 3
public:
	PongGame (Adafruit_SSD1306 &display);
	boolean jugar(byte btn_pin, byte x_pin, byte y_pin);
	void drawPlayers();
	void drawGUI();
	void draw();
	void movePlayers(byte y_pin);
	void moveAll(byte y_pin);
	void moveBall();
	boolean checkCollitions(); //devuelve true si hay un gol y ya hace lo de las colisiones
	void changeBallDirection(byte yBall, byte yPlayer, byte heightPlayer);
	void reset();
};





/**************************************************************************/
class GameMachine {
	//dynamic_cast //para hacer el casting de game a las subclases.
private:
	Adafruit_SSD1306 _display;
	byte btn, y_x_Pin;
	/* y_x_Pin: <----------8bits------------>  (1byte)
	 * 		    <-vacio-> <-yPin->   <-xPin->
	 * 		    <-2bits-> <-3bits-> <-3bits->
	 *
	 * 		    xPin: (y_x_Pin << 5) >> 5
	 * 		    yPin: y_x_Pin >> 3
	 *
	 * 		    No se para que me complico tanto la vida para ahorrar un byte
	 *
	 */
#define i2c_dir 0x3C
#define resetPi -1
#define LIST_SIZE 4

	const char* gameTable[4] = { "Space Game", "PONG", "Game Three",
			"Game Four" };

	/*Cada nombre del juego puede ocupar 15 caracteres maximo*/

public:
	//GameMachine(byte oled_i2c_dir, byte btn_pin, byte x_pin, byte y_pin);
	GameMachine(byte btn_pin, byte x_pin, byte y_pin, byte resetPin = resetPi);
	void menu();
	void begin(byte oled_i2c_dir = i2c_dir);
};

#endif
