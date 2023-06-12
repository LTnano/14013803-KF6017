// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "GameObject.h"
#include "ObjectFactory.h"
#include "WormHead.h"
#include "MousePointer.h"

// The LevelManager class handles various aspects of the game's levels
// It loads the level, tracks the player's score, determines level transitions,
// sets up entities within levels, spawns new enemies, and handles the player UI
class LevelManager : public GameObject
{
private:
    MousePointer* pMP;          // Pointer to the MousePointer object
    WormHead* pWorm;            // Pointer to the Player Character (WormHead)
    ObjectFactory* pOF;         // Pointer to the ObjectFactory

    Vector2D objPoint;          // A Vector representing the location of the enemy base for spawning waves
    Vector2D screenDimensions;  // Screen dimensions for scalable UI placement
    Rectangle2D healthBar;      // Health bar rectangle
    Rectangle2D healthBarBG;    // Health bar background rectangle
    Rectangle2D ArmourBars[6];   // Armour array for hp bars
    Point2D spriteBox;          // Needed because of inheritance

    int playerScore;            // Player's score
    int savedScore;             // Saved at the beginning of a level in case of restarts
    int level;                  // Current level
    int wave;                   // Current wave tracked for level 3

    // Objectives count down to 0
    int objMegaStations;        // Number of mega stations
    int objCapitalShips;        // Number of capital ships
    int objArmedStations;       // Number of armed stations
    int objOutpost;             // Number of outposts

    float spawnTimer;           // Timer for enemy spawn
    bool objectiveFailed;       // Flag indicating if the objective has failed
    bool objectiveComplete;     // Flag indicating if the objective is complete

    // Set up level 1 entities
    void level1Setup();

    // Set up level 2 entities
    void level2Setup();

    // Set up level 3 entities
    void level3Setup();

    // Spawns a new wave of enemies
    void spawnNewWave();

public:
    bool changeLevelPlease;     // Flag requesting a level change to the game class
    bool endGame;               // Flag requesting an end game state to the game class

    // Constructor with ObjectFactory parameter
    LevelManager(ObjectFactory* pObjFac);

    // Destructor
    ~LevelManager();

    // Get the hitbox of the game object
    IShape2D& GetShape();

    // Draw Health and Armour Bar UI
    void CreateHealthArmourUI();

    // Transition to the next level
    void nextLevel();

    // Handle a message received by the game object, used to track score and objectives
    void HandleMessage(Message msg);

    // Necessary blank function due to the virtual nature of parent
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);

    // Update the state of the game object
    void Update(const double frictCoeff, float frameTime);

    // Initialise creates the player character and mouse pointer, ran at the beginning of each level
    void Initialise(Vector2D pos);
};
