#include "GameScreenLevel1.h"
#include "Collisions.h"
#include "Texture2D.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "PowBlock.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	bool circleCollision = false;
	bool boxCollision = false;
	bool powCollision = false;

	enemyTimer = ENEMY_TIMER;
	
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;

	delete mLevelTexture;
	mLevelTexture = NULL;

	delete characterMario;
	characterMario = NULL;

	delete characterLuigi;
	characterLuigi = NULL;

	mLevelMap = NULL;

	delete mPowBlock;
	mPowBlock = NULL;

	mEnemyKoopa.clear();
	mEnemyGoomba.clear();
	mCoins.clear();
}

void GameScreenLevel1::Render()
{	
	// Draw the background.
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);

	// Draw the level map.
	mLevelTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	// Render the player.
	characterMario->Render();
	characterLuigi->Render();

	// Render the POW block.
	mPowBlock->Render();

	// Render the enemies.
	for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
	{
		mEnemyKoopa[i]->Render();
	}
	for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
	{
		mEnemyGoomba[i]->Render();
	}

	// Render the coins.
	for (unsigned int i = 0; i < mCoins.size(); i++)
	{
		mCoins[i]->Render();
	}
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	// Do the screen shake if required
	if (mScreenshake)
	{
		mScreenshakeTime -= deltaTime;
		mWobble++;
		mBackgroundYPos = sin(mWobble);
		mBackgroundYPos *= 3.0f;

		// End the shake after the duration.
		if (mScreenshakeTime <= 0.0f)
		{
			mScreenshake = false;
			mBackgroundYPos = 0.0f;
		}
	}
	
	// Check for collisions.
	if (Collisions::Instance()->Circle(characterMario, characterLuigi))
	{
		if (circleCollision == false)
		{
			cout << "Character Circle Collision" << endl;
		}
		circleCollision = true;
	}
	
	if (Collisions::Instance()->Box(characterMario->GetCollisionBox(), characterLuigi->GetCollisionBox()))
	{
		if (boxCollision == false)
		{
			cout << "Character Box Collision" << endl;
		}
		boxCollision = true;
	}
	
	// Update the players.
	characterMario->Update(deltaTime, e);
	WallWrapping(characterMario);
	characterLuigi->Update(deltaTime, e);
	WallWrapping(characterLuigi);

	// Update POW block for each player.
	UpdatePOWBlock(characterMario);
	UpdatePOWBlock(characterLuigi);

	// Update the enemy characters.
	enemyTimer -= 1.0f;
	if (enemyTimer == 15000.0f)
	{
		CreateKoopa(Vector2D(64, 32), FACING_RIGHT, KOOPA_SPEED);
	}
	if (enemyTimer == 10000.0f)
	{
		CreateGoomba(Vector2D(414, 32), FACING_LEFT);
	}
	if (enemyTimer == 5000.0f)
	{
		CreateGoomba(Vector2D(64, 32), FACING_RIGHT);
	}
	if (enemyTimer == 0.0f)
	{
		enemyTimer = ENEMY_TIMER;
		CreateKoopa(Vector2D(414, 32), FACING_LEFT, KOOPA_SPEED);
	}
	UpdateKoopas(deltaTime, e);
	UpdateGoombas(deltaTime, e);

	// Update the coin characters.
	UpdateCoin(deltaTime, e);
}

bool GameScreenLevel1::SetUpLevel()
{
	SetLevelMap();
	
	// Load the background texture.
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Images/background.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}

	// Load the level map texture.
	mLevelTexture = new Texture2D(mRenderer);
	if (!mLevelTexture->LoadFromFile("Images/BackgroundMB.png"))
	{
		cout << "Failed to load level map texture!";
		return false;
	}

	// Set up the player character.
	characterMario = new CharacterMario(mRenderer, "Images/Mario.png", Vector2D(64, 330), mLevelMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Images/Luigi.png", Vector2D(414, 330), mLevelMap);

	// Set up the POW block.
	mPowBlock = new PowBlock(mRenderer, mLevelMap);
	mScreenshake = false;
	mBackgroundYPos = 0.0f;

	// Set up the enemies.
	CreateKoopa(Vector2D(64, 32), FACING_RIGHT, KOOPA_SPEED);
	CreateGoomba(Vector2D(414, 32), FACING_LEFT);

	// Set up the coin characters.
	// Bottom-left platform
	CreateCoin(Vector2D(25, 266));
	CreateCoin(Vector2D(89, 266));
	CreateCoin(Vector2D(153, 266));

	// Bottom-right platform
	CreateCoin(Vector2D(345, 266));
	CreateCoin(Vector2D(409, 266));
	CreateCoin(Vector2D(471, 266));

	// Middle-left platform
	CreateCoin(Vector2D(25, 170));

	// Middle platform
	CreateCoin(Vector2D(153, 138));
	CreateCoin(Vector2D(217, 138));
	CreateCoin(Vector2D(281, 138));
	CreateCoin(Vector2D(345, 138));

	// Middle-right platform
	CreateCoin(Vector2D(471, 170));

	return true;
}

