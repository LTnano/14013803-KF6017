
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "GameObject.h"
#include "ObjectFactory.h"
#include "Explosion.h"

// Abstract class
// this class is an enemy, each subclass handles its own movement and combat behaviour
// inherits from gameobject
class Spaceship : public GameObject
{
private:
protected:
    int ammo;       // ship ammo
    int health;     // ship hp
    int maxHP;      // max ship hp
    int dimensionH; // Height for spriteBox and damage effects
    int dimensionW; // Width for spriteBox and damage effects

    ObjectFactory* pOF; // Pointer to the object factory
public:
    // constructor and virtual destructor
    Spaceship();
    virtual ~Spaceship() {}

    // Damaged hull effect
    void damageEffects();

    // Getter, setter and damage (health)
    int GetHealth();
    int GetMaxHealth();
    void SetHealth(int hp);
    void takeDamage(int dam);

    // Getter, setter for Ammo
    int GetAmmo();
    void SetAmmo(int newammo);

    // Virtual functions
    virtual void Initialise(Vector2D pos) = 0;
    virtual IShape2D& GetShape() = 0;
    virtual void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel) = 0;
    virtual void Update(const double frictCoeff, float frameTime) = 0;
};
