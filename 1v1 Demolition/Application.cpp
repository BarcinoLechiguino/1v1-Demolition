#include "Application.h"

// --------------------------------------------------
// --------------------------------------------------

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f), frame_count(0)
{
	window			= new ModuleWindow(this);
	input			= new ModuleInput(this);
	audio			= new ModuleAudio(this);
	scene_intro		= new ModuleSceneIntro(this);
	renderer3D		= new ModuleRenderer3D(this);
	camera			= new ModuleCamera3D(this);
	physics			= new ModulePhysics3D(this);
	player			= new ModulePlayer(this);
	player2			= new ModulePlayer2(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(player);
	AddModule(player2);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();
	
	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

// --------------------------------------------
// --------------------------------------------

bool Application::Init()
{
	bool ret = true;

	App = this;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}
	
	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	startup_timer.Start();
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float avg_fps = frame_count / startup_timer.ReadSec();			//Equals seconds to the returning value of the ReadSec() method, which returns the amount of time passed in seconds. Use timer->ReadSec() to have no decimals (as its a low resolution timer)
	
	const char* gameTitle = "1v1 Demolition";

	char title[256];
	sprintf_s(title, "%s / Av.FPS: %.2f /// P1 Ammo: %d / P1 Lives: %d / P1 Rounds Won: %d / P1 Wins: %d /// P2 Ammo: %d / P2 Lives: %d / P2 RoundsWon: %d / P2 Wins: %d ///",
		gameTitle, avg_fps, App->player->ammo, App->player->lives, App->player->roundsWonP1, App->player->winsP1,
		App->player2->ammo, App->player2->lives, App->player2->roundsWonP2 , App->player2->winsP2);
	App->window->SetTitle(title);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
	//modules.push_back(mod);			//modules pushback to vector.
}

Application* App = nullptr;
