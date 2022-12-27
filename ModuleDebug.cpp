#include "ModuleDebug.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include <string>
using namespace std;

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled) {
	debug = false;
}
ModuleDebug::~ModuleDebug() {
}

bool ModuleDebug::Start() {
	debug = false;

	font = App->fonts->Load("Assets/Textures/font.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,&!- ", 1);
	
	return true;
}

update_status ModuleDebug::Update() {

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	App->fonts->BlitText(20, 20, 0, "HOLA ");

	if (debug) {

		#pragma region Menu navigation

		if (currentScreen == Screen::HOME)
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				timeScreen = true;

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
				gravity = true;

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
				colliders = true;
		}

		if (timeScreen == true)
		{
			currentScreen = Screen::TIME;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && targetFPS < 120)
				targetFPS += 10;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && targetFPS > 10)
				targetFPS -= 10;
		}
		if (gravity == true)
		{
			currentScreen = Screen::GRAVITY;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && App->physics->world->GetGravity().y < 20.0f)
			{
				//App->physics->world->SetGravity(p2Point<float>(1.0f, App->physics->world->GetGravity().x + 1.0f));
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && App->physics->world->GetGravity().y > -20.0f)
			{
				p2Point<float> ola;
				ola.x = 1.0f;
				ola.y = 1.0f;


				//App->physics->world->SetGravity(p2Point<float>(1.0f, App->physics->world->GetGravity().x - 1.0f));
			}
		}
		if (colliders == true)
		{
			currentScreen = Screen::COLLIDERS;
		}

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
		{
			currentScreen = Screen::HOME;
			timeScreen = false; gravity = false; colliders = false;
		}

		#pragma endregion

		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
			p2Point<int> position;
			position.x = App->input->GetMouseX();
			position.y = App->input->GetMouseY();
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleDebug::PostUpdate() {

	if (debug)
	{
		DebugDraw();
	}
	return UPDATE_CONTINUE;
}

void ModuleDebug::DebugDraw() {
	
	//Physics objects
	// This will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	if (colliders)
	{
		//for (world->bodies)
		//{
		//	draw colliders
		//}
		
	}

	SDL_Rect bg;
	if (timeScreen)					{ bg = { 2,38,251,92 }; }
	else if (gravity)			{ bg = { 2,38,251,82 }; }
	else if (colliders)			{ bg = { 2,38,251,22 }; }
	else						{ bg = { 2,38,252,72 }; }
	App->renderer->DrawQuad(bg, 0, 0, 0, 125, true);

	switch (currentScreen)
	{
	case Screen::HOME:
		App->fonts->BlitText(5, 40, 0, "PRESS A NUMBER TO OPEN ITS MENU");

		App->fonts->BlitText(5, 60, 0, "1. TIME OPTIONS");
		App->fonts->BlitText(5, 70, 0, "2. GRAVITY OPTIONS");
		App->fonts->BlitText(5, 80, 0, "3. SPRITES OPTIONS");
		App->fonts->BlitText(5, 90, 0, "4. COEFFICIENT OPTIONS");
		App->fonts->BlitText(5, 100, 0, "5. SHOW COLLIDERS");
		break;
		
	case Screen::TIME:
		App->fonts->BlitText(5, 40, 0, "TIME OPTIONS");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "S TO DECREASE THE VALUE");

		LOG("Elapsed Cycle:");
		LOG(std::to_string(elapsedCycle.count()).c_str());
		LOG("Elapsed Frame:");
		LOG(std::to_string(elapsedFrame.count()).c_str());

		App->fonts->BlitText(5, 90, 0, "TARGET FPS ");
		App->fonts->BlitText(100, 90, 0, std::to_string(targetFPS).c_str());
		App->fonts->BlitText(5, 100, 0, "CURRENT FPS ");
		App->fonts->BlitText(100, 100, 0, std::to_string(FPS).c_str());
		LOG("FPS:");
		LOG(std::to_string(FPS).c_str());

		App->fonts->BlitText(5, 120, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::GRAVITY:
		App->fonts->BlitText(5, 40, 0, "GRAVITY OPTIONS");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "S TO DECREASE THE VALUE");

		App->fonts->BlitText(5, 90, 0, "GRAVITY ");
		App->fonts->BlitText(120, 90, 0, std::to_string(App->physics->world->GetGravity().y).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::COLLIDERS:
		App->fonts->BlitText(5, 40, 0, "CLICK ON THE BALL TO DRAG IT");
		App->fonts->BlitText(5, 50, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::NONE:
		break;
	default:
		break;
	}
}