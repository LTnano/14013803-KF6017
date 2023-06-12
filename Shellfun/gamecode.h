// gamecode.h
// Shell engine version 2020
// Chris Rook
// Last modified 25/09/2018
//   Update to prevent game loop from flipping twice
// 14013803 - KF6017 Software Architecture for Games
// added
// pointers to ObjectFactory, ObjectManager and LevelManager
// friction coefficient and frametime

#pragma once
#include "errortype.h"
#include "windows.h"
#include "mydrawengine.h"
#include "gametimer.h"
#include "myinputs.h"

#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "LevelManager.h"


// This is a hack for reading keyboard in situations where you don't want to
// use a full input system.
// Note that this makes the class windows-specific, but it already includes
// windows.h.
// Still - nasty.
#define KEYPRESSED(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// The game class. (Singleton)
// Manages a game loop, as well as menu and paused states.
// The class is not very cohesive - it really needs to be refactored into
// a few separate classes, but this keeps it simple.
class Game
{
private:
	enum GameState{MENU, PAUSED, RUNNING, GAMEOVER};
	GameState m_currentState;      // Current state of the game 
                                  // Menu = start menu
                                  // Paused = paused
                                  // Running = the main game loop
                                  // GAMEOVER = setting this state causes the program to close
                                  //            after tidying up
	void ChangeState(GameState newState);  // Use to change the state of the game to one of the states above
	int m_menuOption;              // Tracks the currently selected menu option, during main or pause menu
	Game();                        // Constructor
	~Game();                       // Destructor
	Game(Game& other);             // Copy constructor disabled
	const double frictCoeff = -0.82; // constant friction coefficient, nearly all objects that move will use this
	float frameTime;				// used to pass frameTime to the objects, (i am aware i could've used gt.mdframeTime)
	ObjectManager* pOM;				// pointer to the objectmanager
	ObjectFactory* pOF;				// pointer to the objectfactory
	LevelManager* pLM;				// pointer to the level manager
	GameTimer gt;				// gametimer
	MyInputs* pInputs;			// pointer to the input engine

public:
	

	static Game instance;          // Singleton instance
	
   // Starts the game engines - Draw Engine, Sound Engine, Input Engine - singletons
   // This is called soon after the program runs
	ErrorType Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance);

   // Terminates the game engines - Draw Engine, Sound Engine, Input Engine
   // This is called just before the program exits
	void Shutdown();

   // This is repeated, called every frame.
   // It will run either Update( ), MainMenu() or PauseMenu() depending on the
   // game state
	ErrorType Main();

   // Called each frame when in the pause state. Manages the pause menu
   // which is currently a basic placeholder
	ErrorType PauseMenu();

   // Called each frame when in the menu state. Manages the main menu,
   // which is currently a basic placeholder.
	ErrorType MainMenu();

   // Called at the start of the game - when changing state from MENU to RUNNING
   // Use this to initialise the core game
	ErrorType StartOfGame();

   // Called each frame when in the RUNNING state.
   // Checks for user pressing escape (which puts the game in the PAUSED state)
   // Flips and clears the back buffer
   // Gameplay programmer will develop this to create an actual game
	ErrorType Update();

   // Called when the player ends the game
   // Currently this is done from the PAUSED state, when returning to the main menu
   // but could be done by the gameplay programmer in other situations
   // This will be used by the gameplay programmer to clean up
	ErrorType EndOfGame();
};




