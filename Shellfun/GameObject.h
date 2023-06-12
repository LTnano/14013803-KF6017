
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "mydrawengine.h"
#include "mysoundengine.h"
#include "myinputs.h"
#include "EventType.h"
#include "objecttypes.h"
#include "ObjectManager.h"

// Pre-declaration of the ObjectManager class
class ObjectManager;

// The base class for all game objects
// This class is abstract and has many subclasses
// It handles the common functionalities and properties of game objects
// A little bloated
class GameObject
{
private:
    PictureIndex sprite;        // Sprite that will be rendered
    PictureIndex extrasprite;   // Extra sprite for some custom effects
    SoundIndex sound;           // Sound index to be played

protected:
    ObjectManager* pOM;         // Pointer to the object manager
    MyDrawEngine* pDE;          // Pointer to the draw engine
    MySoundEngine* pSE;         // Pointer to the sound engine
    MyInputs* pInputs;          // Pointer to the input engine
    Message myMsg;              // Message that will be assembled and sent "EventType.h"
    bool active;                // Used to filter objects for deletion and toggle rendering and updating

    // Deactivate the game object
    void Deactivate();

public:
    int damage;                 // Used by collisions to determine damage
    Vector2D friction;          // Friction applied to relevant objects' velocity
    Vector2D velocity;          // Used to calculate position each tick
    Vector2D position;          // Position of the object
    ObjectType TYPE = ObjectType::DEFAULT;    // Object type determines collision behavior
    float facing;               // Angle the object is facing
    float scaleSprite;          // Size of the 2D sprite
    float timeAlive;            // Tracks time elapsed since creation
    bool friendly;              // Determines if the object is player-aligned or not
    bool isWorm = false;        // Used by the object manager to prevent constant collision checks for worm overlap

    // Check if the game object is active
    bool isActive() const;

    // Create and send a message via the object manager "EventType.h"
    void CreateMessage(EventType typ, Vector2D loc, GameObject* pSource, int other);

    // Prepare the sound associated with the game object
    void PrepareSound(const wchar_t* filename);

    // Prepare the sprite image associated with the game object
    void PrepareSprite(const wchar_t* filename);

    // Prepare an extra sprite image associated with the game object
    void PrepareExtraSprite(const wchar_t* filename);

    // Play a sound effect associated with the game object
    void PlaySEffect(bool repeat);

    // Stop playing the sound effect associated with the game object
    void StopSEffect();

    // Render the game object on the screen
    void Render();

    // Default constructor for the game object
    GameObject();

    // Virtual destructor for the game object
    virtual ~GameObject() = 0;

    // Get the shape (hitbox) of the game object
    virtual IShape2D& GetShape() = 0;

    // Handle a message received by the game object
    virtual void HandleMessage(Message msg) = 0;

    // Process collisions with other game objects
    virtual void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel) = 0;

    // Update the state of the game object
    virtual void Update(const double frictCoeff, float frameTime) = 0;

    // Initialize the game object with a position
    virtual void Initialise(Vector2D pos) = 0;
};
