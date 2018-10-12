#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
class UILabel;
class UIImage;

struct Bumpers {
	PhysBody* bumperBall[3];
	PhysBody* bigBumper[3];
	PhysBody* clock = nullptr;
};

struct Sensors {
	PhysBody* deathSensor = nullptr;
	PhysBody* ballSensor[14];
	PhysBody* starSensor[3];
	PhysBody* liftUpSensor[2];
	PhysBody* stairsSensor = nullptr;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	//Create
	void CreateStage(PhysBody* stage);
	void CreateBumpers();
	void CreateSensors();

	void ChargeScore();

	void ChangeLifeCount();
	bool hasLifeCountChanged = false;

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;


	//Sensors & Bumpers
	Bumpers bumper;
	Sensors sensor;
	bool sensed;

	//Testing
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	SDL_Texture* background = nullptr;
	PhysBody* stage = nullptr;

private:
	bool scoreCharged = false;
	UILabel* score = nullptr;
	UIImage* lifeCount = nullptr;

	//gUI textures
	SDL_Texture* lifeCountTex3 = nullptr;
	SDL_Texture* lifeCountTex2 = nullptr;
	SDL_Texture* lifeCountTex1 = nullptr;

};