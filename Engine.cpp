#include "Engine.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>

const int RUN = 1;
const int JUMP = 2;
const int DASH = 3;
const int FALL = 4;
const int STARDESTROYER = 5;
const int CZAS_WYSW_BONUSU = 200; // czas w liczbie kl wyœwietlania bonusu
const int KAMIENIE_MILOWE = 1000;
const int TEXT_BUFFER = 128;
const int L_WYNIKOW_NA_STR = 5;

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

Engine::Engine(SDL_Window* lwindow, SDL_Renderer* lrenderer, int screenWidth, int screenHeight, int lHeight, int lWidth, int fps) {
	
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	window = lwindow;
	renderer = lrenderer;
	mLevelHeight = lHeight;
	mLevelWidth = lWidth;
	charset = NULL;
	mfps = fps;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, mScreenWidth, mScreenHeight);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "Jednorozec");

	LoadHeart();
	LoadFairy();
	LoadStar();
	ResetBonus();
	LoadDolphin();
	LoadExplosion();

	// inicjalizacja generatora losowych liczb
	srand(time(NULL));

	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	screen = SDL_CreateRGBSurface(0, mScreenWidth, mScreenHeight, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		mScreenWidth, mScreenHeight);

	// przypisanie kolorow do zmiennych
	czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	zloty = SDL_MapRGB(screen->format, 0xD4, 0xAF, 0x37);

	mdelta = 0;

}

