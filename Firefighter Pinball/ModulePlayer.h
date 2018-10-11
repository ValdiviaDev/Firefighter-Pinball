#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

class b2RevoluteJoint;
class b2DistanceJoint;
struct b2Vec2;
struct SDL_Texture;

enum FlipperType {
	FLIP_NO_TYPE,
	FLIP_LEFT,
	FLIP_RIGHT,
	FLIP_RIGHT_UP,
	FLIP_MAX_TYPES
};


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	//Charge the textures
	void ChargeTextures();

	//Flipper creation
	b2RevoluteJoint* CreateFlipper(b2Vec2 pos, FlipperType flipperType);
	void ChargeFlipperData(FlipperType flipperType, b2Vec2 flipperPoints[7], b2Vec2& anchorA, float& lowerAngle, float& higherAngle);

	PhysBody* CreateSpring();

	void UpdateFlippers();
	void UpdateSpring();
	void PrintFlippers();
	void PrintSpring();

private:

	b2RevoluteJoint* flipperLeft = nullptr;
	b2RevoluteJoint* flipperRight = nullptr;
	b2RevoluteJoint* flipperRightUp = nullptr;

	b2Vec2 springImpulse = { 0.0f,0.0f };

	//Spring
	PhysBody* spring = nullptr;

	//Textures
	SDL_Texture* flipLeftTex = nullptr;
	SDL_Texture* flipRightTex = nullptr;
	SDL_Texture* springTex = nullptr;

};