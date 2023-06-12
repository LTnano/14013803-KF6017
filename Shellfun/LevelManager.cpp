
// 14013803 - KF6017 Software Architecture for Games

#include "LevelManager.h"
#include "myinputs.h"
#include "mydrawengine.h"
#include "Asteroid.h"
#include "Outpost.h"
#include "ArmedStation.h"
#include "MegaStation.h"
#include "CapitalShip.h"
#include "PatrolShip.h"
#include "WormArmour.h"
#include "WormModes.h"

// The LevelManager class handles various aspects of the game's levels
// It loads the level, tracks the player's score, determines level transitions,
// Sets up entities within levels, spawns new enemies, and handles the player UI
LevelManager::LevelManager(ObjectFactory* pObjFac)
{
	// Initialise variables
	savedScore = 0;
	playerScore = 0;
	spawnTimer = 0;
	endGame = false;
	pOF = pObjFac;
	pOM = pOF->GetpOMInstance();
	changeLevelPlease = false;
	active = true;
	TYPE = ObjectType::LEVELMANAGER;
	level = 1;
	// Objectives
	objMegaStations = 0;
	objCapitalShips = 0;
	objArmedStations = 0;
	objOutpost = 0;
	// get screen size for UI
	screenDimensions = Vector2D(MyDrawEngine::GetInstance()->GetScreenWidth(), MyDrawEngine::GetInstance()->GetScreenHeight());
}

void LevelManager::Initialise(Vector2D pos)
{
	// Creating mouse pointer for UI
	pMP = pOF->createObject<MousePointer>();
	// Vector2D(-2500, -2500) is the Worm starting location - Worm is the Player Character
	pWorm = pOF->createObject<WormHead>(pOF, pMP, Vector2D(-2500, -2500));

	// Save the current player score in case of level restart
	savedScore = playerScore;
	objectiveFailed = false;
	objectiveComplete = false;
	wave = 1;

	// Call the level1Setup function if the game is just starting
	if (level == 1)
	{
		level1Setup();
	}
}

// destructor
LevelManager::~LevelManager()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pOF = nullptr;
	pWorm = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// Loads the next level, case 0 only ever relevant on level 1 mission restart
void LevelManager::nextLevel()
{
	switch (level)
	{
	case 0:
		level1Setup();
		level = 1;
		changeLevelPlease = false;
		break;
	case 1:
		level = 2;
		level2Setup();
		changeLevelPlease = false;
		break;
	case 2:
		level = 3;
		level3Setup();
		changeLevelPlease = false;
		break;
	default:
		break;
	}
}

