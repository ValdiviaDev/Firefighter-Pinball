#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
class UILabel;
class UIImage;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void CreateStage(PhysBody* stage);

	void ChargeScore();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background = nullptr;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

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