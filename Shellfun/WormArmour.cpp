
// 14013803 - KF6017 Software Architecture for Games

#include "WormArmour.h"
#include "Missile.h"
#include "Fighter.h"
#include "Explosion.h"

// Wormarmour is handled by the Wormhead, it always has a linked Wormbody segment
// It has three configurations, Silo, Hangar or Combined. Powerful utility platform for the player character
WormArmour::WormArmour(Mode mode, ObjectManager* pObjMan)
{
	// Assign sprites to each configuration
	missileConfig[0] = L"assets/wormmissile1.bmp";
	missileConfig[1] = L"assets/wormmissile2.bmp";
	missileConfig[2] = L"assets/wormmissile3.bmp";

	bothConfig[0] = L"assets/wormboth1.bmp";
	bothConfig[1] = L"assets/wormboth2.bmp";
	bothConfig[2] = L"assets/wormboth3.bmp";

	hangarConfig[0] = L"assets/wormhangar1.bmp";
	hangarConfig[1] = L"assets/wormhangar2.bmp";
	hangarConfig[2] = L"assets/wormhangar3.bmp";

	// Initialize object properties
	friendly = true;
	TYPE = ObjectType::WORMARMOUR;
	pOM = pObjMan;
	ammo = 0;
	linkedBody = nullptr;
	wormLoadout = Mode::MISSILES;
	switch (mode)
	{
	case Mode::MISSILES:
		wormLoadout = Mode::MISSILES;
		break;
	case Mode::BOTH:
		wormLoadout = Mode::BOTH;
		break;
	case Mode::FIGHTERS:
		wormLoadout = Mode::FIGHTERS;
		break;
	default:
		wormLoadout = Mode::BOTH;
		break;

	}
}

// Destructor
WormArmour::~WormArmour()
{
	// Resetting pointers to prevent memory leaks
	pOM = nullptr;
	pDE = nullptr;
	pSE = nullptr;
	pInputs = nullptr;
	linkedBody = nullptr;
}

// Initialise, called manually in Wormhead, prepares the empty ammo sprite
// A armour piece cannot change mode
void WormArmour::Initialise(WormSegment* BodySection)
{
	// Initialize more object properties
	linkedBody = BodySection; // Pointer to the wormbody that this is attached to
	switch (wormLoadout)
	{
	case Mode::FIGHTERS:
		GameObject::PrepareSprite(hangarConfig[0]);
		break;
	case Mode::MISSILES:
		GameObject::PrepareSprite(missileConfig[0]);
		break;
	case Mode::BOTH:
		GameObject::PrepareSprite(bothConfig[0]);
		break;
	default:
		GameObject::PrepareSprite(hangarConfig[0]);
	}
	scaleSprite = linkedBody->scaleSprite+0.1f;
	position = linkedBody->position;
	facing = linkedBody->facing;
	spriteBox.SetDimensions(70, 125); // Size of hitbox
	maxHP = 80;
	health = GetMaxHealth();
	active = true;
}

// Deactivate
void WormArmour::Deactivate()
{
	// Send message to notify
	CreateMessage(EventType::OBJECT_DESTROYED, position, this, 0);
	linkedBody->AddCooldown(5.0f); // set cooldown on the associated body
	linkedBody->setArmour(false); // set body to unarmoured
	active = false;
}

// Collision resolving
void WormArmour::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
	// Non friendly projectiles, missiles and fighters damage the armour
	if (!pOtherObj->friendly)
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
			// Effects upon collision with projectiles
			Explosion* pExplosion = new Explosion(false, false, 0.25, pOtherObj->position, 1.5);
			pExplosion->Initialise(Vector2D(0, 0));
			pOM->AddObject(pExplosion);
			takeDamage(pOtherObj->damage);
		}
	}
	
}

