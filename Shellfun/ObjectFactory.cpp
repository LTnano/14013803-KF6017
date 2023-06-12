
// 14013803 - KF6017 Software Architecture for Games

#include "ObjectFactory.h"

// Object factory contains a template function to allow the creation of any object with varying parameters
// probably should be part of the object manager, borrows many functions from it, high coupling
ObjectFactory::ObjectFactory(ObjectManager* pObjManager) : pObjMan(pObjManager)
{

}

// default constructor
ObjectFactory::ObjectFactory()
{
	pObjMan = nullptr;
}

// destructor
ObjectFactory::~ObjectFactory()
{
	// Resetting pointers to prevent memory leaks
	pObjMan = nullptr;
}

// Uses object manager's add object function to add to object list
void ObjectFactory::passObject(GameObject* newobject)
{
	pObjMan->AddObject(newobject);
}

// Uses object manager's add collider function to add to collider list
void ObjectFactory::passCollision(GameObject* collider)
{
	pObjMan->AddColliderObject(collider);
}

// Uses object manager's add Bgobject function to add to BGobject list
void ObjectFactory::passBG(GameObject* backgroundobj)
{
	pObjMan->AddBGObject(backgroundobj);
}

// Uses object manager's add target function to add to target list
void ObjectFactory::passTarget(GameObject* target)
{
	pObjMan->AddTargetObject(target);
}

// Get an instance of the object manager
ObjectManager* ObjectFactory::GetpOMInstance()
{
	return pObjMan;
}
