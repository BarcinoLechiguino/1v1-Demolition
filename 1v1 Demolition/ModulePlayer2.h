#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

//#define MAX_ACCELERATION 1000.0f
//#define TURN_DEGREES 25.0f * DEGTORAD
//#define BRAKE_POWER 1000.0f

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer2();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	void SpawnThrowableItem(Primitive* p);
	void RestartPlayer2(vec3 respawnPosition);
	void GenerateP2Vehicle();
	void DriveInputsP2();

public:

	PhysVehicle3D* P2vehicle;
	float turn;
	float acceleration;
	float brake;

	btRigidBody* prevCollBody;					//Keeps track of the previous body that the vehicle collided with.
	uint lives;
	bool alive;

	float scale;
};