void Engine::UpdateScreen() {
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	//		SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

Engine ::~Engine() {
	Free();
}
void Engine::Free() {
	SDL_FreeSurface(charset);
	if (screen != NULL) {
		SDL_FreeSurface(screen);
	}
	for (int i = 0; i < 21; i++) {
		SDL_FreeSurface(explosionSurf[i]);
	}
	SDL_FreeSurface(fairy);
	SDL_FreeSurface(dolphin);
	SDL_FreeSurface(star);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
}
void Engine::ClearScreen() {
	SDL_FillRect(screen, NULL, czarny);
}
void Engine::setStarsPositions(SDL_Rect* stars) {
	int los = 0, los2 = 0;

	 los = rand() % 100 - 50;
	 los2 = rand() % 2;

	switch (los2)
	{
	case 0:	stars[0] = { 1328 , mLevelHeight - 450, 50, 50 };	break;
	case 1:	stars[0] = { 1328 , mLevelHeight - 150, 50, 50 };	break;
	}
	los2 = rand() % 2;
	los = rand() % 100 - 50;

	switch (los2)
	{
	case 0:	stars[1] = { 4000 + los, mLevelHeight - 150, 50, 50 };	break;
	case 1:	stars[1] = { 4453 , mLevelHeight - 450, 50, 50 };	break;
	}
	los2 = rand() % 2;
	los = rand() % 100 - 50;

	switch (los2)
	{
	case 0:	stars[2] = { 7000 + los, mLevelHeight - 150, 50, 50 };	break;
	case 1:	stars[2] = { 6209 + los, mLevelHeight - 150, 50, 50 };	break;
	}
	los2 = rand() % 2;

	switch (los2)
	{
	case 0:	stars[3] = { 8130 + los, mLevelHeight - 150, 50, 50 };break;
	case 1: stars[3] = { 7300 + los, mLevelHeight - 150, 50, 50 };break;
	}

}
void Engine::setFairiesPositions(SDL_Rect* fairies, SDL_Rect* fairiesBorders) {
	
	int los = rand() % 3;
	switch (los)
	{
	case 0:	fairies[0] = { 1200, mLevelHeight-150, 50, 50 };	break;
	case 1:	fairies[0] = { 1200, mLevelHeight - 550, 50, 50 };	break;
	case 2:	fairies[0] = { 1900, mLevelHeight - 150, 50, 50 };	break;
	}

	los = rand() % 3;
	switch (los)
	{
	case 0:	fairies[1] = { 3400, mLevelHeight - 150, 50, 50 };	break;
	case 1:	fairies[1] = { 3000, mLevelHeight - 350, 50, 50 };	break;
	case 2:	fairies[1] = { 3900, mLevelHeight - 150, 50, 50 };	break;
	}

	los = rand() % 3;
	switch (los)
	{
	case 0:	fairies[2] = { 5400, mLevelHeight - 150, 50, 50 };	break;
	case 1:	fairies[2] = { 5000, mLevelHeight - 350, 50, 50 };	break;
	case 2:	fairies[2] = { 5900, mLevelHeight - 150, 50, 50 };	break;
	}

	los = rand() % 3;
	switch (los)
	{
	case 0:	fairies[3] = { 7400, mLevelHeight - 150, 50, 50 };	break;
	case 1:	fairies[3] = { 7000, mLevelHeight - 350, 50, 50 };	break;
	case 2:	fairies[3] = { 7900, mLevelHeight - 150, 50, 50 };	break;
	}
	
	// ustawienie obszaru poruszania siê i wycentrowanie pozycji
	for (int i = 0; i < 4; i++) {
		fairiesBorders[i].x = fairies[i].x - 200;
		fairiesBorders[i].y = fairies[i].y - 200;
		fairiesBorders[i].w = fairies[i].w + 200;
		fairiesBorders[i].h = fairies[i].h + 200;
		fairies[i].x -= fairiesBorders[i].w / 2;
		fairies[i].y -= fairiesBorders[i].h / 2;
	}
}
void Engine::MoveFairyRandomly(SDL_Rect* activeFairy, SDL_Rect* activeFairyBorder) {
	int czy_wykonywac = rand() % 3;
	int mX = 0;
	int mY = 0;

	for (int i = 0; i < 2; i++) {

		int los = rand() % 100;
		switch (los / 4)
		{
		case 0:
			mX = 10;
			break;
		case 1:
			mY = 10;
			break;
		case 2:
			mX = -10;
			break;
		case 3:
			mY = -10;
			break;

		}
	}
	activeFairy->x += mX;
	activeFairy->y += mY;

	if (activeFairy->x < activeFairyBorder->x || activeFairy->x > activeFairyBorder->x + activeFairyBorder->w - activeFairy->w)
		activeFairy->x -= mX;

	if (activeFairy->y < activeFairyBorder->y || activeFairy->y > activeFairyBorder->y + activeFairyBorder->h - activeFairy->h)
		activeFairy->y -= mY;
}

int Engine::LoadCharset() {
	charset = NULL;
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
		return -1;
	}
	else {
		SDL_SetColorKey(charset, true, 0x000000);
		return 1;
	}
}
void Engine::LoadHeart() {
	heart = NULL;
	heart = SDL_LoadBMP("./heart.bmp");
	if (heart == NULL) {
		printf("SDL_LoadBMP(heart.bmp) error: %s\n", SDL_GetError());
		exit(-1);
	}
	else {
		SDL_SetColorKey(heart, true, 0xFFFFFF);
	}
}
void Engine::LoadDolphin() {
	dolphin = NULL;
	dolphin = SDL_LoadBMP("./delfin.bmp");
	if (dolphin == NULL) {
		printf("SDL_LoadBMP(delfin.bmp) error: %s\n", SDL_GetError());
		exit(-1);
	}
	else {
		SDL_SetColorKey(dolphin, true, 0xFFFFFF);
	}
}
void Engine::LoadStar() {
	star = NULL;
	star = SDL_LoadBMP("./star.bmp");
	if (star == NULL) {
		printf("SDL_LoadBMP(star.bmp) error: %s\n", SDL_GetError());
		exit(-1);
	}
	
}
void Engine::LoadFairy() {
	fairy = NULL;
	fairy = SDL_LoadBMP("./wrozka.bmp");
	if (fairy == NULL) {
		printf("SDL_LoadBMP(heart.bmp) error: %s\n", SDL_GetError());
		exit(-1);
	}
}
void Engine::UpdateInfo(double worldTime, int zycia, int points, double delta) {
	char text[TEXT_BUFFER];

	mdelta = delta;

	// tekst informacyjny / info text
	DrawRectangle( 4, 4, mScreenWidth - 8, 36, czerwony, niebieski);
	
	sprintf(text, "czas biedu = %.1lf s, punkty: %d", worldTime, points);
	DrawString( screen->w / 2 - strlen(text) * 8 / 2, 10, text);
	
	sprintf(text, "Esc - wyjscie, n - nowa gra, d - zmiana sterowania");
	DrawString( screen->w / 2 - strlen(text) * 8 / 2, 26, text);

	for (int i = 1; i <= zycia; i++) {
		DrawSurface(heart, 40*i- 35, 45);
	}

	if (bonusTimer > 0) {
		sprintf(text, "+ %d", bonus);
		DrawString(screen->w / 2 - strlen(text) * 8, 130, text);
		bonusTimer--;
	}

	if (points > KAMIENIE_MILOWE) {
		DrawDolfins(points / KAMIENIE_MILOWE, worldTime );
	}
}

