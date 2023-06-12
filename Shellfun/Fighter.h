
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "Spaceship.h"

// A game object that can be spawned on mass for or against the player.
// Fighters are weak enemies, good for screening, fast and maneuverable.
// They pursue a target and change target upon the destruction of their current target.
// Inherits from Spaceship.
class Fighter : public Spaceship
{
protected:
private:
    float shoottimer;                   // Timer for firing rate.
    GameObject* pTarget;                // Pointer to current target.
    AngledRectangle2D spriteBox;        // Hitbox shape.
public:
    // Get hitbox.
    IShape2D& GetShape();

    // Prepare fighter for destruction, sets active to false, creates explosion, and transmits messages.
    void Deactivate();

    // Constructor with parameters (position, facing, and friendly tag) and destructor.
    Fighter(Vector2D pos, float fac, bool isFriendly, ObjectManager* pObjMan, GameObject* pTar);
    ~Fighter();
    void Initialise(Vector2D temp);

    // Receive messages and recalculate target.
    void HandleMessage(Message msg);

    // A function to update the fighter's movement based on friction and elapsed time.
    void Update(const double frictCoeff, float frameTime);

    // A function to process a collision with another game object and update velocity.
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);
};
