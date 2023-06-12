
// 14013803 - KF6017 Software Architecture for Games

#include "SpaceStation.h"

// this is the smallest station and the second weakest permanent enemy
// inherits from spacestation and gameobject classes
SpaceStation::SpaceStation()
{

}

// virtual destructor
SpaceStation::~SpaceStation()
{

}

// Generates random explosions around the entities sprite area
void SpaceStation::damageEffects()
{
	// Only explode below 60% health
	if (GetHealth() >= 0.6 * GetMaxHealth())
	{
		return;
	}

	// Generate a random number between 0 and 1, multiply this by the Maximum health, if this is higher than current health, generate an explosion
	if ((rand() % 11) / 10 * (rand() % GetMaxHealth()) > GetHealth())
	{
		// Creates the random location of the explosion
		Vector2D fireLocation;
		int randExpDist = (rand() % dimensionW) / 2;
		fireLocation.setBearing((314.0f - rand() % 628) / 100, randExpDist);

		// 20% chance of fireball, 80% chance of smoke
		// random speed
		if (rand() % 10 > 8)
		{
			if (pOF)
			{
				pOF->createObject<Explosion>(true, false, 0.15f, position + fireLocation, ((rand() % 12) / 10) + 1.3f);
			}
		}
		else
		{
			if (pOF)
			{
				pOF->createObject<Explosion>(false, false, 0.15f, position + fireLocation, ((rand() % 8) / 10) + 0.9f);
			}
		}

	}
}

// Getter and Setter for health variables
int SpaceStation::GetHealth()
{
	return health;
}
int SpaceStation::GetMaxHealth()
{
	return maxHP;
}
void SpaceStation::SetHealth(int hp)
{
	health = hp;
}

// Decreases health by the given damage value
void SpaceStation::takeDamage(int dam)
{
	health -= dam;
}

// Handles collision with other game objects, called by the object manager
void SpaceStation::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// bounce upon collision with asteroid
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

	// take damage only when colliding with player missiles, projectiles and fighters
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

	// bounce upon collision with capitalships
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

	// bounce upon collision with other stations
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

	// bounce off wormbody
	if (pOtherObj->TYPE == ObjectType::WORMBODY)
	{
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
	}

	// bounce off wormhead and take damage proportional to the relative velocity
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

	// bounce off wormtail and take damage proportional to the relative velocity
	// tail tends to deal more damage as it clips for longer
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