// Spawns the entitites required for level 1
void LevelManager::level1Setup()
{
	if (level == 0)
	{
		// Re-creates Worm and mouse pointer if restarting level
		Initialise(Vector2D(0, 0));
	}
	// Setting level objectives
	objMegaStations = 0;
	objCapitalShips = 0;
	objArmedStations = 0;
	objOutpost = 3;

	// Setting objective area for wave spawns, approximately around the random spawns below
	objPoint = Vector2D(1425, 1425);

	// Reset camera position
	pMP->position = pWorm->position;
	timeAlive = 0;

	// Spawn starting entities
	int spawnManyOP = 3;
	int spawnManyPS = 2;
	int spawnManyAst = 25;
	for (int i = 0; i < spawnManyOP; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(2000 - rand() % 1800, 2000 - rand() % 1800);
		pOF->createObject<Outpost>(randomLocation, pOF, pWorm, false);
	}
	for (int i = 0; i < spawnManyPS; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(2000 - rand() % 1800, 2000 - rand() % 1800);
		pOF->createObject<PatrolShip>(randomLocation, pOF, pWorm, false);
	}
	for (int i = 0; i < spawnManyAst; i++)
	{
		// Randomize asteroids around player spawn
		Vector2D randomLocation = Vector2D(-1000 - rand() % 2500, -1000 - rand() % 2500);
		Asteroid* pAst = pOF->createObject<Asteroid>(randomLocation, Vector2D(20 - rand() % 200, 20 - rand() % 200), pOM);
	}
}
// Spawns the entitites required for level 2
void LevelManager::level2Setup()
{
	// Initialise to reset for each level
	this->Initialise(Vector2D(0, 0));
	// Setting level objectives
	objMegaStations = 0;
	objCapitalShips = 0;
	objArmedStations = 2;
	objOutpost = 5;

	// Setting objective area for wave spawns, approximately around the random spawns below
	objPoint = Vector2D(1700, 1700);

	// Reset camera position
	pMP->position = pWorm->position;
	timeAlive = 0;

	// Spawn starting entities
	int spawnManyAS = 2;
	int spawnManyOP = 5;
	int spawnManyPS = 5;
	int spawnManyAst = 20;
	// Spawn Armed Stations
	for (int i = 0; i < spawnManyAS; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(2500 - rand() % 3500, 2500 - rand() % 3500);
		pOF->createObject<ArmedStation>(randomLocation, pOF, pWorm, false);
	}
	// Spawn Outpost stations
	for (int i = 0; i < spawnManyOP; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(2500 - rand() % 3500, 2500 - rand() % 3500);
		pOF->createObject<Outpost>(randomLocation, pOF, pWorm, false);
	}
	// Spawn Patrol Ships
	for (int i = 0; i < spawnManyPS; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(2500 - rand() % 3500, 2500 - rand() % 3500);
		pOF->createObject<PatrolShip>(randomLocation, pOF, pWorm, false);
	}
	// Asteroid spawn
	for (int i = 0; i < spawnManyAst; i++)
	{
		// Randomize asteroids around player spawn
		Vector2D randomLocation = Vector2D(-1000 - rand() % 3000, -1000 - rand() % 3000);
		pOF->createObject<Asteroid>(randomLocation, Vector2D(20 - rand() % 200, 20 - rand() % 200), pOM);
	}
	// More Asteroids
	for (int i = 0; i < spawnManyAst; i++)
	{
		// Randomize spawns spproximately beyond the enemy base
		Vector2D randomLocation = Vector2D(4000 - rand() % 3500, 4000 - rand() % 3500);
		pOF->createObject<Asteroid>(randomLocation, Vector2D(20 - rand() % 200, 20 - rand() % 200), pOM);
	}
}
// Spawns the entitites required for level 3
void LevelManager::level3Setup()
{
	// Initialise to reset for each level
	this->Initialise(Vector2D(0, 0));
	// Setting level objectives
	objMegaStations = 2;
	objCapitalShips = 1;
	objArmedStations = 3;
	objOutpost = 5;

	// Setting objective area for wave spawns, approximately around the random spawns below
	objPoint = Vector2D(2500, 2500);

	// Reset camera position
	pMP->position = pWorm->position;
	timeAlive = 0;

	// Spawn starting entities
	int spawnManyMS = 2;
	int spawnManyCS = 1;
	int spawnManyAS = 3;
	int spawnManyOP = 5;
	int spawnManyPS = 5;
	int spawnManyAst = 28;
	// Spawn Mega Stations
	for (int i = 0; i < spawnManyMS; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(2500 - rand() % 3500, 2500 - rand() % 3500);
		pOF->createObject<MegaStation>(randomLocation, pOF, pWorm, false);
	}
	// Spawn Capital Station
	for (int i = 0; i < spawnManyCS; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(2500 - rand() % 3500, 2500 - rand() % 3500);
		pOF->createObject<CapitalShip>(randomLocation, pOF, pWorm, false);
	}
	// Spawn Armed Stations
	for (int i = 0; i < spawnManyAS; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(3200 - rand() % 3500, 3200 - rand() % 3500);
		pOF->createObject<ArmedStation>(randomLocation, pOF, pWorm, false);
	}
	// Spawn Outposts
	for (int i = 0; i < spawnManyOP; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(3200 - rand() % 3500, 3200 - rand() % 3500);
		pOF->createObject<Outpost>(randomLocation, pOF, pWorm, false);
	}
	// Spawn Patrol Ships
	for (int i = 0; i < spawnManyPS; i++)
	{
		// Randomize spawns in an area away from the player spawn
		Vector2D randomLocation = Vector2D(3200 - rand() % 3500, 3200 - rand() % 3500);
		pOF->createObject<PatrolShip>(randomLocation, pOF, pWorm, false);
	}
	// Spawn Asteroids
	for (int i = 0; i < spawnManyAst; i++)
	{
		// Randomize spawns around the player
		Vector2D randomLocation = Vector2D(-1000 - rand() % 3500, -1000 - rand() % 3500);
		pOF->createObject<Asteroid>(randomLocation, Vector2D(20 - rand() % 200, 20 - rand() % 200), pOM);
	}
	// Spawn more Asteroids
	for (int i = 0; i < spawnManyAst; i++)
	{
		// Randomize spawns in an area approximately on the other side of the enemy location
		Vector2D randomLocation = Vector2D(5500 - rand() % 3500, 5500 - rand() % 3500);
		pOF->createObject<Asteroid>(randomLocation, Vector2D(20 - rand() % 200, 20 - rand() % 200), pOM);
	}
}

