#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

class Primitive;
class PhysBody3D;
class PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;
	void SpawnThrowableItem(Primitive* p);								//As the listener registers the module from where the spheres were created,
																		//to put this method in ModulePlayer/2 a new  OnCollision method would need to be created there.
public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	void DebugSpawnPrimitive(Primitive* p);					//Temporal. Just to check that the vehicle input work.

	float GetZoom() const;													//Gets the amount of zoom required taking into account the distance between players (ratio).
	float GetLerpSpeed(vec3 position, vec3 target, float speed) const;		//Gets the lerp speed for the camera according to the position of the camera and its target position.
	void LerpCamera(vec3 cameraPosition, vec3 targetPosition);				//Method that changes the camera's poisition according to the lerp speed received from GetLerpSpeed().

	p2DynArray<Primitive*> primitives;						//Temporal. Just to check that the  vehicle throw item method works. Maybe create an AddPrimitive method.

private:
	void HandleDebugInput();
	//void DebugSpawnPrimitive(Primitive* p);

	//p2DynArray<Primitive*> primitives;
};
