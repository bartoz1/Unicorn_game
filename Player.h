#pragma once
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}
class Player
{
public:
	int posX, posY;
	int zycia;
	int starDestr;
	double punkty;
	int totalPoints;
	double VEL;
	SDL_Rect colider;

	static const int PLAYER_WIDTH = 66;
	static const int PLAYER_HEIGHT = 55;
	SDL_Surface* playerSurf;
	SDL_Surface* playerSurfRun[4];
	SDL_Surface* playerSurfDash;
	SDL_Surface* playerSurfStarDestroyer;
	SDL_Surface* playerSurfJump;
	SDL_Surface* playerSurfFall;

	//konstruktor
	Player(int mWidth, int sWidth, int sHeight);

	// zwraca pozycje gracza
	int GetPosX();
	int GetPosY();
	int GetLives();
	int GetPoints();

	void StarColision();

	void LoadImage();

	void HandleMovement(SDL_Event& event, int tryb_ster);

	// dostosowanie przyœpieszen gracza
	// do trybu sterowania
	void HandleStearingChange(int tryb_ster);

	// wykonanie ruchu graczem
	void Move(SDL_Rect* ground, int groundCount, double delta, int tryb_ster);

	void RestartPosition();

	// dodanie punktów z uwzglêdnieniem rodzaju obiektu
	// obiekty: 1 - wrozka; 2 - gwiazda
	int AddPoints(int object, int number);

	// aktualizacja czasu trwania obecnej akcji
	// i jej zmiana w razie potrzeby
	void HandleActionProps(double time, int tryb_ster);

	//sprawdza czy gracz wykonuje zryw
	// zwraca: 1 - TAK ; 0 - NIE
	int isDashing();

	//odjêcie jednego ¿ycia
	void subtLives();

	// Przyspiesz ruch gracza
	void Faster(int fps);

	~Player();

	double velX, velY;
	double action_dur;
	int action;


private:
	int dos_skoki;
	int dos_zryw;
	int mapWidth, mapHeight, screenWidth;
	int tryb_ster;
	double timeVel;
	int starStreak, fairyStreak;
	int lastStarNr, lastFairyNr;


};