void Engine::DrawDolfins(int count, double animator) {
	int pozycja, x, y, wariacja;

	pozycja = mScreenWidth / 2 - (dolphin->w * count) / 2;

	wariacja = sin(animator) * 40 - 10;

	y = mScreenHeight - dolphin->h + wariacja;

	for (int i = 0; i < count; i++) {
		x = pozycja + i*dolphin->w ;
		DrawSurface(dolphin, x, y);
	}
}

void Engine::ShowBonusPoints(int points) {
	bonus = points;
	bonusTimer = CZAS_WYSW_BONUSU;
}

void Engine::ResetBonus() {
	bonus = 0;
	bonusTimer = 0;
}

void Engine::DebugInfo(int x, int y, int velX, int velY, double vel, int action, double dur, int points) {
	char text[TEXT_BUFFER];

	sprintf(text, "gracz(%d, %d) velX: %d velY: %d VEL: %.4f act: %d dur: %.2f s, punkty: %d /s", x, y, velX, velY, vel, action, dur, points);
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 50, text);

}


void Engine::DrawString(int x, int y, const char* text) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void Engine::DrawSurface(SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x ;
	dest.y = y ;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
void Engine::DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
void Engine::DrawLine(int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		//if (mScreenWidth >= y && mScreenHeight >= y) {
			DrawPixel(screen, x, y, color);
			x += dx;
			y += dy;

		//}
	};
};


