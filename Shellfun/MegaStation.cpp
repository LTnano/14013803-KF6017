
// 14013803 - KF6017 Software Architecture for Games

#include "MegaStation.h"

// this is the largest and most difficult foe with a massive health pool and substantial firepower
// inherits from spacestation and gameobject classes
MegaStation::MegaStation(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly)
{
	// Properties inherited from GameObject and SpaceStation
	TYPE = ObjectType::STATION;
	position = pos;
	facing = 1;
	active = true;
	pWorm = pW;
	pOF = pObjF;
	friendly = isFriendly;
	fireInterval = 5;
	innerRange = 400;
	guns = 5;
	timer = 0;
	maxHP = 960;

	// hitbox size
	dimensionH = 370;
	dimensionW = 370;
	spriteBox.SetDimensions(dimensionH, dimensionW);
}

// deconstructor required
MegaStation::~MegaStation()
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
void MegaStation::HandleMessage(Message msg)
{
}

// method to get the shape of the station
IShape2D& MegaStation::GetShape()
{
	return spriteBox;
}

// method is ran during game loop
void MegaStation::Update(const double frictCoeff, float frameTime)
{
	// Generate damage effects
	damageEffects();
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
				pOF->createObject<Projectile>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), 4, false);
				if ((position - pWorm->position).magnitude() < innerRange)
				{
					pOF->createObject<Projectile>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), 4, false);
				}
				// reset firing timer
				timer = 0;
			}
			// fighters and missiles spawn each time the station fires
			pOF->createObject<Fighter>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
			pOF->createObject<Fighter>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
			pOF->createObject<Missile>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
			pOF->createObject<Missile>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
			pOF->createObject<Missile>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
			pOF->createObject<Missile>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
		}
	}

	// hitbox parameters
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing + (0.25f * 3.141f));

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
void MegaStation::Deactivate()
{
	pOF->createObject<Explosion>(true, true, 3.25f, position, 0.75f);
	pOM = pOF->GetpOMInstance();
	CreateMessage(EventType::OBJECT_DESTROYED, position, this, 250);
	CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 4);
	active = false;
}

// ran in object factory when object is created
// prepares visual and audial elements and sets health + hitbox dimensions
void MegaStation::Initialise(Vector2D pos)
{
	const wchar_t* cannonsound = L"assets/cannon.wav";
	GameObject::PrepareSound(cannonsound);
	const wchar_t* stationimage = L"assets/megastation.bmp";
	GameObject::PrepareSprite(stationimage);
	scaleSprite = 1;
	health = maxHP;
}
