#include "Player.h"
#include<stdio.h>
const int RUN = 1;
const int JUMP = 2;
const int DASH = 3;
const int FALL = 4;
const int STARDESTROYER = 5;
const int L_SKOKOW = 2;
const int L_ZRYWOW = 2;
const int PODSTAWOWE_PRZYSPIESZENIE = 10;
const double PRZYSPIESZENIE = 0.2; // zwiekszanie predkosci
const double MAX_CZAS_AKCJI = 0.3; // ograniczenie wysokosci skoku i dlugosci dasha 
const double MNOZNIK_PUNKTOW = 0.04; 
const int MAC_ZRYWU = 30;
const int MOC_OPADANIA = -15;
const int POCZ_POZYCJA_Y = 800;
const int L_ZYC = 3;
const int PRZESUNIECIE_GRACZA = 50; // odleg³oœæ jednoro¿ca od lewej krawêdzi
const int PKT_GWIAZDA = 100;
const int PKT_WROZKA = 10;
const int L_GWIAZD = 4;
const int L_WROZEK = 4;

Player::Player(int mWidth, int sWidth, int sHeight) {
	posX = PRZESUNIECIE_GRACZA;
	posY = POCZ_POZYCJA_Y;

	colider.x = posX;
	colider.y = posY;
	colider.w = PLAYER_WIDTH;
	colider.h = PLAYER_HEIGHT;

	mapWidth = mWidth;
	mapHeight = sHeight;
	screenWidth = sWidth;
	starDestr = 0;

	action = RUN;
	tryb_ster = 0;
	action_dur = 0;
	dos_skoki = 2;
	dos_zryw = 1;
	zycia = L_ZYC;
	punkty = 0;
	starStreak = 0;
	totalPoints = 0;
	fairyStreak = 0;
	lastStarNr = -1;
	lastFairyNr = -1;

	VEL = PODSTAWOWE_PRZYSPIESZENIE;
	velX = 0;
	velY = 0;
	LoadImage();
}
void Player::Faster(int fps) {

	if (tryb_ster == 1) {
		velX += PRZYSPIESZENIE/fps;
		VEL += PRZYSPIESZENIE/fps;

	}
}

void Player::RestartPosition() {
	punkty = 0;
	posX = PRZESUNIECIE_GRACZA;
	posY = POCZ_POZYCJA_Y;
	VEL = PODSTAWOWE_PRZYSPIESZENIE;

	if (tryb_ster == 0) {
		velX = 0;
		velY = 0;
	}
	else {
		velX = VEL;
		velY = VEL-1;
	}

	colider.x = posX;
	colider.y = posY;

	starStreak = 0;
	fairyStreak = 0;
	lastStarNr = -1;
	lastFairyNr = -1;
	
}
int Player::GetPosX() {
	return posX;
}

int Player::GetPosY() {
	return posY;
}
int Player::GetLives() {
	return zycia;
}
void Player::subtLives() {
	totalPoints += punkty;
	zycia -= 1;
}
int Player::GetPoints() {
	return (int)punkty;
}
int Player::AddPoints(int object, int number) {
	int dodatek = 0;
	// je¿eli wró¿ka
	if (object == 1) {
		if (number == 1 && fairyStreak == L_WROZEK) {
			fairyStreak += 1;
		}
		else if (lastFairyNr == number - 1) {
			fairyStreak += 1;
		}
		else {
			fairyStreak = 1;
		}
		lastFairyNr = number;
		dodatek += PKT_WROZKA * fairyStreak;
	}
	// jezeli gwiazda
	else if (object == 2) {
		if (number == 1 && lastStarNr == L_GWIAZD) {
			starStreak += 1;
		}
		else if (lastStarNr == number - 1) {
			starStreak += 1;
		}
		else {
			starStreak = 1;
		}
		lastStarNr = number;
		dodatek += PKT_GWIAZDA*starStreak;
	}

	punkty += dodatek;
	return dodatek;
}
// Aktualizacja czasu trwania obecnej akcji
// i jej zmiana w razie potrzeby
void Player::HandleActionProps(double time, int tryb_ster) {
	if (tryb_ster == 1) {
		action_dur += time;

		if (action_dur > MAX_CZAS_AKCJI) {
			action = FALL;
			velX = VEL;
			velY = VEL - 1;
			action_dur = 0;
		}
	}
	
}
int Player::isDashing() {
	if (action == DASH)
		return 1;
	else
		return 0;
}
void Player::HandleStearingChange(int tryb_st) {

	tryb_ster = tryb_st;

	if (tryb_ster == 0) {
		velX = 0;
		velY = 0;
	}
	if (tryb_ster == 1) {
		velX = VEL;
		velY = VEL-1;
	}

	printf("zmieniono sterowanie: %d\n", tryb_ster);
};
void Player::HandleMovement(SDL_Event& event, int tryb_ster) {

	//Zwykle sterowanie
	if (tryb_ster == 0) {
		// je¿eli zosta³o coœ naciœniête i nie by³o wczeœniej nacisniête 
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {

			switch (event.key.keysym.sym) {
			case SDLK_UP:
				velY -= VEL;
				break;
			case SDLK_DOWN:
				velY += VEL;
				break;
			case SDLK_LEFT:
				velX -= VEL;
				break;
			case SDLK_RIGHT:
				velX += VEL;
				break;

			}
		} // je¿eli przycisk zosta³ "puszczony"
		else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				velY += VEL;
				break;
			case SDLK_DOWN:
				velY -= VEL;
				break;
			case SDLK_LEFT:
				velX += VEL;
				break;
			case SDLK_RIGHT:
				velX -= VEL;
				break;

			}
		}
	} // drugi rodzja sterowania
	else {
		
		// je¿eli zosta³o coœ naciœniête i nie by³o wczeœniej nacisniête 
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			switch (event.key.keysym.sym) {
			case SDLK_x:
				if (dos_zryw >0) {
					action_dur = 0;
					velY = 0;
					velX = MAC_ZRYWU + VEL;
					action = DASH;
					dos_zryw--;
					if (dos_skoki < L_SKOKOW) {
						dos_skoki++;
					}
				}
				break;
			case SDLK_z:
				if (dos_skoki > 0 && action != DASH) {
					action_dur = 0;
					velY = MOC_OPADANIA;
					action = JUMP;
					dos_skoki--;
				}
				break;
			}
		}
		// je¿eli przycisk zosta³ "puszczony"
		else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
			switch (event.key.keysym.sym) {
			case SDLK_x:
				velY = VEL - 1;
				velX = VEL;
				
				action_dur = 0;
				break;
			case SDLK_z:
				if (action != DASH) {
					velY = VEL - 1;
					action_dur = 0;
				}
					
				break;

			}

			action = FALL;
		}
	}
	

}
void Player::StarColision() {
	action = STARDESTROYER;
}

