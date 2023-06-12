
// 14013803 - KF6017 Software Architecture for Games

#include "Projectile.h"

// fast moving frictionless object that collides and damage
// has a team to prevent friendly fire
// constructor that takes a position, facing, damage value and team
Projectile::Projectile(Vector2D pos, float fac, int dam, bool isFriendly)
{
	// Initialising variables
	TYPE = ObjectType::PROJECTILE;
	position = pos;
	facing = fac;
	friendly = isFriendly;
	damage = dam;
}

// Default constructor
Projectile::Projectile()
{

}

// Loads sprite and sets the velocity of the projectile
void Projectile::Initialise(Vector2D temp)
{
    const wchar_t* filename = L"assets/bullet.bmp";
    GameObject::PrepareSprite(filename);
    scaleSprite = 3;
    velocity.setBearing(facing, 1000);
    active = true;
	timeAlive = 0;
	// sets hitbox size
	spriteBox.SetDimensions(24, 24);
}

// destructor
Projectile::~Projectile()
{
}

// returns hitbox
IShape2D& Projectile::GetShape()
{
    return spriteBox;
}

// not used
void Projectile::HandleMessage(Message msg)
{
}

// called each tick, updates position
void Projectile::Update(const double frictCoeff, float frameTime)
{
	// set hitbox parameters
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing);

	// tracks time alive
	timeAlive += frameTime;

	// translates the projectilr in the game world
    position += velocity * frameTime;

	// deactivates the projectile after a 4.5seconds have elapsed
	if (timeAlive > 4.5f)
	{
		Deactivate();
	}
}

// Collides with opposing objects and asteroids
void Projectile::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// all projectiles are blocked by asteroids
	if (pOtherObj->TYPE == ObjectType::ASTEROID)
	{
		Deactivate();
	}
	// collide with opposing fighters
	if (pOtherObj->TYPE == ObjectType::FIGHTER)
	{
		if (friendly != pOtherObj->friendly)
		{
			Deactivate();
		}
	}
	// if enemy aligned collide iwth player
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
	// if player aligned, collide with ships and stations
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