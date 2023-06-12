
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "GameObject.h"

// Mouse pointer class, creates a UI element at the mouse position
// inherits gameobject
class MousePointer : public GameObject
{
private:
    Point2D spriteBox; // hibox shape (inherited, not used)
protected:
public:
    //Constructor and destructor, initialises variables
    MousePointer();
    ~MousePointer();
    void Initialise(Vector2D pos);

    // Called each frame, updates the mouse position in relation to player movement
    void Update(const double frictCoeff, float frameTime);
    // returns the position of the mouse pointer
    Vector2D GetPos();

    // inherited, not used
    void HandleMessage(Message msg);
    void ProcessCollision(GameObject* pOtherObj, Vector2D initialVel);
    IShape2D& GetShape();
};

