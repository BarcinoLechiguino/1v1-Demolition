#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"		//Delete later (?)

struct PhysVehicle3D;
struct VehicleInfo;

//#define MAX_ACCELERATION 1000.0f
//#define TURN_DEGREES 15.0f * DEGTORAD
//#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SpawnThrowableItem(Primitive* p);
	void RestartPlayer1(vec3 respawnPosition);

public:

	PhysVehicle3D* P1vehicle;
	PhysVehicle3D* vehicle2;
	float turn;
	float acceleration;
	float brake;

private:
	VehicleInfo car;
};