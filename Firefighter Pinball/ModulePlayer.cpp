#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	flipper = CreateFlipper({340.0f,705.0f});
	spring = CreateSpring();

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//Bumper (W)
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
	//
	//	bumper->GetBodyA()->ApplyForce({ 0.0f, -400.0f }, { 0.0f, 0.0f }, true);
	//}

	float angle = flipper->GetJointAngle();
	if (angle < (-45 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) {
		flipper->GetBodyA()->ApplyForce({ 0.0f, 370.0f }, { 0.0f, 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		flipper->GetBodyA()->ApplyForce({ 0.0f, -100.0f }, { 0.0f, 0.0f }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		App->audio->PlayFx(App->audio->GetFX().bumperUp, 0);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		App->audio->PlayFx(App->audio->GetFX().bumperDown, 0);


	//Spring (DOWN)
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {

		if (springImpulse.y < 300.0f)
			springImpulse.y += 20.0f;

		spring->GetBodyA()->ApplyForce(springImpulse, { PIXEL_TO_METERS(490), 0.0f }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		springImpulse = { 0.0f,0.0f };
	}

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

b2RevoluteJoint * ModulePlayer::CreateFlipper(b2Vec2 pos)
{
	b2Vec2 flipperPoints[7];
	flipperPoints[0].Set(119, 50);
	flipperPoints[1].Set(191, 12);
	flipperPoints[2].Set(198, 11);
	flipperPoints[3].Set(207, 20);
	flipperPoints[4].Set(207, 28);
	flipperPoints[5].Set(201, 33);
	flipperPoints[6].Set(156, 51);

	for (int i = 0; i < 8; i++) {
		flipperPoints[i].x = PIXEL_TO_METERS(flipperPoints[i].x);
		flipperPoints[i].y = PIXEL_TO_METERS(flipperPoints[i].y);
	}

	PhysBody* flip = App->physics->CreateShape(pos.x, pos.y, flipperPoints, 7, b2_dynamicBody);//Horizontal

	PhysBody* circ = App->physics->CreateCircle(pos.x, pos.y, 6, b2_staticBody);

	b2Vec2 setA = flip->body->GetLocalCenter();

	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = flip->body;
	revoluteJointDef.bodyB = circ->body;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA = setA;
	revoluteJointDef.localAnchorB.Set(-0.50f, 0.20f);
	revoluteJointDef.lowerAngle = -45 * DEGTORAD;
	revoluteJointDef.upperAngle = 0 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	b2RevoluteJoint* Revloute_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef);

	return Revloute_joint;
}

b2DistanceJoint * ModulePlayer::CreateSpring()
{
	PhysBody* upBody = App->physics->CreateRectangle(490, 800, 38, 73, b2_dynamicBody); //Up
	PhysBody* downBody = App->physics->CreateRectangle(490, 820, 40, 16, b2_staticBody); //Down

	//PhysBody* wallLeft = App->physics->CreateRectangle(460, 770, 10, 80, b2_staticBody);
	//PhysBody* wallRigh = App->physics->CreateRectangle(520, 770, 10, 80, b2_staticBody);

	b2DistanceJointDef distanceJointDef;
	//distanceJointDef.Initialize(upBody->body, downBody->body, downBody->body->GetLocalCenter(), upBody->body->GetLocalCenter());
	distanceJointDef.bodyA = upBody->body;
	distanceJointDef.bodyB = downBody->body;
	distanceJointDef.localAnchorA = upBody->body->GetLocalCenter();
	distanceJointDef.localAnchorB = downBody->body->GetLocalCenter();
	distanceJointDef.collideConnected = false;
	distanceJointDef.frequencyHz = 3.0f;
	distanceJointDef.dampingRatio = 0.3f;

	b2DistanceJoint* distJoint = (b2DistanceJoint*)App->physics->world->CreateJoint(&distanceJointDef);

	return distJoint;
}





