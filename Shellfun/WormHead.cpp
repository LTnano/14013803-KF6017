
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "WormHead.h"
#include "WormBody.h"
#include "mydrawengine.h"
#include "Projectile.h"
#include "Explosion.h"

// Wormhead is the player character, it handles the other worm body parts by looping arrays
// The worm is fast and interacts with the world by consuming anything in its path
// the worm has a resource named nomadPop (Worshipers) that in consumed to create armour
// and to heal the worm
// The worm inherits from two abstract classes wormsegment and gameobject
// the worm is dependant on its parts
WormHead::WormHead(ObjectFactory* pOF, MousePointer* pMP, Vector2D pos)
{
	// Fill collision sounds array
	collisionSounds[0] = L"assets/largebang.wav";
	collisionSounds[1] = L"assets/largebang2.wav";
	collisionSounds[2] = L"assets/largebang3.wav";
	collisionSounds[3] = L"assets/largebang4.wav";
	collisionSounds[4] = L"assets/largeimpact.wav";
	collisionSounds[5] = L"assets/largeimpact2.wav";
	collisionSounds[6] = L"assets/largeimpact3.wav";
	collisionSounds[7] = L"assets/largeimpact4.wav";

	// Initialising attributes
	TYPE = ObjectType::WORMHEAD;
	playerFailState = false;
	pMouse = pMP;
	pObjFac = pOF;
	pOM = pObjFac->GetpOMInstance();
	friendly = true;
	maxHP = 500;
	position = pos;
	health = GetMaxHealth();

	// Create the worm segment body, the last element is a tail
	for (int i = 0; i < WORMLENGTH; i++)
	{
		if (i < WORMLENGTH - 1)
		{
			wormSegments[i] = new WormBody;

		}
		else
		{
			wormSegments[i] = new WormTail;
		}
	}
}

// Destructor, destroys the worm and all pointers
WormHead::~WormHead()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pObjFac = nullptr;
	pMouse = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;

	// Destroy worm and pointers
	for (WormSegment* pWormPart : wormSegments)
	{
		delete pWormPart;
		pWormPart = nullptr;
	}
	for (WormArmour* pArmourPart : wormBodyArmour)
	{
		delete pArmourPart;
		pArmourPart = nullptr;
	}
}

// Return hitbox of the head
IShape2D& WormHead::GetShape()
{
	return spriteBox;
}

// Initialise more parameters and the body
void WormHead::Initialise(Vector2D pos)
{
	// Load wormhead sprite
	const wchar_t* filename = L"assets/wormhead.bmp";
	scaleSprite = 1;
	GameObject::PrepareSprite(filename);
	// Initialise the body
	for (int i = 0; i < WORMLENGTH; i++)
	{
		wormSegments[i]->Initialise(position + Vector2D(-10, -10)*i, pOM);
		pObjFac->passCollision(wormSegments[i]);

	}
	facing = 1.0;
	velocity = Vector2D(0, 0);
	active = true;
}

