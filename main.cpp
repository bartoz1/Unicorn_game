#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"Engine.h"
#include"Player.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FPS = 120;

const int LICZBA_GWIAZD = 4;
const int LICZBA_WROZEK = 4;
const int LICZBA_ZYC = 3;
const int LICZBA_PLATFORM = 13;
const int LICZBA_PRZESZKOD = 13;

const int LEVEL_WIDTH = 10000;
const int LEVEL_HEIGHT = 960;

// zachowanie kamery na krañcach mapy
void AdjustCamera(SDL_Rect* camera, int playX, int playY) {

	//Center the camera
	camera->x = playX-50;
	camera->y = (playY + Player::PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	if (camera->x < 0) {
		camera->x = 0;
	}
	if (camera->y < 0) {
		camera->y = 0;
	}
	if (camera->x > LEVEL_WIDTH - camera->w) {
		camera->x = 0;
	}
	if (camera->y > LEVEL_HEIGHT - camera->h) {
		camera->y = LEVEL_HEIGHT - camera->h;
	}
}

void HandleNewFrame(int& t2, double& delta, int& t1, double& worldTime, double& fpsTimer, double& fps, int& frames) {
	t2 = SDL_GetTicks();
	delta = (t2 - t1) * 0.001;
	t1 = t2;
	worldTime += delta;

	fpsTimer += delta;
	if (fpsTimer > 0.5) {
		fps = frames * 2;
		frames = 0;
		fpsTimer -= 0.5;
	};
}
// sprawdzenie kolizji miêdzy elementami gry
void CheckForCollisions(Engine& engine, SDL_Rect  stars[4], Player& player, SDL_Rect  fairies[4],
	SDL_Rect  objects[13], SDL_Rect  ground[13], int& quit, SDL_Rect& camera, SDL_Rect  fairiesBorders[4], double& worldTime) {
	// sprawdzenie kolizji z gwiazdami
	for (int i = 0; i < LICZBA_GWIAZD; i++) {
		if (engine.CheckCollision(stars, i + 1, &player.colider) && player.isDashing()) {
			// ukrycie gwiazdy
			player.StarColision();
			engine.ShowBonusPoints(player.AddPoints(2, i + 1));
			stars[i].y = -500;
		}
	}
	// sprawdzenie kolizji z wrozka
	for (int i = 0; i < LICZBA_WROZEK; i++) {
		if (engine.CheckCollision(fairies, i + 1, &player.colider)) {
			// dodanie punktow i ukrycie wró¿ki
			engine.ShowBonusPoints(player.AddPoints(1, i + 1));
			fairies[i].y = -500;
		}
	}

	// sprawdzenie kolizji z elementami gry
	if (engine.CheckCollision(objects, LICZBA_PRZESZKOD, &player.colider) || engine.CheckCollision(ground, LICZBA_PLATFORM, &player.colider) ||
		player.GetPosY() > LEVEL_HEIGHT - 100 || engine.CheckCollision(stars, LICZBA_GWIAZD, &player.colider)) {

		engine.DrawExplosion(player.GetPosX(), player.GetPosY(), camera.x, camera.y);
		player.subtLives();
		quit = engine.ShowCollisionScreen(player.GetLives(), player.totalPoints);
		player.RestartPosition();
		AdjustCamera(&camera, player.GetPosX(), player.GetPosY());
		engine.setStarsPositions(stars);
		engine.setFairiesPositions(fairies, fairiesBorders);
		worldTime = 0;

	};
}

// main
#ifdef __cplusplus
extern "C"
#endif


void CapFrames(int& t2, int t1, double des_frame_del)
{
	t2 = SDL_GetTicks() - t1;
	if (des_frame_del > t2)
		SDL_Delay(des_frame_del - t2);
}

int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc;
	int error, tryb_ster = 0;
	double delta, worldTime, fpsTimer, fps;
	double des_frame_del = 1000 / FPS;
	SDL_Event event;
	SDL_Surface* background;
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// SDL INIT
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	background = SDL_LoadBMP("./tlo_gry.bmp");

	// init silnika
	Engine engine(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, LEVEL_HEIGHT, LEVEL_WIDTH, FPS);
	
	error = engine.LoadCharset();
	if(error == -1) {
		engine.Free();
		SDL_Quit();
		return 1;
	};


	t1 = SDL_GetTicks();
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;

	SDL_Rect objects[LICZBA_PRZESZKOD];
	SDL_Rect ground[LICZBA_PLATFORM];
	SDL_Rect stars[LICZBA_GWIAZD];
	SDL_Rect fairies[LICZBA_WROZEK];
	SDL_Rect fairiesBorders[LICZBA_WROZEK]; // przestrzenie poruszania sie wrozek

	// przypisanie pozycji elementom gry
	engine.setGroundPosition(ground);
	engine.setObjectsPosition(objects);
	engine.setStarsPositions(stars);
	engine.setFairiesPositions(fairies, fairiesBorders);

	Player player(LEVEL_WIDTH, SCREEN_WIDTH, LEVEL_HEIGHT);

	// wyœwietlenie menu
	quit = engine.ShowMainMenu();

	while(!quit) {
		// dostoosowanie zmiennych do nowej klatki
		HandleNewFrame(t2, delta, t1, worldTime, fpsTimer, fps, frames);

		// rysowanie tla
		engine.ClearScreen();
		engine.DrawSurface(background, -camera.x, -camera.y);

		// przyœpieszenie
		player.Faster(FPS);
		
		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) 
						quit = 1;

					else if (event.key.keysym.sym == SDLK_d) {		// zmiana trybu sterowania
						tryb_ster = !tryb_ster;
						player.HandleStearingChange(tryb_ster);
					}

					else if (event.key.keysym.sym == SDLK_n) {		// rozpoczêcie nowej rozgrywnki
						player.RestartPosition();
						AdjustCamera(&camera, player.GetPosX(), player.GetPosY());
						engine.setStarsPositions(stars);
						engine.setFairiesPositions(fairies, fairiesBorders);
						engine.ResetBonus();
						worldTime = 0;
					}

					break;
				case SDL_QUIT:
					quit = 1;
					break;
			};
			// obs³uga velocity graczaze wzgledy na naciœniêty klawisz
			player.HandleMovement(event, tryb_ster);
		};

		// wykonanie ruchu graczem
		player.HandleActionProps(delta, tryb_ster);
		player.Move(ground, LICZBA_PLATFORM, delta, tryb_ster);

		// restert pozycji gwiazd po zawiniêciu ekranu
		if (player.GetPosX() < 100) {
			engine.setFairiesPositions(fairies, fairiesBorders);
			engine.setStarsPositions(stars);
		}

		// ustawienie kamery na graczu
		AdjustCamera(&camera, player.GetPosX(), player.GetPosY());

		// rysowanie elementow wzglêdem pozycji kamery
		engine.DrawObjects(objects, LICZBA_PRZESZKOD, &camera);
		engine.DrawStars(stars, LICZBA_GWIAZD, &camera);
		engine.DrawGround(ground, LICZBA_PLATFORM, &camera);
		engine.DrawFairies(fairies, fairiesBorders, LICZBA_WROZEK, &camera);
		engine.DrawPlayer(&player, camera.x, camera.y, frames);
		
		// sprawdzenie i obs³uga kolizji
		CheckForCollisions(engine, stars, player, fairies, objects, ground, quit, camera, fairiesBorders, worldTime);

		// wyswietlenie paska z info
		engine.UpdateInfo(worldTime, player.GetLives(), player.GetPoints(), delta);

		// wyswietlenie informacji do debugowania
		//engine.DebugInfo(player.GetPosX(), player.GetPosY(), player.velX, player.velY, player.VEL, player.action, player.action_dur, player.GetPoints());
		engine.UpdateScreen();

		// wyœwietlenie menu po stracie ¿yæ
		if (player.zycia <= 0) {
			player.totalPoints = 0;
			player.zycia = LICZBA_ZYC;
			quit = engine.ShowMainMenu();
		}
		frames++;

		// zapewnienie w³aœciwej iloœci kl/s
		CapFrames(t2, t1, des_frame_del);

	};

	SDL_FreeSurface(background);
	SDL_Quit();
	return 0;
}

