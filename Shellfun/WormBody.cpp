
// 14013803 - KF6017 Software Architecture for Games

#include "WormBody.h"
#include "Explosion.h"

// Wormbody moves and is handled by the Wormhead
// WormBody class inherits from WormSegment and GameObject
WormBody::WormBody()
{
	TYPE = ObjectType::WORMBODY;
	friendly = true;
}

// Destructor
WormBody::~WormBody()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// Getter for hitbox
IShape2D& WormBody::GetShape()
{
	return spriteBox;
}

// Method to check if the body is armored
bool WormBody::isArmoured()
{
	return hasArmour;
}

// Change armoured status
void WormBody::setArmour(bool armoured)
{
	hasArmour = armoured;
}

// Initialise the body attributes
void WormBody::Initialise(Vector2D pos, ObjectManager* pObjMan)
{
	// Load sprite file
	const wchar_t* filename = L"assets/wormbody.bmp";
	GameObject::PrepareSprite(filename);

	scaleSprite = 1;
	spriteBox.SetDimensions(85, 104);
	position = pos;
	facing = 1.0;
	velocity = Vector2D(0, 0);
	active = true;
	hasArmour = false;
	cooldown = 1.0f;
	pOM = pObjMan;
}

// Process collision and report it to head
void WormBody::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// Check if the other object is not friendly (an enemy)
	if (!pOtherObj->friendly)
	{
		if (pOtherObj->TYPE == ObjectType::FIGHTER)
		{
			// Create a message to report worm damage
			CreateMessage(EventType::WORM_DAMAGE, position, this, pOtherObj->damage);
		}
		if (pOtherObj->TYPE == ObjectType::MISSILE)
		{
			// Create a message to report worm damage
			CreateMessage(EventType::WORM_DAMAGE, position, this, pOtherObj->damage);
		}
		if (pOtherObj->TYPE == ObjectType::PROJECTILE)
		{
			// Create an explosion and add it to the object manager
			Explosion* pExplosion = new Explosion(false, false, 0.25, pOtherObj->position, 1.5);
			pExplosion->Initialise(Vector2D(0, 0));
			pOM->AddObject(pExplosion);

			// Create a message to report worm damage
			CreateMessage(EventType::WORM_DAMAGE, position, this, pOtherObj->damage);
		}
	}
}

// Called each frame, updates the body armour cooldown and hitbox parameters
void WormBody::Update(const double frictCoeff, float frameTime)
{
	if (cooldown >= 0)
	{
		cooldown -= frameTime;
	}
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing);
}
