
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "GameObject.h"
#include "ObjectFactory.h"
#include "Fighter.h"
#include "Missile.h"
#include "Projectile.h"
#include "WormHead.h"
#include "Explosion.h"

// this class is an enemy, it cannot move on it's own
// the level manager treats this station as an objective to progress levels
// subclasses of this object differ in power
// inherits from gameobject class
class SpaceStation : public GameObject
{
protected:
    ObjectFactory* pOF;     // pointer to ObjectFactory
    int health;             // current health of the station
    int maxHP;              // maximum health fo the station
    int guns;               // number of guns the station has
    float fireInterval;    // time between firing intervals
    float innerRange;       // range within which the station can fire
    WormHead* pWorm;        // pointer to the Worm aka palyer character
    int dimensionH;         // Height for spriteBox and damage effects
    int dimensionW;         // Width for spriteBox and damage effects
public:
    // constructor and destructor
    SpaceStation();
    virtual ~SpaceStation();

    // Damaged hull effect
    void damageEffects();

    // getters and setters for health property
    int GetHealth();
    int GetMaxHealth();
    void SetHealth(int hp);

    // method for taking damage
    void takeDamage(int dam);

    // virtual method to return the shape of the station
    virtual IShape2D& GetShape() = 0;

    // method to process collision with other game objects
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);

    // virtual methods for updating and initializing the station
    virtual void Update(const double frictCoeff, float frameTime) = 0;
    virtual void Initialise(Vector2D pos) = 0;
};
