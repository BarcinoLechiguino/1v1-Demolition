#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"		//REVISE THIS

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

	void SetVehicleRotationP1(float angle, const vec3& u);				//Rotates the vehicle according to an angle and an axis.
	
	void SpawnProjectile(Primitive* p);									//Spawns a projectile from the vehicle's position.
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
	float originalAngle;								//Amount of degrees that P1's vehicle will be rotated.
	vec3 axis;											//Axis in which P1's vehicle will be rotated.
	uint lives;											//Keeps track of the amount of lives P1 has left.
	bool alive;											//Keeps track whether P1 is alive or not.
	uint ammo;											//Amount of times P1 can throw items.
	bool loaded;										//Keeps track whether or not the player has recharged ammo.
	bool firstTurbo;									//Keeps track whether or not its the first time that P1 has used the turbo.

	uint roundsWonP1;									//Keeps track of the amount of rounds P1 has won.
	uint winsP1;										//Keeps track of the amount of wins that P1 has.

	float scale;										//Scaling factor of P1's vehicle
};