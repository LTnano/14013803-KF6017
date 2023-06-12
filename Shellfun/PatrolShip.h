
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "Spaceship.h"
#include "Fighter.h"
#include "Missile.h"
#include "Projectile.h"
#include "WormHead.h"

// patrolships are the weakest enemy par from fighters
// fast, manoeuvrable, few guns, but launch potentially scary missiles
// inherits from spaceship and gameobject
class PatrolShip : public Spaceship
{
private:
    float timer; // timer for fire rate
    AngledRectangle2D spriteBox; // hitbox of ship
protected:
    int guns;           // Number of guns the ship has
    float fireInterval;// Time between firing intervals
    WormHead* pWorm;    // Pointer to the Worm aka player character
    
    // Deactivate the patrol ship and create an explosion
    void Deactivate();
public:
    // message handling inherited from virtual method (empty)
    void HandleMessage(Message msg);

    // get the hitbox of the patrol ship
    IShape2D& GetShape();

    // Process collisions with other game objects
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);

    // called each loop, handles movement and combat behaviour
    void Update(const double frictCoeff, float frameTime);

    // Constructor, desctructor and initalise functions
   // initialise is ran immediately after construction from object factory
    void Initialise(Vector2D pos);
    PatrolShip(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly);
    ~PatrolShip();
};

