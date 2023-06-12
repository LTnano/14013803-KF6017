
// 14013803 - KF6017 Software Architecture for Games

#include "WormSegment.h"

// Abstract class that inherits from GameObject
// All worm parts will be segments, the head manipulates arrays of segments
// Virtual Functions, including destructor and constructor are empty (mostly
WormSegment::WormSegment()
{
	// Initialising to stop compiler from complaining
	cooldown = 0;
	wormLoadout = Mode::FIGHTERS;
	isWorm = true;
}
WormSegment::~WormSegment()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}
void WormSegment::Update(const double frictCoeff, float frameTime)
{
}
void WormSegment::Initialise(Vector2D pos)
{
}
void WormSegment::Initialise(Vector2D pos, ObjectManager* pObjM)
{
}
void WormSegment::HandleMessage(Message msg)
{
}
void WormSegment::setArmour(bool Bool)
{
}

// Getter function for the cooldown
float WormSegment::GetCooldown()
{
	return cooldown;
}

// Function to add to the cooldown time
void WormSegment::AddCooldown(float increase)
{
	cooldown += increase;
}


// Function to check if the body is armored (always returns false, virtual)
bool WormSegment::isArmoured()
{
	return false;
}

// Setter function for the mode (wormLoadout) of the segment
void WormSegment::SetMode(Mode newMode)
{
	wormLoadout = newMode;
}

// Getter function for the mode (wormLoadout) of the segment
Mode WormSegment::GetMode() const
{
	return wormLoadout;
}
