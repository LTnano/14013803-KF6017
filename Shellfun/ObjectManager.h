
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include <list>
#include "EventType.h"
#include "GameObject.h"

// Object manager handles all game objects, runs their update and render functions
// Cleans up inactive objects, Cleans up the game after each level and when exiting to main menu
class ObjectManager
{
private:
	// Each object should be unique in these lists
	// they should not share objects
	// e.g. An object in BGList should not be in ObjectList
	std::list<GameObject*> pObjectList; // List of foreground Objects
	std::list<GameObject*> pBGList; // List of background objects

	// Objects can be in both or none of these lists
	std::list<GameObject*> pColliderList; // List of objects that can collide
	std::list<GameObject*> pTargetList; // List of objects that are valid targets
	
	MyDrawEngine* pDE; // Pointer to draw engine
public:
	int entityNumber; // number of entities in the BG + object list

	// Constructor and destructor
	ObjectManager(MyDrawEngine* pDrawEngine);
	~ObjectManager();

	// These functions add objects to the four main lists
	void AddObject(GameObject* pNewObject);
	void AddColliderObject(GameObject* pNewObject);
	void AddTargetObject(GameObject* pNewObject);
	void AddBGObject(GameObject* pNewObject);

	// Called each frame, runs BG List and Object List through their update functions
	void UpdateAll(const double frictCoeff, float frameTime);

	// Called each frame, renders BGList first then ObjectList to keep BG objects behind
	void RenderAll();

	// Called in the update all function on the collider list
	void CheckAllCollisions();

	// Function returns a target to the object that requested it from the target list
	GameObject* GiveTarget(Vector2D pos, bool isFriendly);
	
	// Sends message to all objects
	void TransmitMessage(Message msg);

	// Removes inactive objects from lists and frees memory
	void DeleteInactive();

	// Removes all objects, optionally the level manager can be spared for level transitions
	void DeleteAllObjects(bool leveltransition);
};