// rysowanie prostok¹ta o d³ugoœci boków l i k
void Engine::DrawRectangle( int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine( x, y, k, 0, 1, outlineColor);
	DrawLine( x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine( x, y, l, 1, 0, outlineColor);
	DrawLine( x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine( x + 1, i, l - 2, 1, 0, fillColor);
};

// rysowanie gracza na okranie / na kamerze
void Engine::DrawPlayer(Player* player, int camX, int camY, int frames) {

	SDL_Surface* wybrana = NULL;

	switch (player->action)
	{
	case RUN:
		//wybrana = player->playerSurfRun[(frames / 7) % 4];
		wybrana = player->playerSurfRun[player->GetPosX()/40 % 4];
		break;
	case JUMP:
		wybrana = player->playerSurfJump;
		break;
	case FALL:
		wybrana = player->playerSurfFall;
		break;
	case DASH:
		wybrana = player->playerSurfDash;
		break;
	case STARDESTROYER:
		wybrana = player->playerSurfStarDestroyer;
		break;
	default:
		wybrana = player->playerSurf;
		break;

	}


	DrawSurface(wybrana, player->GetPosX() - camX, player->GetPosY() - camY);

}
void Engine::DrawFairies(SDL_Rect* fairies, SDL_Rect* fairiesBorders, int fairiesCount, SDL_Rect* camera) {

	SDL_Rect resoult = { 0,0,0,0 };

	for (int i = 0; i < fairiesCount; i++) {

		if (SDL_HasIntersection(camera, &fairies[i]) == SDL_TRUE) {

			MoveFairyRandomly(&fairies[i], &fairiesBorders[i]);

			int x = fairies[i].x - camera->x;
			int y = fairies[i].y - camera->y;
			//DrawRectangle(fairiesBorders[i].x - camera->x, fairiesBorders[i].y- camera->y, fairiesBorders[i].w, fairiesBorders[i].h, czerwony, czerwony);
			DrawSurface(fairy, x, y);
		}
	}
}

void Engine::DrawObjects(SDL_Rect* objects, int objectsCount, SDL_Rect* camera) {

	SDL_Rect resoult = {0,0,0,0};

	for (int i = 0; i < objectsCount; i++) {

		if (SDL_IntersectRect(camera, &objects[i], &resoult) == SDL_TRUE) {

			int x = resoult.x - camera->x;
			int y = resoult.y - camera->y;

			DrawRectangle(x, y, resoult.w, resoult.h, czerwony, zielony);
		}
	}
}
void Engine::DrawStars(SDL_Rect* stars, int starsCount, SDL_Rect* camera) {

	for (int i = 0; i < starsCount; i++) {

		if (SDL_HasIntersection(camera, &stars[i]) == SDL_TRUE) {

			int x = stars[i].x - camera->x;
			int y = stars[i].y - camera->y;

			DrawSurface(star, x, y);
		}
	}
}
void Engine::DrawGround(SDL_Rect* objects, int objectsCount, SDL_Rect* camera) {
	SDL_Rect resoult = { 0,0,0,0 };

	for (int i = 0; i < objectsCount; i++) {

		if (SDL_IntersectRect(camera, &objects[i], &resoult) == SDL_TRUE) {

			int x = resoult.x - camera->x;
			int y = resoult.y - camera->y;

			DrawRectangle(x, y, resoult.w, resoult.h, czarny, niebieski);

		}

	}
}

int Engine::CheckCollision(SDL_Rect* objects, int objectsCount, SDL_Rect* colider) {
	for (int i = 0; i < objectsCount; i++) {
		if (SDL_HasIntersection(&objects[i], colider) == SDL_TRUE) {
			return 1;
		}
	}
	return 0;
}
int Engine::ShowCollisionScreen(int lives, int totalPoints) {
	char text[TEXT_BUFFER];
	char text2[TEXT_BUFFER];
	SDL_Event e;

	// tekst informacyjny / info text
	//DrawRectangle(4, 50, mScreenWidth - 8, mScreenHeight-50, czerwony, niebieski);

	sprintf(text, "Niestety umarles!");
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 100, text);

	if (lives <= 0) {
		sprintf(text, "Wykorzystales juz wszystkie zycia");
		sprintf(text2, "Esc - wyjscie, n - powrot do menu, enter - zapisz wynik");

	}
	else {
		sprintf(text, "Jezeli chcesz kontynuowac kliknij n!");
		sprintf(text2, "Esc - wyjscie, n - kontynuuj");

	}
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 120, text);

	DrawString(screen->w / 2 - strlen(text2) * 8 / 2, 140, text2);

	UpdateScreen();
	ResetBonus();

	int q = 0, mreturn = 0;

	while (q == 0) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_n) {
					q = 1;
					mreturn = 0;
				}
				else if (e.key.keysym.sym == SDLK_ESCAPE) {
					q = 1;
					mreturn = 1;
				}else if (e.key.keysym.sym == SDLK_RETURN && lives <= 0) {
					q = 1;
					mreturn = ShowSaveScreen(totalPoints);
				}
				break;
			
			case SDL_QUIT:
				q = 1;
				mreturn = 1;
				break;
			};
			
		};
	}

	return mreturn;
}
void Engine::SavePointsToFile(char* tekst) {
	FILE* plik = NULL;
	//printf(tekst);
	plik = fopen("wyniki.txt", "a");

	if (plik == NULL) {
		printf("Wystapil blad podczas otwieranie pliku wyniki.txt!\n");
		return;
	}
	fprintf_s(plik, "%s\n", tekst);

	fclose(plik);
}

