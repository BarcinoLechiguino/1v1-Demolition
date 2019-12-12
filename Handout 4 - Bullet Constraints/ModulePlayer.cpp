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

	vehicle = App->physics->AddVehicle(car);
	//vehicle2 = App->physics->AddVehicle(car);					//Hive Mind Version (1 player controls 2 Cars and need to do tasks).
	vehicle->SetPos(0, 12, 10);
	//vehicle2->SetPos(0, 12, 15);

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

	//------------------------------------------- PLAYER 1 INPUTS -------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)					//Change to WASD.
	{																		//Need to change the camera controls too (Maybe leave it like that but only activate in debug mode).
		if (vehicle->GetKmh() >= 0.0f)
		{
			acceleration = MAX_ACCELERATION;
		}
		else
		{
			brake = BRAKE_POWER;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		//brake = BRAKE_POWER;

		if (vehicle->GetKmh() <= 0.0f)
		{
			acceleration -= MAX_ACCELERATION;
		}
		else
		{
			brake = BRAKE_POWER;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
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

	p->SetPos(P1vehicle->GetPos().x, P1vehicle->GetPos().y, P1vehicle->GetPos().z);

	p->body.collision_listeners.add(this);
	p->body.Push(-App->camera->Z * 1000.f);
}