#pragma once

#include "Commons.h"
#include "GameScreen.h"
#include "Texture2D.h"

#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

using namespace std;

class Texture2D;

class GameScreenScore : GameScreen
{
public:
	GameScreenScore(SDL_Renderer* renderer);
	~GameScreenScore();

	void Render();
	void Update(float deltaTime, SDL_Event e);

private:
	bool SetUpScreen();
	Texture2D* mBlackBackground;
	Texture2D* characterMario;
	Texture2D* characterLuigi;
	Texture2D* characterPeach;

	// FONTS
	void LoadFont();

	TTF_Font* fontVeryLarge;
	TTF_Font* fontLarge;
	TTF_Font* fontMedium;
	TTF_Font* fontSmall;

	SDL_Color colorFg;
	SDL_Color colorBg;
	SDL_Color colorRed;
	SDL_Color colorGreen;
	SDL_Color colorPink;

	SDL_Surface* titleTextSurface;
	SDL_Texture* titleText;
	SDL_Rect titleTextRect;

	SDL_Surface* marioTextSurface;
	SDL_Texture* marioText;
	SDL_Rect marioTextRect;

	SDL_Surface* marioScoreTextSurface;
	SDL_Texture* marioScoreText;
	SDL_Rect marioScoreTextRect;
	string marioScoreString;

	SDL_Surface* luigiTextSurface;
	SDL_Texture* luigiText;
	SDL_Rect luigiTextRect;

	SDL_Surface* luigiScoreTextSurface;
	SDL_Texture* luigiScoreText;
	SDL_Rect luigiScoreTextRect;
	string luigiScoreString;

	SDL_Surface* peachTextSurface;
	SDL_Texture* peachText;
	SDL_Rect peachTextRect;

	SDL_Surface* winTextSurface;
	SDL_Texture* winText;
	SDL_Rect winTextRect;

	SDL_Surface* exitTextSurface;
	SDL_Texture* exitText;
	SDL_Rect exitTextRect;

	// SOUNDS
	void LoadAudio();

	Mix_Chunk* coinSound;
	Mix_Chunk* victorySound;
};