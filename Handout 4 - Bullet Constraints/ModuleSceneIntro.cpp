#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 40.0f, 0.0f));						//Changes both the camera position and its reference point. Set Move to match the vehicle.
	App->camera->LookAt(vec3(0, 0, 0));								//Initial point of reference. Set it to be the vehicle.

	//App->camera->LookAt(App->player->vehicle->GetTransform());
	/*vec3 vehiclePos;
	App->player->vehicle->GetPos(vehiclePos);
	
	App->camera->LookAt(vec3(vehiclePos.x, 0, 0));*/

	//const int SnakeLength = 7;
	//const float StartingSize = 0.5f;
	//const float SizeIncrement = 0.2f;
	//const float BallDistance = 0.3f;

	//float XPos = 0.f;
	//float Size = StartingSize;

	//Sphere* previousSphere = nullptr;		//Buffer where the previously iterated sphere will be kept.
	//float prevSize = StartingSize;			//Stores the previous sphere's size.

	//for (int n = 0; n < SnakeLength; n++)
	//{
	//	Sphere* s = new Sphere(Size);
	//	primitives.PushBack(s);
	//	s->SetPos(XPos, 10.f, 2.5f);

	//	//int sphereMidPosX = Size + (BallDistance / 2);			//Position in X of the center between 2 spheres in this setup.
	//	btVector3 center = { Size + (BallDistance / 2), 0, 0 };		//Center vector between 2 spheres.

	//	btScalar prevCenterDist = (prevSize + (BallDistance / 2));
	//	btScalar centerDist = (Size + (BallDistance / 2));

	//	//TODO 2: Link all the spheres with your P2P constraints
	//	if (primitives.Count() > 1 && previousSphere != nullptr)		//If there are more than 2 primitives in the primitives list and previousSphere is not NULL.
	//	{
	//		//App->physics->AddConstraintP2P(*s, *previousSphere, center, -center);
	//		App->physics->AddConstraintP2P(*s, *previousSphere, 
	//			vec3(centerDist, 0, 0), vec3(-prevCenterDist, 0, 0));
	//	}

	//	previousSphere = s;					//The new sphere is set in the buffer.
	//	prevSize = Size;					//Previous size of the 

	//	XPos += Size + Size + SizeIncrement + BallDistance;
	//	Size += SizeIncrement;
	//}

	////TODO 4: Link some other spheres with your Hinge constraint
	////Variables for the hinge constraint spheres. The first letter of each variable's name is in lower case.
	//const float startingSize = 0.5f;
	//const float sizeIncrement = 0.2f;
	//const float ballDistance = 0.3f;

	//float xPos = 0.f;
	//float size = startingSize;

	//Sphere* previousHinge = nullptr;
	//float previousSize = startingSize;

	//for (int n = 0; n < SnakeLength; n++)
	//{
	//	Sphere* hinge_S = new Sphere(size);
	//	primitives.PushBack(hinge_S);
	//	hinge_S->SetPos(xPos, 10.f, 10.0f);

	//	//btScalar centerDist = (previousSize + ballDistance + size) / 2;
	//	btScalar prevCenterDist = (previousSize + (ballDistance / 2));
	//	btScalar centerDist = (size + (ballDistance / 2));

	//	//TODO 2: Link all the spheres with your P2P constraints
	//	if (previousHinge != nullptr)			//If there are more than 2 primitives in the primitives list and previousSphere is not NULL.
	//	{
	//		App->physics->AddConstraintHinge(*hinge_S, *previousHinge, vec3(centerDist, 0, 0),
	//			vec3(-prevCenterDist, 0, 0), vec3(0, 1, 0), vec3(0, 1, 0));
	//	}

	//	previousHinge = hinge_S;					//The new sphere is set in the buffer.
	//	previousSize = size;						//The previousSize is set with the size of the current sphere.

	//	xPos += size + size + sizeIncrement + ballDistance;
	//	size += sizeIncrement;
	//}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		DebugSpawnPrimitive(new Sphere());
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		DebugSpawnPrimitive(new Cube(/*vec3(1.f, 1.f, 1.f)*/));				//REVISE THIS. Should be just new Cube(). Check how to disambiguate a class constructor function when creating "new"s (and everything else).
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		DebugSpawnPrimitive(new Cylinder());
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		for (uint n = 0; n < primitives.Count(); n++)
			primitives[n]->body.Push(vec3((float)(std::rand() % 500) - 250, 500, (float)(std::rand() % 500) - 250));

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//TODO: NEW CODE
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
	//p->body.collision_listeners.PushBack(this);
	p->body.collision_listeners.add(this);
	p->body.Push(-App->camera->Z * 1000.f);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	vec3 P1_position = App->player->P1vehicle->GetPos();								//Gets the current position of Player 1.
	vec3 P2_position = App->player2->P2vehicle->GetPos();							//Gets the current position of Player 2.

	vec3 midPos = {																	//Position vector that is set to be at the center poisition of both players.
		(P1_position.x + P2_position.x) * HALF,										//MidPos in the X axis.
		0,																			//MidPos in the Y axis.
		(P1_position.z + P2_position.z) * HALF										//MidPos in the Z axis.
	};

	float camZoom = GetZoom();

	vec3 avgPosition = { midPos.x + CAM_OFFSET, camZoom, midPos.z };
	vec3 avgRefPoint = { (P1_position + P2_position) * HALF };						//Calculates the position of the point of reference of the camera. Set to be at the center of both players.

	vec3 cameraPosition = { App->camera->Position.x, App->camera->Position.y, App->camera->Position.z };
	
	if (!App->debug)																//If debug mode is on, the camera is freed and can be controlled with the mouse.
	{
		App->camera->Position = (avgPosition);									//Changes both the camera position and its reference point. Set Move to match the vehicle. OFFSET on x --> Horizontal, OFFSET on z --> Vertical.

		//LerpCamera(cameraPosition, avgPosition);
		
		App->camera->LookAt(avgRefPoint);											//LookAt cannot look  at the same position the camera is. There needs to be an offset somewhere.
	}

	//LOG("Vehicle pos (%f %f %f)", P1_position.x, P1_position.y, P1_position.z);
	//LOG("Camera pos: (%f %f %f)", App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	
	if (App->debug == true)
		HandleDebugInput();

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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
	Color color = Color((float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f, (float)(std::rand() % 255) / 255.f);

	body1->parentPrimitive->color = color;
	body2->parentPrimitive->color = color;

}

