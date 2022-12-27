#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "ModuleFonts.h"
#include <string>
using namespace std;

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

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("Assets/Textures/circus_background.png");



	test1 = new Body(PIXEL_TO_METERS(30), PIXEL_TO_METERS(700), RECTANGLE, 2.0f, 2.0f, DYNAMIC, 1.0f, 20.0f);
	test1->texture = App->textures->Load("Assets/Textures/lil_clown.png");

	App->physics->world->AddBody(test1);

	p2Point<float> force;
	force.x = 2.0f;
	force.y = -20.0f;
	test1->ApplyExternalForce(force);
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	delete test1;
	test1 = nullptr;

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0);

	test1->Blit();

	return UPDATE_CONTINUE;
}
