#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "ModuleRenderer3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
//, top_constraint(nullptr)
{
	//app->renderer3D->skyBoxColor = vec3(1.f, 1.f, 1.f);		//Setting the skybox's color from ModuleSceneIntro. Color white.
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->audio->PlayMusic("audio/Songs/Main_Theme.ogg");

	//App->renderer3D->skyBoxColor = vec3(1.f, 1.f, 1.f);

	App->camera->Move(vec3(1.0f, 40.0f, 0.0f));						//Changes both the camera position and its reference point. Set Move to match the vehicle.
	App->camera->LookAt(vec3(0, 0, 0));								//Initial point of reference. Set it to be the vehicle.

	top_constrained_cube = nullptr;

	LoadArena();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int i = 0; i < primitives.Count(); i++)									//REVISE THIS
	{
		App->physics->RemoveBodyFromWorld(primitives[i]->body.GetBody());
		primitives.Pop(primitives[i]);

		/*if (primitives[i]->body.is_sensor == false && primitives[i]->body.is_environment == false)
		{
			App->physics->RemoveBodyFromWorld(primitives[i]->body.GetBody());
			primitives.Pop(primitives[i]);
		}	*/
	}

	//delete top_constrained_cube;							//Apply this to all? Crashes on exit
	
	//primitives.Clear();
	//LoadArena();
	//Reload the map? 

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	if (App->debug == false)
	{
		CameraMovement();
	}
	
	if (App->debug == true)
		HandleDebugInput();

	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Update();

	
	//Applying torque to the arena's constraints.
	top_constrained_cube->body.GetBody()->applyTorque(btVector3(0.0f, 50000.0f, 0.0f));

	/*for (uint n = 0; n < arena_elements.Count(); n++)
		arena_elements[n]->Update();*/

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	/*for (uint n = 0; n < arena_elements.Count(); n++)
	{
		arena_elements[n]->Render();
	}*/

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		RestartGame();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);

	if (body1->parentPrimitive != nullptr)
	{
		body1->parentPrimitive->color = color;
	}
	
	if (body2->parentPrimitive != nullptr)
	{
		body2->parentPrimitive->color = color;
	}
}

