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

	bumper = CreateBumpers({330.0f,640.0f});

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

	float angle = bumper->GetJointAngle();
	if (angle < (-45 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) {
		bumper->GetBodyA()->ApplyForce({ 0.0f, 370.0f }, { 0.0f, 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		bumper->GetBodyA()->ApplyForce({ 0.0f, -100.0f }, { 0.0f, 0.0f }, true);
	}

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

b2RevoluteJoint * ModulePlayer::CreateBumpers(b2Vec2 pos)
{
	PhysBody* rect1 = App->physics->CreateRectangle(pos.x, pos.y, 140, 48, b2_dynamicBody); //Horizontal
	PhysBody* rect0 = App->physics->CreateRectangle(pos.x, pos.y, 48, 100, b2_staticBody); //Vertical

	b2Vec2 setA = rect1->body->GetLocalCenter();

	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = rect1->body;
	revoluteJointDef.bodyB = rect0->body;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA = setA;
	revoluteJointDef.localAnchorB.Set(-0.50f, 0.50f);
	revoluteJointDef.lowerAngle = -45 * DEGTORAD;
	revoluteJointDef.upperAngle = 0 * DEGTORAD;
	revoluteJointDef.enableLimit = true;
	b2RevoluteJoint* Revloute_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef);

	return Revloute_joint;
}





