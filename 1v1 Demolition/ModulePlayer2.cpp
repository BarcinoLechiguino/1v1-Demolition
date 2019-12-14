#include "Globals.h"
#include "Application.h"
#include "ModulePlayer2.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer2::ModulePlayer2(Application* app, bool start_enabled) : Module(app, start_enabled)
, P2vehicle(NULL)
, spawnPoint(5, 12, 10)
, lives(MAX_LIVES)
, alive(true)
, ammo(MAX_AMMO)
, loaded(false)
, scale(1.0f)
, prevCollBody()
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading player");

	GenerateP2Vehicle();

	LoadAudioP2();

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

	if (!App->debug)
	{
		DriveInputsP2();										//Inputs P1
	}

	SpecialInputsP2();											//Throw Item & Restart.

	CheckLivesP2();												//Checks how many lives Player 2 has left. If P2 has no lives, s/he is reset.

	P2vehicle->ApplyEngineForce(acceleration);
	P2vehicle->Turn(turn);
	P2vehicle->Brake(brake);

	P2vehicle->Render();

	/*char title[80];
	sprintf_s(title, "%.1f Km/h", P2vehicle->GetKmh());
	App->window->SetTitle(title);*/

	return UPDATE_CONTINUE;
}

void ModulePlayer2::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
 {
	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);

	if (body1->parentPrimitive != nullptr && body2->GetBody() == App->player2->P2vehicle->GetBody())
	{
		//body1->parentPrimitive->color = color;
		body1->parentPrimitive->color = Red;
	}

	if (body2->parentPrimitive != nullptr)
	{
		body2->parentPrimitive->color = color;
	}

	if (body2->GetBody() == P2vehicle->GetBody())
	{
		if (body1->is_sensor == true)
		{
			//RestartPlayer2(spawnPoint);

			if (ammo < MAX_AMMO)
			{
				ammo = MAX_AMMO;
			}

			return;
		}

		for (int i = 0; i < MAX_BODIES; i++)
		{
			if (prevCollBody[i] == body1)
			{
				//continue;
				break;
			}

			if (prevCollBody[i] == NULL)
			{
				lives--;
				LOG("Return Player 2 Lives: %d", lives);

				prevCollBody[i] = body1;

				break;
			}

			if (prevCollBody[MAX_BODIES - 1] != NULL)
			{
				for (int i = 0; i < MAX_BODIES; i++)
				{
					prevCollBody[i] = NULL;
				}
			}
		}
	}
}

void ModulePlayer2::SpawnThrowableItem(Primitive* p)
{
	App->scene_intro->AddPrimitive(p);												//Adds the "item" to the primitives array.

	vec3 P2_Pos = P2vehicle->GetPos();												//Vec3 that will be set to P1vehicl's position vector.
	
	btVector3 buffer = P2vehicle->vehicle->getForwardVector();						//Buffer for P1vehicle's forward vector.					
	vec3 fwdVector = { buffer.getX(), buffer.getY(), buffer.getZ() };				//vec3 set with the buffer of the forward vector.

	p->SetPos(P2_Pos.x + fwdVector.x, P2_Pos.y + 2, P2_Pos.z - fwdVector.z);		//Sets the position from where the item will spawn.
	//p->SetPos(fwdVector.x, fwdVector.y, fwdVector.z);

	p->body.collision_listeners.add(App->player);									//listener set to Player 1 so the collision is detected by Player 1's OnCollision() method.
	p->body.Push(fwdVector * 5000.f);												//Adds a force to the spawned projectile in the directon of fwdVector.

	p->color = Red;																	//The colour of the spawned item will be blue when P1 spawns it.
}

void ModulePlayer2::RestartPlayer2(vec3 respawnPosition)
{
	P2vehicle->GetBody()->clearForces();											//Resets the force and torque values applied to an object.
	P2vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));		//Resets the vehicle's linear velocity (throttle).
	P2vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));		//Resets the vehicle's angular velocity (turn).

	P2vehicle->ResetTransform();													//Set transform to its original position. (1, 1, 1)
	P2vehicle->SetPos(respawnPosition.x, respawnPosition.y, respawnPosition.z);		//Sets the position to the one passed as argument.

	lives = MAX_LIVES;
	ammo = MAX_AMMO;
}

//------------------------------------------- PLAYER 2 INPUTS -------------------------------------------
void ModulePlayer2::DriveInputsP2()
{
	// -------------------------------- MAIN ACTIONS --------------------------------
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
		if (P2vehicle->GetKmh() <= 0.0f)
		{
			acceleration -= MAX_ACCELERATION;
		}
		else
		{
			brake = BRAKE_POWER;
			App->audio->PlayFx(2, 0);
		}
	}
	
	// -------------------------------- EXTRA ACTIONS --------------------------------
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION * 5;
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_1) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
}

void ModulePlayer2::SpecialInputsP2()
{
	if (App->input->GetKey(SDL_SCANCODE_KP_0) == KEY_DOWN)
	{
		if (ammo != 0)
		{
			ammo--;
			SpawnThrowableItem(new Sphere());
			App->audio->PlayFx(3, 0);
		}
		else
		{

		}
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		RestartPlayer2(vec3(5, 12, 10));
	}
}

