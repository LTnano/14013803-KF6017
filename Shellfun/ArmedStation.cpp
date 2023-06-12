
// 14013803 - KF6017 Software Architecture for Games

#include "ArmedStation.h"

// this is a substantial foe with a decent health pool and firepower
// inherits from spacestation and gameobject classes
ArmedStation::ArmedStation(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly)
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
	innerRange = 350;
	guns = 4;
	timer = 0;
	maxHP = 320;

	// hitbox size - circular, use half of W
	dimensionH = 0;
	dimensionW = 184;
}

// deconstructor required
ArmedStation::~ArmedStation()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pOF = nullptr;
	pWorm = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// method to get the shape of the station
IShape2D& ArmedStation::GetShape()
{
	return spriteBox;
}

// method is ran during game loop
void ArmedStation::Update(const double frictCoeff, float frameTime)
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
				pOF->createObject<Projectile>(position + Vector2D(rand() % 50, rand() % 50), (pWorm->position - position).angle(), 4, false);
				if ((position - pWorm->position).magnitude() < innerRange)
				{
					pOF->createObject<Projectile>(position + Vector2D(rand() % 50, rand() % 50), (pWorm->position - position).angle(), 4, false);
				}
				// reset firing timer
				timer = 0;
			}
			// fighters spawn each time the station fires
			pOF->createObject<Fighter>(position + Vector2D(rand() % 50, rand() % 50), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
			pOF->createObject<Fighter>(position + Vector2D(rand() % 50, rand() % 50), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
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

// creates an explosion and sends messages to the object manager alerting other objects of its death
// objective completed is to track score and level progression
// object destroyed is for missile and fighter targetting
void ArmedStation::Deactivate()
{
	pOF->createObject<Explosion>(true, true, 2.5f, position, 0.8f);
	pOM = pOF->GetpOMInstance();
	CreateMessage(EventType::OBJECT_DESTROYED, position, this, 100);
	CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 2);
	active = false;
}

// inherited (empty)
void ArmedStation::HandleMessage(Message msg)
{
}

// ran in object factory when object is created
// prepares visual and audial elements and sets health,
void ArmedStation::Initialise(Vector2D pos)
{
	const wchar_t* cannonsound = L"assets/cannon.wav";
	GameObject::PrepareSound(cannonsound);
	const wchar_t* stationimage = L"assets/armedstation.bmp";
	GameObject::PrepareSprite(stationimage);
	scaleSprite = 1;
	health = maxHP;
}
