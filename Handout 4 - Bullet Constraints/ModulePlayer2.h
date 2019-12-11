#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"   //Delete at some point (?).

struct PhysVehicle3D;
struct VehicleInfo;

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer2();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SpawnThrowableItem(Primitive* p);
	void RestartPlayer2(vec3 respawnPosition);

public:

	PhysVehicle3D* P2vehicle;
	float turn;
	float acceleration;
	float brake;

private:
	VehicleInfo car;
};