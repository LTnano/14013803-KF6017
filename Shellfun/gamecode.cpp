// GameCode.cpp		
// 14013803 - KF6017 Software Architecture for Games
// Modified Start of Game, update and End of game
// creating the assets, running the logic of the game and cleaning up at the end

#include "gamecode.h"
#include "mydrawengine.h"
#include "mysoundengine.h"
#include "myinputs.h"
#include <time.h>
#include "gametimer.h"
#include "errorlogger.h"
#include <math.h>
#include "shapes.h"
#include "MousePointer.h"




Game::Game()
{
   // No-op
}

Game::~Game()
{
	// sets pointers to nullptr (not sure if required)
	pOM = nullptr;
	pOF = nullptr;
	pInputs = nullptr;
}

Game Game::instance;    // Singleton instance

// This is repeated, called every frame.
// It will run either Update( ), MainMenu() or PauseMenu() depending on the
// game state
ErrorType Game::Main()

{
	//Flip and clear the back buffer
	MyDrawEngine* pTheDrawEngine = MyDrawEngine::GetInstance();
	pTheDrawEngine->Flip();
	pTheDrawEngine->ClearBackBuffer();

	ErrorType err=SUCCESS;

	switch(m_currentState)
	{
	case MENU:

		err= MainMenu();     // Menu at start of game
		break;
	case PAUSED:
		err = PauseMenu();   // Player has paused the game
		break;
	case RUNNING:           // Playing the actual game
		err= Update();
		break;
   case GAMEOVER:
		err = FAILURE;       // Error return causes the window loop to exit
	default:
		// Not a valid state
		err = FAILURE;       // Error return causes the window loop to exit
	}

	return err;
}

// Use to change the state of the game. Private function
void Game::ChangeState(GameState newState)
{
	// Very crude state system
	// Close old state
	switch(m_currentState)
	{
	case MENU:
      // Not needed
		break;
	case PAUSED:
      // Not needed
		break;
	case RUNNING:
      // Not needed
		break;
	}

	// Change the state
	m_currentState = newState;
	m_menuOption = 0;

	// Transition to new state
	switch(m_currentState)
	{
	case MENU:
      // Not needed
		break;
	case PAUSED:
      // Not needed
		break;
	case RUNNING:
      // Not needed
		break;
	}
}

// Starts the game engines - Draw Engine, Sound Engine, Input Engine - singletons
// This is called soon after the program runs
ErrorType Game::Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance)
{
	// Create the engines - this should be done before creating other DDraw objects
	if(FAILED(MyDrawEngine::Start(hwnd, bFullScreen)))
	{
		ErrorLogger::Writeln(L"Failed to start MyDrawEngine");
		return FAILURE;
	}
	if(FAILED(MySoundEngine::Start(hwnd)))
	{
		ErrorLogger::Writeln(L"Failed to start MySoundEngine");
		return FAILURE;
	}
	if(FAILED(MyInputs::Start(hinstance, hwnd)))
	{
		ErrorLogger::Writeln(L"Failed to start MyInputs");
		return FAILURE;
	}
	
	return (SUCCESS);
}



// Terminates the game engines - Draw Engine, Sound Engine, Input Engine
// This is called just before the program exits
void Game::Shutdown()

{
   // Any clean up code here 





	// (engines must be terminated last)
	MyDrawEngine::Terminate();
	MySoundEngine::Terminate();
	MyInputs::Terminate();
}




// **********************************************************************************************
// Placeholder menus  ***************************************************************************
// **********************************************************************************************

// Called each frame when in the pause state. Manages the pause menu
// which is currently a basic placeholder
ErrorType Game::PauseMenu()
{
	// Code for a basic pause menu

	MyDrawEngine::GetInstance()->WriteText(450,220, L"Paused", MyDrawEngine::WHITE);

	const int NUMOPTIONS = 2;
	wchar_t options[NUMOPTIONS][11] = {L"Resume", L"Main menu"};

   // Display menu options
	for(int i=0;i<NUMOPTIONS;i++)
	{
		int colour = MyDrawEngine::GREY;       // If not selected, should be grey
		if(i == m_menuOption)
		{
			colour = MyDrawEngine::WHITE;       // Current selection is white
		}
		MyDrawEngine::GetInstance()->WriteText(450,300+50*i, options[i], colour);
	}

	MyInputs* pInputs = MyInputs::GetInstance();

   // Get user input
	pInputs->SampleKeyboard();

   // Move choice up and down
	if(pInputs->NewKeyPressed(DIK_UP))
	{
		m_menuOption--;
	}
	if(pInputs->NewKeyPressed(DIK_DOWN))
	{
		m_menuOption++;
	}
	if(m_menuOption<0)
	{
		m_menuOption=0;
	}
	else if(m_menuOption>=NUMOPTIONS)
	{
		m_menuOption=NUMOPTIONS-1;
	}

   // If player chooses an option ....
	if(pInputs->NewKeyPressed(DIK_RETURN))
	{
		if(m_menuOption ==0)      // Resume
		{
			ChangeState(RUNNING); // Go back to running the game
		}
		if(m_menuOption ==1)      // Quit
		{
			EndOfGame();           // Clear up the game
			ChangeState(MENU);     // Go back to the menu
		}

	}

	return SUCCESS;
}

