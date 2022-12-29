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
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("Assets/Textures/circus_background.png");

	test1 = new Body(PIXEL_TO_METERS(500), PIXEL_TO_METERS(300), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, 1.0f, 2.0f, 2.0f);
	test1->texture = App->textures->Load("Assets/Textures/lil_clown.png");
	player = new Body(PIXEL_TO_METERS(300), PIXEL_TO_METERS(300), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, 1.0f, 2.0f, 2.0f);
	player->texture = App->textures->Load("Assets/Textures/lil_clown.png");

	glider = App->textures->Load("Assets/Textures/plane.png");

	// Physical objects
	App->physics->world->AddBody(test1);
	App->physics->world->AddBody(player);

	p2Point<float> force;
	force.x = 0;
	force.y = -10.0;
	test1->ApplyExternalForce(force);
	player->ApplyExternalForce(force);
	
	//Wall Right
	groundTest1 = new Body(PIXEL_TO_METERS(800), PIXEL_TO_METERS(100), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(500), STATIC, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(groundTest1);

	//Ground
	groundTest2 = new Body(PIXEL_TO_METERS(30), PIXEL_TO_METERS(600), RECTANGLE, PIXEL_TO_METERS(800), PIXEL_TO_METERS(50), STATIC, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(groundTest2);

	//Wall Left
	groundTest3 = new Body(PIXEL_TO_METERS(30), PIXEL_TO_METERS(100), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(500), STATIC, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(groundTest3);


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
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		App->renderer->camera.x = App->renderer->camera.y = 0;

		test1->position.x = PIXEL_TO_METERS(500);
		test1->position.y = PIXEL_TO_METERS(300);
		test1->velocity.SetToZero();
		test1->acceleration.SetToZero();
		test1->externalForce.SetToZero();

		player->position.x = PIXEL_TO_METERS(300);
		player->position.y = PIXEL_TO_METERS(300);
		player->velocity.SetToZero();
		player->acceleration.SetToZero();
		player->externalForce.SetToZero();

		p2Point<float> force;
		force.x = 0;
		force.y = -10.0;
		test1->ApplyExternalForce(force);
		player->ApplyExternalForce(force);

	}

	App->renderer->Blit(background, 0, 0);

	App->renderer->DrawQuad({ 
		METERS_TO_PIXELS(groundTest1->position.x), 
		METERS_TO_PIXELS(groundTest1->position.y), 
		METERS_TO_PIXELS(groundTest1->GetWidth()), 
		METERS_TO_PIXELS(groundTest1->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({ 
		METERS_TO_PIXELS(groundTest2->position.x),
		METERS_TO_PIXELS(groundTest2->position.y),
		METERS_TO_PIXELS(groundTest2->GetWidth()),
		METERS_TO_PIXELS(groundTest2->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({ 
		METERS_TO_PIXELS(groundTest3->position.x),
		METERS_TO_PIXELS(groundTest3->position.y),
		METERS_TO_PIXELS(groundTest3->GetWidth()),
		METERS_TO_PIXELS(groundTest3->GetHeight()) }, 0, 0, 255);

	if (App->player->withGlider)
	{
		App->renderer->Blit(glider, METERS_TO_PIXELS(App->player->body->position.x) - 25, METERS_TO_PIXELS(App->player->body->position.y));
	}

	test1->Blit({ 0, 0, 32, 32 });
	player->Blit({ 32, 0, 32, 32 });

	App->physics->world->BlitProjectiles();

	return UPDATE_CONTINUE;
}
