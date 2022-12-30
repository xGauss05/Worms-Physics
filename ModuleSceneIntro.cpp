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

	winScreen = App->textures->Load("Assets/Textures/win_screen.png");
	loseScreen = App->textures->Load("Assets/Textures/lose_screen.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;
	App->audio->PlayMusic("Assets/Audio/Music/bgm.ogg");
	background = App->textures->Load("Assets/Textures/circus_background.png");

	for (int i = 0; i < 5; i++) {
		clown[i] = new Body(PIXEL_TO_METERS(500), PIXEL_TO_METERS(300), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, ENEMY, 1.0f, 2.0f, 2.0f);
	}
	clown[5] = new Body(PIXEL_TO_METERS(820), PIXEL_TO_METERS(150), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, ENEMY, 1.0f, 2.0f, 2.0f);
	clown[6] = new Body(PIXEL_TO_METERS(820), PIXEL_TO_METERS(150), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, ENEMY, 1.0f, 2.0f, 2.0f);
	clown[7] = new Body(PIXEL_TO_METERS(326), PIXEL_TO_METERS(300), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, ENEMY, 1.0f, 2.0f, 2.0f);
	clown[8] = new Body(PIXEL_TO_METERS(160), PIXEL_TO_METERS(30), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, ENEMY, 1.0f, 2.0f, 2.0f);
	clown[9] = new Body(PIXEL_TO_METERS(660), PIXEL_TO_METERS(270), CIRCLE, PIXEL_TO_METERS(16), DYNAMIC, ENEMY, 1.0f, 2.0f, 2.0f);
	for (int i = 0; i < 10; i++) {
		clown[i]->texture = App->textures->Load("Assets/Textures/lil_clown.png");
	}
	
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

	balloonPos[0].x = PIXEL_TO_METERS(200);	balloonPos[0].y = PIXEL_TO_METERS(20);
	balloonPos[1].x = PIXEL_TO_METERS(200);	balloonPos[1].y = PIXEL_TO_METERS(182);
	balloonPos[2].x = PIXEL_TO_METERS(38);	balloonPos[2].y = PIXEL_TO_METERS(546);
	balloonPos[3].x = PIXEL_TO_METERS(260);	balloonPos[3].y = PIXEL_TO_METERS(560);
	balloonPos[4].x = PIXEL_TO_METERS(428);	balloonPos[4].y = PIXEL_TO_METERS(320);
	balloonPos[5].x = PIXEL_TO_METERS(546);	balloonPos[5].y = PIXEL_TO_METERS(154);
	balloonPos[6].x = PIXEL_TO_METERS(757);	balloonPos[6].y = PIXEL_TO_METERS(370);
	balloonPos[7].x = PIXEL_TO_METERS(775);	balloonPos[7].y = PIXEL_TO_METERS(62);
	balloonPos[8].x = PIXEL_TO_METERS(980);	balloonPos[8].y = PIXEL_TO_METERS(260);
	balloonPos[9].x = PIXEL_TO_METERS(965);	balloonPos[9].y = PIXEL_TO_METERS(495);

	for (int i = 0; i < 10; i++) {
		int colorBalloon = i % 3;
		balloons[i] = new Balloon(colorBalloon, 0, 0, CIRCLE, PIXEL_TO_METERS(16), STATIC, BALLOON, 0.5f, 20.0f, 20.0f);
		balloons[i]->texture = App->textures->Load("Assets/Textures/objects.png");
		balloons[i]->SetLocalRestitution(2.0f);
	}

	glider = App->textures->Load("Assets/Textures/plane.png");

	// Physical objects
	for (int i = 0; i < 10; i++) {
		App->physics->world->AddBody(clown[i]);
	}
	App->physics->world->AddBody(player);

	App->physics->world->AddTrampoline(trampoline1, newTrampoline->GetPosition());
	App->physics->world->AddTrampoline(trampoline2, newTrampoline2->GetPosition());

	for (int i = 0; i < 10; i++) {
		App->physics->world->AddBalloon(balloons[i], balloonPos[i]);
	}

	p2Point<float> force;
	force.x = 0;
	force.y = -10.0;
	for (int i = 0; i < 10; i++) {
		clown[i]->ApplyExternalForce(force);
	}
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

	App->textures->Unload(winScreen);
	App->textures->Unload(loseScreen);
	App->textures->Unload(glider);
	App->textures->Unload(background);

	for (int i = 0; i < 10; i++) {
		delete clown[i];
		clown[i] = nullptr;
		balloons[i]->isAlive = false;
		delete balloons[i];
		balloons[i] = nullptr;
	}

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		App->renderer->camera.x = App->renderer->camera.y = 0;

		for (int i = 0; i < 5; i++) {
			clown[i]->position.x = PIXEL_TO_METERS(500);	clown[i]->position.y = PIXEL_TO_METERS(300);
		}
		clown[5]->position.x = PIXEL_TO_METERS(820);	clown[5]->position.y = PIXEL_TO_METERS(150);
		clown[6]->position.x = PIXEL_TO_METERS(820);	clown[6]->position.y = PIXEL_TO_METERS(150);
		clown[7]->position.x = PIXEL_TO_METERS(326);	clown[7]->position.y = PIXEL_TO_METERS(300);
		clown[8]->position.x = PIXEL_TO_METERS(160);	clown[8]->position.y = PIXEL_TO_METERS(30);
		clown[9]->position.x = PIXEL_TO_METERS(660);	clown[9]->position.y = PIXEL_TO_METERS(270);

		for (int i = 0; i < 10; i++) {
			clown[i]->velocity.SetToZero();
			clown[i]->acceleration.SetToZero();
			clown[i]->externalForce.SetToZero();
		}

		player->position.x = PIXEL_TO_METERS(140);
		player->position.y = PIXEL_TO_METERS(500);
		player->velocity.SetToZero();
		player->acceleration.SetToZero();
		player->externalForce.SetToZero();
		App->player->withGlider = false;
		player->SetDragSurface(2.0f, 2.0f);
		player->SetLocalRestitution(1.0f);
		
		App->physics->world->UnaliveAllProjectiles();
		App->physics->world->UnaliveAllBalloons();

		for (int i = 0; i < 10; i++) {
			balloons[i]->isAlive = false;
			delete balloons[i];
			balloons[i] = nullptr;
		}

		win = false;
		lose = false;
		//App->physics->world->balloonNum = 10;
		// create all the balloons again

		for (int i = 0; i < 10; i++) {
			int colorBalloon = i % 3;
			balloons[i] = new Balloon(colorBalloon, 0, 0, CIRCLE, PIXEL_TO_METERS(16), STATIC, BALLOON, 0.5f, 20.0f, 20.0f);
			balloons[i]->texture = App->textures->Load("Assets/Textures/objects.png");
			balloons[i]->SetLocalRestitution(2.0f);
			App->physics->world->AddBalloon(balloons[i], balloonPos[i]);
		}


		p2Point<float> force;
		force.x = 0;
		force.y = -10.0;
		for (int i = 0; i < 5; i++) {
			clown[i]->ApplyExternalForce(force);
		}
		player->ApplyExternalForce(force);

	}

	if (METERS_TO_PIXELS(player->position.y) > 900) {
		lose = true;
	}

	if (App->physics->world->balloonNum == 0) {
		win = true;
	}

	App->renderer->Blit(background, 0, 0);

	int groundR = 33;
	int groundG = 18;
	int groundB = 95;


	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground1->position.x),
		METERS_TO_PIXELS(ground1->position.y),
		METERS_TO_PIXELS(ground1->GetWidth()),
		METERS_TO_PIXELS(ground1->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground2->position.x),
		METERS_TO_PIXELS(ground2->position.y),
		METERS_TO_PIXELS(ground2->GetWidth()),
		METERS_TO_PIXELS(ground2->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground3->position.x),
		METERS_TO_PIXELS(ground3->position.y),
		METERS_TO_PIXELS(ground3->GetWidth()),
		METERS_TO_PIXELS(ground3->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground4->position.x),
		METERS_TO_PIXELS(ground4->position.y),
		METERS_TO_PIXELS(ground4->GetWidth()),
		METERS_TO_PIXELS(ground4->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground5->position.x),
		METERS_TO_PIXELS(ground5->position.y),
		METERS_TO_PIXELS(ground5->GetWidth()),
		METERS_TO_PIXELS(ground5->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground6->position.x),
		METERS_TO_PIXELS(ground6->position.y),
		METERS_TO_PIXELS(ground6->GetWidth()),
		METERS_TO_PIXELS(ground6->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground7->position.x),
		METERS_TO_PIXELS(ground7->position.y),
		METERS_TO_PIXELS(ground7->GetWidth()),
		METERS_TO_PIXELS(ground7->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground8->position.x),
		METERS_TO_PIXELS(ground8->position.y),
		METERS_TO_PIXELS(ground8->GetWidth()),
		METERS_TO_PIXELS(ground8->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground9->position.x),
		METERS_TO_PIXELS(ground9->position.y),
		METERS_TO_PIXELS(ground9->GetWidth()),
		METERS_TO_PIXELS(ground9->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground10->position.x),
		METERS_TO_PIXELS(ground10->position.y),
		METERS_TO_PIXELS(ground10->GetWidth()),
		METERS_TO_PIXELS(ground10->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground11->position.x),
		METERS_TO_PIXELS(ground11->position.y),
		METERS_TO_PIXELS(ground11->GetWidth()),
		METERS_TO_PIXELS(ground11->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground12->position.x),
		METERS_TO_PIXELS(ground12->position.y),
		METERS_TO_PIXELS(ground12->GetWidth()),
		METERS_TO_PIXELS(ground12->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground13->position.x),
		METERS_TO_PIXELS(ground13->position.y),
		METERS_TO_PIXELS(ground13->GetWidth()),
		METERS_TO_PIXELS(ground13->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground14->position.x),
		METERS_TO_PIXELS(ground14->position.y),
		METERS_TO_PIXELS(ground14->GetWidth()),
		METERS_TO_PIXELS(ground14->GetHeight()) }, groundR, groundG, groundB);

	App->renderer->DrawQuad({
		METERS_TO_PIXELS(ground15->position.x),
		METERS_TO_PIXELS(ground15->position.y),
		METERS_TO_PIXELS(ground15->GetWidth()),
		METERS_TO_PIXELS(ground15->GetHeight()) }, groundR, groundG, groundB);

	if (App->player->withGlider)
	{
		App->renderer->Blit(glider, METERS_TO_PIXELS(App->player->body->position.x) - 25, METERS_TO_PIXELS(App->player->body->position.y));
	}

	App->physics->world->BlitTrampoline();

	for (int i = 0; i < 5; i++) {
		clown[i]->Blit({ 0, 0, 32, 32 });
	}
	clown[5]->Blit({ 64, 0, 32, 32 });
	clown[6]->Blit({ 64, 0, 32, 32 });
	clown[7]->Blit({ 0, 32, 32, 32 });
	clown[8]->Blit({ 32, 32, 32, 32 });
	clown[9]->Blit({ 64, 32, 32, 32 });
	player->Blit({ 32, 0, 32, 32 });
	

	App->physics->world->BlitProjectiles();
	App->physics->world->UpdateProjectiles();

	App->physics->world->BlitBalloon();
	App->physics->world->UpdateBalloon();

	if (win) {
		App->renderer->Blit(winScreen, 0, 0);
	}
	else if (lose) {
		App->renderer->Blit(loseScreen, 0, 0);
	}

	return UPDATE_CONTINUE;
}
