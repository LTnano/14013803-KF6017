
// 14013803 - KF6017 Software Architecture for Games

#include "PatrolShip.h"

// patrolships are the weakest enemy par from fighters
// fast, manoeuvrable, few guns, but launch potentially scary missiles
// inherits from spaceship and gameobject
PatrolShip::PatrolShip(Vector2D pos, ObjectFactory* pObjF, WormHead* pW, bool isFriendly)
{
	// Initialize object properties
	TYPE = ObjectType::PATROLSHIP;
	active = true;
	position = pos;
	facing = 0;
	pWorm = pW;
	pOF = pObjF;
	friendly = isFriendly;
	fireInterval = 4;
	guns = 1;
	timer = 0;
	maxHP = 80;

	// hitbox size
	dimensionH = 51;
	dimensionW = 53;
	spriteBox.SetDimensions(dimensionH, dimensionW);
}

// Destructor
PatrolShip::~PatrolShip()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pOF = nullptr;
	pWorm = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// function is called upon creation in object factory
// if using manager, call will not function
// initialises health and sprites
void PatrolShip::Initialise(Vector2D pos)
{
	const wchar_t* twinplume = L"assets/twinplume.bmp";
	GameObject::PrepareExtraSprite(twinplume);
	const wchar_t* gunsound = L"assets/cannon.wav";
	GameObject::PrepareSound(gunsound);
	const wchar_t* shipimage = L"assets/patrolship.bmp";
	GameObject::PrepareSprite(shipimage);
	scaleSprite = 1;
	health = maxHP;
}

// inherited virtual function (empty)
void PatrolShip::HandleMessage(Message msg)
{
}

// deactivate object, create explosion and transmit messages detailing this event
void PatrolShip::Deactivate()
{
	pOF->createObject<Explosion>(true, true, 0.6, position, 1.3);
	pOM = pOF->GetpOMInstance();
	CreateMessage(EventType::OBJECT_DESTROYED, position, this, 25);
	CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 5);
	active = false;
}

// get the hitbox
IShape2D& PatrolShip::GetShape()
{
	return spriteBox;
}

// handle collisions with other objects
void PatrolShip::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
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
			pOF->createObject<Explosion>(false, false, 0.25, pOtherObj->position, 1.5);
			takeDamage(pOtherObj->damage);
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
		takeDamage(pOtherObj->velocity.magnitude() / 5);
	}

	// bounce off wormtail and take damage proportional to relative velocity
	if (pOtherObj->TYPE == ObjectType::WORMTAIL)
	{
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
		takeDamage(pOtherObj->velocity.magnitude() / 10);
	}
}

// called each loop, handles movement and combat behaviour
// This function updates the state of the PatrolShip object every frame
void PatrolShip::Update(const double frictCoeff, float frameTime)
{
	// Generate damage effects
	damageEffects();

	// Set the position and orientation of the sprite
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing);

	// Determine the orientation of the ship based on its position relative to the player
	facing = (pWorm->position - position).angle();

	// Update the firing timer and fire weapons if the target is within range and the timer has elapsed
	timer += frameTime;
	if ((position - pWorm->position).magnitude() < 950)
	{
		// Circle target
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
				pOF->createObject<Projectile>(position, (pWorm->position - position).angle(), 4, false);
			}
			// Launch missile
			pOF->createObject<Missile>(position, (pWorm->position - position).angle(), false, pOF->GetpOMInstance(), nullptr);

			// Reset the firing timer
			timer = 0;
		}
	}

	// Apply acceleration based on the ship's orientation
	Vector2D acceleration;
	acceleration.setBearing(facing, 300.0f);
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
