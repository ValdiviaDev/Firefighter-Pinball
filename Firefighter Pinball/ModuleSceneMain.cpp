#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneMain.h"
#include "ModuleSceneStart.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneOver.h"
#include "ModuleGui.h"


ModuleSceneMain::ModuleSceneMain(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;

	left_bouncerRect.x = 225;
	left_bouncerRect.y = 295;
	left_bouncerRect.w = 44;
	left_bouncerRect.h = 97;


	//left bouncer

	left_bouncer.PushBack({ 365,295,46,97 });
	left_bouncer.PushBack({ 300,295,45,97 });
	left_bouncer.PushBack({ 225,295,44,97 });
	left_bouncer.loop = false;
	left_bouncer.speed = 0.01f;

	//right bouncer

	right_bouncer.PushBack({ 225,169,46,97 });
	right_bouncer.PushBack({ 291,169,45,97 });
	right_bouncer.PushBack({ 367,169,44,97 });
	right_bouncer.loop = false;
	right_bouncer.speed = 0.01f;

	//leftup bouncer

	leftup_bouncer.PushBack({});
	leftup_bouncer.PushBack({});
	leftup_bouncer.PushBack({});
	leftup_bouncer.PushBack({});
	leftup_bouncer.loop = false;
	leftup_bouncer.speed = 1.5f;


	//medical cross
	//blue circles

}

ModuleSceneMain::~ModuleSceneMain()
{}

// Load assets
bool ModuleSceneMain::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	AnimExe.BumperDown1 = &left_bouncer;
	AnimExe.BumperDown2 = &right_bouncer;
	
	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	//Enable modules
	App->physics->Enable();
	App->fonts->Enable();
	App->gui->Enable();
	App->player->Enable();

	//Play scene music
	App->audio->PlayMusic("assets/audio/music/stageTheme.ogg");

	circle = App->textures->Load("assets/wheel.png"); 
	box = App->textures->Load("assets/crate.png");
	rick = App->textures->Load("assets/rick_head.png");
	bonus_fx = App->audio->LoadFx("assets/bonus.wav");

	//Load background
	background = App->textures->Load("assets/textures/background.png");

	//Load spritesheet	
	spritesheet = App->textures->Load("assets/textures/PinballSprites.png");

	//Create the sensors and bumpers for the ball to interact with
	CreateSensors();
	CreateBumpers();
	resetSensors();

	//Charge life count
	lifeCountTex3 = App->textures->Load("assets/textures/gui/lifeCount3.png");
	lifeCountTex2 = App->textures->Load("assets/textures/gui/lifeCount2.png");
	lifeCountTex1 = App->textures->Load("assets/textures/gui/lifeCount1.png");
	
	lifeCount = App->gui->CreateImage({ 10,10 }, { 0,0,255,76 }, lifeCountTex3, this);

	//Be able to charge the score
	scoreCharged = false;
	score = 0;

	//Background Chain Colliders

	CreateChainColliders();

	

	return ret;
}

