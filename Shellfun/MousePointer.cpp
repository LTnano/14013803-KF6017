
// 14013803 - KF6017 Software Architecture for Games

#include "MousePointer.h"

// Mouse pointer class, creates a UI element at the mouse position
// inherits gameobject
MousePointer::MousePointer()
{
	pInputs = MyInputs::GetInstance();
}

// destructor
MousePointer::~MousePointer()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// returns hitbox (not used)
IShape2D& MousePointer::GetShape()
{
	return spriteBox;
}

// empty inherited
void MousePointer::HandleMessage(Message msg)
{
}

// empty inherited
void MousePointer::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
}

// loads the UI sprite, sets active and sets position
void MousePointer::Initialise(Vector2D pos)
{
	const wchar_t* filename = L"assets/crosshairblue.bmp";
	scaleSprite = 0.4f;
	GameObject::PrepareSprite(filename);
	active = true;
	position.set(0, 0);
}

// each frame updates the mouse position 
void MousePointer::Update(const double frictCoeff, float frameTime)
{
	pInputs->SampleMouse();

	position.XValue += pInputs->GetMouseDX();
	position.YValue -= pInputs->GetMouseDY();
}

// returns the mouse position
Vector2D MousePointer::GetPos()
{
	return position;
}