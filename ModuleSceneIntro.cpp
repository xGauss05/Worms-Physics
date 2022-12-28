#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "ModuleFonts.h"
#include <string>
using namespace std;

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
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



	test1 = new Body(PIXEL_TO_METERS(30), PIXEL_TO_METERS(400), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, 1.0f, 20.0f);
	test1->texture = App->textures->Load("Assets/Textures/lil_clown.png");

	App->physics->world->AddBody(test1);

	p2Point<float> force;
	force.x = 5.0f;
	force.y = 0.0f;
	test1->ApplyExternalForce(force);
	
	groundTest1 = new Body(PIXEL_TO_METERS(220), PIXEL_TO_METERS(100), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(500), STATIC, 1.0f, 20.0f);
	
	App->physics->world->AddBody(groundTest1);

	groundTest2 = new Body(PIXEL_TO_METERS(30), PIXEL_TO_METERS(600), RECTANGLE, PIXEL_TO_METERS(250), PIXEL_TO_METERS(50), STATIC, 1.0f, 20.0f);

	App->physics->world->AddBody(groundTest2);


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