// Load assets
bool ModuleSceneMain::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background);
	App->textures->Unload(lifeCountTex3);
	App->textures->Unload(lifeCountTex2);
	App->textures->Unload(lifeCountTex1);
	
	App->physics->Disable();
	App->player->Disable();
	App->gui->Disable();
	App->fonts->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneMain::Update()
{

	App->renderer->Blit(background, 0, 0);

	UpdateAnimationBumpers();

	if (!scoreCharged)
		ChargeScore();

	if (hasLifeCountChanged)
		ChangeLifeCount();

	if (App->player->lives == 0) {
		App->fade->FadeToBlack(this, App->scene_over, 1.5f);
		App->player->lives = 3;
	}


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

		

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}


	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->fade->FadeToBlack(this, App->scene_over, 1.5f);
			
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneMain::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	PhysBody* ball = App->player->GetBall();

	//Death sensor
	if (bodyA == ball && bodyB == sensor.deathSensor) {
		App->player->resetBall = true;
	}

	//Bumpers
	//Small bumpers
	for (int i = 0; i < 4; i++) {
		if (bodyA == ball && bodyB == bumper.bumperBall[i])
			SmallBumpCollisionInteraction(i, ball);
			
	}

	//Big bumpers
	if (bodyA == ball && bodyB == bumper.bigBumpLeft) {
		ball->body->ApplyLinearImpulse({ 1.0f * 2.5, -1.0f * 2.5 }, { 0.0f,0.0f }, true);
		BigBumpCollisionInteraction(LEFT);
	}

	if (bodyA == ball && bodyB == bumper.bigBumpLeftUp) {
		ball->body->ApplyLinearImpulse({ 1.0f * 2.5, -1.0f * 2.5 }, { 0.0f,0.0f }, true);
		BigBumpCollisionInteraction(LEFT_UP);
	}


	if (bodyA == ball && bodyB == bumper.bigBumpRight) {
		ball->body->ApplyLinearImpulse({ -1.0f * 2.5, -1.0f * 2.5 }, { 0.0f,0.0f }, true);
		BigBumpCollisionInteraction(RIGHT);
	}

	//Sensors

	//Lift up
	for (int i = 0; i < 2; i++) {
		if (bodyA == ball && bodyB == sensor.liftUpSensor[i]) {
			ball->body->ApplyForceToCenter({ 0.0f,-210.0f }, true);
			App->audio->PlayFx(App->audio->GetFX().liftUp);
		}
	}

	//Balls
	for (int i = 0; i < 14; i++) {
		if (bodyA == ball && bodyB == sensor.ballSensor[i])
			SensorsCollisionInteraction(BALL, i);
	}

	//Stars
	for (int i = 0; i < 3; i++) {
		if (bodyA == ball && bodyB == sensor.starSensor[i]) 
			SensorsCollisionInteraction(STAR, i);

	}

	//Stair
	if (bodyA == ball && bodyB == sensor.stairsSensor) {
		App->audio->PlayFx(App->audio->GetFX().sirenHose);
		score += 200;
		ChangeScoreLabel();
	}

}

void ModuleSceneMain::CreateStage(PhysBody * stage)
{
	int scenePoints[124] = {
		515, 755,
		515, 410,
		510, 324,
		502, 273,
		492, 232,
		472, 188,
		442, 147,
		413, 114,
		385, 93,
		350, 70,
		323, 67,
		301, 69,
		270, 72,
		247, 70,
		210, 64,
		179, 55,
		160, 58,
		139, 67,
		115, 81,
		88, 107,
		73, 126,
		63, 143,
		59, 154,
		64, 175,
		79, 198,
		178, 359,
		181, 375,
		190, 384,
		160, 401,
		76, 468,
		60, 483,
		49, 500,
		46, 543,
		47, 583,
		45, 664,
		45, 707,
		45, 733,
		45, 754,
		74, 755,
		75, 704,
		202, 773,
		201, 799,
		0, 800,
		1, 1,
		537, 0,
		533, 799,
		301, 800,
		300, 773,
		426, 705,
		426, 755,
		468, 756,
		468, 648,
		468, 595,
		468, 520,
		469, 408,
		467, 353,
		459, 287,
		441, 236,
		420, 195,
		397, 166,
		369, 132,
		320, 101
	};

	App->physics->CreateChain(0, 0, scenePoints, 124, b2_staticBody);

}