void GameScreenLevel1::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH] = {	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
										{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
										{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
										{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
										{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
										{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
										{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
										{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
										{0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
										{1, 1, 1, 1, 1, 1 ,0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
										{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
										{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
										{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

	// Clear up any old map.
	if (mLevelMap != NULL)
	{
		delete mLevelMap;
	}

	// Set the new one.
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel1::UpdatePOWBlock(Character* character)
{
	if (Collisions::Instance()->Box(character->GetCollisionBox(), mPowBlock->GetCollisionBox()))
	{
		if (mPowBlock->IsAvailable())
		{
			// Collided whilst jumping.
			if (character->IsJumping())
			{
				DoScreenShake();
				mPowBlock->TakeAHit();
				character->CancelJump();
				cout << "POW Block Collision" << endl;
			}
		}
	}
}

void GameScreenLevel1::DoScreenShake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;
	
	for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
	{
		mEnemyKoopa[i]->TakeDamage();
	}

	for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
	{
		mEnemyGoomba[i]->TakeDamage();
	}
}

void GameScreenLevel1::WallWrapping(Character* character)
{
	// Right wall collision - wrap left
	if (character->GetPosition().x >= SCREEN_WIDTH - character->GetCollisionBox().width)
	{
		character->SetPosition(Vector2D(0, character->GetPosition().y));
	}

	// Left wall collision - wrap right
	if (character->GetPosition().x < 0)
	{
		character->SetPosition(Vector2D(SCREEN_WIDTH - character->GetCollisionBox().width, character->GetPosition().y));
	}
}

void GameScreenLevel1::UpdateKoopas(float deltaTime, SDL_Event e)
{
	// Update the Enemies.
	if (!mEnemyKoopa.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
		{
			// Check if enemy is on the bottom row of tiles.
			if (mEnemyKoopa[i]->GetPosition().y > 300.0f)
			{
				// Is the enemy off screen to the left / right?
				if (mEnemyKoopa[i]->GetPosition().x < (float)(-mEnemyKoopa[i]->GetCollisionBox().width * 0.5f) || mEnemyKoopa[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemyKoopa[i]->GetCollisionBox().width * 0.5f))
				{
					mEnemyKoopa[i]->SetAlive(false);;
				}
			}

			// Now do the update.
			mEnemyKoopa[i]->Update(deltaTime, e);

			// Check to see if the enemy collides with the player.
			if (mEnemyKoopa[i]->GetPosition().y > 300.0f || mEnemyKoopa[i]->GetPosition().y <= 64.0f && (mEnemyKoopa[i]->GetPosition().x < 64.0f || mEnemyKoopa[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				// Ignore the collisions if the enemy is behind a pipe?
			}
			else
			{
				if (Collisions::Instance()->Circle(mEnemyKoopa[i], characterMario))
				{
					if (mEnemyKoopa[i]->GetInjured())
					{
						mEnemyKoopa[i]->SetAlive(false);
					}
					else
					{
						// Kill Mario.
						characterMario->SetAlive(false);
						cout << "Mario is dead." << endl;
					}
				}

				if (Collisions::Instance()->Circle(mEnemyKoopa[i], characterLuigi))
				{
					if (mEnemyKoopa[i]->GetInjured())
					{
						mEnemyKoopa[i]->SetAlive(false);
					}
					else
					{
						// Kill Luigi.
						characterLuigi->SetAlive(false);
						cout << "Luigi is dead." << endl;
					}
				}
			}

			// If the enemy is no longer alive, then schedule it for deletion.
			if (!mEnemyKoopa[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		// Remove dead enemies - 1 each update.
		if (enemyIndexToDelete != -1)
		{
			mEnemyKoopa.erase(mEnemyKoopa.begin() + enemyIndexToDelete);
		}
	}
}

void GameScreenLevel1::UpdateGoombas(float deltaTime, SDL_Event e)
{
	// Update the Enemies.
	if (!mEnemyGoomba.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
		{
			// Check if enemy is on the bottom row of tiles.
			if (mEnemyGoomba[i]->GetPosition().y > 300.0f)
			{
				// Is the enemy off screen to the left / right?
				if (mEnemyGoomba[i]->GetPosition().x < (float)(-mEnemyGoomba[i]->GetCollisionBox().width) || mEnemyGoomba[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemyGoomba[i]->GetCollisionBox().width))
				{
					mEnemyGoomba[i]->SetAlive(false);;
				}
			}

			// Now do the update.
			mEnemyGoomba[i]->Update(deltaTime, e);

			// Check to see if the enemy collides with the player.
			if (mEnemyGoomba[i]->GetPosition().y > 300.0f || mEnemyGoomba[i]->GetPosition().y <= 64.0f && (mEnemyGoomba[i]->GetPosition().x < 64.0f || mEnemyGoomba[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				// Ignore the collisions if the enemy is behind a pipe?
			}
			else
			{
				if (Collisions::Instance()->Circle(mEnemyGoomba[i], characterMario))
				{
					if (mEnemyGoomba[i]->GetInjured())
					{
						mEnemyGoomba[i]->SetAlive(false);
					}
					else
					{
						// Kill Mario.
						characterMario->SetAlive(false);
						cout << "Mario is dead." << endl;
					}
				}

				if (Collisions::Instance()->Circle(mEnemyGoomba[i], characterLuigi))
				{
					if (mEnemyGoomba[i]->GetInjured())
					{
						mEnemyGoomba[i]->SetAlive(false);
					}
					else
					{
						// Kill Luigi.
						characterLuigi->SetAlive(false);
						cout << "Luigi is dead." << endl;
					}
				}
			}

			// If the enemy is no longer alive, then schedule it for deletion.
			if (!mEnemyGoomba[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		// Remove dead enemies - 1 each update.
		if (enemyIndexToDelete != -1)
		{
			mEnemyGoomba.erase(mEnemyGoomba.begin() + enemyIndexToDelete);
		}
	}
}

void GameScreenLevel1::UpdateCoin(float deltaTime, SDL_Event e)
{
	// Update the coins.
	if (!mCoins.empty())
	{
		int coinIndexToDelete = -1;
		for (unsigned int i = 0; i < mCoins.size(); i++)
		{
			// Now do the update.
			mCoins[i]->Update(deltaTime, e);

			// Check to see if the enemy collides with the player.
			if (Collisions::Instance()->Box(mCoins[i]->GetCollisionBox(), characterMario->GetCollisionBox()))
			{
				if (mCoins[i]->GetCollected())
				{
					mCoins[i]->SetAlive(false);
					cout << "Mario collected a coin." << endl;
				}
			}

			if (Collisions::Instance()->Box(mCoins[i]->GetCollisionBox(), characterLuigi->GetCollisionBox()))
			{
				if (mCoins[i]->GetCollected())
				{
					mCoins[i]->SetAlive(false);
					cout << "Luigi collected a coin." << endl;
				}
			}

			// If the coin is has been collected, then schedule it for deletion.
			if (!mCoins[i]->GetAlive())
			{
				coinIndexToDelete = i;
			}
		}

		// Remove dead enemies - 1 each update.
		if (coinIndexToDelete != -1)
		{
			mCoins.erase(mCoins.begin() + coinIndexToDelete);
		}
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	CharacterKoopa* characterKoopa = new CharacterKoopa(mRenderer, "Images/Koopa.png", mLevelMap, position, direction, speed);
	mEnemyKoopa.push_back(characterKoopa);
}

void GameScreenLevel1::CreateGoomba(Vector2D position, FACING direction)
{
	CharacterGoomba* characterGoomba = new CharacterGoomba(mRenderer, "Images/GoombaNPC.png", mLevelMap, position, direction);
	mEnemyGoomba.push_back(characterGoomba);
}

void GameScreenLevel1::CreateCoin(Vector2D position)
{
	CharacterCoin* characterCoin = new CharacterCoin(mRenderer, "Images/Coin.png", mLevelMap, position);
	mCoins.push_back(characterCoin);
}