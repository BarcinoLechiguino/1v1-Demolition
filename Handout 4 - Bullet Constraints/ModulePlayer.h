#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#define MAX_BODIES 30

struct PhysVehicle3D;

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

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	void SpawnThrowableItem(Primitive* p);
	void RestartPlayer1(vec3 respawnPosition);
	void GenerateP1Vehicle();
	void DriveInputsP1();

public:

	PhysVehicle3D* P1vehicle;
	PhysVehicle3D* vehicle2;
	float turn;
	float acceleration;
	float brake;

	btRigidBody* prevCollBody;								//Keeps track of the previous body that the vehicle collided with.
	//btRigidBody* prevCollBody[MAX_BODIES];				//Keeps track of up to 30 of the bodies that the vehicle collided with previously.
	uint lives;
	bool alive;
};