int Engine::ShowSaveScreen(int points) {
	char text[TEXT_BUFFER] = "";
	char text3[TEXT_BUFFER] = "";
	char text2[TEXT_BUFFER] = "";
	SDL_Event e;
	
	// t³o
	DrawRectangle(0, 0, mScreenWidth, mScreenHeight, zloty, zloty);

	// informacje
	sprintf(text3, "Zapisujesz wynik %d punktow", points);
	DrawString(screen->w / 2 - strlen(text3) * 8 / 2, 80, text3);
	sprintf(text, "Wprowadz swoj nick i wcisnij enter");
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 100, text);

	UpdateScreen();

	int q = 0, mreturn = 0, shouldRender = 0;
	SDL_StartTextInput();

	while (q == 0) {
		while (SDL_PollEvent(&e)) {
			
			if (e.type == SDL_KEYDOWN) {

				if (e.key.keysym.sym == SDLK_ESCAPE) {
					q = 1;
					mreturn = 1;
					break;
				}
				else if (e.key.keysym.sym == SDLK_BACKSPACE) {
					if (strlen(text2) > 0) {			// jezeli jest tekst to usuniêcie jednej litery
						text2[strlen(text2) - 1] = '\0';
						shouldRender = 1;
					}
					
					
				}
				else if (e.key.keysym.sym == SDLK_RETURN) {
					if (strlen(text2) > 0) {			// zapisanie wyniku
						q = 1;
						mreturn = 0;
						sprintf(text, "%s %d", text2, points);
						SavePointsToFile(text);
					}


				}
			}
			else if (e.type == SDL_TEXTINPUT) {
				strncat(text2, e.text.text, 1);			// dodanie litery do tekstu
				shouldRender = 1;
			}

			else if(e.type == SDL_QUIT) {
				q = 1;
				mreturn = 1;
				break;
			};

			if (shouldRender) {							// renderowanie
				DrawRectangle(0, 0, mScreenWidth, mScreenHeight, zloty, zloty);
				DrawString(screen->w / 2 - strlen(text3) * 8 / 2, 80, text3);
				DrawString(screen->w / 2 - strlen(text) * 8 / 2, 100, text);
				DrawString(screen->w / 2 - strlen(text2) * 8 / 2, 150, text2);
				UpdateScreen();
				shouldRender = 0;
			}

		};
	}
	SDL_StopTextInput();
	return mreturn;
}

int Engine::ShowMainMenu() {
	char text[TEXT_BUFFER];
	SDL_Event e;

	// tekst informacyjny / info text
	//DrawRectangle(4, 50, mScreenWidth - 8, mScreenHeight-50, czerwony, niebieski);
	DrawRectangle(0, 0, mScreenWidth, mScreenHeight, niebieski, zloty);

	sprintf(text, "Robot Unicorn Attack");
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 100, text);

	sprintf(text, "Bartosz Zylwis, nr albumu: 184477");
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 120, text);

	sprintf(text, "Esc - wyjscie, n - rozpocznij nowa gre, tab - wyniki");
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 140, text);

	UpdateScreen();
	ResetBonus();

	int q = 0;

	while (q == 0) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_n) {
					q = 0;
					return 0;
				} 
				else if (e.key.keysym.sym == SDLK_TAB) {
					q = ShowLeaderBoard();
					return q;
				}
				else if (e.key.keysym.sym == SDLK_ESCAPE) {
					q = 1;
					return 1;
				}
				break;

			case SDL_QUIT:
				q = 1;
				return 1;
				break;
			};

		};
	}
}

