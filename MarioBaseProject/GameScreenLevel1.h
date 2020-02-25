#pragma once

#include "Commons.h"
#include "GameScreen.h"
#include "LevelMap.h"
#include "CharacterKoopa.h"
#include "CharacterCoin.h"
#include "CharacterGoomba.h"

#include <vector>

class Texture2D;
class Character;
class PowBlock;

class GameScreenLevel1 : GameScreen
{
public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	// Character pointers
	Character* characterMario;
	Character* characterLuigi;

	void UpdatePOWBlock(Character* character);

private:
	// Set up level
	bool SetUpLevel();
	Texture2D* mBackgroundTexture;
	Texture2D* mLevelTexture;

	// Bools to check for collisions
	bool circleCollision;
	bool boxCollision;

	// Set up the Level Map
	void SetLevelMap();
	LevelMap* mLevelMap;

	// POW Block pointer
	PowBlock* mPowBlock;

	// Set up Screenshake
	bool mScreenshake;
	float mScreenshakeTime;
	float mWobble;
	float mBackgroundYPos;
	
	void DoScreenShake();

	// Characters
	void WallWrapping(Character* character);

	// Enemy Characters
	void UpdateKoopas(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	vector<CharacterKoopa*> mEnemyKoopa;

	void UpdateGoombas(float deltaTime, SDL_Event e);
	void CreateGoomba(Vector2D position, FACING direction);
	vector<CharacterGoomba*> mEnemyGoomba;

	float enemyTimer;

	// Coin Characters
	void UpdateCoin(float deltaTime, SDL_Event e);
	void CreateCoin(Vector2D position);

	vector<CharacterCoin*> mCoins;
};