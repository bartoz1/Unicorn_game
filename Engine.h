#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"Player.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}
class Engine
{
public:
	// inicjalizacja
	Engine(SDL_Window* lwindow, SDL_Renderer* lrenderer, int screenWidth, int screenHeight, int lHeight, int lWidth, int fps);

	// narysowanie napisu txt na powierzchni screen, zaczynaj�c od punktu (x, y)
	void DrawString(int x, int y, const char* text);

	// narysowanie na ekranie powierzchni sprite w punkcie (x, y)
	// (x, y) to punkt �rodka obrazka sprite na ekranie
	void DrawSurface(SDL_Surface* sprite, int x, int y);

	// rysowanie pojedynczego pixela
	void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

	// rysowanie linii o d�ugo�ci l w pionie (gdy dx = 0, dy = 1) 
	// b�d� poziomie (gdy dx = 1, dy = 0)
	void DrawLine(int x, int y, int l, int dx, int dy, Uint32 color);

	// rysowanie prostok�ta o d�ugo�ci bok�w l i k
	void DrawRectangle( int x, int y, int l, int k,
		Uint32 outlineColor, Uint32 fillColor);

	// Rysowanie gracza w (x,y)
	void DrawPlayer(Player* player, int camX, int camY, int frames);

	// rysowanie przeszk�d je�eli s� one w widoku kamery
	void DrawObjects(SDL_Rect* objects, int objectsCount, SDL_Rect* camera);

	// rysowanie pod�o�a
	void DrawGround(SDL_Rect* objects, int objectsCount, SDL_Rect* camera);

	// rysowanie gwiaz
	void DrawStars(SDL_Rect* stars, int starsCount, SDL_Rect* camera);

	// rysowanie wrozek
	void DrawFairies(SDL_Rect* fairies, SDL_Rect* fairiesBorders, int fairiesCount, SDL_Rect* camera);

	// sprawdza kolizj� mi�dzy coliderem gracza a przeszkodami
	int CheckCollision(SDL_Rect* objects, int onjectsCount, SDL_Rect* colider);

	// przypisanie gwiazdom pozycji
	void setStarsPositions(SDL_Rect* stars);

	// przypisanie wartosci platform
	void setGroundPosition(SDL_Rect* ground);

	// przypisanie wartosci przeszkod
	void setObjectsPosition(SDL_Rect* objects);

	// przypisanie warto�ci wr�kom
	void setFairiesPositions(SDL_Rect* fairies, SDL_Rect* fairiesBorders);

	// losowe poruszanie wr�ka
	void MoveFairyRandomly(SDL_Rect* activeFairy, SDL_Rect* activeFairyBorder);

	// zwolnienie wsszystkich zmiennych
	// i elementow sdl
	void Free();

	// aktualizuje czas i zycia w menu
	void UpdateInfo(double worldTime, int lives, int points, double delta);

	// wyswietlenie informacji pomocniczych
	void DebugInfo(int x, int y, int velX, int velY, double vel,int action, double dur, int points );

	int ShowCollisionScreen(int lives, int totalPoints);

	// wy�wietlenie ekranu z mo�liwo�ci� zapisania wyniku
	int ShowSaveScreen(int points);

	int ShowLeaderBoard();

	// wype�niene ekrany jednolitym kolorem
	void ClearScreen();
	// ustawienie parametrow do wyswietlenia bonusu
	void ShowBonusPoints(int points);
	void ResetBonus();

	int ShowMainMenu();
	int LoadCharset();			// za�adowanie tablicy znak�w

	void UpdateScreen();		// wyswietlenie na ekranie

	void DrawExplosion(int x, int y, int cx, int cy);

	// Destruktor
	~Engine();

private: 
	int  mScreenWidth, mScreenHeight, mLevelHeight, mLevelWidth, mfps;
	double mdelta;
	int bonus, bonusTimer;
	Uint32 czarny, zloty, zielony, czerwony, niebieski;

	SDL_Window* window; SDL_Renderer* renderer;
	SDL_Surface* screen, * charset, * heart, *fairy, *dolphin, *star;
	SDL_Texture* scrtex;
	SDL_Surface* explosionSurf[21];

	void DrawDolfins(int count, double animator);

	void LoadHeart();		// za��dowanie grafiki serc
	void LoadStar();		// za��dowanie grafiki gwiazdy
	void LoadFairy();		// za�adowanie grafiki wrozki
	void LoadDolphin();		// zaladowanie grafiki delfina
	void LoadExplosion();
	void SavePointsToFile(char *tekst);
	int SortSaveFile();

};

