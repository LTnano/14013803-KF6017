
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "ObjectManager.h"
#include "objecttypes.h"

// Object factory contains a template function to allow the creation of any object with varying parameters
// probably should be part of the object manager, borrows many functions from it, high coupling
class ObjectFactory
{
public:
    // Constructor and destructor
    ObjectFactory();
    ObjectFactory(ObjectManager* pObjManager);
    ~ObjectFactory();

    // Functions call their corresponding object manager functions
    void passObject(GameObject* newobject);
    void passCollision(GameObject* collider);
    void passBG(GameObject* backgroundobj);
    void passTarget(GameObject* target);
    ObjectManager* GetpOMInstance();

    // template that takes any number of arguments and adds them to the objectlist
    template<typename T, typename... Args>
    T* createObject(Args&&... args)
    {
        // template for all objects to be created and added to the object manager
        if (pObjMan)
        {
            T* newObject = new T(std::forward<Args>(args)...);
            newObject->Initialise(Vector2D(0, 0));
            // sorting hat
            // an object should not be in the BG and Obj list
            // they have mutually exclusive contents
            switch (newObject->TYPE)
            {
            case ObjectType::DEFAULT: // mouse
                passObject(newObject);
                return newObject;
            case ObjectType::ASTEROID:
                passBG(newObject);
                passCollision(newObject);
                return newObject;
            case ObjectType::CAPITALSHIP:
                passObject(newObject);
                passCollision(newObject);
                passTarget(newObject);
                return newObject;
            case ObjectType::EXPLOSION:
                passObject(newObject);
                return newObject;
            case ObjectType::FIGHTER:
                passObject(newObject);
                passCollision(newObject);
                passTarget(newObject);
                return newObject;
            case ObjectType::LEVELMANAGER:
                passObject(newObject);
                return newObject;
            case ObjectType::MISSILE:
                passCollision(newObject);
                passObject(newObject);
                return newObject;
            case ObjectType::PATROLSHIP:
                passObject(newObject);
                passCollision(newObject);
                passTarget(newObject);
                return newObject;
            case ObjectType::PROJECTILE:
                passObject(newObject);
                passCollision(newObject);
                return newObject;
            case ObjectType::STATION:
                passBG(newObject);
                passCollision(newObject);
                passTarget(newObject);
                return newObject;
            case ObjectType::WORMHEAD:
                passObject(newObject);
                passCollision(newObject);
                passTarget(newObject);
                return newObject;
            default:
                return nullptr;
            }
            
        }
        return nullptr;

    }
private:
    ObjectManager* pObjMan; // pointer to the Object Manager
};
