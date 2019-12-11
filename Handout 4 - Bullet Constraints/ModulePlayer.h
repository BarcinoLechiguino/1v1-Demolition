#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1500.0f
#define TURN_DEGREES 20.0f * DEGTORAD
#define BRAKE_POWER 1500.0f
#define IDENTITY mat4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SpawnThrowableItem(Primitive* p);

public:

	PhysVehicle3D* vehicle;
	PhysVehicle3D* vehicle2;
	float turn;
	float acceleration;
	float brake;
	vec3 position = { 0.0f,0.0f,0.0f };
	vec3 startPos = { 0.0f,200.0f,0.0f };
};