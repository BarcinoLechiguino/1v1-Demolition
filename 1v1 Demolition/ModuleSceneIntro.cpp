#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "ModuleRenderer3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
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

	InitGame();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int i = 0; i < primitives.Count(); i++)
	{
		DeletePrimitive(primitives[i]);
	}

	ClearConstraintElements();
	
	ClearDisplayElements();

	primitives.Clear();
	LoadArena();

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


	CheckProjectileCount();										// Checking the Projectile Count.
	
	CheckWins();											// Checking Victory Conditions
	
	ApplyTorqueToConstrainedElements();							// Applying torque to the arena's constraints.

	
	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Update();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		RestartEverything();
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

void ModuleSceneIntro::CameraMovement()
{
	vec3 P1_position = App->player->P1vehicle->GetPos();						//Gets the current position of Player 1.
	vec3 P2_position = App->player2->P2vehicle->GetPos();						//Gets the current position of Player 2.

	vec3 midPos = {																//Position vector that is set to be at the center poisition of both players.
		(P1_position.x + P2_position.x) * HALF,									//MidPos in the X axis.
		0,																		//MidPos in the Y axis.
		(P1_position.z + P2_position.z) * HALF									//MidPos in the Z axis.
	};

	float camZoom = GetZoom();

	vec3 avgPosition = { midPos.x + CAM_OFFSET, camZoom, midPos.z };
	vec3 avgRefPoint = (P1_position + P2_position) * HALF;						//Calculates the position of the point of reference of the camera. Set to be at the center of both players.

	App->camera->Position = (avgPosition);										//Changes both the camera position and its reference point. Set Move to match the vehicle. OFFSET on x --> Horizontal, OFFSET on z --> Vertical.
	App->camera->LookAt(avgRefPoint);											//LookAt cannot look  at the same position the camera is. There needs to be an offset somewhere.
	//LerpCamera(cameraPosition, avgPosition);
}

//Gets the amount of zoom required taking into account the distance between players (ratio).
float ModuleSceneIntro::GetZoom() const
{
	vec3 P1_position = App->player->P1vehicle->GetPos();						//Gets the current position of Player 1.
	vec3 P2_position = App->player2->P2vehicle->GetPos();						//Gets the current position of Player 2.

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

// --- Spawns a Primitive if an input is pressed in debug mode.
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

void ModuleSceneIntro::DeleteBody(Primitive* p)
{
	vec3 farAway(300, -300, 300);

	if (p != nullptr && p->body.HasBody())
	{
		p->SetPos(farAway.x, farAway.y, farAway.z);
		p->body.is_environment = false;
		p = nullptr;
	}
}

void ModuleSceneIntro::AddTorque(Primitive* p, vec3 torque)
{
	p->body.GetBody()->applyTorque(btVector3(torque.x, torque.y, torque.z));
}

Cube* ModuleSceneIntro::SetCube(const vec3& position, const vec3& size, float mass, float angle, const vec3& axis, bool is_sensor, bool is_environment)
{
	furniture = new Cube(size, mass, is_sensor, is_environment);										//Creates a cube primitive (and its body at Cube's class constructor).
	furniture->SetPos(position.x, position.y, position.z);												//Sets the position of the element in the world.
	primitives.PushBack(furniture);																		//Adds the new element to the primitives array.

	furniture->SetRotation(angle, axis);																//Sets the amount of rotation that the element will have.

	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
	furniture->color = color;																			//Sets the element's colour.

	return (Cube*)furniture;
}

Sphere* ModuleSceneIntro::SetSphere(const vec3& position, float radius, float mass, bool is_sensor, bool is_environment)
{
	furniture = new Sphere(radius, mass, is_sensor, is_environment);									//Creates a sphere primitive (and its body at Sphere's class constructor).
	furniture->SetPos(position.x, position.y, position.z);												//Sets the position of the element in the world.
	primitives.PushBack(furniture);																		//Adds the new element to the primitives array.

	furniture->color = White;																			//Sets the element's colour.

	return (Sphere*)furniture;
}

Cylinder* ModuleSceneIntro::SetCylinder(const vec3& position, float radius, float height, float mass, float angle, const vec3& axis, bool is_sensor, bool is_environment)
{
	furniture = new Cylinder(radius, height, mass, is_sensor, is_environment);							//Creates a cylinder primitive (and its body at Cylinder's class constructor).
	furniture->SetPos(position.x, position.y, position.z);												//Sets the position of the element in the world.
	primitives.PushBack(furniture);																		//Adds the new element to the primitives array.

	furniture->SetRotation(angle, axis);																//Sets the amount of rotation that the element will have.

	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);
	furniture->color = color;																			//Sets the element's colour.

	return (Cylinder*)furniture;
}

