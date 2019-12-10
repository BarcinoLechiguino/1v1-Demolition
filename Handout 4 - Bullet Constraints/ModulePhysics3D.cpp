#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "Primitive.h"

#include "glut/glut.h"

#ifdef _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled), world(nullptr)/*, constraint(nullptr)*/
{
	collision_conf	= new btDefaultCollisionConfiguration();
	dispatcher		= new btCollisionDispatcher(collision_conf);
	broad_phase		= new btDbvtBroadphase();
	solver			= new btSequentialImpulseConstraintSolver();
	debug_draw		= new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;

	p2List_item<btTypedConstraint*>* it = constraints.getFirst();		//Each constraint is a new so it needs to be deleted.

	for (p2List_item<btTypedConstraint*>* it = constraints.getFirst(); it != NULL; it = it->next)
	{
		delete it->data;
	}
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);								//Without this stepSimulation crashes. It sends an reading access error because m_dynamicsWorld cannot be read as it has not been allocated/set (is nullptr).

	// Big rectangle as ground
	{
		btCollisionShape* colShape = new btBoxShape(btVector3(200.0f, 2.0f, 200.0f));

		mat4x4 glMatrix = IdentityMatrix;
		glMatrix.translate(0.f, -2.f, 0.f);
		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(&glMatrix);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	//REVISE THIS 
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if (pbodyA && pbodyB)
			{
				p2List_item<Module*>* item = pbodyA->collision_listeners.getFirst();
				while (item)
				{
					item->data->OnCollision(pbodyA, pbodyB);
					item = item->next;
				}

				item = pbodyB->collision_listeners.getFirst();
				while (item)
				{
					item->data->OnCollision(pbodyB, pbodyA);
					item = item->next;
				}
			}
		}
	}
	
	/*for (int n = 0; n < world->getDispatcher()->getNumManifolds(); n++)
	{
		btPersistentManifold* manifold = world->getDispatcher()->getManifoldByIndexInternal(n);
		if (manifold->getNumContacts() > 0)
		{
			PhysBody3D* body1 = (PhysBody3D*)manifold->getBody0()->getUserPointer();
			PhysBody3D* body2 = (PhysBody3D*)manifold->getBody1()->getUserPointer();

			if (body1 != nullptr && body2 != nullptr)
			{
				for (uint n = 0; n < body1->collision_listeners.Count(); n++)
				{
					body1->collision_listeners[n]->OnCollision(body1, body2);
				}

				for (uint n = 0; n < body2->collision_listeners.Count(); n++)
				{
					body2->collision_listeners[n]->OnCollision(body2, body1);
				}
			}
		}
	}*/

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if (App->debug == true)
	{
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		world->debugDrawWorld();
		glEnable(GL_LIGHTING);
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	for (p2List_item<btTypedConstraint*>* item = constraints.getFirst(); item; item = item->next)
	{
		world->removeConstraint(item->data);
		delete item->data;
	}

	constraints.clear();

	for (p2List_item<btDefaultMotionState*>* item = motions.getFirst(); item; item = item->next)
		delete item->data;

	motions.clear();

	for (p2List_item<btCollisionShape*>* item = shapes.getFirst(); item; item = item->next)
		delete item->data;

	shapes.clear();

	for (p2List_item<PhysBody3D*>* item = bodies.getFirst(); item; item = item->next)
		delete item->data;

	bodies.clear();

	for (p2List_item<PhysVehicle3D*>* item = vehicles.getFirst(); item; item = item->next)
		delete item->data;

	vehicles.clear();

	delete vehicle_raycaster;
	delete world;

	return true;
}

PhysBody3D * ModulePhysics3D::RayCast(const vec3 & Origin, const vec3 & Direction, vec3& HitPoint)
{
	//TODO: NEW CODE
	//A snippet of new code that may be useful for you. Nothing to do here really
	vec3 Dir = normalize(Direction);

	btVector3 Start = btVector3(Origin.x, Origin.y, Origin.z);
	btVector3 End = btVector3(Origin.x + Dir.x * 1000.f, Origin.y + Dir.y * 1000.f, Origin.z + Dir.z * 1000.f);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	// Perform raycast
	world->rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit()) {

		HitPoint = vec3(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z());
		return (PhysBody3D*)RayCallback.m_collisionObject->getUserPointer();
	}
	return nullptr;
}

void ModulePhysics3D::AddBodyToWorld(btRigidBody * body)
{
	world->addRigidBody(body);
}

