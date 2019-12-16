#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"

struct PhysVehicle3D;

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer2();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;
	void LoadAudioP2();

	void SetVehicleRotationP2(float angle, const vec3& u);				//Rotates the vehicle according to an angle and an axis.
	
	void SpawnProjectile(Primitive* p);
	void RestartPlayer2(vec3 respawnPosition);
	void GenerateP2Vehicle();
	void DriveInputsP2();
	void SpecialInputsP2();
	void CheckLivesP2();


public:
	VehicleInfo car;
	PhysVehicle3D* P2vehicle;
	float turn;
	float acceleration;
	float brake;

	PhysBody3D* prevCollBody[MAX_BODIES];				//Keeps track of up to 30 of the bodies that the vehicle collided with previously.
	vec3 spawnPoint;									//vec3 that has the first spawn position in the map recorded down.
	float originalAngle;								//Amount of degrees that P2's vehicle will be rotated.
	vec3 axis;											//Axis in which P2's vehicle will be rotated.
	uint lives;											//Lives of P2.
	bool alive;											//Keeps track whether P2 is alive or not.
	uint ammo;											//Amount of times P2 can throw items.
	bool loaded;										//Keeps track whether or not the player has recharged ammo.
	bool firstTurbo;									//Keeps track whether or not its the first time that P2 has used the turbo.

	uint roundsWonP2;									//Keeps track of the amount of rounds that P2 has won.
	uint winsP2;										//Keeps track of the amount of times P2 has won.

	float scale;										//Scaling factor of P2's vehicle.
};