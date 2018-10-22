#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Animation.h"
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
	ModulePlayer(Application* app, bool start_enabled = false);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	//Charge the textures
	void ChargeTextures();

	//Ball creation and access
	PhysBody* CreateBall();
	PhysBody* GetBall();
	void ResetBall();
	bool resetBall = false;
	uint lives = 3;

	//Flipper creation
	b2RevoluteJoint* CreateFlipper(b2Vec2 pos, FlipperType flipperType);
	void ChargeFlipperData(FlipperType flipperType, b2Vec2 flipperPoints[7], b2Vec2& anchorA, float& lowerAngle, float& higherAngle);

	//Spring creation
	PhysBody* CreateSpring(b2Vec2 anchorPos);

	//Update player elements
	void UpdateFlippers();
	void UpdateSpring();

	//Print player elements
	void PrintBall();
	void PrintFlippers();
	void PrintSpring();

	

private:

	//Ball
	PhysBody* ball = nullptr;
	iPoint ballInitialPos = { 0,0 };

	//Bumpers
	b2RevoluteJoint* flipperLeft = nullptr;
	b2RevoluteJoint* flipperRight = nullptr;
	b2RevoluteJoint* flipperRightUp = nullptr;

	//Spring
	PhysBody* spring = nullptr;
	b2Vec2 springImpulse = { 0.0f,0.0f };

	//Textures
	SDL_Texture* ballTex = nullptr;
	SDL_Texture* flipLeftTex = nullptr;
	SDL_Texture* flipRightTex = nullptr;
	SDL_Texture* springTex = nullptr;
	



};