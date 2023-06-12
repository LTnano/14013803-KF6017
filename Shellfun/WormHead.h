
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "MousePointer.h"
#include "WormTail.h"
#include "WormArmour.h"
#include "WormSegment.h"
#include "ObjectFactory.h"

// Wormhead is the player character, it handles the other worm body parts by looping arrays
// The worm is fast and interacts with the world by consuming anything in its path
// the worm has a resource named nomadPop (Worshipers) that is consumed to create armour
// and to heal the worm
// The worm inherits from two abstract classes WormSegment and GameObject
// the worm is dependent on its parts
class WormHead : public WormSegment
{
private:
	const wchar_t* collisionSounds[8];  // Sound array for collision sounds
	bool playerFailState;				// Is the player dead
	int health;							// current health
	int maxHP;							// maximum HP
	float regenTimer;					// timer to regenerate health
	int nomadPop;						// worshipers count
	Circle2D spriteBox;					// Wormhead hitbox

	// Functions to break down the update function into easier blocks
	void UpdateNeck(const double frictCoeff, float frameTime);
	void UpdateBody(const double frictCoeff, float frameTime, int i);
	void UpdateTail(const double frictCoeff, float frameTime);
	void PlayerController(float frameTime);
	void ArmourTheWorm();
public:
	// Maximum ammunition
	int totalAmmo;

	// Handles messages sent to the worm
	void HandleMessage(Message msg);

	// Constructor and Destructor
	WormHead(ObjectFactory* pOF, MousePointer* pMP, Vector2D pos);
	~WormHead();

	// Getter for hitbox
	IShape2D& GetShape();

	// Initialise the worm head
	void Initialise(Vector2D pos);

	// Update the worm head, called once per frame, calls all other update functions
	// complicated movement calculations per body part
	// handles the player controls
	void Update(const double frictCoeff, float frameTime);

	// Process collision with other game objects
	void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);

	// Get the current number of nomadPop
	int GetNomads();

	// Get the current health of the worm
	int GetHealth();

	// Get the maximum health of the worm
	int GetMaxHealth();

	// Set the health of the worm
	void SetHealth(int hp);

	// Inflict damage to the worm, negative values heal
	void takeDamage(int totalDam);

	// Array of worm body armours
	WormArmour* wormBodyArmour[6];

protected:
	ObjectFactory* pObjFac;		// Pointer to the Object factory
	MousePointer* pMouse;		// Pointer to the Mouse pointer

	// Array of worm segments (Body)
	WormSegment* wormSegments[7];
	const int WORMLENGTH = 7;
};
