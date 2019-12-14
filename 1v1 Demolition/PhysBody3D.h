#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2DynArray.h"
#include "p2List.h"
#include "glmath.h"

class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class Module;

class Sphere;
class Cube;
class Cylinder;
class Plane;

class Primitive;

class PhysBody3D
{
public:
	PhysBody3D();
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void SetBody(Sphere* primitive, float mass, bool is_sensor = false, bool is_environment = false);
	void SetBody(Cube* primitive, vec3 size, float mass, bool is_sensor = false, bool is_environment = false);
	void SetBody(Cylinder* primitive, float depth, float mass, bool is_sensor = false, bool is_environment = false);
	bool HasBody() const;
	btRigidBody* GetBody() const;
	void SetAsSensor(bool is_sensor);

	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void ResetTransform();

	void SetPos(float x, float y, float z);
	void SetPos(vec3 position);
	vec3 GetPos() const;

	void SetSpeed(vec3 speed);
	void Push(vec3 force);
	void Stop();

	float DistanceFromWorldOrigin(vec3 bodyPos) const;
	float DistanceBetweenBodies(vec3 bodyPos) const;

private:
	void SetBody(btCollisionShape* shape, Primitive* parent, float mass, bool is_sensor = false, bool is_environment = false);

	btRigidBody* body;
	btCollisionShape* colShape;
	btDefaultMotionState* motionState;

public:
	Primitive* parentPrimitive;
	p2List<Module*> collision_listeners;

	bool is_sensor;										//Bool that keeps track whether a physBody is a sensor or not.
	bool is_environment;								//Bool that keeps track whether a physBody is an environment element.
};

#endif // __PhysBody3D_H__