void Engine::setGroundPosition(SDL_Rect* ground) {

	ground[0] = { 0, mLevelHeight - 100, 4750, 50 };
	ground[1] = { 1900, mLevelHeight - 400, 700, 20 };
	ground[2] = { 5900, mLevelHeight - 400, 700, 20 };
	ground[3] = { 6900, mLevelHeight - 400, 400, 20 };
	ground[4] = { 7400, mLevelHeight - 650, 700, 20 };
	ground[5] = { 2400, mLevelHeight - 650, 700, 20 };
	ground[6] = { 7900, mLevelHeight - 400, 700, 20 };
	ground[7] = { 1000, mLevelHeight - 400, 500, 20 };
	ground[8] = { 4300, mLevelHeight - 400, 400, 20 };
	ground[9] = { 5300, mLevelHeight - 400, 350, 20 };
	ground[10] = { 2250, mLevelHeight - 150, 500, 50 };
	ground[11] = { 4850, mLevelHeight - 100, 3400, 50 };
	ground[12] = { 8550, mLevelHeight - 100,  mLevelWidth- 8550, 50 };

};
void Engine::setObjectsPosition(SDL_Rect* objects) {

	objects[0] = { 7600, 0, 50, 170 };
	objects[1] = { 4500, 0, 50, 400 };
	objects[2] = { 9000, mLevelHeight - 150, 50, 100 };
	objects[3] = { 7600, mLevelHeight - 150, 20, 50 };
	objects[4] = { 3000, mLevelHeight - 150, 20, 50 };
	objects[5] = { 1200, mLevelHeight - 1050, 20, 50 };
	objects[6] = { 1600, mLevelHeight - 150, 20, 50 };
	objects[7] = { 5600, mLevelHeight - 150, 20, 50 };
	objects[8] = { 5200, mLevelHeight - 150, 20, 50 };
	objects[9] = { 3600, mLevelHeight - 150, 20, 50 };
	objects[10] = { 9200, mLevelHeight - 150, 20, 50 };
	objects[11] = { 4500, mLevelHeight - 150, 20, 50 };
	objects[12] = { 2800, 0, 50, 170 };

};

void Engine::DrawExplosion(int x, int y, int cx, int cy) {
	char text[200];
	int move = explosionSurf[0]->w/4;
	Uint32 format = SDL_PIXELFORMAT_ARGB8888;

	SDL_Surface* screen = SDL_CreateRGBSurfaceWithFormat(0, mScreenWidth, mScreenHeight, 32, format);
	SDL_RenderReadPixels(renderer, NULL, format, screen->pixels, screen->pitch);

	for (int i = 0; i <21; i++) {

		//DrawAndUpdatePart(explosionSurf[i], x, y, cx, cy);
		DrawSurface(screen, 0, 0);

		DrawSurface(explosionSurf[i], x - cx- move, y - cy- move);
		UpdateScreen();
		//SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		//SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		//SDL_RenderPresent(renderer);
		
		SDL_Delay(50);

	}

	SDL_FreeSurface(screen);
}
void Engine::LoadExplosion() {
	char text[200];

	for (int i = 1; i <= 21; i++) {
		explosionSurf[i - 1] = NULL;
		sprintf(text, "./wybuch/%d.bmp", i);

		explosionSurf[i - 1] = SDL_LoadBMP(text);
		if (explosionSurf[i - 1] == NULL) {
			printf("SDL_LoadBMP(/wybuch/%d.bmp) error: %s\n", i, SDL_GetError());
			return;
		};

	}
}
int porownaj(const void * pier, const void * drug) {
	//return *(int*)pier[0] - *(int*)drug[0];

	int *_pier = (int*)pier;
	int *_drug = (int*)drug;

	if (_pier[0] < _drug[0]) 
		return 1;
	if (_pier[0] > _drug[0])
		return -1;
	return 0;
}