// Called each frame, updates the player position, all associated worm parts
// Updates the health of the worm and is responsible for player inputs
// Tell the body armour to launch attacks
void WormHead::Update(const double frictCoeff, float frameTime)
{
	// Listens for player inputs
	PlayerController(frameTime);

	// Move the worm
	friction = velocity * frictCoeff;
	velocity += friction * frameTime;
	Vector2D oldPos = position;
	position += velocity * frameTime;
	Vector2D dPos = position - oldPos;

	// Move camera and translate mouse with it
	pMouse->position += dPos;
	pDE->theCamera.PlaceAt(Vector2D(position.XValue, -position.YValue));

	// Set parameters hitbox for head
	spriteBox.PlaceAt(position, 55);

	// Create or disable worm parts if possible
	ArmourTheWorm();

	// Iterates backwards to draw tail on bottom
	for (int i = WORMLENGTH-1; i > -1; i--) 
	{
		// segment nearest head
		if (i == 0) 
		{
			wormSegments[i]->Update(frictCoeff, frameTime);
			if (GetHealth() > 0)
			{
				// Calculates the velocity of the worm neck
				UpdateNeck(frictCoeff, frameTime);
			}
		}
		//tail segment
		else if (i == WORMLENGTH - 1) 
		{
			wormSegments[i]->Update(frictCoeff, frameTime);
			if (GetHealth() > 0)
			{
				// Calculates the velocity of the worm tail
				UpdateTail(frictCoeff, frameTime);
			}
		}
		// middle contents of worm anatomy
		else 
		{
			wormSegments[i]->Update(frictCoeff, frameTime);
			if (GetHealth() > 0)
			{
				// Calculates the velcoity of the worm body parts
				UpdateBody(frictCoeff, frameTime, i);
			}
		}

		// Move worm parts
		wormSegments[i]->velocity += friction * frameTime;
		wormSegments[i]->position += wormSegments[i]->velocity * frameTime;

		// Render worm parts
		wormSegments[i]->GameObject::Render();
	}

	// Reset total ammo counter to 0 for UI
	totalAmmo = 0;

	// Cycle through the worm body armour
	for (WormArmour* armour : wormBodyArmour)
	{

		if (armour)
		{
			if (armour->isActive())
			{
				// Render and update the armour pieces
				armour->GameObject::Render();
				armour->Update(frictCoeff, frameTime);
				// Update Ui ammo levels
				totalAmmo += armour->GetAmmo();
			}
			else
			{
				armour = nullptr;
			}
		}
	}

	// Cooldown timer for regeneration
	regenTimer += frameTime;
	if (regenTimer > 1)
	{
		// Heal slowly, 1 hp every second
		takeDamage(-1);
		if (GetHealth() > GetMaxHealth())
		{
			SetHealth(GetMaxHealth());
		}
		// Heal rapidly and consume worshipers if above 625 HP (Full worm length armour refill)
		if (nomadPop > 625 && GetHealth() < GetMaxHealth() - 25)
		{
			nomadPop -= 25;
			takeDamage(-25);
		}
		// Reset regen timer
		regenTimer = 0;
	}

	// If player falls below 0 hp, trigger failure state
	if (GetHealth() < 0)
	{
		// Send message to level manager that player has died
		CreateMessage(EventType::WORM_DEAD, position, this, 0);
		playerFailState = true;
	}
}

// Get the nomads (worshipers)
int WormHead::GetNomads()
{
	return nomadPop;
}

// Get current health
int WormHead::GetHealth()
{
	return health;
}

// Get max health
int WormHead::GetMaxHealth()
{
	return maxHP;
}

// Set health to a value
void WormHead::SetHealth(int hp)
{
	health = hp;
}

// Take damage, minus values heal
void WormHead::takeDamage(int totalDam)
{
	health -= totalDam;
}

// Handles messages sent by the object manager
void WormHead::HandleMessage(Message msg)
{
	// Worm damage is recieved from the body and tail
	if (msg.type == EventType::WORM_DAMAGE)
	{
		takeDamage(msg.otherData);
	}
	// Increases worshipers when relevant targets die
	if (msg.type == EventType::OBJECT_DESTROYED)
	{
		switch (msg.pSource->TYPE)
		{
		case ObjectType::CAPITALSHIP:
		case ObjectType::PATROLSHIP:
		case ObjectType::STATION:
			nomadPop += msg.otherData;
			break;
		}
	}
}

