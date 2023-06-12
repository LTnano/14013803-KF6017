
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "WormBody.h"
#include "ObjectFactory.h"

// Wormarmour is handled by the Wormhead, it always has a linked Wormbody segment
// It has three configurations, Silo, Hangar or Combined. Powerful utility platform for the player character
class WormArmour : public WormSegment
{
protected:
    float reloadTimer;              // Timer for reloading
private:
    int health;                     // Current health of the worm armour
    int maxHP;                      // Maximum health of the worm armour
    float regenTimer;               // Timer for health regeneration
    unsigned int ammo;              // Current ammo count
    const int maxammo = 2;          // Maximum ammo count
    const wchar_t* missileConfig[3];   // Silo config sprite file locations
    const wchar_t* bothConfig[3];      // Combined config sprite file locations
    const wchar_t* hangarConfig[3];    // Hangar config sprite file locations
    AngledRectangle2D spriteBox;       // Hitbox shape
    void Deactivate();                 // Deactivates worm armour and creates crumble explosion
public:
    // Constructor and Destructor
    WormArmour(Mode mode, ObjectManager* pObjMan);
    ~WormArmour();
    void Initialise(WormSegment* BodySection);  // Initialize the worm armour and recieve linked host segment
    WormSegment* linkedBody;     // Pointer to the linked WormBody segment

    // Getters and Setters
    IShape2D& GetShape();   // Hitbox
    int GetAmmo();
    int GetHealth(); 
    int GetMaxHealth();
    void SetHealth(int hp);
    void takeDamage(int dam);

    // Use ammunition to launch all fighters and missiles
    void LaunchPayload(ObjectFactory* pOF, float fac, GameObject* pTar);   
    
    // Resolves all collision events
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);

    // Called each frame, updates ammunition
    void Update(const double frictCoeff, float frameTime);
};
