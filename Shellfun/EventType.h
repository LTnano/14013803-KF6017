
// 14013803 - KF6017 Software Architecture for Games

#pragma once
#include "vector2D.h"

// messaging system - four different types of messages that are broadcast through the object manager
// criticla for level manager and targetting
enum class EventType {OBJECT_DESTROYED, OBJECTIVE_COMPLETED, WORM_DAMAGE, WORM_DEAD};
class GameObject;

struct Message
{
	EventType type; // type of event
	Vector2D location; // position of the sender
	GameObject* pSource; // pointer to the sender
	int otherData; // misc data
};