// ------------------------------------------ GAME LOGIC & ARENA ------------------------------------------
void ModuleSceneIntro::InitGame()
{
	App->audio->PlayMusic("audio/Songs/Main_Theme.ogg");

	//App->renderer3D->skyBoxColor = vec3(1.f, 1.f, 1.f);

	App->camera->Move(vec3(1.0f, 40.0f, 0.0f));						//Changes both the camera position and its reference point. Set Move to match the vehicle.
	App->camera->LookAt(vec3(0, 0, 0));								//Initial point of reference. Set it to be the vehicle.

	// --- Initializing the Arena's Constraints
	furniture			= nullptr;
	
	centerLeft_Rotor	= nullptr;
	centerRight_Rotor	= nullptr;
	northWest_Rotor		= nullptr;
	southEast_Rotor		= nullptr;

	// --- Initializing the round wins and game wins display cubes.
	roundWinCube1_P1	= nullptr; 
	roundWinCube2_P1	= nullptr;
	roundWinCube3_P1	= nullptr;
	gameWinCube1_P1		= nullptr;
	gameWinCube2_P1		= nullptr;
	gameWinCube3_P1		= nullptr;

	roundWinCube1_P2	= nullptr;
	roundWinCube2_P2	= nullptr;
	roundWinCube3_P2	= nullptr;
	gameWinCube1_P2		= nullptr;																			
	gameWinCube2_P2		= nullptr;
	gameWinCube3_P2		= nullptr;

	// --- Initializing the RespawnOnNewRound
	RespawnOnRoundEnd	= true;

	// --- Initializing Projectile Count
	projectileCount = 0;

	// --- Loading all the Arena's environment elements.
	LoadArena();
}

