
// 14013803 - KF6017 Software Architecture for Games

#include "Explosion.h"

// class is called upon most objects deactivation
// inherits from gameobject
// there are two types of explosion, crumbling and fireball, each with different Sound effects
// explosions can be customised on creation with the given parameters.
Explosion::Explosion(bool fire, bool loud, float size, Vector2D pos, float speed)
{
	// Initialize the crumble and fireball bitmap paths.
	crumble[0] = L"assets/crumble1.bmp";
	crumble[1] = L"assets/crumble2.bmp";
	crumble[2] = L"assets/crumble3.bmp";
	crumble[3] = L"assets/crumble4.bmp";
	crumble[4] = L"assets/crumble5.bmp";
	crumble[5] = L"assets/crumble6.bmp";
	crumble[6] = L"assets/crumble7.bmp";
	crumble[7] = L"assets/crumble8.bmp";

	fireball[0] = L"assets/fireball1.bmp";
	fireball[1] = L"assets/fireball2.bmp";
	fireball[2] = L"assets/fireball3.bmp";
	fireball[3] = L"assets/fireball4.bmp";
	fireball[4] = L"assets/fireball5.bmp";
	fireball[5] = L"assets/fireball6.bmp";
	fireball[6] = L"assets/fireball7.bmp";
	fireball[7] = L"assets/fireball8.bmp";
	fireball[8] = L"assets/fireball9.bmp";
	fireball[9] = L"assets/fireball10.bmp";
	fireball[10] = L"assets/fireball11.bmp";

	// Initialize the explosion sound paths.
	explosionSounds[0] = L"assets/rumbleexplosion.wav";
	explosionSounds[1] = L"assets/explosion.wav";

	TYPE = ObjectType::EXPLOSION;
	// assign parameters to their respective variables
	isFire = fire;
	playsSound = loud;
	scaleSprite = size;
	position = pos;
	rate = speed;
}

Explosion::~Explosion()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;

}

// prepare explosion sound based on parameters, randomise facing
void Explosion::Initialise(Vector2D pos)
{
	GameObject::PrepareSound(explosionSounds[isFire]);
	if (playsSound)
	{
		GameObject::PlaySEffect(false);
	}
	facing = ((628 - rand() % 1256) / 100);
	
	timeAlive = 0;
	active = true;
	// if fireball, -> fireball bmp and sound
	// if crumble, -> crumble bmp and sound
}

// collision box not used but inherited
IShape2D& Explosion::GetShape()
{
	return spriteBox;
}

// messaging not used but inherited
void Explosion::HandleMessage(Message msg)
{
}

// called once per frame, tracks time alive and deactives after animation ends
void Explosion::Update(const double frictCoeff, float frameTime)
{
	// frameTime is muliplied by the rate parameter
	// to scale the speed of the animation
	timeAlive += frameTime * rate;

	// fire ball is a 2.75s animation at 1.0 rate
	if (isFire)
	{
		if (timeAlive < 0.25)
		{
			GameObject::PrepareSprite(fireball[0]);
		}
		else if (timeAlive < 0.5)
		{
			GameObject::PrepareSprite(fireball[1]);
		}
		else if (timeAlive < 0.75)
		{
			GameObject::PrepareSprite(fireball[2]);
		}
		else if (timeAlive < 1.0)
		{
			GameObject::PrepareSprite(fireball[3]);
		}
		else if (timeAlive < 1.25)
		{
			GameObject::PrepareSprite(fireball[4]);
		}
		else if (timeAlive < 1.5)
		{
			GameObject::PrepareSprite(fireball[5]);
		}
		else if (timeAlive < 1.75)
		{
			GameObject::PrepareSprite(fireball[6]);
		}
		else if (timeAlive < 2.0)
		{
			GameObject::PrepareSprite(fireball[7]);
		}
		else if (timeAlive < 2.25)
		{
			GameObject::PrepareSprite(fireball[8]);
		}
		else if (timeAlive < 2.50)
		{
			GameObject::PrepareSprite(fireball[9]);
		}		
		else if (timeAlive < 2.75)
		{
			GameObject::PrepareSprite(fireball[10]);
		}
		else
		{
			Deactivate();
		}
	}
	// crumble is a 2.00s animation at 1.0 rate
	else
	{
		if (timeAlive < 0.25)
		{
			GameObject::PrepareSprite(crumble[0]);
		}
		else if (timeAlive < 0.5)
		{
			GameObject::PrepareSprite(crumble[1]);
		}
		else if (timeAlive < 0.75)
		{
			GameObject::PrepareSprite(crumble[2]);
		}
		else if (timeAlive < 1.0)
		{
			GameObject::PrepareSprite(crumble[3]);
		}
		else if (timeAlive < 1.25)
		{
			GameObject::PrepareSprite(crumble[4]);
		}
		else if (timeAlive < 1.5)
		{
			GameObject::PrepareSprite(crumble[5]);
		}
		else if (timeAlive < 1.75)
		{
			GameObject::PrepareSprite(crumble[6]);
		}
		else if (timeAlive < 2)
		{
			GameObject::PrepareSprite(crumble[7]);
		}
		else
		{
			Deactivate();
		}
	}
	
}

// collisions not used but inherited
void Explosion::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
}