// Called every frame, updates position and the sprite associated with the armour
void WormArmour::Update(const double frictCoeff, float frameTime)
{
	if (linkedBody)
	{
		// Sprite increases in size as ammo fills up
		scaleSprite = linkedBody->scaleSprite+0.1f*0.9f*(ammo+1);
		switch (wormLoadout)
		{
		case Mode::FIGHTERS:
			GameObject::PrepareSprite(hangarConfig[ammo]);
			break;
		case Mode::MISSILES:
			GameObject::PrepareSprite(missileConfig[ammo]);
			break;
		case Mode::BOTH:
			GameObject::PrepareSprite(bothConfig[ammo]);
			break;
		default:
			GameObject::PrepareSprite(hangarConfig[ammo]);
		}

		// 8 seconds reload
		reloadTimer += frameTime;
		if (reloadTimer >= 8)
		{
			if (ammo < 2)
			{
				ammo += 1;
			}
			reloadTimer = 0;
		}

		// updating position to equal the linked body
		position = linkedBody->position;
		facing = linkedBody->facing;
		spriteBox.SetCentre(position);
		spriteBox.SetAngle(facing);

		// every 5 seconds heal for 1 health
		regenTimer += frameTime;
		if (regenTimer > 5)
		{
			takeDamage(-1);
			if (GetHealth() > GetMaxHealth())
			{
				SetHealth(GetMaxHealth());
			}
			regenTimer = 0;
		}
	
		// Explode and deactivate upon reaching 0 health
		if (GetHealth() < 0)
		{
			Explosion* pExplosion = new Explosion(false, true, 1, position, 1);
			pExplosion->Initialise(Vector2D(0, 0));
			pOM->AddObject(pExplosion);
			Deactivate();
		}
	}
}

// Returns the hitbox of the armour
IShape2D& WormArmour::GetShape()
{
	return spriteBox;
}

// Returns the ammo count
int WormArmour::GetAmmo()
{
	return ammo;
}

// REturns health
int WormArmour::GetHealth()
{
	return health;
}

// Returns max health
int WormArmour::GetMaxHealth()
{
	return maxHP;
}

// Sets health
void WormArmour::SetHealth(int hp)
{
	health = hp;
}

// Takes damage, negative numbers heal
void WormArmour::takeDamage(int dam)
{
	health -= dam;
}

// Launch all stored ammo in the configuration associated with the armour piece
void WormArmour::LaunchPayload(ObjectFactory* pOF, float fac, GameObject* pTar)
{
	const wchar_t* missilesound = L"assets/launch.wav";
	if (active)
	{
		while (ammo > 0)
		{
			// launches them on opposite sides
			// offsets position to stop stacking
			switch (wormLoadout)
			{
			case Mode::FIGHTERS:
				if (ammo == 1)
				{
					pOF->createObject<Fighter>(position * 0.98, fac + 1.57, friendly, pOM, pTar);
					pOF->createObject<Fighter>(position * 0.98, fac - 1.57, friendly, pOM, pTar);
				}
				else
				{
					pOF->createObject<Fighter>(position, fac + 1.57, friendly, pOM, pTar);
					pOF->createObject<Fighter>(position, fac - 1.57, friendly, pOM, pTar);
				}
				
				// create 2 fighters
				ammo -= 1;
				break;
			case Mode::MISSILES:
				GameObject::PrepareSound(missilesound);
				GameObject::PlaySEffect(false);
				if (ammo == 1)
				{
					pOF->createObject<Missile>(position * 0.98, fac + 1.57, friendly, pOM, pTar);
					pOF->createObject<Missile>(position * 0.98, fac - 1.57, friendly, pOM, pTar);
				}
				else
				{
					pOF->createObject<Missile>(position, fac + 1.57, friendly, pOM, pTar);
					pOF->createObject<Missile>(position, fac - 1.57, friendly, pOM, pTar);
				}
				// create 2 missiles
				ammo -= 1;
				break;
			case Mode::BOTH:
				GameObject::PrepareSound(missilesound);
				GameObject::PlaySEffect(false);
				if (ammo == 1)
				{
					pOF->createObject<Missile>(position * 0.98, fac + 1.57, friendly, pOM, pTar);
					pOF->createObject<Fighter>(position, fac - 1.57, friendly, pOM, pTar);
				}
				else
				{
					pOF->createObject<Missile>(position, fac - 1.57, friendly, pOM, pTar);
					pOF->createObject<Fighter>(position * 0.98, fac + 1.57, friendly, pOM, pTar);
				}
				// create 1 fighter and 1 missile
				ammo -= 1;
				break;
			default:
				break;
			}
		}
	}
}