void ModuleSceneIntro::LoadArena()
{
	// ---------------------------------- GROUND -----------------------------------
	SetCylinder(vec3(0.0f, -1.0f, 0.0f), 80.f, 2.1f, 0.0f, 90, vec3(0, 0, 1), false, true);											//Arena's ground.

	// ----------------------------------- WALLS -----------------------------------
	// --- Walls at the center of the arena.
	SetCube(vec3(-25.0f, 2.5f, 0.0f), vec3(5.f, 5.f, 18.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//North Center Wall.
	SetCube(vec3(25.0f, 2.5f, 0.0f), vec3(5.f, 5.f, 18.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//South Center Wall.
	SetCube(vec3(0.0f, 2.5f, -25.0f), vec3(18.f, 5.f, 5.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//West Center Wall.
	SetCube(vec3(0.0f, 2.5f, 25.0f), vec3(18.f, 5.f, 5.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//East Center Wall.

	// --- Arena's bounds
	SetCube(vec3(-75.0f, 3.5f, 0.0f), vec3(7.f, 7.1f, 65.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//North Border Wall.
	SetCube(vec3(75.0f, 3.5f, 0.0f), vec3(7.f, 7.1f, 65.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//South Border Wall.
	SetCube(vec3(0.0f, 3.5f, -75.0f), vec3(65.f, 7.1f, 7.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//West Border Wall.
	SetCube(vec3(0.0f, 3.5f, 75.0f), vec3(65.f, 7.1f, 7.f), 0.0f, 0, vec3(1, 0, 0), false, true);									//East Border Wall.

	SetCube(vec3(-53.0f, 3.5f, 53.0f), vec3(7.f, 7.f, 65.f), 0.0f, 45, vec3(0, 1, 0), false, true);									//North-West Border Wall.
	SetCube(vec3(53.0f, 3.5f, 53.0f), vec3(7.f, 7.f, 65.f), 0.0f, 45, vec3(0, -1, 0), false, true);									//South-West Border Wall.
	SetCube(vec3(-53.0f, 3.5f, -53.0f), vec3(65.f, 7.f, 7.f), 0.0f, 45, vec3(0, 1, 0), false, true);								//North-East Border Wall.
	SetCube(vec3(53.0f, 3.5f, -53.0f), vec3(65.f, 7.f, 7.f), 0.0f, 45, vec3(0, -1, 0), false, true);								//South-East Border Wall.

	// ---------------------------- COLUMNS & CONSTRAINTS ----------------------------
	// --- Columns
	Cube* centerLeft_column = SetCube(vec3(-20.0f, 5.0f, 20.0f), vec3(5.0f, 10.0f, 5.0f), 0.0f, 0, vec3(1, 0, 0), false, true);		//Center Right Column.
	centerLeft_Rotor = SetCube(vec3(-27.0f, 5.0f, 20.0f), vec3(5.0f, 9.0f, 1.0f), 1000.0f, 0, vec3(1, 0, 0), false, true);			//Center Right Column Rotor.

	Cube* centerRight_column = SetCube(vec3(20.0f, 5.0f, -20.0f), vec3(5.0f, 10.0f, 5.0f), 0.0f, 0, vec3(1, 0, 0), false, true);	//Center Left Column.
	centerRight_Rotor = SetCube(vec3(27.0f, 5.0f, -20.0f), vec3(5.0f, 9.0f, 1.0f), 1000.0f, 0, vec3(1, 0, 0), false, true);			//Center Left Column Rotor.

	Cube* northWest_column = SetCube(vec3(-37.0f, 5.0f, 37.0f), vec3(5.0f, 10.0f, 5.0f), 0.0f, 0, vec3(1, 0, 0), false, true);		//North-West Column.
	northWest_Rotor = SetCube(vec3(-44.0f, 5.0f, 37.0f), vec3(5.0f, 9.0f, 1.0f), 1000.0f, 0, vec3(1, 0, 0), false, true);			//North-West Column Rotor.

	Cube* southEast_column = SetCube(vec3(37.0f, 5.0f, -37.0f), vec3(5.0f, 10.0f, 5.0f), 0.0f, 0, vec3(1, 0, 0), false, true);		//South-East Column.
	southEast_Rotor = SetCube(vec3(44.0f, 5.0f, -37.0f), vec3(5.0f, 9.0f, 1.0f), 1000.0f, 0, vec3(1, 0, 0), false, true);			//South-East Column Rotor.

	// --- Constraints
	App->physics->AddConstraintHinge(*centerLeft_column, *centerLeft_Rotor,															//Center Right Column Constraint.
		vec3(0.0f, 0.0f, 0.0f), vec3(-7.0f, 0.0f, 0.0f), vec3(0, 1, 0), vec3(0, 1, 0), true);

	App->physics->AddConstraintHinge(*centerRight_column, *centerRight_Rotor,														//Center Left Column Constraint.
		vec3(0.0f, 0.0f, 0.0f), vec3(-7.0f, 0.0f, 0.0f), vec3(0, 1, 0), vec3(0, 1, 0), true);

	App->physics->AddConstraintHinge(*northWest_column, *northWest_Rotor,															//North-West Column Constraint.
		vec3(0.0f, 0.0f, 0.0f), vec3(-7.0f, 0.0f, 0.0f), vec3(0, 1, 0), vec3(0, 1, 0), true);

	App->physics->AddConstraintHinge(*southEast_column, *southEast_Rotor,
		vec3(0.0f, 0.0f, 0.0f), vec3(-7.0f, 0.0f, 0.0f), vec3(0, 1, 0), vec3(0, 1, 0), true);

	// ---------------------------- AMMO PICK-UP SENSORS -----------------------------
	SetSphere(vec3(0.0f, 0.0f, 0.0f), 2.0f, 0.0f, true, true);																		//Ammo Pick-up at the Arena's center.		
	SetSphere(vec3(58.0f, 0.0f, 0.0f), 2.0f, 0.0f, true, true);																		//North Ammo Pick-up.
	SetSphere(vec3(-58.0f, 0.0f, 0.0f), 2.0f, 0.0f, true, true);																	//South Ammo Pick-up.
	SetSphere(vec3(0.0f, 0.0f, 58.0f), 2.0f, 0.0f, true, true);																		//West Ammo Pick-up.
	SetSphere(vec3(0.0f, 0.0f, -58.0f), 2.0f, 0.0f, true, true);																	//East Ammp Pick-up.


	// ----------------------------- ROUND WINS DISPLAY ------------------------------
	RoundWinsDisplay();
	GameWinsDisplay();
}

void ModuleSceneIntro::ApplyTorqueToConstrainedElements()
{
	vec3 constraintTorque(0.0f, ROTOR_TORQUE, 0.0f);													//Vector that is set with the amount of torque and in which axis it will be applied.
	
	AddTorque(centerLeft_Rotor, -constraintTorque);														//Applying torque to the Center-Left Rotor.
	AddTorque(centerRight_Rotor, -constraintTorque);													//Applying torque to the Center-Right Rotor.
	AddTorque(northWest_Rotor, constraintTorque);														//Applying torque to the North-West Rotor.
	AddTorque(southEast_Rotor, constraintTorque);														//Applying torque to the South-East Rotor.
}

void ModuleSceneIntro::ClearConstraintElements()
{
	App->physics->DeleteConstraints();																	//Deletes all constraints existing in the physics world.

	DeleteBody(centerLeft_Rotor);															//Deleting the element constrained to the Center-Left column.
	DeleteBody(centerRight_Rotor);															//Deleting the element constrained to the Center-Right column.
	DeleteBody(northWest_Rotor);																//Deleting the element constrained to the North-West column.
	DeleteBody(southEast_Rotor);																//Deleting the element constrained to the South-East column.
}

void ModuleSceneIntro::CheckProjectileCount()
{
	if (projectileCount > MAX_PROJECTILES)																//If Count is > MAX_PROJECTILES, all projectiles are cleared.
	{
		ClearProjectiles();
	}
}

void ModuleSceneIntro::ClearProjectiles()
{
	for (int i = 0; i < primitives.Count(); i++)														//Deletes all currently existing projectiles when the MAX_PROJECTILE cap is reached.
	{
		DeletePrimitive(primitives[i]);
	}

	projectileCount = 0;
}

void ModuleSceneIntro::CheckWins()
{
	if (App->player->roundsWonP1 == 3 || App->player2->roundsWonP2 == 3)
	{
		if (App->player->roundsWonP1 == 3)
		{
			App->player->winsP1++;																		//Adds a win to P1

			App->player->roundsWonP1 = 0;
			App->player2->roundsWonP2 = 0;
			App->audio->PlayFx(9, 0);
		}

		if (App->player2->roundsWonP2 == 3)
		{
			App->player2->winsP2++;																		//Adds a win to P2

			App->player->roundsWonP1 = 0;
			App->player2->roundsWonP2 = 0;
			App->audio->PlayFx(10, 0);
		}

		if (App->player->winsP1 > 3 || App->player2->winsP2 > 3)
		{
			App->player->winsP1		= 0;
			App->player2->winsP2	= 0;
		}
		
		RestartGame();
	}
}

void ModuleSceneIntro::RoundWinsDisplay()
{
	//float roundPos = 65.0f;
	float roundPos = 70.0f;
	
	// --- PLAYER 1'S ROUND WIN CUBES
	if (App->player->roundsWonP1 == 1)
	{
		roundWinCube1_P1 = SetCube(vec3(roundPos, 3.5f, 75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's First Win Cube
		roundWinCube1_P1->color = Blue;
	}

	if (App->player->roundsWonP1 == 2)
	{
		roundWinCube2_P1 = SetCube(vec3(roundPos, 3.5f, 84.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's Second Win Cube
		roundWinCube2_P1->color = Blue;

	}

	if (App->player->roundsWonP1 == 3)
	{
		roundWinCube3_P1 = SetCube(vec3(roundPos, 3.5f, 93.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's Third Win Cube
		roundWinCube3_P1->color = Blue;
	}

	// --- PLAYER 2'S ROUND WIN CUBES
	if (App->player2->roundsWonP2 == 1)
	{
		roundWinCube1_P2 = SetCube(vec3(-65.0f, 3.5f, -75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's First Win Cube
		roundWinCube1_P2->color = Red;
	}

	if (App->player2->roundsWonP2 == 2)
	{
		roundWinCube2_P2 = SetCube(vec3(-65.0f, 3.5f, -84.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's Second Win Cube
		roundWinCube2_P2->color = Red;

	}

	if (App->player2->roundsWonP2 == 3)
	{
		roundWinCube3_P2 = SetCube(vec3(-65.0f, 3.5f, -93.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's Third Win Cube
		roundWinCube3_P2->color = Red;
	}
}

void ModuleSceneIntro::GameWinsDisplay()
{
	//float winPos = 75.0f;
	float winPos = 60.0f;
	
	// --- PLAYER 1's GAME WIN CUBES
	if (App->player->winsP1 == 1)
	{
		gameWinCube1_P1 = SetCube(vec3(winPos, 3.5f, 75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's First Win Cube
		gameWinCube1_P1->color = Cyan;
	}

	if (App->player->winsP1 == 2)
	{
		gameWinCube1_P1 = SetCube(vec3(winPos, 3.5f, 75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's First Win Cube
		gameWinCube1_P1->color = Cyan;
		
		gameWinCube2_P1 = SetCube(vec3(winPos, 3.5f, 84.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's Second Win Cube
		gameWinCube2_P1->color = Cyan;

	}

	if (App->player->winsP1 >= 3)
	{
		gameWinCube1_P1 = SetCube(vec3(winPos, 3.5f, 75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's First Win Cube
		gameWinCube1_P1->color = Cyan;

		gameWinCube2_P1 = SetCube(vec3(winPos, 3.5f, 84.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's Second Win Cube
		gameWinCube2_P1->color = Cyan;
		
		gameWinCube3_P1 = SetCube(vec3(winPos, 3.5f, 93.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P1's Third Win Cube
		gameWinCube3_P1->color = Cyan;
	}

	// --- PLAYER 2's GAME WIN CUBES
	if (App->player2->winsP2 == 1)
	{
		gameWinCube1_P2 = SetCube(vec3(-75.0f, 3.5f, -75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's First Win Cube
		gameWinCube1_P2->color = Magenta;
	}

	if (App->player2->winsP2 == 2)
	{
		gameWinCube1_P2 = SetCube(vec3(-75.0f, 3.5f, -75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's First Win Cube
		gameWinCube1_P2->color = Magenta;
		
		gameWinCube2_P2 = SetCube(vec3(-75.0f, 3.5f, -84.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's Second Win Cube
		gameWinCube2_P2->color = Magenta;

	}

	if (App->player2->winsP2 >= 3)
	{
		gameWinCube1_P2 = SetCube(vec3(-75.0f, 3.5f, -75.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's First Win Cube
		gameWinCube1_P2->color = Magenta;

		gameWinCube2_P2 = SetCube(vec3(-75.0f, 3.5f, -84.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's Second Win Cube
		gameWinCube2_P2->color = Magenta;
		
		gameWinCube3_P2 = SetCube(vec3(-75.0f, 3.5f, -93.0f), vec3(7.f, 7.1f, 7.1f), 0.0f, 0, vec3(1, 0, 0), false, true);		//P2's Third Win Cube
		gameWinCube3_P2->color = Magenta;
	}
}

void ModuleSceneIntro::ClearDisplayElements()
{
	// --- P1'S DISPLAY ELEMENTS
	DeleteBody(roundWinCube1_P1);
	DeleteBody(roundWinCube2_P1);
	DeleteBody(roundWinCube3_P1);
	DeleteBody(gameWinCube1_P1);
	DeleteBody(gameWinCube2_P1);
	DeleteBody(gameWinCube3_P1);
	
	// --- P2'S DISPLAY ELEMENTS
	DeleteBody(roundWinCube1_P2);
	DeleteBody(roundWinCube2_P2);
	DeleteBody(roundWinCube3_P2);
	DeleteBody(gameWinCube1_P2);
	DeleteBody(gameWinCube2_P2);
	DeleteBody(gameWinCube3_P2);
}

// --- This method starts a new game when a player reaches 3 
void ModuleSceneIntro::RestartGame()
{
	App->player->RestartPlayer1(App->player->spawnPoint);
	App->player2->RestartPlayer2(App->player2->spawnPoint);

	CleanUp();
}

// --- This method restarts everything. Used with the R debug Key to restart the game.
void ModuleSceneIntro::RestartEverything()
{
	App->player->RestartPlayer1(App->player->spawnPoint);
	App->player2->RestartPlayer2(App->player2->spawnPoint);

	App->player->roundsWonP1	= 0;
	App->player->winsP1			= 0;
	App->player2->roundsWonP2	= 0;
	App->player2->winsP2		= 0;

	CleanUp();
}