// Called each frame when in the menu state. Manages the menu
// which is currently a basic placeholder
ErrorType Game::MainMenu()
{
	MyDrawEngine::GetInstance()->WriteText(450,220, L"Main menu", MyDrawEngine::WHITE);

	const int NUMOPTIONS = 2;
	wchar_t options[NUMOPTIONS][15] = {L"Start game", L"Exit"};

   // Display the options
	for(int i=0;i<NUMOPTIONS;i++)
	{
		int colour = MyDrawEngine::GREY;
		if(i == m_menuOption)
		{
			colour = MyDrawEngine::WHITE;
		}
		MyDrawEngine::GetInstance()->WriteText(450,300+50*i, options[i], colour);
	}

   // Get keyboard input
	MyInputs* pInputs = MyInputs::GetInstance();

	pInputs->SampleKeyboard();
	if(pInputs->NewKeyPressed(DIK_UP))
	{
		m_menuOption--;
	}
	if(pInputs->NewKeyPressed(DIK_DOWN))
	{
		m_menuOption++;
	}
	if(m_menuOption<0)
	{
		m_menuOption=0;
	}
	else if(m_menuOption>=NUMOPTIONS)
	{
		m_menuOption=NUMOPTIONS-1;
	}

   // User selects an option
	if(pInputs->NewKeyPressed(DIK_RETURN))
	{
		if(m_menuOption ==0)          // Play
		{  
			StartOfGame();             // Initialise the game
			ChangeState(RUNNING);      // Run it
		}

		if(m_menuOption ==1)          //Quit
		{
			ChangeState(GAMEOVER);
		}
	}

	return SUCCESS;
}


// **********************************************************************************************
// The game !!! *********************************************************************************
// **********************************************************************************************


// Called at the start of the game - when changing state from MENU to RUNNING
// Use this to initialise the core game
ErrorType Game::StartOfGame()
{
	// set random seed as time
	srand(time(nullptr));

	// create object manager
	pOM = new ObjectManager(MyDrawEngine::GetInstance());

	// create object factory
	pOF = new ObjectFactory(pOM);

	// create level manager
	pLM = pOF->createObject<LevelManager>(pOF);

	// set frametime
	frameTime = gt.mdFrameTime;

	// assign pointer
	pInputs = MyInputs::GetInstance();

	// check gametimer frametime
	gt.mark();
	gt.mark();


	return SUCCESS;
}


// Called each frame when in the RUNNING state.
// Checks for user pressing escape (which puts the game in the PAUSED state)
// Flips and clears the back buffer
// Gameplay programmer will develop this to create an actual game
ErrorType Game::Update()
{
	// mark to record frametime
	gt.mark();

	// Check for entry to pause menu
	static bool escapepressed = true;
	if(KEYPRESSED(VK_ESCAPE))
	{
		if(!escapepressed)
			ChangeState(PAUSED);
		escapepressed=true;

		
	}
	else
		escapepressed=false;
	
	// set frametime each frame
	frameTime = gt.mdFrameTime;

	// object manager updates all objects and checks collision
	pOM->UpdateAll(frictCoeff, frameTime);

	// object manager renders all elligible objects
	pOM->RenderAll();

	// object manager cleans up inactive objects
	pOM->DeleteInactive();

	// check to see if the level manager needs to change lever
	if (pLM->changeLevelPlease)
	{
		// deletes all objects other than level manager
		pOM->DeleteAllObjects(true);

		// loads next level
		pLM->nextLevel();
	}

	// check if the level manager wishes to end the game
	if (pLM->endGame)
	{
		// ends games
		EndOfGame();
		ChangeState(MENU);
	}
   // *********************************************************************
   // *********************************************************************
	
	return SUCCESS;
}

// Called when the player ends the game
// Currently this is done from the PAUSED state, when returning to the main menu
// but could be done by the gameplay programmer in other situations
// This will be used by the gameplay programmer to clean up
ErrorType Game::EndOfGame()
// called when the game ends by returning to main menu
{
	// stops all sounds
	MySoundEngine::GetInstance()->StopAllSounds();

	// delets all objects
	pOM->DeleteAllObjects(false);

	

	return SUCCESS;
}

