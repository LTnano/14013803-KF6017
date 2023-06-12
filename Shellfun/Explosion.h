
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "GameObject.h"

// class is called upon most objects deactivation
// inherits from gameobject
// there are two types of explosion, crumbling and fireball, each with different Sound effects
// explosions can be customised on creation
class Explosion : public GameObject // define a new class called Explosion that inherits from GameObject
{
public:
    // constructor that takes several parameters
    Explosion(bool fire, bool loud, float size, Vector2D pos, float speed); 
    ~Explosion();

    // these three functions are required due to architecture, they are not used
    IShape2D& GetShape(); 
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);
    void HandleMessage(Message msg);

    // method progresses through explosion animation
    void Update(const double frictCoeff, float frameTime);

    // randomises facing and beings timer, called in object factory upon creation
    void Initialise(Vector2D pos);     

    // inherits pointers to draw, sound and inputs
private:
    // Properties of the asteroid
    float rate; // rate at which the explosion grows
    bool isFire; // flag indicating whether the explosion is a fireball or not
    bool playsSound; // flag indicating whether the explosion should play a sound or not
    Point2D spriteBox; // hitbox not used, required to exist
    const wchar_t* crumble[8]; // array of file paths for the sprite images of the crumbling explosion
    const wchar_t* fireball[11]; // array of file paths for the sprite images of the fireball explosion
    const wchar_t* explosionSounds[2]; // array of file paths for the sound effects of the explosion
};


