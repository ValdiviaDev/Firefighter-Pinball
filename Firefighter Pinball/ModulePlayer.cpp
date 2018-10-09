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

	flipperRight = CreateFlipper({340.0f,705.0f}, FLIP_RIGHT);
	spring = CreateSpring();

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	UpdateFlippers();
	UpdateSpring();

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

b2RevoluteJoint * ModulePlayer::CreateFlipper(b2Vec2 pos, FlipperType flipperType)
{
	//Charge Data
	b2Vec2 flipperPoints[7];
	b2Vec2 anchorA = { 0.0f,0.0f };
	float lowerAngle = 0.0f;
	float higherAngle = 0.0f;

	ChargeFlipperData(flipperType, flipperPoints, anchorA, lowerAngle, higherAngle);

	//Define bodies
	PhysBody* flip = App->physics->CreateShape(pos.x, pos.y, flipperPoints, 7, b2_dynamicBody);//Horizontal
	PhysBody* circ = App->physics->CreateCircle(pos.x, pos.y, 6, b2_staticBody);

	b2Vec2 setB = circ->body->GetLocalCenter();

	//Make revolute joint
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = flip->body;
	revoluteJointDef.bodyB = circ->body;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA = anchorA;
	revoluteJointDef.localAnchorB = setB;
	revoluteJointDef.lowerAngle = lowerAngle;
	revoluteJointDef.upperAngle = higherAngle;
	revoluteJointDef.enableLimit = true;
	b2RevoluteJoint* Revloute_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef);

	return Revloute_joint;
}

void ModulePlayer::ChargeFlipperData(FlipperType flipperType, b2Vec2 flipperPoints[7], b2Vec2& anchorA, float& lowerAngle, float& higherAngle)
{
	switch (flipperType)
	{
	case FLIP_LEFT:
		break;
	case FLIP_RIGHT:
		flipperPoints[0].Set(PIXEL_TO_METERS(1), PIXEL_TO_METERS(36));
		flipperPoints[1].Set(PIXEL_TO_METERS(78), PIXEL_TO_METERS(-3));
		flipperPoints[2].Set(PIXEL_TO_METERS(88), PIXEL_TO_METERS(3));
		flipperPoints[3].Set(PIXEL_TO_METERS(91), PIXEL_TO_METERS(11));
		flipperPoints[4].Set(PIXEL_TO_METERS(86), PIXEL_TO_METERS(21));
		flipperPoints[5].Set(PIXEL_TO_METERS(34), PIXEL_TO_METERS(40));
		flipperPoints[6].Set(PIXEL_TO_METERS(4), PIXEL_TO_METERS(40));

		anchorA = { PIXEL_TO_METERS(73.0f), PIXEL_TO_METERS(12.0f) };
		lowerAngle = (-45 * DEGTORAD);
		higherAngle = (0 * DEGTORAD);
		break;
	case FLIP_RIGHT_UP:
		break;
	default:
		flipperPoints[0].Set(PIXEL_TO_METERS(1), PIXEL_TO_METERS(36));
		flipperPoints[1].Set(PIXEL_TO_METERS(78), PIXEL_TO_METERS(-3));
		flipperPoints[2].Set(PIXEL_TO_METERS(88), PIXEL_TO_METERS(3));
		flipperPoints[3].Set(PIXEL_TO_METERS(91), PIXEL_TO_METERS(11));
		flipperPoints[4].Set(PIXEL_TO_METERS(86), PIXEL_TO_METERS(21));
		flipperPoints[5].Set(PIXEL_TO_METERS(34), PIXEL_TO_METERS(40));
		flipperPoints[6].Set(PIXEL_TO_METERS(4), PIXEL_TO_METERS(40));

		anchorA = { PIXEL_TO_METERS(73.0f), PIXEL_TO_METERS(12.0f) };
		lowerAngle = (-45 * DEGTORAD);
		higherAngle = (0 * DEGTORAD);
		break;
	}
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

void ModulePlayer::UpdateFlippers()
{
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
	//
	//	flipper->GetBodyA()->ApplyForce({ 0.0f, -400.0f }, { 0.0f, 0.0f }, true);
	//}

	//Functionality
	float angle = flipperRight->GetJointAngle();
	if (angle < (-45 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) {
		flipperRight->GetBodyA()->ApplyForce({ 0.0f, 100.0f }, { 0.0f, 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		flipperRight->GetBodyA()->ApplyForce({ 0.0f, -100.0f }, { 0.0f, 0.0f }, true);
	}

	//Sound
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		App->audio->PlayFx(App->audio->GetFX().flipperUp, 0);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		App->audio->PlayFx(App->audio->GetFX().flipperDown, 0);

}

void ModulePlayer::UpdateSpring()
{
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {

		if (springImpulse.y < 300.0f)
			springImpulse.y += 20.0f;

		spring->GetBodyA()->ApplyForce(springImpulse, { PIXEL_TO_METERS(490), 0.0f }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		springImpulse = { 0.0f,0.0f };
	}
}





