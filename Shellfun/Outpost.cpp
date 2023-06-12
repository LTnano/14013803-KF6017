
// 14013803 - KF6017 Software Architecture for Games

#include "Outpost.h"

// this is the smallest station and the second weakest permanent enemy
// inherits from spacestation and gameobject classes
Outpost::Outpost(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly)
{
	// properties inherited from gameobject and spaceship
	TYPE = ObjectType::STATION;
	position = pos;
	facing = 1;
	active = true;
	pWorm = pW;
	pOF = pObjF;
	friendly = isFriendly;
	fireInterval = 5;
	innerRange = 300;
	guns = 1;
	timer = 0;
	maxHP = 160;

	// hitbox size - circular, use half of W
	dimensionH = 0;
	dimensionW = 110;
}

// deconstructor required
Outpost::~Outpost()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pOF = nullptr;
	pWorm = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;	
}

// inherited (empty)
void Outpost::HandleMessage(Message msg)
{
}


// creates an explosion and sends messages to the object manager alerting other objects of its death
// objective completed is to track score and level progression
// object destroyed is for missile and fighter targetting
void Outpost::Deactivate()
{
	pOF->createObject<Explosion>(true, true, 2.0, position, 1.4);
	pOM = pOF->GetpOMInstance();
	CreateMessage(EventType::OBJECT_DESTROYED, position, this, 50);
	CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 1);
	active = false;

}

// method to get the shape of the station
IShape2D& Outpost::GetShape()
{
	return spriteBox;
}

// method is ran during game loop
void Outpost::Update(const double frictCoeff, float frameTime)
{
	// Generate damage effects
	damageEffects();

	// station slowly rotates
	facing += 0.1f * frameTime;

	// station fires on a timer at range, firepower is doubled at close range
	// randomised spawn locations to prevent projectile stacking
	timer += frameTime;
	if ((position - pWorm->position).magnitude() < 1500)
	{
		if (timer > fireInterval)
		{
			// triggers firing sound effect
			GameObject::PlaySEffect(false);
			for (int i = 0; i < guns; i++)
			{
				// projectiles and missiles fire when interval met
				pOF->createObject<Missile>(position + Vector2D(rand() % 25, rand() % 25), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
				pOF->createObject<Projectile>(position + Vector2D(rand() % 25, rand() % 25), (pWorm->position - position).angle(), 4, false);
				if ((position - pWorm->position).magnitude() < innerRange)
				{
					pOF->createObject<Projectile>(position + Vector2D(rand() % 25, rand() % 25), (pWorm->position - position).angle(), 4, false);
				}
				timer = 0;
			}
			
		}
	}

	// hitbox 
	spriteBox.PlaceAt(position, dimensionW/2);

	// station has velocity in the case of collisions with large objects
	friction = velocity * frictCoeff;
	velocity += friction * frameTime;
	position += velocity * frameTime;

	// upon death, triggers the deactivate function
	if (GetHealth() < 0)
	{
		Deactivate();
	}
}

// ran in object factory when object is created
// prepares visual and audial elements and sets health + hitbox dimensions
void Outpost::Initialise(Vector2D pos)
{
	const wchar_t* cannonsound = L"assets/cannon.wav";
	GameObject::PrepareSound(cannonsound);
	const wchar_t* stationimage = L"assets/outpost.bmp";
	GameObject::PrepareSprite(stationimage);
	scaleSprite = 1;
	health = maxHP;
}
