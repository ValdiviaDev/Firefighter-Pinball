#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

class b2RevoluteJoint;
class b2DistanceJoint;
struct b2Vec2;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	b2RevoluteJoint* CreateFlipper(b2Vec2 pos);
	b2DistanceJoint* CreateSpring();

private:

	b2RevoluteJoint* flipper = nullptr;
	b2DistanceJoint* spring = nullptr;
	b2Vec2 springImpulse = { 0.0f,0.0f };

};