#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3.0f, 1.0f, 6.0f);
	car.chassis_offset.Set(0.0f, 0.7f, 0.0f);

	car.cabin_size.Set(2.9f, 1.5f, 3.0f);
	car.cabin_offset.Set(0.0f, 1.2f, -0.5f);

	car.L_light_size.Set(0.4f, 0.2f, 6.05f);
	car.L_light_offset.Set(1.0f, 0.9f, 0.0f);

	car.R_light_size.Set(0.4f, 0.2f, 6.05f);
	car.R_light_offset.Set(-1.0f, 0.9f, 0.0f);

	car.L_spoiler_foot_size.Set(0.2f, 1.0f, 0.4f);
	car.L_spoiler_foot_offset.Set(-1.0f, 1.3f, -2.7f);

	car.R_spoiler_foot_size.Set(0.2f, 1.0f, 0.4f);
	car.R_spoiler_foot_offset.Set(1.0f, 1.3f, -2.7f);

	car.spoiler_size.Set(3.0f, 0.2f, 1.0f);
	car.spoiler_offset.Set(0.0f, 1.9f, -3.0f);

	car.front_size.Set(3.0f, 0.2f, 1.0f);
	car.front_offset.Set(0.0f, -0.4f, 2.9f);

	car.front2_size.Set(3.0f, 0.5f, 0.2f);
	car.front2_offset.Set(0.0f, 0.0f, 3.0f);

	car.back_size.Set(3.0f, 0.5f, 0.2f);
	car.back_offset.Set(0.0f, 0.0f, -2.9f);

	car.L_size.Set(0.2f, 0.5f, 3.0f);
	car.L_offset.Set(-1.5f, 0.0f, 0.0f);

	car.R_size.Set(0.2f, 0.5f, 3.0f);
	car.R_offset.Set(1.5f, 0.0f, 0.0f);

	car.neon_size.Set(3.2f, 0.1f, 6.1f);
	car.neon_offset.Set(0.0f, -0.25f, 0.0f);

	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.8f;
	float wheel_width = 0.7f;
	float suspensionRestLength = 1.0f;

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

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 20, 0);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		acceleration = -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn = TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{

		//resets the game
		IdentityMatrix = IDENTITY;
		vehicle->SetTransform(IdentityMatrix.M);
		vehicle->SetPos(startPos.x, startPos.y, startPos.z);
		vehicle->GetBody()->setAngularVelocity({ 0, 0, 0 });
		vehicle->GetBody()->setLinearVelocity({ 0, 0, 0 });

		//App->scene_intro->DebugSpawnPrimitive(new Sphere());
		//SpawnThrowableItem(new Sphere());
<<<<<<< HEAD

=======
>>>>>>> parent of a6c4ae1... Camera Zoom has been fully Implemented. Throw Item method has been partially implemented.
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		//Cambiar input SPACE ES PARA FRENAR
		//App->scene_intro->DebugSpawnPrimitive(new Sphere());
		//SpawnThrowableItem(new Sphere());
	}

	//Player 2 Inputs
	//

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	/*vehicle2->ApplyEngineForce(acceleration);					//Hive Mind Version.
	vehicle2->Turn(turn);
	vehicle2->Brake(brake);*/

	vehicle->Render();
	//vehicle2->Render();

	char title[80];
	sprintf_s(title, "P1 Speed: %.1f Km/h / P2 Speed: %.1f Km/h", 
		vehicle->GetKmh(), App->player2->P2vehicle->GetKmh());			//Temporal measure to show both vehicles Km/h.
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModulePlayer::SpawnThrowableItem(Primitive* p)
{
	App->scene_intro->primitives.PushBack(p);
	//p->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	//p->SetPos(vehicle->parentPrimitive->transform.translation().x, vehicle->parentPrimitive->transform.translation().y, vehicle->parentPrimitive->transform.translation().z);
	
	//vec3 transform = vehicle->GetTransform();
<<<<<<< HEAD

	p->SetPos(vehicle->GetPos().x, vehicle->GetPos().y, vehicle->GetPos().z);

	p->body.collision_listeners.add(this);
	p->body.Push(-App->camera->Z * 1000.f);
=======
>>>>>>> parent of a6c4ae1... Camera Zoom has been fully Implemented. Throw Item method has been partially implemented.

	p->SetPos(P1vehicle->GetPos().x, P1vehicle->GetPos().y, P1vehicle->GetPos().z);

	p->body.collision_listeners.add(this);
	p->body.Push(-App->camera->Z * 1000.f);
}

void ModulePlayer::RestartPlayer1(vec3 respawnPosition)
{
	//delete P1vehicle;
	//P1vehicle = App->physics->AddVehicle(car);

	P1vehicle->GetBody()->clearForces();											//Resets the force and torque values applied to an object.
	P1vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));		//Resets the vehicle's linear velocity (throttle).
	P1vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));		//Resets the vehicle's angular velocity (turn).

	P1vehicle->ResetTransform();													//Set transform to its original position. (1, 1, 1)
	P1vehicle->SetPos(respawnPosition.x, respawnPosition.y, respawnPosition.z);		//Sets the position to the one passed as argument.
}