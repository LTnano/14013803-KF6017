
// 14013803 - KF6017 Software Architecture for Games

#include "GameObject.h"

// The base class for all game objects
// This class is abstract and has many subclasses
// It handles the common functionalities and properties of game objects
// A little bloated
GameObject::GameObject()
{
    // Pre-setting pointers to nullptr
    pOM = nullptr;
    pDE = nullptr;
    pSE = nullptr;
    pInputs = nullptr;
}

// Called whenever the sound index needs to be played
void GameObject::PlaySEffect(bool repeat)
{
    if (pSE)
    {
        // Set volume and play the sound effect
        pSE->SetVolume(sound, -2000);
        pSE->Play(sound, repeat);
    }
}

// Destructor
GameObject::~GameObject()
{
    // Resetting pointers to prevent memory leaks
    pOM = nullptr;
    pDE = nullptr;
    pSE = nullptr;
    pInputs = nullptr;
}

// Loads the filename into the sprite
void GameObject::PrepareSprite(const wchar_t* filename)
{
    pDE = MyDrawEngine::GetInstance();
    if (pDE)
    {
        // Load the picture and assign it to the sprite
        sprite = pDE->LoadPicture(filename);
    }
}

// Loads the filename into the extrasprite
void GameObject::PrepareExtraSprite(const wchar_t* filename)
{
    pDE = MyDrawEngine::GetInstance();
    if (pDE)
    {
        // Load the picture and assign it to the extrasprite
        extrasprite = pDE->LoadPicture(filename);
    }
}

// Loads the filename into the soundindex
void GameObject::PrepareSound(const wchar_t* filename)
{
    pSE = MySoundEngine::GetInstance();
    if (pSE)
    {
        // Load the WAV file and assign it to the sound
        sound = pSE->LoadWav(filename);
    }
}

// Stops the sound from playing
void GameObject::StopSEffect()
{
    pSE = MySoundEngine::GetInstance();
    if (pSE)
    {
        // Stop the currently playing sound effect
        pSE->Stop(sound);
    }
}

// Called each tick by render all within object manager
// Special logic for extra effects can be added for the extra sprite
void GameObject::Render()
{
    // Check for draw engine
    if (pDE)
    {
        // If there is an extra sprite, display it, needs special logic
        if (extrasprite)
        {
            Vector2D jet;
            float jetFacing = facing + 3.14f;
            if (TYPE == ObjectType::MISSILE)
            {
                if (timeAlive > 0.45f)
                {
                    // Positioned to the rear of the missile for plume effect
                    jet.setBearing(jetFacing, 15.0f);
                    jet += position;
                    pDE->DrawAt(jet, extrasprite, scaleSprite * 1.2f, (628.0f - rand() % 1256) / 100);
                }
            }
            if (TYPE == ObjectType::CAPITALSHIP)
            {
                if (velocity.magnitude() > 100)
                {
                    // Calculate the position for the extrasprite and draw it
                    jet.setBearing(jetFacing, 325.0f);
                    jet += position;
                    pDE->DrawAt(jet, extrasprite, scaleSprite * 1.1f, facing);
                }
            }
            if (TYPE == ObjectType::PATROLSHIP)
            {
                if (velocity.magnitude() > 150)
                {
                    // Calculate the position for the extrasprite and draw it
                    jet.setBearing(jetFacing, 35.0f);
                    jet += position;
                    pDE->DrawAt(jet, extrasprite, scaleSprite * 0.2f, facing);
                }
            }
        }
        // Draw the main object sprite with the following parameters
        pDE->DrawAt(position, sprite, scaleSprite, facing);
    }
}

// Checks if the object is active
bool GameObject::isActive() const
{
    return active;
}

// Create a message that will be sent via the object manager to all objects "EventType.h"
void GameObject::CreateMessage(EventType typ, Vector2D loc, GameObject* pSource, int other)
{
    myMsg.type = typ;
    myMsg.location = loc;
    myMsg.pSource = pSource;
    myMsg.otherData = other;
    pOM->TransmitMessage(myMsg);
}

// Set active to false, object manager picks up on this and will delete the object
void GameObject::Deactivate()
{
    active = false;
}

