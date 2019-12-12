#include "Globals.h"
#include "Application.h"
#include "ModulePlayer2.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer2::ModulePlayer2(Application* app, bool start_enabled) : Module(app, start_enabled), P2vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3.5, 3, 4);		//(2, 2, 4)		//._______________.
	car.chassis_offset.Set(0, 1.5, 0);		//(0, 1.5, 0)
	car.mass = 500.0f;						//500.0f		//FV
	car.suspensionStiffness = 150.88f;		//15.88f
	car.suspensionCompression = 0.83f;		//0.83f
	car.suspensionDamping = 10.0f;			//0.88f
	car.maxSuspensionTravelCm = 1000.0f;	//1000.0f
	car.frictionSlip = 50.5f;				//50.5f
	car.maxSuspensionForce = 6000.0f;		//6000.0f

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------
	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	P2vehicle = App->physics->AddVehicle(car);
	P2vehicle->SetPos(5, 12, 10);

	return true;
}

// Unload assets
bool ModulePlayer2::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer2::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	//------------------------------------------- PLAYER 2 INPUTS -------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT)
	{
		if (P2vehicle->GetKmh() >= 0.0f)
		{
			acceleration = MAX_ACCELERATION;
		}
		else
		{
			brake = BRAKE_POWER;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT)
	{
		//brake = BRAKE_POWER;

		if (P2vehicle->GetKmh() <= 0.0f)
		{
			acceleration -= MAX_ACCELERATION;
		}
		else
		{
			brake = BRAKE_POWER;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN)
	{

	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		RestartPlayer2(vec3(5, 12, 10));
	}

	P2vehicle->ApplyEngineForce(acceleration);
	P2vehicle->Turn(turn);
	P2vehicle->Brake(brake);

	P2vehicle->Render();

	/*char title[80];
	sprintf_s(title, "%.1f Km/h", P2vehicle->GetKmh());
	App->window->SetTitle(title);*/

	return UPDATE_CONTINUE;
}

void ModulePlayer2::SpawnThrowableItem(Primitive* p)
{
	App->scene_intro->primitives.PushBack(p);

	//P2vehicle->vehicle->getForwardVector();

	p->SetPos(P2vehicle->GetPos().x, P2vehicle->GetPos().y, P2vehicle->GetPos().z);

	p->body.collision_listeners.add(this);
	p->body.Push(-App->camera->Z * 1000.f);
}

void ModulePlayer2::RestartPlayer2(vec3 respawnPosition)
{
	//delete P1vehicle;
	//P1vehicle = App->physics->AddVehicle(car);

	P2vehicle->GetBody()->clearForces();											//Resets the force and torque values applied to an object.
	P2vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));		//Resets the vehicle's linear velocity (throttle).
	P2vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));		//Resets the vehicle's angular velocity (turn).

	P2vehicle->ResetTransform();													//Set transform to its original position. (1, 1, 1)
	P2vehicle->SetPos(respawnPosition.x, respawnPosition.y, respawnPosition.z);		//Sets the position to the one passed as argument.
}