void ModuleSceneMain::CreateChainColliders() {

	int background1[22] = {
		520, 804,
		518, 401,
		506, 268,
		473, 189,
		438, 139,
		380, 81,
		340, 59,
		258, 42,
		275, 0,
		580, -7,
		572, 807
	};
	collider1.add(App->physics->CreateChain(0, 0, background1, 22, b2_staticBody));

	int background2[44] = {
		199, 800,
		197, 765,
		80, 704,
		80, 762,
		42, 767,
		41, 508,
		74, 468,
		10, 325,
		5, 245,
		25, 145,
		64, 74,
		98, 36,
		140, 9,
		186, 5,
		213, 12,
		230, 25,
		246, 35,
		262, 38,
		252, -11,
		-14, -24,
		-31, 845,
		144, 801,

	};

	collider1.add(App->physics->CreateChain(0, 0, background2, 44, b2_staticBody));

	int background3[56] = {
	66, 353,
	41, 295,
	36, 245,
	45, 182,
	73, 125,
	126, 70,
	177, 50,
	196, 97,
	196, 142,
	114, 172,
	112, 190,
	124, 249,
	173, 358,
	153, 399,
	103, 318,
	87, 272,
	87, 223,
	100, 180,
	116, 145,
	138, 111,
	116, 89,
	75, 134,
	52, 183,
	45, 231,
	45, 279,
	56, 320,
	117, 429,
	112, 433
	};

	collider1.add(App->physics->CreateChain(0, 0, background3, 56, b2_staticBody));

	int background4[30] = {
	467, 798,
	470, 400,
	465, 320,
	413, 365,
	410, 455,
	420, 470,
	438, 476,
	450, 491,
	456, 507,
	459, 535,
	461, 767,
	425, 770,
	424, 703,
	298, 776,
	298, 807
	};

	collider1.add(App->physics->CreateChain(0, 0, background4, 30, b2_staticBody));

	int smallQuadL[8] = {
	225, 100,
	225, 132,
	234, 132,
	234, 100
	};

	collider1.add(App->physics->CreateChain(0, 0, smallQuadL, 8, b2_staticBody));

	int smallQuadR[8] = {
	264, 102,
	264, 133,
	273, 133,
	273, 102
	};

	collider1.add(App->physics->CreateChain(0, 0, smallQuadR, 8, b2_staticBody));

	int background5[22] = {
	453, 266,
	428, 206,
	403, 173,
	304, 142,
	306, 94,
	317, 96,
	353, 116,
	375, 134,
	396, 153,
	431, 204,
	452, 256
	};

	collider1.add(App->physics->CreateChain(0, 0, background5, 22, b2_staticBody));

	int LeftSupport[24] = {
	88, 638,
	104, 655,
	153, 686,
	145, 690,
	145, 703,
	91, 670,
	79, 649,
	77, 627,
	75, 554,
	80, 549,
	83, 553,
	84, 616
	};

	collider1.add(App->physics->CreateChain(0, 0, LeftSupport, 24, b2_staticBody));


	int RightSupport[32] = {
	354, 706,
	408, 672,
	417, 659,
	422, 640,
	425, 610,
	423, 551,
	416, 553,
	416, 604,
	415, 626,
	410, 646,
	382, 663,
	368, 671,
	343, 687,
	352, 689,
	355, 695,
	357, 699
	};

	collider1.add(App->physics->CreateChain(0, 0, RightSupport, 32, b2_staticBody));

}

void ModuleSceneMain::CreateBumpers()
{
	//Bumper balls
	bumper.bumperBall[0] = App->physics->CreateCircle(208, 219, 24, b2_staticBody);
	bumper.bumperBall[1] = App->physics->CreateCircle(277, 192, 24, b2_staticBody);
	bumper.bumperBall[2] = App->physics->CreateCircle(289, 265, 24, b2_staticBody);
	bumper.bumperBall[3] = App->physics->CreateCircle(202, 362, 24, b2_staticBody);

	//Big bumpers
	int leftBumpCoords[20] = {
		0, 77,
		0, 5,
		3, 1,
		7, 1,
		10, 5,
		47, 92,
		47, 98,
		44, 101,
		40, 101,
		4, 81
	};
	App->physics->CreateChain(119, 547, leftBumpCoords, 20, b2_staticBody); //Bumper creation
	

	bumper.bigBumpLeft = App->physics->CreateRectangleSensor(146, 595, 10, 85);
	bumper.bigBumpLeft->body->SetTransform(bumper.bigBumpLeft->body->GetPosition(), DEGTORAD * -23); //Sensor for the bumper creation and rotation

	int rightBumpCoords[20] = {
		0, 93,
		36, 6,
		40, 1,
		43, 1,
		45, 4,
		48, 74,
		46, 79,
		8, 100,
		4, 100,
		1, 98
	};
	App->physics->CreateChain(333, 547, rightBumpCoords, 20, b2_staticBody); //Bumper creation

	bumper.bigBumpRight = App->physics->CreateRectangleSensor(355, 595, 10, 85);
	bumper.bigBumpRight->body->SetTransform(bumper.bigBumpRight->body->GetPosition(), DEGTORAD * 23); //Sensor for the bumper creation and rotation

	int rightUpBumpCoords[26] = {
		0, 8,
		0, 3,
		3, 1,
		7, 2,
		70, 75,
		69, 82,
		63, 83,
		58, 85,
		54, 88,
		51, 92,
		48, 97,
		44, 97,
		3, 15
	};
	bumper.bigBumpLeftUp = App->physics->CreateChain(132, 257, rightUpBumpCoords, 26, b2_staticBody); //Bumper creation

}

