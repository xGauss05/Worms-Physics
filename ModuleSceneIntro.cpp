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

	test1 = new Body(PIXEL_TO_METERS(500), PIXEL_TO_METERS(300), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, ENEMY, 1.0f, 2.0f, 2.0f);
	test1->texture = App->textures->Load("Assets/Textures/lil_clown.png");
	player = new Body(PIXEL_TO_METERS(140), PIXEL_TO_METERS(500), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, EntityType::PLAYER, 1.0f, 2.0f, 2.0f);
	player->texture = App->textures->Load("Assets/Textures/lil_clown.png");
	Body* newTrampoline = new Body(PIXEL_TO_METERS(300), PIXEL_TO_METERS(585), RECTANGLE, PIXEL_TO_METERS(48), PIXEL_TO_METERS(16), STATIC, EntityType::TRAMPOLINE, 1.0f, 2.0f, 2.0f);
	trampoline1 = new Trampoline();
	trampoline1->body = newTrampoline;
	newTrampoline->texture = App->textures->Load("Assets/Textures/trampoline.png");

	Body* newTrampoline2 = new Body(PIXEL_TO_METERS(640), PIXEL_TO_METERS(585), RECTANGLE, PIXEL_TO_METERS(48), PIXEL_TO_METERS(16), STATIC, EntityType::TRAMPOLINE, 1.0f, 2.0f, 2.0f);
	trampoline2 = new Trampoline();
	trampoline2->body = newTrampoline2;
	newTrampoline2->texture = App->textures->Load("Assets/Textures/trampoline.png");

	glider = App->textures->Load("Assets/Textures/plane.png");

	// Physical objects
	App->physics->world->AddBody(test1);
	App->physics->world->AddBody(player);

	App->physics->world->AddTrampoline(trampoline1, newTrampoline->GetPosition());
	App->physics->world->AddTrampoline(trampoline2, newTrampoline2->GetPosition());

	p2Point<float> force;
	force.x = 0;
	force.y = -10.0;
	test1->ApplyExternalForce(force);
	player->ApplyExternalForce(force);

	//Wall left
	ground1 = new Body(PIXEL_TO_METERS(10), PIXEL_TO_METERS(20), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(500), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground1);

	//Wall left 2
	ground2 = new Body(PIXEL_TO_METERS(10), PIXEL_TO_METERS(650), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(100), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground2);

	//Wall Left
	ground3 = new Body(PIXEL_TO_METERS(85), PIXEL_TO_METERS(700), RECTANGLE, PIXEL_TO_METERS(150), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground3);

	ground4 = new Body(PIXEL_TO_METERS(300), PIXEL_TO_METERS(700), RECTANGLE, PIXEL_TO_METERS(300), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground4);

	ground5 = new Body(PIXEL_TO_METERS(300), PIXEL_TO_METERS(600), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(100), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground5);

	ground6 = new Body(PIXEL_TO_METERS(550), PIXEL_TO_METERS(600), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(100), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground6);

	ground7 = new Body(PIXEL_TO_METERS(600), PIXEL_TO_METERS(600), RECTANGLE, PIXEL_TO_METERS(100), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground7);

	ground8 = new Body(PIXEL_TO_METERS(700), PIXEL_TO_METERS(550), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(100), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground8);

	ground9 = new Body(PIXEL_TO_METERS(750), PIXEL_TO_METERS(550), RECTANGLE, PIXEL_TO_METERS(100), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground9);

	ground10 = new Body(PIXEL_TO_METERS(850), PIXEL_TO_METERS(550), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(200), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground10);

	ground11 = new Body(PIXEL_TO_METERS(135), PIXEL_TO_METERS(100), RECTANGLE, PIXEL_TO_METERS(150), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground11);

	ground12 = new Body(PIXEL_TO_METERS(135), PIXEL_TO_METERS(150), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground12);

	ground13 = new Body(PIXEL_TO_METERS(235), PIXEL_TO_METERS(150), RECTANGLE, PIXEL_TO_METERS(50), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground13);

	ground14 = new Body(PIXEL_TO_METERS(900), PIXEL_TO_METERS(400), RECTANGLE, PIXEL_TO_METERS(150), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground14);

	ground15 = new Body(PIXEL_TO_METERS(700), PIXEL_TO_METERS(250), RECTANGLE, PIXEL_TO_METERS(150), PIXEL_TO_METERS(50), STATIC, EntityType::WALL, 1.0f, 20.0f, 20.0f);
	App->physics->world->AddBody(ground15);

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
		METERS_TO_PIXELS(ground1->position.x),
		METERS_TO_PIXELS(ground1->position.y),
		METERS_TO_PIXELS(ground1->GetWidth()),
		METERS_TO_PIXELS(ground1->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground2->position.x),
		METERS_TO_PIXELS(ground2->position.y),
		METERS_TO_PIXELS(ground2->GetWidth()),
		METERS_TO_PIXELS(ground2->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground3->position.x),
		METERS_TO_PIXELS(ground3->position.y),
		METERS_TO_PIXELS(ground3->GetWidth()),
		METERS_TO_PIXELS(ground3->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground4->position.x),
		METERS_TO_PIXELS(ground4->position.y),
		METERS_TO_PIXELS(ground4->GetWidth()),
		METERS_TO_PIXELS(ground4->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground5->position.x),
		METERS_TO_PIXELS(ground5->position.y),
		METERS_TO_PIXELS(ground5->GetWidth()),
		METERS_TO_PIXELS(ground5->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground6->position.x),
		METERS_TO_PIXELS(ground6->position.y),
		METERS_TO_PIXELS(ground6->GetWidth()),
		METERS_TO_PIXELS(ground6->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground7->position.x),
		METERS_TO_PIXELS(ground7->position.y),
		METERS_TO_PIXELS(ground7->GetWidth()),
		METERS_TO_PIXELS(ground7->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground8->position.x),
		METERS_TO_PIXELS(ground8->position.y),
		METERS_TO_PIXELS(ground8->GetWidth()),
		METERS_TO_PIXELS(ground8->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground9->position.x),
		METERS_TO_PIXELS(ground9->position.y),
		METERS_TO_PIXELS(ground9->GetWidth()),
		METERS_TO_PIXELS(ground9->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground10->position.x),
		METERS_TO_PIXELS(ground10->position.y),
		METERS_TO_PIXELS(ground10->GetWidth()),
		METERS_TO_PIXELS(ground10->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground11->position.x),
		METERS_TO_PIXELS(ground11->position.y),
		METERS_TO_PIXELS(ground11->GetWidth()),
		METERS_TO_PIXELS(ground11->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground12->position.x),
		METERS_TO_PIXELS(ground12->position.y),
		METERS_TO_PIXELS(ground12->GetWidth()),
		METERS_TO_PIXELS(ground12->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground13->position.x),
		METERS_TO_PIXELS(ground13->position.y),
		METERS_TO_PIXELS(ground13->GetWidth()),
		METERS_TO_PIXELS(ground13->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground14->position.x),
		METERS_TO_PIXELS(ground14->position.y),
		METERS_TO_PIXELS(ground14->GetWidth()),
		METERS_TO_PIXELS(ground14->GetHeight()) }, 0, 0, 255);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground15->position.x),
		METERS_TO_PIXELS(ground15->position.y),
		METERS_TO_PIXELS(ground15->GetWidth()),
		METERS_TO_PIXELS(ground15->GetHeight()) }, 0, 0, 255);

	if (App->player->withGlider)
	{
		App->renderer->Blit(glider, METERS_TO_PIXELS(App->player->body->position.x) - 25, METERS_TO_PIXELS(App->player->body->position.y));
	}

	App->physics->world->BlitTrampoline();
	App->physics->world->UpdateTrampoline();

	test1->Blit({ 0, 0, 32, 32 });
	player->Blit({ 32, 0, 32, 32 });
	

	App->physics->world->BlitProjectiles();
	App->physics->world->UpdateProjectiles();

	return UPDATE_CONTINUE;
}
