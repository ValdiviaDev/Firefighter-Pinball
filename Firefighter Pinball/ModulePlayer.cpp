#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
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

	//Charge Textures
	ChargeTextures();

	//Create flippers
	flipperLeft = CreateFlipper({ 162.0f,702.0f }, FLIP_LEFT);
	flipperRight = CreateFlipper({332.0f,704.0f}, FLIP_RIGHT);
	flipperRightUp = CreateFlipper({ 455.0f,297.0f }, FLIP_RIGHT_UP);

	//Create spring
	spring = CreateSpring({ 495, 802 });


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
	//Unload textures
	App->textures->Unload(flipLeftTex);
	App->textures->Unload(flipRightTex);
	App->textures->Unload(springTex);
	return true;
}

void ModulePlayer::ChargeTextures()
{
	flipLeftTex = App->textures->Load("assets/textures/flipperLeft.png");
	flipRightTex = App->textures->Load("assets/textures/flipperRight.png");
	springTex = App->textures->Load("assets/textures/spring.png");
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
		flipperPoints[0].Set(PIXEL_TO_METERS(86), PIXEL_TO_METERS(37));
		flipperPoints[1].Set(PIXEL_TO_METERS(13), PIXEL_TO_METERS(0));
		flipperPoints[2].Set(PIXEL_TO_METERS(4), PIXEL_TO_METERS(1));
		flipperPoints[3].Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(8));
		flipperPoints[4].Set(PIXEL_TO_METERS(2), PIXEL_TO_METERS(18));
		flipperPoints[5].Set(PIXEL_TO_METERS(48), PIXEL_TO_METERS(37));
		flipperPoints[6].Set(PIXEL_TO_METERS(84), PIXEL_TO_METERS(40));

		anchorA = { PIXEL_TO_METERS(13.0f), PIXEL_TO_METERS(12.0f) };
		lowerAngle = (0 * DEGTORAD);
		higherAngle = (45 * DEGTORAD);
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
		flipperPoints[0].Set(PIXEL_TO_METERS(1), PIXEL_TO_METERS(36));
		flipperPoints[1].Set(PIXEL_TO_METERS(78), PIXEL_TO_METERS(-3));
		flipperPoints[2].Set(PIXEL_TO_METERS(88), PIXEL_TO_METERS(3));
		flipperPoints[3].Set(PIXEL_TO_METERS(91), PIXEL_TO_METERS(11));
		flipperPoints[4].Set(PIXEL_TO_METERS(86), PIXEL_TO_METERS(21));
		flipperPoints[5].Set(PIXEL_TO_METERS(34), PIXEL_TO_METERS(40));
		flipperPoints[6].Set(PIXEL_TO_METERS(4), PIXEL_TO_METERS(40));

		anchorA = { PIXEL_TO_METERS(73.0f), PIXEL_TO_METERS(12.0f) };
		lowerAngle = (-25 * DEGTORAD);
		higherAngle = (20 * DEGTORAD);
		break;
	default:
		//Right flipper
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

PhysBody * ModulePlayer::CreateSpring(b2Vec2 anchorPos)
{
	PhysBody* springBox = App->physics->CreateRectangle(anchorPos.x, anchorPos.y - 50, 38, 10, b2_dynamicBody); //Up
	PhysBody* anchor = App->physics->CreateRectangle(anchorPos.x, anchorPos.y, 40, 5, b2_staticBody); //Down

	b2DistanceJoint* distJoint = App->physics->CreateDistanceJoint(springBox->body, anchor->body, 3.0f, 0.3f);
	b2PrismaticJoint* prismJoint = App->physics->CreatePrismaticJoint(springBox->body, anchor->body);

	return springBox;
}

void ModulePlayer::UpdateFlippers()
{
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
	//
	//	flipper->GetBodyA()->ApplyForce({ 0.0f, -400.0f }, { 0.0f, 0.0f }, true);
	//}

	//Functionality
	//Left
	float angleLeft = flipperLeft->GetJointAngle();
	if (angleLeft < (45 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_Q) != KEY_REPEAT) {
		flipperLeft->GetBodyA()->ApplyForce({ 0.0f, 200.0f }, { PIXEL_TO_METERS(237), 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		flipperLeft->GetBodyA()->ApplyForce({ 0.0f, -100.0f }, { PIXEL_TO_METERS(237), 0.0f }, true);
	}

	//Right
	float angleRight = flipperRight->GetJointAngle();
	if (angleRight < (-45 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) {
		flipperRight->GetBodyA()->ApplyForce({ 0.0f, 100.0f }, { 0.0f, 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		flipperRight->GetBodyA()->ApplyForce({ 0.0f, -100.0f }, { 0.0f, 0.0f }, true);
	}

	//Right Up
	float angleRightUp = flipperRightUp->GetJointAngle();
	if (angleRightUp < (-25 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT) {
		flipperRightUp->GetBodyA()->ApplyForce({ 0.0f, 100.0f }, { 0.0f, 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		flipperRightUp->GetBodyA()->ApplyForce({ 0.0f, -100.0f }, { 0.0f, 0.0f }, true);
	}

	//Print
	PrintFlippers();

	//Sound
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		App->audio->PlayFx(App->audio->GetFX().flipperUp, 0);

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_UP || App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		App->audio->PlayFx(App->audio->GetFX().flipperDown, 0);

}

void ModulePlayer::UpdateSpring()
{
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {

		if (springImpulse.y < 100.0f)
			springImpulse.y += 2.0f;

		spring->body->ApplyForce(springImpulse, { PIXEL_TO_METERS(490), 0.0f }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		spring->body->ApplyForce({ 0.0f,-springImpulse.y * 5.0f }, { PIXEL_TO_METERS(490), 0.0f }, true);
		springImpulse = { 0.0f,0.0f };
	}

	//Print spring
	PrintSpring();

}

void ModulePlayer::PrintFlippers()
{
	//Left
	PhysBody* flipLeftPB = (PhysBody*)flipperLeft->GetBodyA()->GetUserData();
	iPoint pos = { 0, 0 };
	flipLeftPB->GetPosition(pos.x, pos.y);

	App->renderer->Blit(flipLeftTex, pos.x, pos.y, NULL, 1.0f, flipLeftPB->GetRotation(), 0, 0);

	//Right
	PhysBody* flipRightPB = (PhysBody*)flipperRight->GetBodyA()->GetUserData();
	pos = { 0, 0 };
	flipRightPB->GetPosition(pos.x, pos.y);

	App->renderer->Blit(flipRightTex, pos.x, pos.y, NULL, 1.0f, flipRightPB->GetRotation(), 0, 0);

	//Right Up
	PhysBody* flipRightUpPB = (PhysBody*)flipperRightUp->GetBodyA()->GetUserData();
	pos = { 0, 0 };
	flipRightUpPB->GetPosition(pos.x, pos.y);

	App->renderer->Blit(flipRightTex, pos.x, pos.y, NULL, 1.0f, flipRightUpPB->GetRotation(), 0, 0);
}

void ModulePlayer::PrintSpring()
{
	iPoint pos = { 0, 0 };
	spring->GetPosition(pos.x, pos.y);

	App->renderer->Blit(springTex, pos.x, pos.y, NULL);
}





