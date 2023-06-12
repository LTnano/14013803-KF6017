
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "Projectile.h"

// Missiles are used by stations, ships and the worm PC
// fast moving high damage weapons that are vulnerable to projectiles
class Missile : public Projectile
{
private:
    GameObject* pTarget; // Pointer to current target
    AngledRectangle2D spriteBox; // Hitbox shape
public:
    // Recieves messages and recalculates target if necessary
    void HandleMessage(Message msg);

    // Prepare missile for destruction, flips active and creates explosion
    void Deactivate();

    // Constructor with parameters, position, facing and alignment
    Missile(Vector2D pos, float fac, bool isFriendly, ObjectManager* pObjMan, GameObject* pTar);
    ~Missile();
    void Initialise(Vector2D temp);

    // Ran each frame, updates velocity of the missile (friction less)
    void Update(const double frictCoeff, float frameTime);
    // Processes collisions with other objects, deactivates
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);
    // Get hitbox
    IShape2D& GetShape();
};

