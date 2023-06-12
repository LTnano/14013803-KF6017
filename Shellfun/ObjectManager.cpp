
// 14013803 - KF6017 Software Architecture for Games

#include "ObjectManager.h"

// Object manager handles all game objects, runs their update and render functions
// Cleans up inactive objects, Cleans up the game after each level and when exiting to main menu
ObjectManager::ObjectManager(MyDrawEngine* pDrawEngine)
{
	entityNumber = 0;
	pDE = pDrawEngine;
}

//destructor
ObjectManager::~ObjectManager()
{
	// Resetting pointers to prevent memory leaks
	pDE = nullptr;
	DeleteAllObjects(false);
}

// Returns pointer to target
GameObject* ObjectManager::GiveTarget(Vector2D pos, bool isFriendly)
{
	GameObject* closestTarget = nullptr;
	// initialize with a large value
	double closestDistance = DBL_MAX;
	if (pTargetList.empty())
	{
		return nullptr;
	}

	// sets opposing team to the opposite of isFriendly parameter
	bool isOpposingTeam = !isFriendly;

	// cycle through target list to determine closest target
	for (GameObject* pObject : pTargetList)
	{
		if (pObject)
		{
			// check if the target is active and not on their team
			if (pObject->isActive() && (isOpposingTeam == pObject->friendly)) // check if pObject is on the opposing team
			{
				// check the distance to target
				double distance = (pos - pObject->position).magnitude();
				if (distance < closestDistance)
				{
					// set the closest target to this if it is the smallest so far
					closestTarget = pObject;
					closestDistance = distance;
				}
			}
		}
	}
	// return the closest target
	return closestTarget;
}


// loop through the object list, delete inactive objects and deal with pointers
// call this at the end of a frame
// lists objectlist and bglist have mutually exclusive contents
// target and collider can contain duplicates from either lists
void ObjectManager::DeleteInactive()
{
	std::list<GameObject*> objectsToDelete;

	// Iterate through pObjectList
	for (GameObject*& pObject : pObjectList)
	{
		if (pObject && !pObject->isActive())
		{
			objectsToDelete.push_back(pObject);
			pObject = nullptr;
		}
	}

	// Remove deleted objects from pObjectList
	pObjectList.remove(nullptr);
	// Iterate through pBGList
	for (GameObject*& pObject : pBGList)
	{
		if (pObject && !pObject->isActive())
		{
			objectsToDelete.push_back(pObject);
			pObject = nullptr;
		}
	}

	// Remove deleted objects from pBGList
	pBGList.remove(nullptr);

	// Iterate through pColliderList
	for (GameObject*& pObject : pColliderList)
	{
		if (pObject && !pObject->isActive())
		{
			pObject = nullptr;
		}
	}

	// Remove deleted objects from pColliderList
	pColliderList.remove(nullptr);

	// Iterate through pTargetList
	for (GameObject*& pObject : pTargetList)
	{
		if (pObject && !pObject->isActive())
		{
			pObject = nullptr;

		}
	}

	// Remove deleted objects from pTargetList
	pTargetList.remove(nullptr);

	// Delete objects that are marked for deletion
	for (GameObject* pObject : objectsToDelete)
	{
		delete pObject;
	}
}

// iterates through the collider lists to check collisions
void ObjectManager::CheckAllCollisions()
{
	std::list<GameObject*>::iterator it1;
	std::list<GameObject*>::iterator it2;
	for (it1 = pColliderList.begin(); it1 != pColliderList.end(); it1++)
	{
		for (it2 = std::next(it1); it2 != pColliderList.end(); it2++)
		{
			// Handles constant self worm collisions
			if (!( (*it1)->isWorm && (*it2)->isWorm))
			{
				// checks the objects are both active
				if ( (*it1) && (*it2) && (*it1)->isActive() && (*it2)->isActive() )
				{
					// gets their hitboxes and checks if they intersect
					if ( (*it1)->GetShape().Intersects( (*it2)->GetShape() ) )
					{
						// records their initial velocity for bounce calculations
						Vector2D initialVel1 = (*it1)->velocity;
						Vector2D initialVel2 = (*it2)->velocity;
						// process the collision, each object gets a request
						(*it2)->ProcessCollision( (*it1), initialVel1);
						(*it1)->ProcessCollision( (*it2), initialVel2);
					}
				}
			}				
		}
	}
}

// Sends the message that the gameobject created to each other game object
// objects handle messages themselves
void ObjectManager::TransmitMessage(Message msg)
{
	for (GameObject* pNext : pObjectList)
	{
		if (pNext)
		{
			pNext->HandleMessage(msg);
		}
	}
}

// adds an object to the object list
void ObjectManager::AddObject(GameObject* pNewObject)
{
	pObjectList.push_back(pNewObject);
}

// adds an object to the colider list
void ObjectManager::AddColliderObject(GameObject* pNewObject)
{
	pColliderList.push_back(pNewObject);
}

// adds an object to the target list
void ObjectManager::AddTargetObject(GameObject* pNewObject)
{
	pTargetList.push_back(pNewObject);
}

// adds an object to the background objects list
void ObjectManager::AddBGObject(GameObject* pNewObject)
{
	pBGList.push_back(pNewObject);
}

// called each frame, loops through objects and runs the update function
// then checks for collisions
void ObjectManager::UpdateAll(const double frictCoeff, float frameTime)
{
	// update Bg list
	for (GameObject* pObject : pBGList)
	{
		if (pObject)
		{
			if (pObject->isActive())
			{
				pObject->Update(frictCoeff, frameTime);
			}
		}
	}
	// update object list
	for (GameObject* pObject : pObjectList)
	{
		if (pObject)
		{
			if (pObject->isActive())
			{
				
				pObject->Update(frictCoeff, frameTime);
			}
		}
	}
	
	// set entity number for spawning
	entityNumber = pObjectList.size() + pBGList.size();
	
	// check for collisions
	CheckAllCollisions();

}

// Renders all objects 
void ObjectManager::RenderAll()
{
	// Renders BG list first as they appear behind
	for (GameObject* pObject : pBGList)
	{
		if (pObject)
		{
			if (pObject->isActive())
			{
				pObject->GameObject::Render();
			}
		}
	}
	// Renders object list in front of that
	for (GameObject* pObject : pObjectList)
	{
		if (pObject)
		{
			// stops level manager from rendering
			if (pObject->isActive() && pObject->TYPE != ObjectType::LEVELMANAGER)
			{
				pObject->GameObject::Render();
			}
		}
	}
}

// Creates a list of objects to delete, then sets the pointers to null
// clears the objects afterwards
void ObjectManager::DeleteAllObjects(bool leveltransition)
{
	GameObject* pLM = nullptr;
	for (GameObject*& pNext : pObjectList)
	{
		if (leveltransition)
		{
			// level transition prevents Level manager from being deleted
			if (!(pNext->TYPE == ObjectType::LEVELMANAGER))
			{
				delete pNext;
				pNext = nullptr;
			}
			else
			{
				pLM = pNext;
			}
		}
		else
		{
			delete pNext;
			pNext = nullptr;
		}
	}
	for (GameObject*& pNext : pBGList)
	{
		delete pNext;
		pNext = nullptr;
	}
	pObjectList.clear();
	pBGList.clear();
	pColliderList.clear();
	pTargetList.clear();
	if (pLM)
	{
		pObjectList.push_back(pLM);
	}	
}
