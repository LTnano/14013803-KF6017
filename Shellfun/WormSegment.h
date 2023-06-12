// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "Spaceship.h"
#include "WormModes.h"

// Abstract class that inherits from GameObject
// All worm parts will be segments, the head manipulates arrays of segments
class WormSegment : public GameObject
{
protected:
    Mode wormLoadout;   // wormLoadout is used for the head and armor
    float cooldown;     // Cooldown is utilized by the armor
public:
    // Default constructor
    WormSegment();

    // Pure virtual function to get the shape of the segment
    virtual IShape2D& GetShape() = 0;

    // Getter function for the cooldown
    float GetCooldown();

    // Function to set the armour generation on cooldown
    void AddCooldown(float increase);

    // Virtual function to update the segment
    virtual void Update(const double frictCoeff, float frameTime);

    // Overloaded initialization functions
    virtual void Initialise(Vector2D pos);
    virtual void Initialise(Vector2D pos, ObjectManager* pObjM);

    // Virtual function to check if the body is armoured
    virtual bool isArmoured();

    // Virtual function to set the armor status of a worm body
    virtual void setArmour(bool Bool);

    // Pure virtual function to process collisions with other game objects
    virtual void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel) = 0;

    // Function to handle messages
    void HandleMessage(Message msg);

    // Virtual destructor
    virtual ~WormSegment();

    // Setter function for the mode (wormLoadout) of the segment
    void SetMode(Mode mode);

    // Getter function for the mode (wormLoadout) of the segment
    Mode GetMode() const;
};
