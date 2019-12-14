#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"		//REVISE THIS

#define MAX_BODIES 3

struct PhysVehicle3D;
struct VehicleInfo;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;
	void LoadAudioP1();													//Loads all player relevant audio.

	void SpawnThrowableItem(Primitive* p);
	void RestartPlayer1(vec3 respawnPosition);
	void GenerateP1Vehicle();
	void DriveInputsP1();
	void SpecialInputsP1();
	void CheckLivesP1();

public:

	VehicleInfo car;
	PhysVehicle3D* P1vehicle;
	PhysVehicle3D* vehicle2;
	float turn;
	float acceleration;
	float brake;

	PhysBody3D* prevCollBody[MAX_BODIES];				//Keeps track of up to 30 of the bodies that the vehicle collided with previously.
	vec3 spawnPoint;									//vec3 that has the first spawn position in the map recorded down.
	uint lives;
	bool alive;
	float scale;
};