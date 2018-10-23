#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class Animation;
class PhysBody;
class UILabel;
class UIImage;

enum BigBumpType {
	NO_BUMP_TYPE,
	LEFT,
	LEFT_UP,
	RIGHT
};

enum SensorType {
	NO_SENSOR_TYPE,
	BALL,
	STAR,
};

struct Bumpers {
	PhysBody* bigBumpLeft;
	PhysBody* bigBumpLeftUp;
	PhysBody* bigBumpRight;
	PhysBody* bumperBall[4];
};

struct Sensors {
	PhysBody* deathSensor = nullptr;
	PhysBody* ballSensor[14];
	PhysBody* starSensor[3];
	PhysBody* liftUpSensor[2];
	PhysBody* stairsSensor = nullptr;

	bool isBallSensorActive[14];
	bool isStarSensorActive[3];
};

struct AnimationExecute {

	Animation* BumperBall1 = nullptr;
	Animation* BumperBall2 = nullptr;
	Animation* BumperBall3 = nullptr;
	Animation* BumperDown1 = nullptr;
	Animation* BumperDown2 = nullptr;
	Animation* BumperUp = nullptr;
	Animation* BumperClock = nullptr;

};

class ModuleSceneMain : public Module
{
public:
	ModuleSceneMain(Application* app, bool start_enabled = true);
	~ModuleSceneMain();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	//Create
	void CreateStage(PhysBody* stage);
	void CreateBumpers();
	void CreateSensors();
	void CreateChainColliders();
	void UpdateAnimationBumpers();

	void ChargeScore();

	void ChangeLifeCount();
	void ChangeScoreLabel();
	bool hasLifeCountChanged = false;

	//On collision interaction
	void SmallBumpCollisionInteraction(int bumpNum, PhysBody* ball);
	void BigBumpCollisionInteraction(BigBumpType bumpType);
	void SensorsCollisionInteraction(SensorType sensorType, int sensorNum);

	//Check on sensors being active
	void PrintActiveSensors();
	iPoint GetSensorsLocation(SensorType sensorType, int sensorNum);
	void ResetSensors();

	uint GetScore();
	uint GetPrevScore();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> collider1;

	Animation* animation = nullptr;

	Animation left_bouncer = Animation();
	Animation right_bouncer = Animation();
	Animation leftup_bouncer = Animation();

	SDL_Texture* spritesheet = nullptr;

	SDL_Rect left_bouncerRect;
	SDL_Rect lightBallRect;
	SDL_Rect lightStarRect;

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
	
	uint score = 0;
	uint prevScore = 0;

	bool scoreCharged = false;
	UILabel* scoreLabel = nullptr;
	UIImage* lifeCount = nullptr;

	//gUI textures
	SDL_Texture* lifeCountTex3 = nullptr;
	SDL_Texture* lifeCountTex2 = nullptr;
	SDL_Texture* lifeCountTex1 = nullptr;
	AnimationExecute  AnimExe;

};