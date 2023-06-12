
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "SpaceStation.h"

// this is the smallest station and the second weakest permanent enemy
// inherits from spacestation and gameobject classes
class Outpost : public SpaceStation
{
protected:
	// sets entity to inactive and triggers destruction events
	void Deactivate();
public:
	// inherited from a virtual function (empty implementation)
	void HandleMessage(Message msg);

	// Properties of the station
	Circle2D spriteBox; // hitbox of station
	// uses properties from spacestation and gameobject
	// key inherited properties listed below
	// float timer, fireInterval, innerRange
	// int guns
	
	// constructor and destructor,
	void Initialise(Vector2D pos);
	Outpost(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly);
	~Outpost();
	
private:
	float timer; // timer for fire rate
	IShape2D& GetShape();
	void Update(const double frictCoeff, float frameTime);
	
};

