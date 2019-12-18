#pragma once

#include "PhysBody3D.h"
#include "Primitive.h"
#include "glmath.h"

class btQuaternion;
class btRaycastVehicle;
struct PhysBody3D;

struct Wheel
{
	vec3 connection;				// origin of the ray. Must come from within the chassis
	vec3 direction;
	vec3 axis;
	float suspensionRestLength;		// max length for suspension in meters
	float radius;
	float width;
	bool front;						// is front wheel ?
	bool drive;						// does this wheel receive engine power ?
	bool brake;						// do the breakes affect this wheel ?
	bool steering;					// does this wheel turn ?
};

struct VehicleInfo
{
	~VehicleInfo();

	// --- P1's VEHICLE PARTS
	vec3 chassis_size;
	vec3 chassis_offset;
	vec3 stabilizer_size;
	vec3 stabilizer_offset;
	vec3 cabin_size;
	vec3 cabin_offset;
	vec3 spoilerLeftPivot_size;
	vec3 spoilerLeftPivot_offset;
	vec3 spoilerRightPivot_size;
	vec3 spoilerRightPivot_offset;
	vec3 spoiler_size;
	vec3 spoiler_offset;
	vec3 frontBumper_size;
	vec3 frontBumper_offset;
	vec3 frontBumperPivot_size;
	vec3 frontBumperPivot_offset;
	vec3 rearBumper_size;
	vec3 rearBumper_offset;
	vec3 leftSkirt_size;
	vec3 leftSkirt_offset;
	vec3 rightSkirt_size;
	vec3 rightSkirt_offset;
	vec3 leftLight_size;
	vec3 leftLight_offset;
	vec3 rightLight_size;
	vec3 rightLight_offset;

	// --- P2's Vehicle Parts
	vec3 chassis2_size;
	vec3 chassis2_offset;
	vec3 stabilizer2_size;
	vec3 stabilizer2_offset;
	vec3 cabin2_size;
	vec3 cabin2_offset;
	vec3 spoilerLeftPivot2_size;
	vec3 spoilerLeftPivot2_offset;
	vec3 spoilerRightPivot2_size;
	vec3 spoilerRightPivot2_offset;
	vec3 spoiler2_size;
	vec3 spoiler2_offset;
	vec3 frontBumper2_size;
	vec3 frontBumper2_offset;
	vec3 frontBumperPivot2_size;
	vec3 frontBumperPivot2_offset;
	vec3 rearBumper2_size;
	vec3 rearBumper2_offset;
	vec3 leftSkirt2_size;
	vec3 leftSkirt2_offset;
	vec3 rightSkirt2_size;
	vec3 rightSkirt2_offset;
	vec3 leftLight2_size;
	vec3 leftLight2_offset;
	vec3 rightLight2_size;
	vec3 rightLight2_offset;

	// --- GENERAL VEHICLE PROPERTIES
	float mass;
	float suspensionStiffness;			// default to 5.88 / 10.0 offroad / 50.0 sports car / 200.0 F1 car
	float suspensionCompression;		// default to 0.83
	float suspensionDamping;			// default to 0.88 / 0..1 0 bounces / 1 rigid / recommended 0.1...0.3
	float maxSuspensionTravelCm;		// default to 500 cm suspension can be compressed
	float frictionSlip;					// defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
	float maxSuspensionForce;			// defaults to 6000 / max force to the chassis

	Wheel* wheels;
	int num_wheels;
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~PhysVehicle3D();

	void Render();
	Cube SetVehicleCubeElement(vec3 part, vec3 offset, Color partColor, btQuaternion rotQuat);
	void RenderVehicles(btQuaternion q);

	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;

public:

	VehicleInfo info;
	btRaycastVehicle* vehicle;
};