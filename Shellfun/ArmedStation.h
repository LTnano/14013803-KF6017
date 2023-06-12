
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "SpaceStation.h"

// this is a substantial foe with a decent health pool and firepower
// inherits from spacestation and gameobject classes
class ArmedStation : public SpaceStation
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
	// inherits pointers to draw, sound and inputs

	// constructor and destructor,
	void Initialise(Vector2D pos);
	ArmedStation(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly);
	~ArmedStation();
private:
	float timer; // timer for fire rate
	IShape2D& GetShape();
	void Update(const double frictCoeff, float frameTime);

};