//Gets the amount of zoom required taking into account the distance between players (ratio).
float ModuleSceneIntro::GetZoom() const
{
	vec3 P1_position = App->player->P1vehicle->GetPos();								//Gets the current position of Player 1.
	vec3 P2_position = App->player2->P2vehicle->GetPos();							//Gets the current position of Player 2.

	float distanceNoSqrt = (P1_position.x * P2_position.x) + (P1_position.z * P2_position.z);

	//LOG("Distance %.2f: ", distanceNoSqrt);

	//Zoom Limits
	/*if (distanceNoSqrt < 0)
	{
		distanceNoSqrt = distanceNoSqrt * (-1);
	}*/

	/*if (distanceNoSqrt < 500)
	{
		distanceNoSqrt = 500;
	}*/

	/*if (distanceNoSqrt > 2000)
	{
		distanceNoSqrt = 2000;
	}*/

	float cameraZoom = distanceNoSqrt * 0.1f;

	if (cameraZoom < 0)
	{
		cameraZoom = cameraZoom * (-1);
	}

	if (cameraZoom < 50)
	{
		cameraZoom = 50;
	}

	if (cameraZoom > 200)
	{
		cameraZoom = 200;
	}

	return cameraZoom;
}

float ModuleSceneIntro::GetLerpSpeed(vec3 position, vec3 target, float speed) const
{
	float pos = sqrt(position.x * position.x + position.y * position.y + position.z * position.z);

	float posTarget = sqrt(target.x * target.x + target.y * target.y + target.z * target.z);

	float lerpSpeed = (posTarget - pos) * speed;

	return lerpSpeed;
	//return pos + lerpSpeed;
}

void ModuleSceneIntro::LerpCamera(vec3 cameraPosition, vec3 targetPosition)
{
	if (App->camera->Position.x < targetPosition.x && App->camera->Position.z < targetPosition.z);
	{
		App->camera->Position += GetLerpSpeed(cameraPosition, targetPosition, 0.025f);
	}
	if (App->camera->Position.x > targetPosition.x && App->camera->Position.z > targetPosition.z)
	{
		App->camera->Position -= GetLerpSpeed(cameraPosition, targetPosition, 0.025f);
	}
}