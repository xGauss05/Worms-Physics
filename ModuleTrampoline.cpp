#include "Globals.h"
#include "Application.h"
#include "ModuleTrampoline.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include <time.h>
ModuleTrampoline::ModuleTrampoline(bool start_enabled) : Module(start_enabled)
{

}

ModuleTrampoline::~ModuleTrampoline()
{

}

// Load assets
bool ModuleTrampoline::Start()
{
	LOG("Loading trampoline");
	body = App->scene_intro->trampoline1;
	body->SetLocalRestitution(3.0);

	//texture = App->textures->Load("Assets/Textures/trampoline.png");

	return true;
}

// Unload assets
bool ModuleTrampoline::CleanUp()
{
	LOG("Unloading trampoline");

	if (body != nullptr)
	{
		delete body;
		body = nullptr;
	}

	return true;
}

update_status ModuleTrampoline::Update()
{
	

	return UPDATE_CONTINUE;
}
