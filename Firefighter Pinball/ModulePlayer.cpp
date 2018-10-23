#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleSceneMain.h"

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

	//Create ball
	ball = CreateBall();

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
	if(ball != nullptr)
		PrintBall();

	UpdateFlippers();
	UpdateSpring();

	if (resetBall)
		ResetBall();

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	//Unload textures
	App->textures->Unload(ballTex);
	App->textures->Unload(flipLeftTex);
	App->textures->Unload(flipRightTex);
	App->textures->Unload(springTex);
	return true;
}

void ModulePlayer::ChargeTextures()
{
	ballTex = App->textures->Load("assets/textures/ball.png");
	flipLeftTex = App->textures->Load("assets/textures/flipperLeft.png");
	flipRightTex = App->textures->Load("assets/textures/flipperRight.png");
	springTex = App->textures->Load("assets/textures/spring.png");
}

PhysBody * ModulePlayer::CreateBall()
{
	ballInitialPos = { 495,750 };
	PhysBody* ballPB = App->physics->CreateCircle(ballInitialPos.x, ballInitialPos.y, 12, b2_dynamicBody);
	ballPB->listener = (Module*)App->scene_main;
	return ballPB;
}

PhysBody* ModulePlayer::GetBall()
{
	return ball;
}

void ModulePlayer::ResetBall()
{
	if (ball->body != nullptr) {
		App->physics->world->DestroyBody(ball->body);
		ball = nullptr;
	}

	lives--;
	App->scene_main->hasLifeCountChanged = true;
	//Play death sound
	App->audio->PlayFx(App->audio->GetFX().deathSound);

	if (lives > 0)
		ball = CreateBall();

	//Reset sensors for the ball to sense again
	App->scene_main->ResetSensors();

	resetBall = false;
}

b2RevoluteJoint * ModulePlayer::CreateFlipper(b2Vec2 pos, FlipperType flipperType)
{
	//Charge Data
	b2Vec2 flipperPoints[7];
	b2Vec2 anchorA = { 0.0f,0.0f };
	float lowerAngle = 0.0f;
	float upperAngle = 0.0f;

	ChargeFlipperData(flipperType, flipperPoints, anchorA, lowerAngle, upperAngle);

	//Create bodies
	PhysBody* flip = App->physics->CreateShape(pos.x, pos.y, flipperPoints, 7, b2_dynamicBody);
	PhysBody* circ = App->physics->CreateCircle(pos.x, pos.y, 6, b2_staticBody);

	b2Vec2 anchorB = circ->body->GetLocalCenter();

	//Create revolute joint
	b2RevoluteJoint* revoluteJoint = App->physics->CreateRevoluteJoint(flip->body, circ->body, anchorA, anchorB, lowerAngle, upperAngle, true);

	return revoluteJoint;
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
	//Create bodies
	PhysBody* springBox = App->physics->CreateRectangle(anchorPos.x, anchorPos.y - 50, 38, 15, b2_dynamicBody); //Up
	PhysBody* anchor = App->physics->CreateRectangle(anchorPos.x, anchorPos.y, 40, 5, b2_staticBody); //Down

	//Create pertinent joints
	b2DistanceJoint* distJoint = App->physics->CreateDistanceJoint(springBox->body, anchor->body, 3.0f, 0.3f);
	b2PrismaticJoint* prismJoint = App->physics->CreatePrismaticJoint(springBox->body, anchor->body);

	return springBox;
}

void ModulePlayer::UpdateFlippers()
{
	//Functionality
	//Left
	float angleLeft = flipperLeft->GetJointAngle();
	if (angleLeft < (45 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_LEFT) != KEY_REPEAT) {
		flipperLeft->GetBodyA()->ApplyForce({ 0.0f, 30.0f }, { PIXEL_TO_METERS(237), 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		flipperLeft->GetBodyA()->ApplyForce({ 0.0f, -60.0f }, { PIXEL_TO_METERS(237), 0.0f }, true);
	}

	//Right
	float angleRight = flipperRight->GetJointAngle();
	if (angleRight < (-45 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT) {
		flipperRight->GetBodyA()->ApplyForce({ 0.0f, 10.0f }, { 0.0f, 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		flipperRight->GetBodyA()->ApplyForce({ 0.0f, -20.0f }, { 0.0f, 0.0f }, true);
	}

	//Right Up
	float angleRightUp = flipperRightUp->GetJointAngle();
	if (angleRightUp < (-25 * DEGTORAD) && App->input->GetKey(SDL_SCANCODE_RIGHT) != KEY_REPEAT) {
		flipperRightUp->GetBodyA()->ApplyForce({ 0.0f, 10.0f }, { 0.0f, 0.0f }, true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		flipperRightUp->GetBodyA()->ApplyForce({ 0.0f, -20.0f }, { 0.0f, 0.0f }, true);
	}

	//Print
	PrintFlippers();

	//Sound
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		App->audio->PlayFx(App->audio->GetFX().flipperUp, 0);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		App->audio->PlayFx(App->audio->GetFX().flipperDown, 0);

}

void ModulePlayer::UpdateSpring()
{
	//Functionality
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {

		if (springImpulse.y < 100.0f)
			springImpulse.y += 2.0f;

		spring->body->ApplyForce(springImpulse, { PIXEL_TO_METERS(490), 0.0f }, true);
	}

	//Sound
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		App->audio->PlayFx(App->audio->GetFX().springDown);
	}

	//Sound and functionality
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		App->audio->PlayFx(App->audio->GetFX().springLaunch);

		spring->body->ApplyForce({ 0.0f,-springImpulse.y * 5.0f }, { PIXEL_TO_METERS(490), 0.0f }, true);
		springImpulse = { 0.0f,0.0f };
	}

	//Print spring
	PrintSpring();

}

void ModulePlayer::PrintBall()
{
	iPoint ballPos = { 0,0 };
	ball->GetPosition(ballPos.x, ballPos.y);

	App->renderer->Blit(ballTex, ballPos.x, ballPos.y, NULL);
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





