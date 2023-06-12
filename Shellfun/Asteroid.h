
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "GameObject.h"

// object is a collidable, destructable object
// very simple, spawn many
// inherits from GameObject
class Asteroid : public GameObject
{
private:
	// Properties of the station
    Circle2D spriteBox; // hitbox of asteroid
	// uses properties from spacestation and gameobject
	// key inherited properties listed below
	// float timer, fireInterval, innerRange
	// int guns
	// inherits pointers to draw, sound and inputs

protected:
public:
	// handle message necessary
    void HandleMessage(Message msg);

	// constructor and destructor
	void Initialise(Vector2D temp);
	Asteroid(Vector2D pos, Vector2D vel, ObjectManager* pObjMan);
    ~Asteroid();

	// method for processing collisions with other game objects
	void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);

	// method to return hitbox
    IShape2D& GetShape();

	// update is called each frame
    void Update(const double frictCoeff, float frameTime);
    
	
};