// These functions allows the body to flow, if acceleration is increased rapidly
// then they risk detaching visually (not perfect)
// Handles the neck segment pathing
void WormHead::UpdateNeck(const double frictCoeff, float frameTime)
{
	int i = 0;	
	Vector2D difference = position - wormSegments[i]->position;
	// follow head
	if (difference.magnitude() > 80)
	{
		wormSegments[i]->velocity += difference * frameTime * 30 * (i + 2);
		friction = wormSegments[i]->velocity * frictCoeff * 5 * (i + 1);
	}
	// stop neck from stacking 
	else
	{
		friction = wormSegments[i]->velocity * frictCoeff * 15 * (i + 1);

	}
	wormSegments[i]->facing = difference.angle();
}
// handles the main body segments pathing
void WormHead::UpdateBody(const double frictCoeff, float frameTime, int i)
{
	Vector2D difference = wormSegments[i - 1]->position - wormSegments[i]->position;
	if (difference.magnitude() > 100)
	// if body is far away, speed up to prevent gaps
	{
		// Last two segments are faster as they lag behind (frame rate messes with this drasticly)
		if (i == 5)
		{
			wormSegments[i]->velocity += difference * frameTime * 180 * (i + 2); //4, 5, 6, 7, 8, 9, 10
			friction = wormSegments[i]->velocity * frictCoeff * 9 * (i + 2.5);
		}
		if (i == 4)
		{
			wormSegments[i]->velocity += difference * frameTime * 300 * (i + 2); //4, 5, 6, 7, 8, 9, 10
			friction = wormSegments[i]->velocity * frictCoeff * 9 * (i + 2.5);
		}
		wormSegments[i]->velocity += difference * frameTime * 100 * (i + 2); //4, 5, 6, 7, 8, 9, 10
		friction = wormSegments[i]->velocity * frictCoeff * 9 * (i + 1.5);

	}
	// follow part in front
	else if (difference.magnitude() > 65)
	{
		if (i == 5)
		{
			wormSegments[i]->velocity += difference * frameTime * 125 * (i);
			friction = wormSegments[i]->velocity * frictCoeff * 10 * (i + 2);
		}
		if (i == 4)
		{
			wormSegments[i]->velocity += difference * frameTime * 275 * (i);
			friction = wormSegments[i]->velocity * frictCoeff * 10 * (i + 2);
		}
		wormSegments[i]->velocity += difference * frameTime * 100 * (i);
		friction = wormSegments[i]->velocity * frictCoeff * 9 * (i + 1);

	}
	// stop body from stacking 
	else
	{
		friction = wormSegments[i]->velocity * frictCoeff * 3 * (i + 2);
	}
	wormSegments[i]->facing = difference.angle();
}
// handles the tail segment pathing
void WormHead::UpdateTail(const double frictCoeff, float frameTime)
{
	int i = WORMLENGTH - 1;
	Vector2D difference = wormSegments[i - 1]->position - wormSegments[i]->position;
	// if tail is far away, speed up to prevent gaps
	if (difference.magnitude() > 180)
	{
		wormSegments[i]->velocity += difference * frameTime * 70 * (i + 2);
		friction = wormSegments[i]->velocity * frictCoeff * 5 * (i + 1);

	}
	// follow 
	else if (difference.magnitude() > 110)
	{
		wormSegments[i]->velocity += difference * frameTime * 30 * (i + 2);
		friction = wormSegments[i]->velocity * frictCoeff * 5 * (i + 1);

	}
	// stop tail from stacking 
	else
	{
		friction = wormSegments[i]->velocity * frictCoeff * 10 * (i + 1);
	}
	wormSegments[i]->facing = difference.angle();
}