// Spawns a new wave based on the level and entities present
void LevelManager::spawnNewWave()
{
	// Direction is determined by the objective point to aid in orientating the player if they get lost
	float spawnDirection = (objPoint - pWorm->position).angle();
	Vector2D spawnPoint = pWorm->position;
	spawnPoint.setBearing(spawnDirection, 5000);

	// Don't spawn more entities if there are many already present to avoid performance issues
	if (pOM->entityNumber < 160)
	{
		switch (level)
		{
			// Level 1 spawns a Patrol ship and 3 fighters
			case 1:
				pOF->createObject<PatrolShip>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), pOF, pWorm, false);
				for (int i = 0; i < 3; i++)
				{
					pOF->createObject<Fighter>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), 0, false, pOM, pWorm);
				}
				wave += 1;
				break;
			// Level 2 spawns 2 Patrol ship and 6 fighters
			case 2:
				pOF->createObject<PatrolShip>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), pOF, pWorm, false);
				pOF->createObject<PatrolShip>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), pOF, pWorm, false);
				for (int i = 0; i < 3; i++)
				{
					pOF->createObject<Fighter>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), 0, false, pOM, pWorm);
					pOF->createObject<Fighter>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), 0, false, pOM, pWorm);
				}
				wave += 1;
				break;
			// Level 3 spawns a Capital Ship every 4th wave, 2 Patrol ships and 6 fighters regularly
			case 3:
				if (wave >= 3)
				{
					pOF->createObject<CapitalShip>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), pOF, pWorm, false);
					objCapitalShips += 1;
					wave = 0;
				}
				for (int i = 0; i < 2; i++)
				{
					pOF->createObject<PatrolShip>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), pOF, pWorm, false);
					pOF->createObject<Fighter>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), 0, false, pOM, pWorm);
					pOF->createObject<Fighter>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), 0, false, pOM, pWorm);
					pOF->createObject<Fighter>(spawnPoint + Vector2D(175 - rand() % 350, 175 - rand() % 350), 0, false, pOM, pWorm);
				}
				wave += 1;
				break;
		}
	}
}

// Takes messages and counts score and objectives
void LevelManager::HandleMessage(Message msg)
{
	if (msg.type == EventType::OBJECTIVE_COMPLETED)
	{
		switch (msg.otherData)
		{
		// outpost
		case 1:
			objOutpost -= 1;
			playerScore += 50;
			break;
		// armedstation
		case 2: 
			objArmedStations -= 1;
			playerScore += 150;
			break;
		// capitalship
		case 3:
			objCapitalShips -= 1;
			playerScore += 250;
			break;
		// megastation
		case 4:
			objMegaStations -= 1;
			playerScore += 500;
			break;
		// patrolship
		case 5:
			playerScore += 10;
			break;
		// fighter
		case 6:
			playerScore += 1;
			break;
		// missile
		case 7:
			playerScore += 2;
			break;
		// asteroid
		case 8:
			playerScore += 5;
			break;
		default:
			break;
		}

	}

	// Sets the objective to failed if the player character reports their death
	if (msg.type == EventType::WORM_DEAD)
	{
		objectiveFailed = true;
	}
}

// Necessary blank function due to the virtual nature of parent
void LevelManager::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
}

// Get the hitbox of the game object
IShape2D& LevelManager::GetShape()
{
	return spriteBox;
}