// --- Checks how many lives Player 2 has left. If P2 has no lives, s/he is reset.
void ModulePlayer2::CheckLivesP2()
{
	uint prevLives;

	if (lives <= MAX_LIVES)
	{
		prevLives = lives;
	}

	if (lives > MAX_LIVES)
	{
		lives = prevLives - 1;
	}
	
	if (lives <= 0)								//lives > 3 is a dirty safety measure for when lives are less than 0 and lives return the max uint value.
	{
		RestartPlayer2(spawnPoint);
	}
}

void ModulePlayer2::GenerateP2Vehicle()
{
	VehicleInfo car;

	// Car properties ----------------------------------------
	//car.chassis_size.Set(3.5, 3, 4);		//(2, 2, 4)		//._______________.
	//car.chassis_offset.Set(0, 1.5, 0);	//(0, 1.5, 0)
	//car.mass = 500.0f;					//500.0f		//FV
	//car.suspensionStiffness = 150.88f;	//15.88f
	//car.suspensionCompression = 0.83f;	//0.83f
	//car.suspensionDamping = 10.0f;		//0.88f
	//car.maxSuspensionTravelCm = 1000.0f;	//1000.0f
	//car.frictionSlip = 50.5f;				//50.5f
	//car.maxSuspensionForce = 6000.0f;		//6000.0f

	// Car properties ----------------------------------------
	car.chassis_size.Set(3.0f * scale, 1.0f * scale, 6.0f * scale);
	car.chassis_offset.Set(0.0f * scale, 0.7f * scale, 0.0f * scale);

	car.cabin_size.Set(2.9f * scale, 1.5f * scale, 3.0f * scale);
	car.cabin_offset.Set(0.0f * scale, 1.2f * scale, -0.5f * scale);

	car.L_light_size.Set(0.4f * scale, 0.2f * scale, 6.05f * scale);
	car.L_light_offset.Set(1.0f * scale, 0.9f * scale, 0.0f * scale);

	car.R_light_size.Set(0.4f * scale, 0.2f * scale, 6.05f * scale);
	car.R_light_offset.Set(-1.0f * scale, 0.9f * scale, 0.0f * scale);

	car.L_spoiler_foot_size.Set(0.2f * scale, 1.0f * scale, 0.4f * scale);
	car.L_spoiler_foot_offset.Set(-1.0f * scale, 1.3f * scale, -2.7f * scale);

	car.R_spoiler_foot_size.Set(0.2f * scale, 1.0f * scale, 0.4f * scale);
	car.R_spoiler_foot_offset.Set(1.0f * scale, 1.3f * scale, -2.7f * scale);

	car.spoiler_size.Set(3.0f * scale, 0.2f * scale, 1.0f * scale);
	car.spoiler_offset.Set(0.0f * scale, 1.9f * scale, -3.0f * scale);

	car.front_size.Set(3.0f * scale, 0.2f * scale, 1.0f * scale);
	car.front_offset.Set(0.0f * scale, -0.4f * scale, 2.9f * scale);

	car.front2_size.Set(3.0f * scale, 0.5f * scale, 0.2f * scale);
	car.front2_offset.Set(0.0f * scale, 0.0f * scale, 3.0f * scale);

	car.back_size.Set(3.0f * scale, 0.5f * scale, 0.2f * scale);
	car.back_offset.Set(0.0f * scale, 0.0f * scale, -2.9f * scale);

	car.L_size.Set(0.2f * scale, 0.5f * scale, 3.0f * scale);
	car.L_offset.Set(-1.5f * scale, 0.0f * scale, 0.0f * scale);

	car.R_size.Set(0.2f * scale, 0.5f * scale, 3.0f * scale);
	car.R_offset.Set(1.5f * scale, 0.0f * scale, 0.0f * scale);

	/*car.neon_size.Set(3.2f * scale, 0.1f * scale, 6.1f * scale);
	car.neon_offset.Set(0.0f * scale, -0.25f * scale, 0.0f * scale);*/

	car.mass					= 1600.0f;		//Original: 1500.0f, Heavy: 1950.0f
	car.suspensionStiffness		= 15.88f;
	car.suspensionCompression	= 0.83f;
	car.suspensionDamping		= 0.88f;
	car.maxSuspensionTravelCm	= 1000.0f;
	car.frictionSlip			= 50.5;
	car.maxSuspensionForce		= 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f * scale;
	float wheel_radius = 0.6f * scale;
	float wheel_width = 0.5f *  scale;
	float suspensionRestLength = 1.2f * scale;

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
	P2vehicle->SetPos(spawnPoint);
}

void ModulePlayer2::LoadAudioP2()
{
	//Loading FX
	App->audio->LoadFx("audio/FX/Accelerate_First.wav");
	App->audio->LoadFx("audio/FX/Car_Braking.wav");
	App->audio->LoadFx("audio/FX/Shoot_1.wav");
	App->audio->LoadFx("audio/FX/Hit_Car_With_Object.wav");
	App->audio->LoadFx("audio/FX/Crash_With_Obstacles.wav");
	App->audio->LoadFx("audio/FX/Car_Crash_With_Car.wav");
	App->audio->LoadFx("audio/FX/Gun_Reload_sound_effect.wav");
}