// Handles player inputs
void WormHead::PlayerController(float frameTime)
{
	// Pointer to My inputs
	pInputs = MyInputs::GetInstance();
	
	// Disables controls if the player has failed
	if (pInputs && !playerFailState)
	{
		pInputs->SampleMouse();
		Vector2D direction = pMouse->position - position;
		facing = direction.angle();

		// Left mouse button causes worm to head towards the mouse cursor
		if (pInputs->IfMouseLeftDown())
		{
			Vector2D acceleration;
			float angleDiff = direction.angle() - velocity.angle();
			if (angleDiff > 3.141f) angleDiff -= 6.242f;
			if (angleDiff < -3.141f) angleDiff += 6.242f;
			if (angleDiff > 2.0)
			{
				acceleration += velocity.perpendicularVector()*3;
			}
			else 
			{
				acceleration.setBearing(facing, 700.0f);
			}
			velocity = velocity + acceleration * frameTime;

		}

		// Tab switches type of armour that will be built next ooprtunity
		if (pInputs->NewKeyPressed(DIK_TAB))
		{
			switch (WormSegment::GetMode())
			{
			case Mode::MISSILES:
				wormLoadout = Mode::BOTH;
				break;
			case Mode::BOTH:
				wormLoadout = Mode::FIGHTERS;
				break;
			case Mode::FIGHTERS:
				wormLoadout = Mode::MISSILES;
				break;
			default:
				wormLoadout = Mode::BOTH;
				break;
			}
		}

		// Space launches all stored ammo at the closest target
		if (pInputs->NewKeyPressed(DIK_SPACE))
		{
			GameObject* pTar;
			pTar = pOM->GiveTarget(position, friendly);
			for (WormArmour* armour : wormBodyArmour)
			{
				if (armour)
				{
					armour->LaunchPayload(pObjFac, armour->facing, pTar);
				}
			}
		}
	}
}

// Armour all body parts if possible, consumes 100 worshipers to build
// Type is the same as current wormloadout, has an internal cooldown
void WormHead::ArmourTheWorm()
{
	for (int i = 0; i < WORMLENGTH-1; i++)
	{
		if (!wormSegments[i]->isArmoured())
		{
			if (nomadPop >= 100)
			{
				if (wormSegments[i]->GetCooldown() < 0.5f)
				{
					WormArmour* pArmour = new WormArmour(wormLoadout, pOM);
					wormBodyArmour[i] = pArmour;
					WormSegment* pI = wormSegments[i];
					wormBodyArmour[i]->Initialise(pI);
					wormSegments[i]->setArmour(true);
					pObjFac->passCollision(pArmour);
					pObjFac->passTarget(pArmour);
					nomadPop -= 100;
				}
			}
		}
	}
}

// Handles various collisions
void WormHead::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// Generates a random sound to play on collision with large objects
	int randomSound1 = rand() % 4;
	int randomSound2 = 4 + rand() % 4;

	// Asteroids heal the worm and award worshipers 
	if (pOtherObj->TYPE == ObjectType::ASTEROID)
	{
		velocity = velocity * 0.6f;
		nomadPop += 25;
		takeDamage(-25);
	}
	// Capitalships bounce off the worm and play a random collision sound
	if (pOtherObj->TYPE == ObjectType::CAPITALSHIP)
	{
		GameObject::PrepareSound(collisionSounds[randomSound1]);
		GameObject::PlaySEffect(false);
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
	}
	// Stations bounce off the worm and play a random collision sound
	if (pOtherObj->TYPE == ObjectType::STATION)
	{
		GameObject::PrepareSound(collisionSounds[randomSound2]);
		GameObject::PlaySEffect(false);
		Vector2D normal = (position - pOtherObj->position).unitVector();
		Vector2D relVel = velocity - initialVel;
		if (normal * relVel <= 0)
		{
			velocity = relVel - 2 * (relVel * normal) * normal;
			velocity = velocity * 0.6f;
		}
	}
	// Enemy missiles and projectiles cause damage to worm
	if (!pOtherObj->friendly)
	{
		if (pOtherObj->TYPE == ObjectType::MISSILE)
		{
			takeDamage(pOtherObj->damage);
		}
		if (pOtherObj->TYPE == ObjectType::PROJECTILE)
		{
			Explosion* pExplosion = new Explosion(false, false, 0.25, pOtherObj->position, 1.5);
			pExplosion->Initialise(Vector2D(0, 0));
			pOM->AddObject(pExplosion);
			takeDamage(pOtherObj->damage);
		}
	}
}
