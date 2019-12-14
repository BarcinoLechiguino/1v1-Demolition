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

void PhysBody3D::SetBody(Sphere* primitive, float mass, bool is_sensor)
{
	SetBody(new btSphereShape(primitive->GetRadius()), 
		primitive, mass, is_sensor);
}

void PhysBody3D::SetBody(Cube* primitive, vec3 size, float mass, bool is_sensor)
{
	//btVector3 btSize = { primitive->GetSize().x, primitive->GetSize().y, primitive->GetSize().z };
	btVector3 btSize = { size.x, size.y, size.z };

	SetBody(new btBoxShape(btSize * 0.5f), primitive, mass, is_sensor);
}

void PhysBody3D::SetBody(Cylinder* primitive, float depth, float mass, bool is_sensor)
{
	btVector3 btSize = { primitive->GetRadius(), primitive->GetHeight(), depth };
	
	SetBody(new btCylinderShape(btSize * 0.5f), primitive, mass, is_sensor);
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

vec3 PhysBody3D::GetPos() const
{
	if (HasBody() == false)
		return vec3(0, 0, 0);

	btTransform trans = body->getWorldTransform();
	btVector3 buffer = trans.getOrigin();

	vec3 position = {buffer.getX(), buffer.getY(), buffer.getZ()};

	return position;
}

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

void PhysBody3D::SetBody(btCollisionShape * shape, Primitive* parent, float mass, bool is_sensor)
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

 	App->physics->AddBodyToWorld(body);
}