// Called each frame
// Handles UI, objectives and wavespawning
// Notified game when to change level
void LevelManager::Update(const double frictCoeff, float frameTime)
{
	// Samples keyboard
	MyInputs::GetInstance()->SampleKeyboard();

	// Runs HP bar function
	CreateHealthArmourUI();

	// Tracks timers
	timeAlive += frameTime;
	spawnTimer += frameTime;

	// Worshipers counter
	MyDrawEngine::GetInstance()->WriteInt((screenDimensions.XValue / 4) + 110, (screenDimensions.YValue / 4) * 3, pWorm->GetNomads(), pDE->WHITE);
	MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 4), (screenDimensions.YValue / 4)* 3 , L"Worshipers:", pDE->WHITE);

	// Ammo counter and Mouse number
	MyDrawEngine::GetInstance()->WriteInt((screenDimensions.XValue / 4) + 155, (screenDimensions.YValue / 4) * 3 + 30, pWorm->totalAmmo * 2, pDE->WHITE);
	MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 4), (screenDimensions.YValue / 4) * 3 + 30, L"Ready to Launch:", pDE->WHITE);
	MyDrawEngine::GetInstance()->WriteInt(pMP->GetPos() - Vector2D(50, 10), pWorm->totalAmmo * 2, pDE->WHITE);

	// UI using Wormmodes
	const wchar_t* modeBoth = L"DEVOTED TO COMBINED ARMS";
	const wchar_t* modeFighters = L"DEVOTED TO HANGAR BAYS";
	const wchar_t* modeMissiles = L"DEVOTED TO MISSILE SILOS";
	switch (pWorm->WormSegment::GetMode())
	{
	case Mode::MISSILES:
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 4), (screenDimensions.YValue / 4) * 3 -30, modeMissiles, pDE->WHITE);
		break;
	case Mode::BOTH:
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 4), (screenDimensions.YValue / 4) * 3 - 30, modeBoth, pDE->WHITE);
		break;
	case Mode::FIGHTERS:
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 4), (screenDimensions.YValue / 4) * 3 - 30, modeFighters, pDE->WHITE);
		break;
	default:
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 4), (screenDimensions.YValue / 4) * 3 - 30, modeBoth, pDE->WHITE);
		break;
	}

	// Once 15 seconds have passed, spawns a wave and reset the timer
	if (spawnTimer > 15)
	{
		spawnNewWave();
		spawnTimer = 0;
	}
	
	// Player has died before completing the objectives
	if (objectiveFailed && !objectiveComplete)
	{
		// Display UI to restart level
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2)-10, screenDimensions.YValue / 8, L"EXTINCT", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2)-80, (screenDimensions.YValue / 8)+30, L"Press Enter to Restart Level...", MyDrawEngine::WHITE);
		
		// Listen for enter to restart level
		if (MyInputs::GetInstance()->NewKeyPressed(DIK_RETURN))
		{
			level -= 1;
			playerScore = savedScore;
			changeLevelPlease = true;
		}
	}
	// Displays score
	if (!objectiveComplete)
	{
		MyDrawEngine::GetInstance()->WriteText(screenDimensions.XValue / 4, screenDimensions.YValue / 4-25, L"SCORE: ", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteInt((screenDimensions.XValue / 4) + 80, screenDimensions.YValue / 4-25, playerScore, MyDrawEngine::WHITE);
	}
	// Completed objective and on final level
	else if (objectiveComplete && level == 3)
	{
		// Display UI to return to main menu
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2) - 65, screenDimensions.YValue / 8, L"FINAL LEVEL DEVOURED", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2) - 80, (screenDimensions.YValue / 8) + 30, L"Press Enter to return to Main Menu...", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2) - 30, screenDimensions.YValue / 8 + 60, L"FINAL SCORE: ", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteInt((screenDimensions.XValue / 2) + 110, screenDimensions.YValue / 8 + 60, playerScore, MyDrawEngine::WHITE);		
		
		// Listen for return to end the game
		if (MyInputs::GetInstance()->NewKeyPressed(DIK_RETURN))
		{
			endGame = true;
		}
	}
	else // Completed objective but not final level
	{
		// Display UI to advance to the next level
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2) - 25, screenDimensions.YValue / 8, L"LEVEL DEVOURED", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2) - 55, (screenDimensions.YValue / 8) + 30, L"Press Enter to Continue...", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteText((screenDimensions.XValue / 2) - 20, screenDimensions.YValue / 8 + 60, L"SCORE: ", MyDrawEngine::WHITE);
		MyDrawEngine::GetInstance()->WriteInt((screenDimensions.XValue / 2) + 70, screenDimensions.YValue / 8 + 60, playerScore, MyDrawEngine::WHITE);
		if (MyInputs::GetInstance()->NewKeyPressed(DIK_RETURN))
		{
			changeLevelPlease = true;
		}
	}

	// Set objective complete once objectives are destroyed
	if ((objOutpost < 1) && (objArmedStations < 1) && (objCapitalShips < 1) && (objMegaStations < 1))
	{
		objectiveComplete = true;
	}
}

