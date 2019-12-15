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
public:
	Cube* top_constraint;
	Primitive* furniture;
	
	/*PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	PhysBody3D* sensor_pb;
	Sphere Sensor;*/

	void AddPrimitive(Primitive* p);											//Adds a primitive to the primitives array.
	//void DeletePrimitive();
	void DeletePrimitive(Primitive* p);

	void CameraMovement();
	float GetZoom() const;														//Gets the amount of zoom required taking into account the distance between players (ratio).
	float GetLerpSpeed(vec3 position, vec3 target, float speed) const;			//Gets the lerp speed for the camera according to the position of the camera and its target position.
	void LerpCamera(vec3 cameraPosition, vec3 targetPosition, float speed);		//Method that changes the camera's poisition according to the lerp speed received from GetLerpSpeed().

	void LoadArena();															//
	void RestartGame();

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

	p2DynArray<Primitive*> primitives;
	p2DynArray<Primitive*> arena_elements;
};