void ModuleSceneIntro::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		DebugSpawnPrimitive(new Sphere());
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		DebugSpawnPrimitive(new Cube(/*vec3(1.f, 1.f, 1.f)*/));
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		DebugSpawnPrimitive(new Cylinder());

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)		//Teleports all free primitives to random places.
	{
		for (uint n = 0; n < primitives.Count(); n++)
		{
			if (primitives[n]->body.is_sensor == false && primitives[n]->body.is_environment == false)
			{
				primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)		//Adds a random force to all free primitives.
	{
		for (uint n = 0; n < primitives.Count(); n++)
		{
			if (primitives[n]->body.is_sensor == false && primitives[n]->body.is_environment == false)
			{
				primitives[n]->body.Push(vec3((float)(std::rand() % 500) - 250, 500, (float)(std::rand() % 500) - 250));
			}
		}

	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//TODO: NEW CODE		//Change Ball colour on click.
		//A snippet of new code that may be useful for you. Nothing to do here really

		//Get a vector indicating the direction from the camera viewpoint to the "mouse"
		const vec2 mousePos(((float)App->input->GetMouseX() / (float)App->window->Width()) * 2.f - 1.f,
			-((float)App->input->GetMouseY() / (float)App->window->Height()) * 2.f + 1.f);

		const vec4 rayEye = inverse(App->renderer3D->ProjectionMatrix) * vec4(mousePos.x, mousePos.y, -1.f, 1.f);
		const vec4 rayWorld(inverse(App->camera->GetViewMatrix()) * vec4(rayEye.x, rayEye.y, -1.f, 0.f));

		vec3 Dir(rayWorld.x, rayWorld.y, rayWorld.z);

		//Cast a ray from the camera, in the "mouse" direction
		PhysBody3D* body = App->physics->RayCast(App->camera->Position, Dir);
		if (body)
		{
			//Change the color of the clicked primitive
			body->parentPrimitive->color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
		}
	}
}

void ModuleSceneIntro::DebugSpawnPrimitive(Primitive * p)
{
	primitives.PushBack(p);
	p->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	//p->body.collision_listeners.add(this);													//Uncomment this to add this primitive to the colision_listener array
	//p->body.collision_listeners.add(App->player);
	p->body.Push(-App->camera->Z * 1000.f);
}

// --- Adds a primitive to the primitives dynArray.
void ModuleSceneIntro::AddPrimitive(Primitive * p)
{
	primitives.PushBack(p);
}

void ModuleSceneIntro::DeletePrimitive(Primitive* p)
{
	for (int i = 0; i < primitives.Count(); i++)									//Revise this.
	{
		if (primitives[i] == p)
		{
			if (primitives[i]->body.is_sensor == false && primitives[i]->body.is_environment == false)
			{
				App->physics->RemoveBodyFromWorld(primitives[i]->body.GetBody());
				delete primitives[i];
				primitives.Pop(primitives[i]);
			}

			break;
		}
	}
}

void ModuleSceneIntro::CameraMovement()
{
	vec3 P1_position = App->player->P1vehicle->GetPos();							//Gets the current position of Player 1.
	vec3 P2_position = App->player2->P2vehicle->GetPos();							//Gets the current position of Player 2.

	vec3 midPos = {																	//Position vector that is set to be at the center poisition of both players.
		(P1_position.x + P2_position.x) * HALF,										//MidPos in the X axis.
		0,																			//MidPos in the Y axis.
		(P1_position.z + P2_position.z) * HALF										//MidPos in the Z axis.
	};

	float camZoom = GetZoom();

	vec3 avgPosition = { midPos.x + CAM_OFFSET, camZoom, midPos.z };
	vec3 avgRefPoint = (P1_position + P2_position) * HALF;							//Calculates the position of the point of reference of the camera. Set to be at the center of both players.

	App->camera->Position = (avgPosition);										//Changes both the camera position and its reference point. Set Move to match the vehicle. OFFSET on x --> Horizontal, OFFSET on z --> Vertical.
	App->camera->LookAt(avgRefPoint);											//LookAt cannot look  at the same position the camera is. There needs to be an offset somewhere.
	//LerpCamera(cameraPosition, avgPosition);
}

//Gets the amount of zoom required taking into account the distance between players (ratio).
float ModuleSceneIntro::GetZoom() const
{
	vec3 P1_position = App->player->P1vehicle->GetPos();				//Gets the current position of Player 1.
	vec3 P2_position = App->player2->P2vehicle->GetPos();				//Gets the current position of Player 2.

	float posX = P1_position.x - P2_position.x;
	float posZ = P1_position.z - P2_position.z;

	if (posX < 0)
	{
		posX = posX * (-1);
	}

	if (posZ < 0)
	{
		posZ = posZ * (-1);
	}

	float distance = posX + posZ;


	float cameraZoom = distance;

	if (cameraZoom < 60)
	{
		cameraZoom = 60;
	}

	if (cameraZoom > 150)
	{
		cameraZoom = 150;
	}

	return cameraZoom;
}

// --- Gets the lerp speed according to a position, a target, and an offset.
float ModuleSceneIntro::GetLerpSpeed(vec3 position, vec3 target, float speed) const
{
	float pos = sqrt(position.x * position.x + position.y * position.y + position.z * position.z);

	float posTarget = sqrt(target.x * target.x + target.y * target.y + target.z * target.z);

	float lerpSpeed = (posTarget - pos) * speed;

	return lerpSpeed;
	//return pos + lerpSpeed;
}


// --- Method that moves the camera according to the lerp speed calculated 
void ModuleSceneIntro::LerpCamera(vec3 cameraPosition, vec3 targetPosition, float speed)
{
	if (App->camera->Position.x < targetPosition.x && App->camera->Position.z < targetPosition.z);
	{
		App->camera->Position += GetLerpSpeed(cameraPosition, targetPosition, speed);
	}
	if (App->camera->Position.x > targetPosition.x && App->camera->Position.z > targetPosition.z)
	{
		App->camera->Position -= GetLerpSpeed(cameraPosition, targetPosition, speed);
	}
}

void ModuleSceneIntro::LoadArena()
{
	// -------------------------------- GROUND & BOUNDS -----------------------------
	Cylinder* ground = new Cylinder(80.f, 2.1f, 0.f, false, true);
	ground->SetPos(0.0f, -1.0f, 0.0f);
	primitives.PushBack(ground);
	ground->color = Beige;

	ground->SetRotation(90, vec3(0, 0, 1));										//The cylinder appears rotated 90 degrees in Yaw, so that needs to be corrected.

	// ----------------------------------- WALLS -----------------------------------
	Cube* north_center_wall = new Cube(vec3(5.f, 5.f, 18.f), 0.0f, false, true);
	north_center_wall->SetPos(-25.0f, 2.5f, 0.0f);
	primitives.PushBack(north_center_wall);

	Cube* south_center_wall = new Cube(vec3(5.f, 5.f, 18.f), 0.0f, false, true);
	south_center_wall->SetPos(25.0f, 2.5f, 0.0f);
	primitives.PushBack(south_center_wall);

	Cube* west_center_wall = new Cube(vec3(18.f, 5.f, 5.f), 0.0f, false, true);
	west_center_wall->SetPos(0.0f, 2.5f, -25.0f);
	primitives.PushBack(west_center_wall);
	//west_center_wall->SetRotation();
	//arena_elements.PushBack(wall1);

	Cube* east_center_wall = new Cube(vec3(18.f, 5.f, 5.f), 0.0f, false, true);
	east_center_wall->SetPos(0.0f, 2.5f, 25.0f);
	primitives.PushBack(east_center_wall);

	// ---------------------------- COLUMNS & CONSTRAINTS ----------------------------
	Cube* top_column = new Cube(vec3(5.0f, 10.0f, 5.0f), 0.0f, false, true);
	top_column->SetPos(20.0f, 5.0f, 20.0f);
	primitives.PushBack(top_column);
	top_column->color = Red;

	/*Cube* */top_constrained_cube = new Cube(vec3(5.0f, 9.0f, 1.0f), 1000.0f, false, true);
	top_constrained_cube->SetPos(26.0f, 5.0f, 20.0f);
	primitives.PushBack(top_constrained_cube);
	top_constrained_cube->color = Blue;

	App->physics->AddConstraintHinge(*top_column, *top_constrained_cube,
		vec3(0.0f, 0.0f, 0.0f), vec3(-6.0f, 0.0f, 0.0f), vec3(0, 1, 0), vec3(0, 1, 0), true);

	//SetCube(vec3(0, 0, 0), vec3(10, 1, 10), 0.0f, false, true, 90, vec3(1, 0, 0));

	/*furniture = new Cube(vec3(5.0f, 9.0f, 1.0f), 1000.0f, false, true);
	furniture->SetPos(26.0f, 5.0f, 20.0f);
	primitives.PushBack(furniture);
	furniture->color = Blue;*/

	// ---------------------------- AMMO PICK-UP SENSORS -----------------------------
	Sphere* ammo_pickup = new Sphere(2.0f, 0.0f, true, true);
	ammo_pickup->SetPos(0.0f, 0.0f, 0.0f);
	primitives.PushBack(ammo_pickup);
	//arena_elements.PushBack(ammo_pickup);
	//ammo_pickup->color = Red;

	Cube* cube = new Cube(vec3(1.f, 1.f, 1.f), 0.0f, true, true);
	cube->SetPos(5.0f, 1.0f, 0.0f);
	primitives.PushBack(cube);
	//arena_elements.PushBack(cube);

	/*Sphere ammo_pickup(1.0f, 0.0f);
	ammo_pickup.SetPos(0.0f, 0.0f, 0.0f);
	primitives.PushBack(&ammo_pickup);
	App->physics->AddBody(ammo_pickup, 0.0f, true);*/
}

void ModuleSceneIntro::SetCube(vec3 position, vec3 size, float mass, bool is_sensor, bool is_environment, float angle, vec3 axis)
{
	furniture = new Cube(size, mass, is_sensor, is_environment);
	furniture->SetPos(position.x, position.y, position.z);
	primitives.PushBack(furniture);

	furniture->SetRotation(angle, axis);

	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
	furniture->color = color;
}

void ModuleSceneIntro::SetSphere(vec3 position, float radius, float mass, bool is_sensor, bool is_environment)
{
	furniture = new Sphere(radius, mass, is_sensor, is_environment);
	furniture->SetPos(position.x, position.y, position.z);
	primitives.PushBack(furniture);

	furniture->color = White;
}

void ModuleSceneIntro::SetCylinder(vec3 position, float radius, float height, float mass, bool is_sensor, bool is_environment, float angle, vec3 axis)
{
	furniture = new Cylinder(radius, height, mass, is_sensor, is_environment);
	furniture->SetPos(position.x, position.y, position.z);
	primitives.PushBack(furniture);

	furniture->SetRotation(angle, axis);

	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
	furniture->color = color;
}

void ModuleSceneIntro::RestartGame()
{
	App->player->RestartPlayer1(vec3(0, 12, 10));
	App->player2->RestartPlayer2(vec3(5, 12, 10));

	for (int i = 0; i < primitives.Count(); i++)									//REVISE THIS
	{
		if (primitives[i]->body.is_sensor == false && primitives[i]->body.is_environment == false)
		{
			App->physics->RemoveBodyFromWorld(primitives[i]->body.GetBody());
			delete primitives[i];
			primitives.Pop(primitives[i]);
		}
	}
	//CleanUp();
}