void ModuleSceneMain::CreateSensors()
{
	//Death barrier
	sensor.deathSensor = App->physics->CreateRectangleSensor(0, 850, 1200, 50);

	//Balls
	sensor.ballSensor[0] = App->physics->CreateCircleSensor(138, 183, 11);
	sensor.ballSensor[1] = App->physics->CreateCircleSensor(163, 175, 11);
	sensor.ballSensor[2] = App->physics->CreateCircleSensor(189, 166, 11);
	sensor.ballSensor[3] = App->physics->CreateCircleSensor(316, 166, 11);
	sensor.ballSensor[4] = App->physics->CreateCircleSensor(341, 175, 11);
	sensor.ballSensor[5] = App->physics->CreateCircleSensor(366, 183, 11);
																	   
	sensor.ballSensor[6] = App->physics->CreateCircleSensor(270, 322, 11);
	sensor.ballSensor[7] = App->physics->CreateCircleSensor(295, 313, 11);
	sensor.ballSensor[8] = App->physics->CreateCircleSensor(320, 306, 11);

	sensor.ballSensor[9] = App->physics->CreateCircleSensor(391, 369, 11);
	sensor.ballSensor[10] = App->physics->CreateCircleSensor(391, 395, 11);
	sensor.ballSensor[11] = App->physics->CreateCircleSensor(391, 421, 11);

	sensor.ballSensor[12] = App->physics->CreateCircleSensor(101, 606, 11);
	sensor.ballSensor[13] = App->physics->CreateCircleSensor(400, 606, 11);

	//Star sensors
	sensor.starSensor[0] = App->physics->CreateRectangleSensor(212, 117, 23, 23);
	sensor.starSensor[1] = App->physics->CreateRectangleSensor(250, 117, 23, 23);
	sensor.starSensor[2] = App->physics->CreateRectangleSensor(288, 117, 23, 23);

	//Lift up sensors
	sensor.liftUpSensor[0] = App->physics->CreateRectangleSensor(61, 746, 30, 30);
	sensor.liftUpSensor[1] = App->physics->CreateRectangleSensor(442, 746, 30, 30);

	//Stair sensor
	sensor.stairsSensor = App->physics->CreateCircleSensor(107, 125, 20);

}



void ModuleSceneMain::ChargeScore()
{
	scoreLabel = App->gui->CreateLabel({ 150,40 }, "000000", App->gui->GetFont(FONT), { 255,255,255,255 }, this);
	scoreCharged = true;
}

void ModuleSceneMain::ChangeLifeCount()
{
	if (App->player->lives == 2)
		lifeCount->ChangeImage(lifeCountTex2);
	else if (App->player->lives == 1)
		lifeCount->ChangeImage(lifeCountTex1);

	hasLifeCountChanged = false;
}

void ModuleSceneMain::ChangeScoreLabel()
{
	if (score < 100) 
		scoreLabel->ChangeText((p2SString("0000%i", (score))));
	else if (score < 1000)
		scoreLabel->ChangeText((p2SString("000%i", (score))));
	else if (score < 10000)
		scoreLabel->ChangeText((p2SString("00%i", (score))));
	else if (score < 100000)
		scoreLabel->ChangeText((p2SString("0%i", (score))));
	else if (score < 1000000)
		scoreLabel->ChangeText((p2SString("%i", (score))));
	else if (score > 999999)
		scoreLabel->ChangeText((p2SString("%i", (999999))));
}

void ModuleSceneMain::SmallBumpCollisionInteraction(int bumpNum, PhysBody* ball)
{
	//Play sound
	int randSound = rand() % 2;
	if(randSound == 0)
		App->audio->PlayFx(App->audio->GetFX().smallBumper1);
	else if(randSound == 1)
		App->audio->PlayFx(App->audio->GetFX().smallBumper2);

	//Animation set up
	animation = &left_bouncer;

	//Bump
	int rng = rand() % 5;

	b2Vec2 norm_vec = ball->body->GetLinearVelocity();
	norm_vec.Normalize();

	float impulse = 0.0f;

	switch (rng) {
	case 0:
		impulse = 0.5f;
		break;
	case 1:
		impulse = 0.7f;
		break;
	case 2:
		impulse = 1.0f;
		break;
	case 3:
		impulse = 1.3f;
		break;
	case 4:
		impulse = 2.0f;
		break;
	}
	ball->body->ApplyLinearImpulse({ -norm_vec.x * impulse,-norm_vec.y * impulse }, { 0.0f,0.0f }, true);

	//Change score
	score += 30;
	ChangeScoreLabel();
}

