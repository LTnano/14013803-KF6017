
// 14013803 - KF6017 Software Architecture for Games

#include "Spaceship.h"

// Abstract class
// this class is an enemy, each subclass handles its own movement and combat behaviour
// inherits from gameobject
Spaceship::Spaceship()
{
    // initialising variables, will be overriden (hopefully)
    ammo = 0;
}

// Generates random explosions around the entities sprite area
void Spaceship::damageEffects()
{
	// Only explode below 60% health
	if (GetHealth() >= 0.6 * GetMaxHealth())
	{
		return;
	}

	// Generate a random number between 0 and 1, multiply this by the Maximum health, if this is higher than current health, generate an explosion
	if ( (rand() % 11)/10 * (rand() % GetMaxHealth()) > GetHealth())
	{
		// Creates the random location of the explosion
		Vector2D fireLocation;
		int randExpDist = (rand() % dimensionW)/2;
		fireLocation.setBearing((314.0f - rand() % 628) / 100, randExpDist);

		// 20% chance of fireball, 80% chance of smoke
		// random speed
		if (rand() % 10 > 8)
		{
			if (pOF)
			{
				pOF->createObject<Explosion>(true, false, 0.15, position + fireLocation, ((rand() % 12) / 10) + 1.3);
			}
		}
		else
		{
			if (pOF)
			{
				pOF->createObject<Explosion>(false, false, 0.15, position + fireLocation, ((rand() % 8) / 10) + 0.9);
			}
		}

	}
}

// health getter
int Spaceship::GetHealth()
{
    return health;
}

// max hp getter
int Spaceship::GetMaxHealth()
{
    return maxHP;
}

// health setter
void Spaceship::SetHealth(int hp)
{
    health = hp;
}

// take damage
void Spaceship::takeDamage(int dam)
{
    health -= dam;
}

// ammo getter
int Spaceship::GetAmmo()
{
    return ammo;
}

// ammo setter
void Spaceship::SetAmmo(int newammo)
{
    ammo = newammo;
}


