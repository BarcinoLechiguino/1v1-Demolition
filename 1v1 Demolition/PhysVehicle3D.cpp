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
	// --- P1'S VEHICLE PARTS
	Cube chassis_P1				= SetVehicleCubeElement(info.chassis_size, info.chassis_offset, Yellow, q);							// Chassis.
	Cube cabin_P1				= SetVehicleCubeElement(info.cabin_size, info.cabin_offset, Purple, q);								// Cabin.
	Cube leftLight_P1			= SetVehicleCubeElement(info.leftLight_size, info.leftLight_offset, Green, q);						// Left Light.
	Cube rightLight_P1			= SetVehicleCubeElement(info.rightLight_size, info.rightLight_offset, Green, q);					// Right Light.
	Cube spoilerLeftPivot_P1	= SetVehicleCubeElement(info.spoilerLeftPivot_size, info.spoilerLeftPivot_offset, Green, q);		// Down Left Bumper.
	Cube spoilerRightPivot_P1	= SetVehicleCubeElement(info.spoilerRightPivot_size, info.spoilerRightPivot_offset, Green, q);		// Down Right Bumper.
	Cube spoiler_P1				= SetVehicleCubeElement(info.spoiler_size, info.spoiler_offset, Blue, q);							// Spoiler.
	Cube frontBumper_P1			= SetVehicleCubeElement(info.frontBumper_size, info.frontBumper_offset, Blue, q);					// Front Bumper.
	Cube frontBumperPivot_P1	= SetVehicleCubeElement(info.frontBumperPivot_size, info.frontBumperPivot_offset, Blue, q);			// Front Bumper Pivot.
	Cube rearBumper_P1			= SetVehicleCubeElement(info.rearBumper_size, info.rearBumper_offset, Blue, q);						// Rear Bumper.
	Cube leftSkirt_P1			= SetVehicleCubeElement(info.leftSkirt_size, info.leftSkirt_offset, Blue, q);						// Left Skirt.
	Cube rightSkirt_P1			= SetVehicleCubeElement(info.rightSkirt_size, info.rightSkirt_offset, Blue, q);						// Right Skirt.

	// --- P2'S VEHICLE PARTS
	Cube chassis_P2				= SetVehicleCubeElement(info.chassis2_size, info.chassis2_offset, Green, q);						// Chassis.
	Cube cabin_P2				= SetVehicleCubeElement(info.cabin2_size, info.cabin2_offset, Orange, q);							// Cabin.
	Cube leftLight_P2			= SetVehicleCubeElement(info.leftLight2_size, info.leftLight2_offset, Yellow, q);					// Left Light.
	Cube rightLight_P2			= SetVehicleCubeElement(info.rightLight2_size, info.rightLight2_offset, Yellow, q);					// Right Light.
	Cube spoilerLeftPivot_P2	= SetVehicleCubeElement(info.spoilerLeftPivot2_size, info.spoilerLeftPivot2_offset, Yellow, q);		// Down Left Bumper.
	Cube spoilerRightPivot_P2	= SetVehicleCubeElement(info.spoilerRightPivot2_size, info.spoilerRightPivot2_offset, Yellow, q);	// Down Right Bumper.
	Cube spoiler_P2				= SetVehicleCubeElement(info.spoiler2_size, info.spoiler2_offset, Red, q);							// Spoiler.
	Cube frontBumper_P2			= SetVehicleCubeElement(info.frontBumper2_size, info.frontBumper2_offset, Red, q);					// Front Bumper.
	Cube frontBumperPivot_P2	= SetVehicleCubeElement(info.frontBumperPivot2_size, info.frontBumperPivot2_offset, Red, q);		// Front Bumper Pivot.
	Cube rearBumper_P2			= SetVehicleCubeElement(info.rearBumper2_size, info.rearBumper2_offset, Red, q);					// Rear Bumper.
	Cube leftSkirt_P2			= SetVehicleCubeElement(info.leftSkirt2_size, info.leftSkirt2_offset, Red, q);						// Left Skirt.
	Cube rightSkirt_P2			= SetVehicleCubeElement(info.rightSkirt2_size, info.rightSkirt2_offset, Red, q);					// Right Skirt.


	// --- P1'S VEHICLE'S RENDER
	chassis_P1.Render();
	cabin_P1.Render();
	leftLight_P1.Render();
	rightLight_P1.Render();
	spoilerLeftPivot_P1.Render();
	spoilerRightPivot_P1.Render();
	spoiler_P1.Render();
	frontBumper_P1.Render();
	frontBumperPivot_P1.Render();
	rearBumper_P1.Render();
	leftSkirt_P1.Render();
	rightSkirt_P1.Render();

	// --- P2'S VEHICLE'S RENDER
	chassis_P2.Render();
	cabin_P2.Render();
	leftLight_P2.Render();
	rightLight_P2.Render();
	spoilerLeftPivot_P2.Render();
	spoilerRightPivot_P2.Render();
	spoiler_P2.Render();
	frontBumper_P2.Render();
	frontBumperPivot_P2.Render();
	rearBumper_P2.Render();
	leftSkirt_P2.Render();
	rightSkirt_P2.Render();
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