void ModuleSceneMain::BigBumpCollisionInteraction(BigBumpType bumpType)
{
	score += 50;
	App->audio->PlayFx(App->audio->GetFX().bigBumper);
	ChangeScoreLabel();
}

void ModuleSceneMain::SensorsCollisionInteraction(SensorType sensorType, int sensorNum)
{
	switch (sensorType) {

	case BALL:
		if (!sensor.isBallSensorActive[sensorNum]) {
			//Sound and score change
			App->audio->PlayFx(App->audio->GetFX().lightBallSensor);
			score += 10;

			sensor.isBallSensorActive[sensorNum] = true;
		}

		break;
	case STAR:
		if (!sensor.isStarSensorActive[sensorNum]) {
			//Sound and score change
			App->audio->PlayFx(App->audio->GetFX().lightStarSensor);
			score += 20;

			sensor.isStarSensorActive[sensorNum] = true;
		}
		break;
	default:
		break;
	}

	ChangeScoreLabel();

}

void ModuleSceneMain::PrintActiveSensors()
{
	//Ball sensors
	for (int i = 0; i < 14; i++) {
		if (sensor.isBallSensorActive[i]){}
			//Blit
	}

	//Star sensors
	for (int i = 0; i < 3; i++) {
		if (sensor.isStarSensorActive[i]){}
			//Blit
	}
}

void ModuleSceneMain::resetSensors()
{
	//Ball sensors
	for (int i = 0; i < 14; i++)
		sensor.isBallSensorActive[i] = false;
	//Star sensors
	for (int i = 0; i < 3; i++)
		sensor.isStarSensorActive[i] = false;
}

uint ModuleSceneMain::GetScore()
{
	return score;
}


void ModuleSceneMain::UpdateAnimationBumpers() 
{

	/*if (AnimExe.BumperBall1 != NULL) {
		
		SDL_Rect r = AnimExe.BumperBall1->GetCurrentFrame();
		App->renderer->Blit(spritesheet, 118, 547, &r);
		
		if (AnimExe.BumperBall1->Finished()) {
			AnimExe.BumperBall1 = NULL;
		}
	}

	if (AnimExe.BumperBall2 != NULL) {

		SDL_Rect r = AnimExe.BumperBall2->GetCurrentFrame();
		App->renderer->Blit(spritesheet, 334, 547, &r);

		if (AnimExe.BumperBall2->Finished()) {
			AnimExe.BumperBall2 = NULL;
		}
	}


	if (AnimExe.BumperBall3 != NULL) {

		SDL_Rect r = AnimExe.BumperBall3->GetCurrentFrame();
		App->renderer->Blit(spritesheet, 118, 547, &r);

		if (AnimExe.BumperBall3->Finished()) {
			AnimExe.BumperBall3 = NULL;
		}
	}


	if (AnimExe.BumperClock != NULL) {

		SDL_Rect r = AnimExe.BumperClock->GetCurrentFrame();
		App->renderer->Blit(spritesheet, 118, 547, &r);

		if (AnimExe.BumperClock->Finished()) {
			AnimExe.BumperClock = NULL;
		}
	}


	if (AnimExe.BumperUp != NULL) {

		SDL_Rect r = AnimExe.BumperUp->GetCurrentFrame();
		App->renderer->Blit(spritesheet, 118, 547, &r);

		if (AnimExe.BumperUp->Finished()) {
			AnimExe.BumperUp = NULL;
		}
	}
	*/

	if (AnimExe.BumperDown1 != NULL) {

		SDL_Rect r = AnimExe.BumperDown1->GetCurrentFrame();
		App->renderer->Blit(spritesheet, 118, 547, &r);

		if (AnimExe.BumperDown1->Finished()) {
			AnimExe.BumperDown1 = NULL;
		}
	}


	if (AnimExe.BumperDown2 != NULL) {

		SDL_Rect r = AnimExe.BumperDown2->GetCurrentFrame();
		App->renderer->Blit(spritesheet, 334, 547, &r);

		if (AnimExe.BumperDown2->Finished()) {
			AnimExe.BumperDown2 = NULL;
		}
	}

}

