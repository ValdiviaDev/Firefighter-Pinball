#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = false);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	//Shapes and sensors
	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType bodyType);
	PhysBody* CreateCircleSensor(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType bodyType);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType bodyType);
	PhysBody* CreateShape(int x, int y, b2Vec2 points[], int size, b2BodyType bodyType);

	//Joints
	b2RevoluteJoint* CreateRevoluteJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 anchorA, b2Vec2 anchorB, float lowerAngle, float upperAngle, bool limitEnabled);
	b2DistanceJoint* CreateDistanceJoint(b2Body* bodyA, b2Body* bodyB, float frequency, float dampRatio);
	b2PrismaticJoint* CreatePrismaticJoint(b2Body* bodyA, b2Body* bodyB);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	b2World* world;

private:
	bool debug;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};