int Engine::SortSaveFile() {
	FILE* plik = NULL;
	char nazwy[TEXT_BUFFER][TEXT_BUFFER];
	int wyniki[TEXT_BUFFER][2];
	int temp = 0, l_wynikow = 0;
	char tempStr[TEXT_BUFFER];

	plik = fopen("wyniki.txt", "r");
	if (plik == NULL) {
		printf("Brak wynikow do posortowania lub problem z plikiem!\n");
		return 0;
	}
	// wczytanie danych z pliku
	while (fscanf(plik, "%s %d\n", &tempStr, &temp) == 2) {
		
		sprintf(nazwy[l_wynikow], "%s", tempStr);

		wyniki[l_wynikow][0] = temp;
		wyniki[l_wynikow][1] = l_wynikow;
		l_wynikow += 1;
	}

	fclose(plik);
	qsort(wyniki, l_wynikow, sizeof(wyniki[0]), porownaj);

	// zapisanie posortowanych wynikow
	plik = fopen("wyniki.txt", "w");
	for (int i = 0; i < l_wynikow; i++) {
		
		fprintf(plik, "%s %d\n", nazwy[wyniki[i][1]], wyniki[i][0]);
	}
	fclose(plik);
	return 1;
}
int Engine::ShowLeaderBoard() {
	char text[TEXT_BUFFER];
	char temp[TEXT_BUFFER];
	int teampPoints, pages;
	FILE* plik;
	SDL_Event e;

	int dostepne = SortSaveFile();

	DrawRectangle(0, 0, mScreenWidth, mScreenHeight, niebieski, zloty);

	sprintf(text, "Tabela wynikow");
	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 100, text);


	if (dostepne) {
		sprintf(text, "Esc - wyjscie, n - rozpocznij nowa gre, k - kolejna strona");
	}
	else {
		sprintf(text, "Brak zapisanych wynikow");
		DrawString(screen->w / 2 - strlen(text) * 8 / 2, 170, text);
		sprintf(text, "Esc - wyjscie, n - rozpocznij nowa gre");
	}

	DrawString(screen->w / 2 - strlen(text) * 8 / 2, 140, text);

	UpdateScreen();

	int q = 0, shouldRender = 1, strona = 0, k;
	pages = 2;

	while (q == 0) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_n) {
					q = 0;
					return 0;
				}
				else if (e.key.keysym.sym == SDLK_k) {
					strona += 1;
					if (strona > pages) {
						strona = 0;
					}
					shouldRender = 1;

				}
				else if (e.key.keysym.sym == SDLK_ESCAPE) {
					q = 1;
					return 1;
				}
				break;

			case SDL_QUIT:
				q = 1;
				return 1;
				break;
			};

		};
		if (dostepne && shouldRender) {

			plik = fopen("wyniki.txt", "r");
			DrawRectangle(0, 165, mScreenWidth, mScreenHeight-170, zloty, zloty);
			k = 0; // k - numer obecnie wczytywanego wyniku
			while (fscanf(plik, "%s %d\n", temp, &teampPoints) == 2) {
				// jezeli wynik powinien zostac wyswietlony
				if (k >= strona * L_WYNIKOW_NA_STR && k < strona * L_WYNIKOW_NA_STR + L_WYNIKOW_NA_STR) {
					sprintf(text, "%d. %s %d",k+1, temp, teampPoints);
					DrawString(screen->w / 2 - strlen(text) * 8 / 2, 200 + k% L_WYNIKOW_NA_STR*20, text);
				}
				k += 1;

			}
			UpdateScreen();
			pages = (k + 1) / L_WYNIKOW_NA_STR ;
			shouldRender = 0;
			fclose(plik);
		}

	}
}