void ModulePhysics3D::RemoveBodyFromWorld(btRigidBody * body)
{
	world->removeRigidBody(body);
}

PhysBody3D* ModulePhysics3D::AddBody(const Sphere& sphere, float mass)
{
	btCollisionShape* colShape = new btSphereShape(sphere.GetRadius());								//REVISE THIS Getters here.
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&sphere.transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}


// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cube& cube, float mass)
{
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.GetSize().x*0.5f, cube.GetSize().y*0.5f, cube.GetSize().z*0.5f));	//REVISE THIS Getters here.
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cube.transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cylinder& cylinder, float mass)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.GetHeight()*0.5f, cylinder.GetRadius(), 0.0f));			//REVISE THIS Getters here.
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysVehicle3D* ModulePhysics3D::AddVehicle(const VehicleInfo& info)
{
	btCompoundShape* comShape = new btCompoundShape();
	shapes.add(comShape);

	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x*0.5f, info.chassis_size.y*0.5f, info.chassis_size.z*0.5f));
	shapes.add(colShape);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z));

	comShape->addChildShape(trans, colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);
	comShape->calculateLocalInertia(info.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, myMotionState, comShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setActivationState(DISABLE_DEACTIVATION);

	world->addRigidBody(body);

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_frictionSlip = info.frictionSlip;
	tuning.m_maxSuspensionForce = info.maxSuspensionForce;
	tuning.m_maxSuspensionTravelCm = info.maxSuspensionTravelCm;
	tuning.m_suspensionCompression = info.suspensionCompression;
	tuning.m_suspensionDamping = info.suspensionDamping;
	tuning.m_suspensionStiffness = info.suspensionStiffness;

	btRaycastVehicle* vehicle = new btRaycastVehicle(tuning, body, vehicle_raycaster);

	vehicle->setCoordinateSystem(0, 1, 2);

	for (int i = 0; i < info.num_wheels; ++i)
	{
		btVector3 conn(info.wheels[i].connection.x, info.wheels[i].connection.y, info.wheels[i].connection.z);
		btVector3 dir(info.wheels[i].direction.x, info.wheels[i].direction.y, info.wheels[i].direction.z);
		btVector3 axis(info.wheels[i].axis.x, info.wheels[i].axis.y, info.wheels[i].axis.z);

		vehicle->addWheel(conn, dir, axis, info.wheels[i].suspensionRestLength, info.wheels[i].radius, tuning, info.wheels[i].front);
	}
	// ---------------------

	PhysVehicle3D* pvehicle = new PhysVehicle3D(body, vehicle, info);
	world->addVehicle(vehicle);
	vehicles.add(pvehicle);

	return pvehicle;
}

void ModulePhysics3D::AddConstraintP2P(const Primitive& bodyA, const Primitive& bodyB, const vec3& pivotInA, const vec3& pivotInB)
{
	//To be able to use a P2P constraint with primitives (or PhysBodies), the btRigidBody of the Primitive/PhysBody needs to be passed.
	//Constraint pointer that will be used to create new constraints.
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*bodyA.body.GetBody(),
		*bodyB.body.GetBody(),
		btVector3(pivotInA.x, pivotInA.y, pivotInA.z),
		btVector3(pivotInB.x, pivotInB.y, pivotInB.z));
	world->addConstraint(p2p, false);						//addConstraint receives as arguments a constraint (btTypedConstraint and its subclasses) and a bool that determines whether or not the constrained bodies will collide between them. 
	constraints.add(p2p);									//Adds the new p2p constraint to the constraints list.
	p2p->setDbgDrawSize(2.0f);								//REVISE THIS here.
}

void ModulePhysics3D::AddConstraintHinge(const Primitive& bodyA, const Primitive& bodyB, const vec3& pivotInA, const vec3& pivotInB, const vec3& axisInA, const vec3& axisInB)
{
	btTypedConstraint* hinge = new btHingeConstraint(
		*bodyA.body.GetBody(),
		*bodyB.body.GetBody(),
		btVector3(pivotInA.x, pivotInA.y, pivotInA.z),
		btVector3(pivotInB.x, pivotInB.y, pivotInB.z),
		btVector3(axisInA.x, axisInA.y, axisInA.z),
		btVector3(axisInB.x, axisInB.y, axisInB.z));
	world->addConstraint(hinge, false);
	constraints.add(hinge);								//Adds the new hinge constraint to the constraints list.
	hinge->setDbgDrawSize(2.0f);						//REVISE THIS here.
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
