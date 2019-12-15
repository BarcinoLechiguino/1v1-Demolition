#include "PhysBody3D.h"
#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ---------------------------------------------------------
PhysBody3D::PhysBody3D()
	: body(nullptr)
	, colShape(nullptr)
	, motionState(nullptr)
	, parentPrimitive(nullptr)
	, is_sensor(false)
	/*, collision_listeners()*/
{
	
}

PhysBody3D::PhysBody3D(btRigidBody* body)		//REVISE THIS. Second PhysBody3D constructor. May be problematic(?).
	: body(body)
	, colShape(nullptr)
	, motionState(nullptr)
	, parentPrimitive(nullptr)
	/*, collision_listeners()*/
{}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	if (HasBody() == true)
	{
		App->physics-> RemoveBodyFromWorld(body);
		delete body;
		delete colShape;
		delete motionState;
	}
}

void PhysBody3D::SetBody(Sphere* primitive, float mass, bool is_sensor, bool is_environment)
{
	SetBody(new btSphereShape(primitive->GetRadius()),
		primitive, mass, is_sensor, is_environment);
}

void PhysBody3D::SetBody(Cube* primitive, vec3 size, float mass, bool is_sensor, bool is_environment)
{
	//btVector3 btSize = { primitive->GetSize().x, primitive->GetSize().y, primitive->GetSize().z };
	btVector3 btSize = { size.x, size.y, size.z };

	SetBody(new btBoxShape(btSize * HALF),
		primitive, mass, is_sensor, is_environment);
}

void PhysBody3D::SetBody(Cylinder* primitive, float mass, bool is_sensor, bool is_environment)
{
	//btVector3 btSize = { primitive->GetRadius(), primitive->GetHeight() * 0.5f, depth };
	btVector3 btSize = { primitive->GetHeight() * HALF, primitive->GetRadius(), 0.0f };
	
	SetBody(new btCylinderShapeX(btSize),
		primitive, mass, is_sensor, is_environment);
}

bool PhysBody3D::HasBody() const
{
	return body != nullptr;
}

btRigidBody * PhysBody3D::GetBody() const
{
	return body;
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if (HasBody() == false)
		return;

	body->getWorldTransform().getOpenGLMatrix(matrix);
	//body->activate();
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if (HasBody() == false)
		return;

	btTransform trans;
	trans.setFromOpenGLMatrix(matrix);
	body->setWorldTransform(trans);
	body->activate();
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	if (HasBody() == false)
		return;

	btTransform trans = body->getWorldTransform();
	trans.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(trans);
	body->activate();
}

void PhysBody3D::SetPos(vec3 position)
{
	if (HasBody() == false)
		return;

	btVector3 origin = { position.x, position.y, position.z };

	btTransform trans = body->getWorldTransform();
	trans.setOrigin(origin);
	body->setWorldTransform(trans);
	body->activate();
}

// --- Gets the position of an object in the physics world.
vec3 PhysBody3D::GetPos() const
{
	if (HasBody() == false)
		return vec3(0, 0, 0);

	btTransform trans = body->getWorldTransform();
	btVector3 buffer = trans.getOrigin();

	vec3 position = {buffer.getX(), buffer.getY(), buffer.getZ()};

	return position;
}

// --- Gets the distance between a physBody and the world origin (vec3(0.0f, 0.0f, 0.0f))
float PhysBody3D::DistanceFromWorldOrigin(vec3 bodyPos) const
{
	vec3 origin(0.0f, 0.0f, 0.0f);

	float posX = origin.x - bodyPos.x;
	float posY = origin.y - bodyPos.y;
	float posZ = origin.z - bodyPos.z;

	if (posX < 0)
	{
		posX = posX * (-1);
	}

	if (posY < 0)
	{
		posY = posY * (-1);
	}

	if (posZ < 0)
	{
		posZ = posZ * (-1);
	}

	float distance = posX + posY + posZ;
	//float distance = posX + posZ;

	return distance;
}

float PhysBody3D::DistanceBetweenBodies(vec3 bodyPos) const
{
	vec3 firstBody = this->GetPos();

	float posX = firstBody.x - bodyPos.x;
	float posY = firstBody.y - bodyPos.y;
	float posZ = firstBody.z - bodyPos.z;

	if (posX < 0)
	{
		posX = posX * (-1);
	}

	if (posY < 0)
	{
		posY = posY * (-1);
	}

	if (posZ < 0)
	{
		posZ = posZ * (-1);
	}

	float distance = posX + posY + posZ;

	return distance;
}

// --- Sets a body as a Sensor, which means that it will detect a collision but it can be gone through (returns no contact)
void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);		//Add “CF_NO_CONTACT_RESPONSE”” to Current Flags
		}
		else
		{
			body->setCollisionFlags(body->getCollisionFlags() &~ btCollisionObject::CF_NO_CONTACT_RESPONSE);	//Remove “CF_NO_CONTACT_RESPONSE” from Current Flags
		}
	}
}

// --- Resets the transform of a body to a neutral state (IdentityMatrix)
void PhysBody3D::ResetTransform()
{
	mat4x4 matrix = IdentityMatrix;
	btTransform transform;
	transform.setFromOpenGLMatrix(&matrix);

	body->setWorldTransform(transform);
}

void PhysBody3D::SetSpeed(vec3 speed)
{
	Stop();
	Push(speed);
}

void PhysBody3D::Push(vec3 force)
{
	if (HasBody())
	{
		body->activate();
		body->applyCentralForce(btVector3(force.x, force.y, force.z));
	}
}

void PhysBody3D::Stop()
{
	if (HasBody())
		body->clearForces();
}

void PhysBody3D::SetBody(btCollisionShape * shape, Primitive* parent, float mass, bool is_sensor, bool is_environment)
{
	assert(HasBody() == false);

	parentPrimitive = parent;

	colShape = shape;

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&parent->transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);

	body = new btRigidBody(rbInfo);

	body->setUserPointer(this);

	this->SetAsSensor(is_sensor);

	if (is_sensor == true)								//With this both players will detect a collision with the sensor.
	{
		this->collision_listeners.add(App->player);
		this->collision_listeners.add(App->player2);
	}

	this->is_environment = is_environment;				//Way to keep track of elements in the physics world that are part of the arena.

 	App->physics->AddBodyToWorld(body);
}
