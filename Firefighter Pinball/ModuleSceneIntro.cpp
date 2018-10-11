#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleGui.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("assets/wheel.png"); 
	box = App->textures->Load("assets/crate.png");
	rick = App->textures->Load("assets/rick_head.png");
	bonus_fx = App->audio->LoadFx("assets/bonus.wav");

	//background = App->textures->Load("assets/textures/testScene.png");
	background = App->textures->Load("assets/textures/background.png");

	deathSensor = App->physics->CreateRectangleSensor(0, 850, 1200, 50);

	//Create the chains for the stage
	//CreateStage(stage);

	//Play stage music
	App->audio->PlayMusic("assets/audio/music/stageTheme.ogg");

	//Charge life count
	lifeCountTex3 = App->textures->Load("assets/textures/gui/lifeCount3.png");
	lifeCountTex2 = App->textures->Load("assets/textures/gui/lifeCount2.png");
	lifeCountTex1 = App->textures->Load("assets/textures/gui/lifeCount1.png");
	
	lifeCount = App->gui->CreateImage({ 10,10 }, { 0,0,255,76 }, lifeCountTex3, this);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background);
	App->textures->Unload(lifeCountTex3);
	App->textures->Unload(lifeCountTex2);
	App->textures->Unload(lifeCountTex1);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (!scoreCharged) {
		ChargeScore();
	}

	if (hasLifeCountChanged)
		ChangeLifeCount();

	App->renderer->Blit(background, 0, 0);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50, b2_dynamicBody));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64, b2_dynamicBody));
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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	if (bodyA == App->player->GetBall() && bodyB == deathSensor) {
		App->player->resetBall = true;
	}

}

void ModuleSceneIntro::CreateStage(PhysBody * stage)
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

void ModuleSceneIntro::ChargeScore()
{
	score = App->gui->CreateLabel({ 150,40 }, "000000", App->gui->GetFont(FONT), { 0,255,0,255 }, this);
	scoreCharged = true;
}

void ModuleSceneIntro::ChangeLifeCount()
{
	if (App->player->lives == 2)
		lifeCount->ChangeImage(lifeCountTex2);
	else if (App->player->lives == 1)
		lifeCount->ChangeImage(lifeCountTex1);

	hasLifeCountChanged = false;
}
