// 14013803 - KF6017 Software Architecture for Games

#include "WormTail.h"
#include "Explosion.h"


// Part of the worm as it moves, causes huge damage on collision with enemies
// Handled on enemies, not here
// WormTail class inherits from WormSegment and GameObject
WormTail::WormTail()
{
	// Initialise
	pWorm = nullptr;
	friendly = true;
	TYPE = ObjectType::WORMTAIL;

	// Fill grinding sound array;
	grindingSounds[0] = L"assets/largebang.wav";
	grindingSounds[1] = L"assets/largebang2.wav";
	grindingSounds[2] = L"assets/largebang3.wav";
}

// Destructor
WormTail::~WormTail()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pWorm = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// Getter for the hitbox shape
IShape2D& WormTail::GetShape()
{
	return spriteBox;
}

// Initialize the Tail's parameters
void WormTail::Initialise(Vector2D pos, ObjectManager* pObjMan)
{
	const wchar_t* filename = L"assets/wormtail.bmp";
	GameObject::PrepareSprite(filename);
	scaleSprite = 0.9f;
	spriteBox.SetDimensions(200, 40);
	position = pos;
	facing = 1.0;
	velocity = Vector2D(0, 0);
	active = true;
	pOM = pObjMan;
}

// Handle collision outcomes, mostly messaging wormhead
void WormTail::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// Generate random sound
	int randomSound = rand() % 3;

	// Report collision with fighters, missiles and projectiles
	if (!pOtherObj->friendly)
	{
		if (pOtherObj->TYPE == ObjectType::FIGHTER)
		{
			CreateMessage(EventType::WORM_DAMAGE, position, this, pOtherObj->damage);
		}
		if (pOtherObj->TYPE == ObjectType::MISSILE)
		{
			CreateMessage(EventType::WORM_DAMAGE, position, this, pOtherObj->damage);
		}
		if (pOtherObj->TYPE == ObjectType::PROJECTILE)
		{
			Explosion* pExplosion = new Explosion(false, false, 0.25, pOtherObj->position, 1.5);
			pExplosion->Initialise(Vector2D(0, 0));
			pOM->AddObject(pExplosion);
			CreateMessage(EventType::WORM_DAMAGE, position, this, pOtherObj->damage);
		}
	}

	// Play grinding sound on contact with large objects
	if (pOtherObj->TYPE == ObjectType::CAPITALSHIP)
	{
		GameObject::PrepareSound(grindingSounds[randomSound]);
		GameObject::PlaySEffect(false);
	}
	if (pOtherObj->TYPE == ObjectType::STATION)
	{
		GameObject::PrepareSound(grindingSounds[randomSound]);
		GameObject::PlaySEffect(false);
	}
}

// Update function called each frame, sets hitbox dimensions
void WormTail::Update(const double frictCoeff, float frameTime)
{
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing);
}