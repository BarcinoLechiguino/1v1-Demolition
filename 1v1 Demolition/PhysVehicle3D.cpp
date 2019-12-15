#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	//wheel.color = Blue;
	wheel.color = Black;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		/*wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;*/

		wheel.SetRadius(info.wheels[0].radius);			//Same as the above commented code, but as radius and height are declared as private
		wheel.SetHeight(info.wheels[0].width);			//members of Cylinder (primitive), then the Setter function needs to be used.

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	//---CHASSIS
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.color = Magenta;

	//info.chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	/*info.chassis = new Cube(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&info.chassis->transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	info.chassis->transform.M[12] += offset.getX();
	info.chassis->transform.M[13] += offset.getY();
	info.chassis->transform.M[14] += offset.getZ();

	info.chassis->color = Black;*/

	//---CABIN
	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	btVector3 cabinOffset(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	cabinOffset = cabinOffset.rotate(q.getAxis(), q.getAngle());

	cabin.transform.M[12] += cabinOffset.getX();
	cabin.transform.M[13] += cabinOffset.getY();
	cabin.transform.M[14] += cabinOffset.getZ();

	cabin.color = Cyan;

	//---LIGTHS---
	//---LEFT
	Cube L_light(info.L_light_size.x, info.L_light_size.y, info.L_light_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&L_light.transform);
	btVector3 LlightOffset(info.L_light_offset.x, info.L_light_offset.y, info.L_light_offset.z);
	LlightOffset = LlightOffset.rotate(q.getAxis(), q.getAngle());

	L_light.transform.M[12] += LlightOffset.getX();
	L_light.transform.M[13] += LlightOffset.getY();
	L_light.transform.M[14] += LlightOffset.getZ();

	L_light.color = Yellow;

	//---RIGHT
	Cube R_light(info.R_light_size.x, info.R_light_size.y, info.R_light_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&R_light.transform);
	btVector3 RlightOffset(info.R_light_offset.x, info.R_light_offset.y, info.R_light_offset.z);
	RlightOffset = RlightOffset.rotate(q.getAxis(), q.getAngle());

	R_light.transform.M[12] += RlightOffset.getX();
	R_light.transform.M[13] += RlightOffset.getY();
	R_light.transform.M[14] += RlightOffset.getZ();

	R_light.color = Yellow;

	//---SPOILER L FOOT
	Cube L_foot(info.L_spoiler_foot_size.x, info.L_spoiler_foot_size.y, info.L_spoiler_foot_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&L_foot.transform);
	btVector3 LfootOffset(info.L_spoiler_foot_offset.x, info.L_spoiler_foot_offset.y, info.L_spoiler_foot_offset.z);
	LfootOffset = LfootOffset.rotate(q.getAxis(), q.getAngle());

	L_foot.transform.M[12] += LfootOffset.getX();
	L_foot.transform.M[13] += LfootOffset.getY();
	L_foot.transform.M[14] += LfootOffset.getZ();

	L_foot.color = Purple;

	//---SPOILER R FOOT
	Cube R_foot(info.R_spoiler_foot_size.x, info.R_spoiler_foot_size.y, info.R_spoiler_foot_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&R_foot.transform);
	btVector3 RfootOffset(info.R_spoiler_foot_offset.x, info.R_spoiler_foot_offset.y, info.R_spoiler_foot_offset.z);
	RfootOffset = RfootOffset.rotate(q.getAxis(), q.getAngle());

	R_foot.transform.M[12] += RfootOffset.getX();
	R_foot.transform.M[13] += RfootOffset.getY();
	R_foot.transform.M[14] += RfootOffset.getZ();

	R_foot.color = Purple;

	//---SPOILER
	Cube spoiler(info.spoiler_size.x, info.spoiler_size.y, info.spoiler_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&spoiler.transform);
	btVector3 spoilerOffset(info.spoiler_offset.x, info.spoiler_offset.y, info.spoiler_offset.z);
	spoilerOffset = spoilerOffset.rotate(q.getAxis(), q.getAngle());

	spoiler.transform.M[12] += spoilerOffset.getX();
	spoiler.transform.M[13] += spoilerOffset.getY();
	spoiler.transform.M[14] += spoilerOffset.getZ();

	spoiler.color = Yellow;

	//---FRONT
	Cube front(info.front_size.x, info.front_size.y, info.front_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front.transform);
	btVector3 frontOffset(info.front_offset.x, info.front_offset.y, info.front_offset.z);
	frontOffset = frontOffset.rotate(q.getAxis(), q.getAngle());

	front.transform.M[12] += frontOffset.getX();
	front.transform.M[13] += frontOffset.getY();
	front.transform.M[14] += frontOffset.getZ();

	front.color = Blue;

	//---FRONT2
	Cube front2(info.front2_size.x, info.front2_size.y, info.front2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front2.transform);
	btVector3 front2Offset(info.front2_offset.x, info.front2_offset.y, info.front2_offset.z);
	front2Offset = front2Offset.rotate(q.getAxis(), q.getAngle());

	front2.transform.M[12] += front2Offset.getX();
	front2.transform.M[13] += front2Offset.getY();
	front2.transform.M[14] += front2Offset.getZ();

	front2.color = Blue;

	//---BACK
	Cube back(info.back_size.x, info.back_size.y, info.back_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back.transform);
	btVector3 backOffset(info.back_offset.x, info.back_offset.y, info.back_offset.z);
	backOffset = backOffset.rotate(q.getAxis(), q.getAngle());

	back.transform.M[12] += backOffset.getX();
	back.transform.M[13] += backOffset.getY();
	back.transform.M[14] += backOffset.getZ();

	back.color = LightBlue;

	//---L
	Cube L(info.L_size.x, info.L_size.y, info.L_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&L.transform);
	btVector3 LOffset(info.L_offset.x, info.L_offset.y, info.L_offset.z);
	LOffset = LOffset.rotate(q.getAxis(), q.getAngle());

	L.transform.M[12] += LOffset.getX();
	L.transform.M[13] += LOffset.getY();
	L.transform.M[14] += LOffset.getZ();

	L.color = Blue;

	//---R
	Cube R(info.R_size.x, info.R_size.y, info.R_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&R.transform);
	btVector3 ROffset(info.R_offset.x, info.R_offset.y, info.R_offset.z);
	ROffset = ROffset.rotate(q.getAxis(), q.getAngle());

	R.transform.M[12] += ROffset.getX();
	R.transform.M[13] += ROffset.getY();
	R.transform.M[14] += ROffset.getZ();

	R.color = Blue;

	//---NEON
	Cube neon(info.neon_size.x, info.neon_size.y, info.neon_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&neon.transform);
	btVector3 neonOffset(info.neon_offset.x, info.neon_offset.y, info.neon_offset.z);
	neonOffset = neonOffset.rotate(q.getAxis(), q.getAngle());

	neon.transform.M[12] += neonOffset.getX();
	neon.transform.M[13] += neonOffset.getY();
	neon.transform.M[14] += neonOffset.getZ();

	neon.color = Green;

	chassis.Render();
	//info.chassis->Render();
	cabin.Render();
	L_light.Render();
	R_light.Render();
	L_foot.Render();
	R_foot.Render();
	spoiler.Render();
	front.Render();
	front2.Render();
	back.Render();
	L.Render();
	R.Render();
	neon.Render();

	/*Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();


	chassis.Render();*/


}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}