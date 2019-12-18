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

	wheel.color = Black;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.SetRadius(info.wheels[0].radius);			//Same as the above commented code, but as radius and height are declared as private
		wheel.SetHeight(info.wheels[0].width);			//members of Cylinder (primitive), then the Setter function needs to be used.

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	RenderVehicles(q);
	
	// --- P1'S VEHICLE PARTS
	Cube chassis_P1				= SetVehicleCubeElement(info.chassis_size, info.chassis_offset, Magenta, q);					// Chassis.
	Cube cabin_P1				= SetVehicleCubeElement(info.cabin_size, info.cabin_offset, Cyan, q);							// Cabin.
	Cube leftLight_P1			= SetVehicleCubeElement(info.leftLight_size, info.leftLight_offset, Yellow, q);					// Left Light.
	Cube rightLight_P1			= SetVehicleCubeElement(info.rightLight_size, info.rightLight_offset, Yellow, q);				// Right Light.
	Cube spoilerLeftPivot_P1	= SetVehicleCubeElement(info.spoilerLeftPivot_size, info.spoilerLeftPivot_offset, Purple, q);	// Down Left Bumper.
	Cube spoilerRightPivot_P1	= SetVehicleCubeElement(info.spoilerRightPivot_size, info.spoilerRightPivot_offset, Purple, q);	// Down Right Bumper.
	Cube spoiler_P1				= SetVehicleCubeElement(info.spoiler_size, info.spoiler_offset, Yellow, q);						// Spoiler.
	Cube front_P1				= SetVehicleCubeElement(info.front_size, info.front_offset, Blue, q);							// Front.
	Cube front2_P1				= SetVehicleCubeElement(info.front2_size, info.front2_offset, Blue, q);							// Front 2.
	Cube back_P1				= SetVehicleCubeElement(info.back_size, info.back_offset, Blue, q);								// Back.
	Cube L_P1					= SetVehicleCubeElement(info.L_size, info.L_offset, Blue, q);									// L.
	Cube R_P1					= SetVehicleCubeElement(info.R_size, info.R_offset, Blue, q);									// R.
	//Cube neon_P1				= SetVehicleCubeElement(info.neon_size, info.neon_offset, Green, q);						//Neon. Should Erase?

	// --- P2'S VEHICLE PARTS
	Cube chassis_P2 = SetVehicleCubeElement(info.chassis2_size, info.chassis2_offset, Grey, q);							//Chassis.


	// --- P1'S VEHICLE'S RENDER
	chassis_P1.Render();
	cabin_P1.Render();
	leftLight_P1.Render();
	rightLight_P1.Render();
	spoilerLeftPivot_P1.Render();
	spoilerRightPivot_P1.Render();
	spoiler_P1.Render();
	front_P1.Render();
	front2_P1.Render();
	back_P1.Render();
	L_P1.Render();
	R_P1.Render();
	//neon_P1.Render();

	// --- P2'S VEHICLE'S RENDER
	chassis_P2.Render();
}

// --- Sets a cube vehicle structure element's data members.
Cube PhysVehicle3D::SetVehicleCubeElement(vec3 part, vec3 offset, Color partColor, btQuaternion rotQuat)
{
	Cube cubePart(part.x, part.y, part.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cubePart.transform);

	btVector3 partOffset(offset.x, offset.y, offset.z);
	partOffset = partOffset.rotate(rotQuat.getAxis(), rotQuat.getAngle());

	cubePart.transform.M[12] += partOffset.getX();
	cubePart.transform.M[13] += partOffset.getY();
	cubePart.transform.M[14] += partOffset.getZ();

	cubePart.color = partColor;

	return cubePart;
}

// --- Renders all vehicles.
void PhysVehicle3D::RenderVehicles(btQuaternion q)
{

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

// ----------------------------------------------------------------------------