// Creates health UI
void LevelManager::CreateHealthArmourUI()
{
	// Worm Healthbar
	// Current HP Length = (Current HP / 500) * 760

	// Calculate the top-right and bottom-left locations of the health bar
	Vector2D TopRightLocation = Vector2D(pWorm->position.XValue + 380, pWorm->position.YValue - 685);
	Vector2D BottomLeftLocation = Vector2D(pWorm->position.XValue - 380, pWorm->position.YValue - 700);

	// Calculate Worm health percentage
	float WormHealthPercent = (pWorm->GetHealth() / 500.0f) * 760.0f;
	if (WormHealthPercent < 0)
	{
		WormHealthPercent = 0;
	}

	// Place the health bar and its background at the calculated positions
	healthBar.PlaceAt(TopRightLocation, BottomLeftLocation);
	healthBarBG.PlaceAt(TopRightLocation, BottomLeftLocation + Vector2D(WormHealthPercent, 0));

	// Fill the health bar and its background with appropriate colors
	if (!(pWorm->GetHealth() >= pWorm->GetMaxHealth()))
	{
		MyDrawEngine::GetInstance()->FillRect(healthBar, MyDrawEngine::DARKRED);
		MyDrawEngine::GetInstance()->FillRect(healthBarBG, MyDrawEngine::GREY);
	}

	// Worm armour bars
	int numArmourBars = 6;
	int totalBarWidth = 720; // Total width available for the armor bars
	int gapWidth = 10; // Gap width between the armor bars

	// Calculate the width of each armour bar and the starting X position
	int barWidth = (totalBarWidth - (numArmourBars - 1) * gapWidth) / numArmourBars;
	int startX = pWorm->position.XValue - totalBarWidth / 2 + barWidth / 2;

	for (int i = 0; i < numArmourBars; i++)
	{
		// Calculate the X position of the current armour bar
		int xPos = startX + (barWidth + gapWidth) * i;

		// Get the health of the current armour bar, or 0 if it doesn't exist
		int armourHealth = 0;
		if (pWorm->wormBodyArmour[i])
		{
			armourHealth = pWorm->wormBodyArmour[i]->GetHealth();
		}
		else
		{
			armourHealth = 0;
		}

		// Calculate the ratio of the current armour bar's health to the maximum health (80 HP)
		float healthRatio = static_cast<float>(armourHealth) / 80.0f;

		// Calculate the scaled width of the current armour bar based on its health ratio
		int scaledBarWidth = static_cast<int>(barWidth * healthRatio);

		// Calculate the top-left and bottom-right corners of the current armour bar
		Vector2D barTopLeft = Vector2D(xPos - scaledBarWidth / 2, pWorm->position.YValue - 655);
		Vector2D barBottomRight = Vector2D(xPos + scaledBarWidth / 2, pWorm->position.YValue - 640);
		ArmourBars[i].PlaceAt(barTopLeft, barBottomRight);

		// Change the color of the current armour bar based on the amount of ammunition
		int ammo = 0;
		if (pWorm->wormBodyArmour[i])
		{
			ammo = pWorm->wormBodyArmour[i]->GetAmmo();
		}
		else
		{
			ammo = 0;
		}
		if (ammo == 0)
		{
			MyDrawEngine::GetInstance()->FillRect(ArmourBars[i], MyDrawEngine::GREY);
		}
		else if (ammo == 1)
		{
			MyDrawEngine::GetInstance()->FillRect(ArmourBars[i], MyDrawEngine::YELLOW);
		}
		else
		{
			MyDrawEngine::GetInstance()->FillRect(ArmourBars[i], MyDrawEngine::GREEN);
		}
	}
}

