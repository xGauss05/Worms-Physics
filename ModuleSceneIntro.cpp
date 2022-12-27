#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "ModuleFonts.h"
#include <string>
using namespace std;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	test1 = new Body();
	test1->position.x = PIXEL_TO_METERS(30);
	test1->position.y = PIXEL_TO_METERS(0);
	test1->velocity.x = 0.0f;
	test1->velocity.y = 0.0f;
	test1->acceleration.x = 0.0f;
	test1->acceleration.y = 0.0f;
	test1->width = (2.0f);
	test1->height = 2.0f;
	test1->radius = 0;
	test1->shape = RECTANGLE;
	test1->type = DYNAMIC;
	test1->externalForce.x = 0;
	test1->externalForce.y = 0;
	test1->dragSurface = 20.0f;
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
	test1->Blit();

	App->fonts->BlitText(20, 50, 0, std::to_string(METERS_TO_PIXELS(test1->position.x)).c_str());
	App->fonts->BlitText(20, 60, 0, std::to_string(METERS_TO_PIXELS(test1->position.y)).c_str());
	App->fonts->BlitText(20, 70, 0, std::to_string(test1->velocity.x).c_str());
	App->fonts->BlitText(20, 80, 0, std::to_string(test1->velocity.y).c_str());
	App->fonts->BlitText(20, 90, 0, std::to_string(test1->acceleration.y).c_str());

	return UPDATE_CONTINUE;
}