void Player::Move(SDL_Rect* ground, int groundCount, double delta, int tryb_ster) {
	int moveX = velX * (50 * delta);
	int moveY = velY * (50 * delta);

	if (action == DASH) {
		moveY = 0;
	}
	if (moveY > 100) {
		moveY = 0;
	}
	if (moveX > 100) {
		moveX = 0;
	}
	posX += moveX;
	punkty += moveX * MNOZNIK_PUNKTOW;
	posY += moveY;

	//Sprawdzenie granic ekranu
	if (posX >= mapWidth - screenWidth) {
		posX = PRZESUNIECIE_GRACZA;
		
		// reset streaku zebranych obiektow
		if (lastStarNr != L_GWIAZD)
			lastStarNr = -1;
		if (lastFairyNr != L_WROZEK)
			lastFairyNr = -1;
	};
	if (posY < 0) {
		posY = 0;
	}
	if (posY > mapHeight - PLAYER_HEIGHT) {
		
		posY = mapHeight - PLAYER_HEIGHT;
	}
	if (posX < 0) {
		posX = 0;
	}

	colider.x = posX;
	colider.y = posY;

	SDL_Rect intersection = { 0,0,0,0 };
	//Sprawdzenie pod³o¿a
	for (int i = 0; i < groundCount; i++) {
		// jezeli jednorozec udezy w podloze
		if (SDL_IntersectRect(&colider, &ground[i], &intersection)) {
			if (intersection.h <= moveY && moveY > 0 ) {
				action = RUN;
				posY -= intersection.h ;
				dos_skoki = L_SKOKOW;
				dos_zryw = L_ZRYWOW;

			}
		}
	}

	colider.x = posX;
	colider.y = posY;
}
  
void Player::LoadImage() {
	char text[200];

	for (int i = 1; i <= 4; i++) {
		playerSurf = NULL;
		sprintf(text, "./jednorozec%d.bmp",  i);

		playerSurf = SDL_LoadBMP(text);
		if (playerSurf == NULL) {
			printf("SDL_LoadBMP(jednorozec%d.bmp) error: %s\n",i, SDL_GetError());
			return;
		};
		SDL_SetColorKey(playerSurf, true, 0xFFFFFF);
		playerSurfRun[i-1] = playerSurf;

	}
	playerSurf = NULL;

	playerSurf = SDL_LoadBMP("./jednorozec.bmp");
	if (playerSurf == NULL) {
		printf("SDL_LoadBMP(jednorozec.bmp) error: %s\n", SDL_GetError());
		return;
	};
	SDL_SetColorKey(playerSurf, true, 0xFFFFFF);

	playerSurfJump = NULL;

	playerSurfJump = SDL_LoadBMP("./jump.bmp");
	if (playerSurfJump == NULL) {
		printf("SDL_LoadBMP(jump.bmp) error: %s\n", SDL_GetError());
		return;
	};
	
	playerSurfFall = NULL;

	playerSurfFall = SDL_LoadBMP("./fall.bmp");
	if (playerSurfFall == NULL) {
		printf("SDL_LoadBMP(fall.bmp) error: %s\n", SDL_GetError());
		return;
	};

	playerSurfDash = NULL;

	playerSurfDash = SDL_LoadBMP("./dash.bmp");
	if (playerSurfDash == NULL) {
		printf("SDL_LoadBMP(dash.bmp) error: %s\n", SDL_GetError());
		return;
	};
	playerSurfStarDestroyer = NULL;

	playerSurfStarDestroyer = SDL_LoadBMP("./stardestroyer.bmp");
	if (playerSurfStarDestroyer == NULL) {
		printf("SDL_LoadBMP(stardestroyer.bmp) error: %s\n", SDL_GetError());
		return;
	};
}

Player::~Player() {
	SDL_FreeSurface(playerSurf);
	SDL_FreeSurface(playerSurfDash);
	SDL_FreeSurface(playerSurfStarDestroyer);
	SDL_FreeSurface(playerSurfFall);
	SDL_FreeSurface(playerSurfJump);

	for (int i = 0; i < 4; i++) {
		SDL_FreeSurface(playerSurfRun[i]);
	}
}