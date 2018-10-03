#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

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

	b2RevoluteJoint* CreateBumpers(b2Vec2 pos);

public:

	b2RevoluteJoint* bumper;

};