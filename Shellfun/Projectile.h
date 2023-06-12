
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "GameObject.h"

// fast moving frictionless object that collides and damage
// has a team to prevent friendly fire
class Projectile : public GameObject
{
private:
    AngledRectangle2D spriteBox; // hitbox
public:
    // default constructor, constructor, destructor and initialisation
    Projectile();
    Projectile(Vector2D pos, float fac, int dam, bool isFriendly);
    ~Projectile();
    void Initialise(Vector2D temp);

    // returns hitbox
    IShape2D& GetShape();

    // inherited message handling (not used)
    void HandleMessage(Message msg);
    
    // runs each frame, moves projectile through the world and deactivates if around too long
    void Update(const double frictCoeff, float frameTime);

    // deactivates upon collision with relevant object 
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);
};
