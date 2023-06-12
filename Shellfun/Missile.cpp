
// 14013803 - KF6017 Software Architecture for Games

#include "Missile.h"
#include "Explosion.h"

// Missiles are used by stations, ships and the worm PC
// fast moving high damage weapons that are vulnerable to projectiles
Missile::Missile(Vector2D pos, float fac, bool isFriendly, ObjectManager* pObjMan, GameObject* pTar)
{
	// Initialise variables
	friendly = isFriendly;
	pTarget = nullptr;
	if (pTar)
	{
		pTarget = pTar;
	}
    pOM = pObjMan;
    TYPE = ObjectType::MISSILE;
    position = pos;
    facing = fac;
	damage = 15;
}

// destructor
Missile::~Missile()
{
	// Spawns explosion when destroyed
	Explosion* pExplosion = new Explosion(true, false, 0.7f, position, 1.1f);
	pExplosion->Initialise(Vector2D(0, 0));
	pOM->AddObject(pExplosion);

	// Stop the rocket sound effect destruction
	GameObject::StopSEffect();

	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
	pTarget = nullptr;

}

// return the hitbox
IShape2D& Missile::GetShape()
{
	return spriteBox;
}

// Loads sprites and sound effects
// sets up rest of variables
// called in object factory upon creation
void Missile::Initialise(Vector2D temp)
{
	// Missile sound effect played on loop
	const wchar_t* missilesound = L"assets/missileengine.wav";
	GameObject::PrepareSound(missilesound);
	GameObject::PlaySEffect(true);
	// Missile sprite and star sprite for exhaust plume
    const wchar_t* missileimage = L"assets/missile2.bmp";
	const wchar_t* missilethruster = L"assets/missiledrive.bmp";
	GameObject::PrepareExtraSprite(missilethruster);
    GameObject::PrepareSprite(missileimage);
    scaleSprite = 1;
	timeAlive = 0;
    velocity.setBearing(facing, 200);
    active = true;
    spriteBox.SetDimensions(26, 16);
	// Ask the object manager for a target
	if (!pTarget)
	{
		pTarget = pOM->GiveTarget(position, friendly);
	}
}

// Called each frame, updates the position and behaviour of the missile
void Missile::Update(const double frictCoeff, float frameTime)
{
	friction = velocity * frictCoeff;
	timeAlive += frameTime;
	
	// If missile is alive for over 9 seconds, deactivate
	if (timeAlive > 9)
	{
		Deactivate();
	}

	// Check for valid target
	if (pTarget)
	{
		// If target is far away, ask for a new target
		if ((pTarget->position - position).magnitude() > 5000)
		{
			pTarget = pOM->GiveTarget(position, friendly);
		}

		// If successful, pursue target
		if (pTarget)
		{
			if (pTarget->isActive())
			{
				// Calculate the angle to the target and the distance
				Vector2D acceleration;
				float angleDiff = facing - velocity.angle();
				Vector2D toTarget = pTarget->position - position;
				float targetDistance = toTarget.magnitude();
				// when far away, accelerate at high speed towards target
				if (targetDistance > 350)
				{
					if (angleDiff > 3.141f) angleDiff -= 6.242f;
					if (angleDiff < -3.141f) angleDiff += 6.242f;
					// apply force to smooth turning when making sharp turns
					if (angleDiff > 2.0)
					{
						acceleration += velocity.perpendicularVector() * 1.5;
						acceleration.setBearing(facing, 250.0f * timeAlive);
					}
					// If just spawned, fly in a straight line away from launcher
					else if (timeAlive < 0.5f)
					{
						acceleration.setBearing(facing, 250.0f);
						velocity = velocity + acceleration * frameTime;
						velocity += friction * frameTime;
						position += velocity * frameTime;
					}
					// Speed towards target
					else
					{
						acceleration.setBearing(facing, 400.0f * timeAlive);
					}
					velocity = velocity + acceleration * frameTime;

				}
				// When close, accelerate rapidly to target
				else
				{
					toTarget = toTarget.unitVector() * 500;
					velocity += toTarget * frameTime;
				}

				facing = toTarget.angle();
				velocity += friction * frameTime;
				position += velocity * frameTime;
			}
		}
	}
	// If no target, drift without acceleration
	if (!pTarget)
	{
		pTarget = pOM->GiveTarget(position, friendly);
		velocity += friction * frameTime;
		position += velocity * frameTime;
	}
	// Set hitbox parameters
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing);
}

// Handle message
void Missile::HandleMessage(Message msg)
{
	// ensure missile target is still valid after a object is destroyed
	if (msg.type == EventType::OBJECT_DESTROYED && msg.pSource == pTarget)
	{
		pTarget = nullptr;
	}
}

// send a message if not friendly for score
void Missile::Deactivate()
{
	GameObject::StopSEffect();
	active = false;
	if (!friendly)
	{
		CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 7);
	}

}

// deactivate missile on collision with relevant objects
void Missile::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// Asteroids block missiles
	if (pOtherObj->TYPE == ObjectType::ASTEROID)
	{
		Deactivate();
	}
	// Collides with opposing fighters
	if (pOtherObj->TYPE == ObjectType::FIGHTER)
	{
		if (friendly != pOtherObj->friendly)
		{
			Deactivate();
		}
	}
	// Collides with opposing missiles
	if (pOtherObj->TYPE == ObjectType::MISSILE)
	{
		if (friendly != pOtherObj->friendly)
		{
			Deactivate();
		}
	}
	// Collides with opposing projectiles
	if (pOtherObj->TYPE == ObjectType::PROJECTILE)
	{
		if (friendly != pOtherObj->friendly)
		{
			Deactivate();
		}
	}
	// Collides with worm if enemy missile
	if (!friendly)
	{
		if (pOtherObj->TYPE == ObjectType::WORMARMOUR)
		{
			Deactivate();
		}
		if (pOtherObj->TYPE == ObjectType::WORMBODY)
		{
			Deactivate();
		}
		if (pOtherObj->TYPE == ObjectType::WORMHEAD)
		{
			Deactivate();
		}
		if (pOtherObj->TYPE == ObjectType::WORMTAIL)
		{
			Deactivate();
		}
	}
	// Collides with enemy ships and stations
	if (friendly)
	{
		if (pOtherObj->TYPE == ObjectType::CAPITALSHIP)
		{
			Deactivate();
		}
		if (pOtherObj->TYPE == ObjectType::PATROLSHIP)
		{
			Deactivate();
		}
		if (pOtherObj->TYPE == ObjectType::STATION)
		{
			Deactivate();
		}
	}
}
