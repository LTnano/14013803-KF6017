
// 14013803 - KF6017 Software Architecture for Games

#include "Asteroid.h"
#include "Explosion.h"

// object is a collidable, destructable object
// very simple, spawn many
// inherits from GameObject
Asteroid::Asteroid(Vector2D pos, Vector2D vel, ObjectManager* pObjMan)
{
	active = false;
	TYPE = ObjectType::ASTEROID;
	position = pos;
	velocity = vel;
	pOM = pObjMan;
}

Asteroid::~Asteroid()
{
	// create explosion, initialise and add to the object manager upon destruction
	Explosion* pExplosion = new Explosion(false, true, scaleSprite, position, 0.8f);
	pExplosion->Initialise(Vector2D(0, 0));
	pOM->AddObject(pExplosion);

	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
}

// necessary to satisfy gameobject virtual message function (empty)
void Asteroid::HandleMessage(Message msg)
{
}

// Get the hitbox
IShape2D& Asteroid::GetShape()
{
	return spriteBox;
}

// Update the asteroid object each frame
void Asteroid::Update(const double frictCoeff, float frameTime)
{
	if (active)
	{
		// asteroid has no self propulsion, only reactionary movement from collisions
		friction = velocity * frictCoeff;
		velocity += friction * frameTime;
		position += velocity * frameTime;
		spriteBox.PlaceAt(position, 64 * scaleSprite);
	}
	
}

// randomise the different aspects of the asteroid sprite and set to active
void Asteroid::Initialise(Vector2D temp)
{
	const wchar_t* filename[4] = { L"assets/rock2.bmp", L"assets/rock4.bmp", L"assets/rock3.bmp", L"assets/rock1.bmp"};
	int randomImg = rand() % 4;
	float randomSize = (3.0f - rand() % 5) / 10.0f;
	scaleSprite = 1.0f + randomSize;
	facing = (628 - rand() % 1256) /100;
	active = true;
	GameObject::PrepareSprite(filename[randomImg]);
}

// collisions handling with other game objects
void Asteroid::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// asteroids bounce off eachother
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
	// bounce off capitalships
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

	// create puffs of smoke when shot
	if (pOtherObj->TYPE == ObjectType::PROJECTILE)
	{
		if (pOtherObj->friendly)
		{
			Explosion* pExplosion = new Explosion(false, false, 0.25, pOtherObj->position, 1.5);
			pExplosion->Initialise(Vector2D(0, 0));
			pOM->AddObject(pExplosion);
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

	// wormhead eats asteroids
	// notify level manager of destruction and deactivate
	if (pOtherObj->TYPE == ObjectType::WORMHEAD)
	{
		CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 8);
		Deactivate();
	}

}
