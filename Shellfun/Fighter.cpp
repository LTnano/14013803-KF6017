
// 14013803 - KF6017 Software Architecture for Games

#include "Fighter.h"
#include "Projectile.h"

// a game object that can be spawned on mass for or against the player
// fighers are weak enemies, good for screening, fast and manouverable
// they pursue a target and change target upon the destruction of their current target
// inherits from Spaceship
Fighter::Fighter(Vector2D pos, float fac, bool isFriendly, ObjectManager* pObjMan, GameObject* pTar)
{
    // initialising fighter from parameters
    friendly = isFriendly;
    pTarget = nullptr;
    if (pTar)
    {
        pTarget = pTar;
    }
    pOM = pObjMan;
    TYPE = ObjectType::FIGHTER;
    position = pos;
    facing = fac;
    damage = 2;
    maxHP = 0;
    health = maxHP;

    // hitbox size
    dimensionH = 26;
    dimensionW = 30;
    spriteBox.SetDimensions(dimensionH, dimensionW);
}

// method is used immediately after creation in object factory
// if using manager, call or fighter will not function
// prepares sprites and sounds based on friendly flag
void Fighter::Initialise(Vector2D temp)
{
    const wchar_t* shootsound = L"assets/shoot.wav";
    const wchar_t* shipimage = L"assets/wormfighter.bmp";
    const wchar_t* shipimage2 = L"assets/fighter.bmp";
    GameObject::PrepareSound(shootsound);
    if (friendly)
    {
        GameObject::PrepareSprite(shipimage);
    }
    else
    {
        GameObject::PrepareSprite(shipimage2);
    }
    scaleSprite = 1;
    active = true;
    velocity.setBearing(facing, 100);
  
    // requests target from the object manager
    if (!pTarget)
    {
        pTarget = pOM->GiveTarget(position, friendly);
    }

    timeAlive = 0;
    shoottimer = 0;
    SetAmmo(4);
}

// deactivates the fighter, triggers explosion and sends message to other objects
void Fighter::Deactivate()
{
    Explosion* pExplosion = new Explosion(true, false, 0.5, position, 1.5);
    pExplosion->Initialise(Vector2D(0, 0));
    pOM->AddObject(pExplosion);
    CreateMessage(EventType::OBJECT_DESTROYED, position, this, 0);
    if (!friendly)
    {
        CreateMessage(EventType::OBJECTIVE_COMPLETED, position, this, 6);
    }
    active = false;
}

// deconstructor
Fighter::~Fighter()
{
    // Resetting pointers to prevent memory leaks
    pOM = nullptr;
    pDE = nullptr;
    pSE = nullptr;
    pInputs = nullptr;
    pTarget = nullptr;
}

// called each tick, handles fighter movement and firing
// keeps fighter from overstaying and give it a target if needed
void Fighter::Update(const double frictCoeff, float frameTime)
{
    // calculate friction
    friction = velocity * frictCoeff;
    timeAlive += frameTime;

    if (pTarget)
    {
        // ask for target if current target is far away
        // added to prevent fighters flying to bottom right corner of map
        // target requesting is strange and unpredictable sometimes
        if ((pTarget->position - position).magnitude() > 5000)
        {
            pTarget = pOM->GiveTarget(position, friendly);
        }
        // double check for valid target to prevent nullptr error
        if (pTarget)
        {
            if (pTarget->isActive())
            {
                // calculates the angle difference
                // calcualtes the distance to the target
                Vector2D acceleration;
                float angleDiff = facing - velocity.angle();
                Vector2D toTarget = pTarget->position - position;
                float targetDistance = toTarget.magnitude();

                // If target is far away, move towards it while adjusting the facing angle
                if (targetDistance > 300)
                {
                    facing = toTarget.angle();
                    if (angleDiff > 3.141f) angleDiff -= 6.242f;
                    if (angleDiff < -3.141f) angleDiff += 6.242f;
                    // fighters turns gradually
                    if (angleDiff > 2.0)
                    {
                        acceleration += velocity.perpendicularVector() * 1.5;
                    }
                    {
                        acceleration.setBearing(facing, 400.0f);
                    }
                    velocity = velocity + acceleration * frameTime;
                    velocity += friction * frameTime;
                    position += velocity * frameTime;
                }
                // launch from worm
                else if (timeAlive < 1.0f)
                {
                    acceleration.setBearing(facing, 275.0f);
                    velocity = velocity + acceleration * frameTime;
                    velocity += friction * frameTime;
                    position += velocity * frameTime;
                }
                // charge when running out of time
                else if (timeAlive > 9)
                {
                    acceleration.setBearing(facing, 1200.0f);
                    velocity = velocity + acceleration * frameTime;
                    velocity += friction * frameTime;
                    position += velocity * frameTime;
                }
                // If target is within 300 units, orbit around it while facing it
                // potentially strange looking behaviour, intended, makes fighter look agile, alive
                else
                {
                    position = position.rotatedBy(0.0005f);
                    position += velocity.perpendicularVector() * frameTime;
                }
            }
        }
        shoottimer += frameTime;

        // fires every 2 seconds until out of ammo
        if (shoottimer > 2)
        {
            // does not fire when out of ammo
            if (GetAmmo() > 0)
            {
                // creates projectile and adds to the object manager
                // has no object factory pointer so has to do things manually (confusing sorry)
                GameObject::PlaySEffect(false);
                Projectile* pProj = new Projectile(position, facing, 4, friendly);
                pProj->Initialise(position);
                pOM->AddObject(pProj);
                pOM->AddColliderObject(pProj);

                // lowers ammo
                SetAmmo(GetAmmo() - 1);
            }
            // resets timer
            shoottimer = 0;
        }

    }

	// if no target, slowly comes to a halt, attempt to gain new target
	if (!pTarget)
    {
        pTarget = pOM->GiveTarget(position, friendly);
        velocity += friction * frameTime;
        position += velocity * frameTime;
    }

    // sets hitbox parameters
	spriteBox.SetCentre(position);
	spriteBox.SetAngle(facing);
    
    // deactivates if alive for too long
    if (timeAlive > 14)
    {
        Deactivate();
    }
}

// messages sent from valid targets are checked against own target to stop nullptr errors
// also to stop strange behaviour (sometimes)
void Fighter::HandleMessage(Message msg)
{
    if (msg.type == EventType::OBJECT_DESTROYED && msg.pSource == pTarget)
    {
        pTarget = nullptr;
    }
}

// return the hitbox
IShape2D& Fighter::GetShape()
{
    return spriteBox;
}

// process collisions
// player aligned fighters (friendly = true) collide with enemy ships, missiles, fighters, stations and enemy projectiles
// enemy aligned fighters (friendly = false) collide with worm, friendly missiles fighters and projectiles
// fighters have no health so immediately deactivate upon collision
void Fighter::ProcessCollision(GameObject* pOtherObj, Vector2D initialVel)
{
    if (pOtherObj->TYPE == ObjectType::FIGHTER)
    {
        if (friendly != pOtherObj->friendly)
        {
            Deactivate();
        }
    }
    if (pOtherObj->TYPE == ObjectType::MISSILE)
    {
        if (friendly != pOtherObj->friendly)
        {
            Deactivate();
        }
    }
    if (pOtherObj->TYPE == ObjectType::PROJECTILE)
    {
        if (friendly != pOtherObj->friendly)
        {
            Deactivate();
        }
    }
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
