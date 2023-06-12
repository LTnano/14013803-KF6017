
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "WormSegment.h"
// Part of the worm as it moves, causes huge damage on collision with enemies
// Handled on enemies, not here
// WormTail class inherits from WormSegment and GameObject
class WormTail : public WormSegment
{
private:
	const wchar_t* grindingSounds[3];  // Sound array for grinding sounds
	AngledRectangle2D spriteBox;  // Sprite hitbox
	WormSegment* pWorm;  // Pointer to the Worm PC
public:
	// Getter for hitbox
	IShape2D& GetShape();

	// Initialise the Tail's parameters
	void Initialise(Vector2D pos, ObjectManager* pObjMan);

	// Collision outcomes, mostly messaging wormhead
	void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel); 

	// Update function called each frame, sets hitbox dimensions
	void Update(const double frictCoeff, float frameTime);

	// Constructor and destructor
	WormTail();
	~WormTail();
};
