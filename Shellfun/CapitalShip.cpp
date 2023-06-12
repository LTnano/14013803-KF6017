
// 14013803 - KF6017 Software Architecture for Games

#include "CapitalShip.h"

// the level manager treats this ship as an objective to progress levels
// capitalships are the most powerful enemies that can move and track
// slow, durable and high damage
// inherits from spaceship and gameobject
CapitalShip::CapitalShip(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly)
{
	// Initialize object properties
	TYPE = ObjectType::CAPITALSHIP;
	active = true;
	position = pos;
	facing = 0;
	pWorm = pW;
	pOF = pObjF;
	friendly = isFriendly;
	fireInterval = 6.5;
	guns = 6;
	timer = 0;
	maxHP = 480;

	// hitbox size
	dimensionH = 380;
	dimensionW = 140;
	spriteBox.SetDimensions(dimensionH, dimensionW);
}

CapitalShip::~CapitalShip()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pOF = nullptr;
	pWorm = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// deactivate object, create explosion and transmit messages detailing this event
void CapitalShip::Deactivate()
{
	pOF->createObject<Explosion>(true, true, 2.5f, position, 0.8f);
	pOM = pOF->GetpOMInstance();
	CreateMessage(EventType::OBJECT_DESTROYED, position, this, 200);
	CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 3);
	active = false;
}

// inherited virtual function (empty)
void CapitalShip::HandleMessage(Message msg)
{
}

// get the hitbox
IShape2D& CapitalShip::GetShape()
{
	return spriteBox;
}

// handle collisions with other objects
void CapitalShip::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// bounce off asteroids
	if (pOtherObj->TYPE == ObjectType::ASTEROID)
	{
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
	}
	// bounce off other capital ships
	if (pOtherObj->TYPE == ObjectType::CAPITALSHIP)
	{
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
	}

	// take damage from colliding with player aligned projectiles, missiles and fighters
	if (pOtherObj->friendly)
	{
		if (pOtherObj->TYPE == ObjectType::FIGHTER)
		{
			takeDamage(pOtherObj->damage);
		}
		if (pOtherObj->TYPE == ObjectType::MISSILE)
		{
			takeDamage(pOtherObj->damage);
		}
		if (pOtherObj->TYPE == ObjectType::PROJECTILE)
		{
			pOF->createObject<Explosion>(false, false, 0.25f, pOtherObj->position, 1.5f);
			takeDamage(pOtherObj->damage);
		}
	}
	// bounce off stations
	if (pOtherObj->TYPE == ObjectType::STATION)
	{
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
	}

	// bounce off wormhead and take damage proportional to relative velocity
	if (pOtherObj->TYPE == ObjectType::WORMHEAD)
	{
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
		takeDamage(int(pOtherObj->velocity.magnitude() / 5));
	}

	// bounce off wormtail and take damage proportional to relative velocity
	// tail deals less damage, but also collides for longer
	if (pOtherObj->TYPE == ObjectType::WORMTAIL)
	{
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
		takeDamage(int(pOtherObj->velocity.magnitude() / 10));
	}
}

// called each loop, handles movement and combat behaviour
// This function updates the state of the CapitalShip object every frame
void CapitalShip::Update(const double frictCoeff, float frameTime)
{
	// Generate damage effects
	damageEffects();
	damageEffects();

	// Set the position and orientation of the sprite
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing);

	// Determine the orientation of the ship based on its position relative to the player
	facing = (pWorm->position - position).angle();

	// Update the firing timer and fire weapons if the target is within range and the timer has elapsed
	timer += frameTime;
	if ((position - pWorm->position).magnitude() < 1500)
	{
		// Point the guns at the target
		facing = (position - pWorm->position).perpendicularVector().angle();

		// begin attack once interval has been met
		if (timer > fireInterval)
		{
			// play the stored fire sound
			GameObject::PlaySEffect(false);

			// Fire all guns
			for (int i = 0; i < guns; i++)
			{
				// randomise projectile spawn to avoid clipping
				pOF->createObject<Projectile>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), 7, false);
			}

			// Launch two fighters
			pOF->createObject<Fighter>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);
			pOF->createObject<Fighter>(position + Vector2D(rand() % 100, rand() % 100), (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);

			// Reset the firing timer
			timer = 0;
		}
	}

	// Apply acceleration based on the ship's orientation
	Vector2D acceleration;
	acceleration.setBearing(facing, 175.0f);
	velocity += acceleration * frameTime;

	// Apply friction to the ship's velocity
	friction = velocity * frictCoeff;
	velocity += friction * frameTime;

	// Update the ship's position based on its velocity
	position += velocity * frameTime;

	// Deactivate the ship if it has no health remaining
	if (GetHealth() < 0)
	{
		Deactivate();
	}
}

// function is called upon creation in object factory
// if using manager, call will not function
// initialises health and sprites
void CapitalShip::Initialise(Vector2D pos)
{
	const wchar_t* bigplume = L"assets/bigplume.bmp";
	GameObject::PrepareExtraSprite(bigplume);
	const wchar_t* gunsound = L"assets/macrocannon.wav";
	GameObject::PrepareSound(gunsound);
	const wchar_t* shipimage = L"assets/capitalship.bmp";
	GameObject::PrepareSprite(shipimage);
	scaleSprite = 1;
	health = maxHP;
}

