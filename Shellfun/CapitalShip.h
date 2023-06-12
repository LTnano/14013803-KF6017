// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "Spaceship.h"
#include "Fighter.h"
#include "Missile.h"
#include "Projectile.h"
#include "WormHead.h"

// the level manager treats this ship as an objective to progress levels
// capitalships are the most powerful enemies that can move and track
// slow, durable and high damage
// inherits from spaceship and gameobject
class CapitalShip : public Spaceship
{
private:
    float timer; // timer for fire rate
    AngledRectangle2D spriteBox; // hitbox of ship

protected:
    int guns;               // Number of guns the ship has
    float fireInterval;    // Time between firing intervals
    WormHead* pWorm;        // Pointer to the Worm aka player character

    // Deactivate the capital ship and create an explosion
    void Deactivate();

public:
    // message handling inherited from virtual method (empty)
    void HandleMessage(Message msg);

    // get the hitbox of the capital ship
    IShape2D& GetShape();

    // Process collisions with other game objects
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);

    // called each loop, handles movement and combat behaviour
    void Update(const double frictCoeff, float frameTime);

    // Constructor, desctructor and initalise functions
    // initialise is ran immediately after construction from object factory
    void Initialise(Vector2D pos);
    CapitalShip(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly);
    ~CapitalShip();
};
