#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "Primitive.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
class PhysBody3D;
struct PhysVehicle3D;
struct VehicleInfo;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void AddBodyToWorld(btRigidBody* body);
	void RemoveBodyFromWorld(btRigidBody* body);

	PhysBody3D* RayCast(const vec3& Origin, const vec3& Direction, vec3& HitPoint = vec3());

	PhysBody3D* AddBody(const Sphere& sphere, float mass = 1.0f, bool is_sensor = false);				//REVISE THIS AddBody Methods. No virtual, just an override for different shapes.
	PhysBody3D* AddBody(const Cube& cube, float mass = 1.0f);
	PhysBody3D* AddBody(const Cylinder& cylinder, float mass = 1.0f);
	PhysVehicle3D* AddVehicle(const VehicleInfo& info);

	//A P2P constraint takes the center between the centers of the 2 bodies as the pivot. 
	//Change btVector3s for vec3s? --> With this change the method does not depend on Bullet at argument level. It depends on glmath.h. REVISE THIS HERE --> Should it be changed?
	//Change "Primitive&"s for PhysBodies ?
	void AddConstraintP2P(const Primitive& bodyA, const Primitive& bodyB, const vec3& pivotInA, const vec3& pivotInB, bool can_collide = false);
	void AddConstraintHinge(const Primitive& bodyA, const Primitive& bodyB, const vec3& pivotInA, const vec3& pivotInB, const vec3& axisInA, const vec3& axisInB, bool can_collide = false);

	p2List<PhysVehicle3D*>					vehicles;

private:
	bool debug;		//Bool for debug mode. Used to spawn bodies.
	
	btDefaultCollisionConfiguration*		collision_conf;
	btCollisionDispatcher*					dispatcher;
	btBroadphaseInterface*					broad_phase;
	btSequentialImpulseConstraintSolver*	solver;
	btDiscreteDynamicsWorld*				world;
	btDefaultVehicleRaycaster*				vehicle_raycaster;
	DebugDrawer*							debug_draw;

	p2List<btCollisionShape*>				shapes;
	p2List<PhysBody3D*>						bodies;
	p2List<btDefaultMotionState*>			motions;
	p2List<btTypedConstraint*>				constraints;
	//p2List<PhysVehicle3D*>					vehicles;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line()
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
	Primitive point;
};