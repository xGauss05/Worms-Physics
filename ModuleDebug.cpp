#include "ModuleDebug.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include <string>
using namespace std;

#include "p2List.h"

ModuleDebug::ModuleDebug(bool start_enabled) : Module(start_enabled) 
{
	debug = false;
}

ModuleDebug::~ModuleDebug() 
{

}

bool ModuleDebug::Start() 
{
	debug = false;

	//font = App->fonts->Load("Assets/Textures/font.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,&!- ", 1);
	font = App->fonts->Load("Assets/Textures/font1.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,&!-() ", 1);
	
	return true;
}

update_status ModuleDebug::Update() 
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debug = !debug;

	if (debug) 
	{

		#pragma region Menu navigation

		if (currentScreen == Screen::HOME)
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) timeScreen = true;

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) gravity = true;

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) variables = true;
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
				//App->physics->world->SetGravity(p2Point<float>(1.0f, App->physics->world->GetGravity().x - 1.0f));
			}
		}
		
		if (variables == true) currentScreen = Screen::VARIABLES;
		

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
		{
			currentScreen = Screen::HOME;
			timeScreen = false; gravity = false; variables = false;
		}

		#pragma endregion
	}

	return UPDATE_CONTINUE;
}

update_status ModuleDebug::PostUpdate() 
{

	if (debug)
	{
		DebugDraw();
	}

	return UPDATE_CONTINUE;
}

void ModuleDebug::DebugDraw() 
{
	
	// This will iterate all objects in the world and draw the circles and rectangles needed
	for (p2List_item<Body*>* b = App->physics->world->bodies.getFirst(); b; b = b->next) {

		switch (b->data->GetShape())
		{
		case CIRCLE:
			App->renderer->DrawCircle(METERS_TO_PIXELS(b->data->position.x), METERS_TO_PIXELS(b->data->position.y), METERS_TO_PIXELS(b->data->GetRadius()), 255, 255, 255, 255);
			break;
		case RECTANGLE:

			SDL_Rect rect;
			rect.x = METERS_TO_PIXELS(b->data->position.x);
			rect.y = METERS_TO_PIXELS(b->data->position.y);
			rect.w = METERS_TO_PIXELS(b->data->GetWidth());
			rect.h = METERS_TO_PIXELS(b->data->GetHeight());

			App->renderer->DrawQuad(rect, 255, 255, 255, 255, false);

			break;
		default:
			break;
		}
	}

	//This draws all the debug menu screens
	SDL_Rect bg;
	if (timeScreen)				{ bg = { 2,38,270,92 }; }
	else if (gravity)			{ bg = { 2,38,270,102 }; }
	else if (variables)			{ bg = { 2,38,270,142 }; }
	else						{ bg = { 2,38,270,52 }; }
	App->renderer->DrawQuad(bg, 0, 0, 0, 125, true);

	switch (currentScreen)
	{
	case Screen::HOME:
		App->fonts->BlitText(5, 40, 0, "PRESS A NUMBER TO OPEN ITS MENU");

		App->fonts->BlitText(5, 60, 0, "1. TIME OPTIONS");
		App->fonts->BlitText(5, 70, 0, "2. GRAVITY OPTIONS");
		App->fonts->BlitText(5, 80, 0, "3. SHOW VARIABLES");
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

		App->fonts->BlitText(5, 90, 0, "NOT IMPLEMENTED YET");

		App->fonts->BlitText(5, 110, 0, "GRAVITY ");
		App->fonts->BlitText(120, 110, 0, std::to_string(App->physics->world->GetGravity().y).c_str());

		App->fonts->BlitText(5, 130, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::VARIABLES:
		App->fonts->BlitText(5, 40, 0, "VARIABLES");

		App->fonts->BlitText(5, 60, 0, "TEST POS X (IN PIXELS)");
		App->fonts->BlitText(190, 60, 0, std::to_string(METERS_TO_PIXELS(App->scene_intro->player->position.x)).c_str());
		App->fonts->BlitText(5, 70, 0, "TEST POS Y (IN PIXELS)");
		App->fonts->BlitText(190, 70, 0, std::to_string(METERS_TO_PIXELS(App->scene_intro->player->position.y)).c_str());
		App->fonts->BlitText(5, 80, 0, "TEST POS X (IN METERS)");
		App->fonts->BlitText(190, 80, 0, std::to_string(App->scene_intro->player->position.x).c_str());
		App->fonts->BlitText(5, 90, 0, "TEST POS Y (IN METERS)");
		App->fonts->BlitText(190, 90, 0, std::to_string(App->scene_intro->player->position.y).c_str());

		App->fonts->BlitText(5, 110, 0, "TEST VEL X");
		App->fonts->BlitText(190, 110, 0, std::to_string(App->scene_intro->player->velocity.x).c_str());
		App->fonts->BlitText(5, 120, 0, "TEST VEL Y");
		App->fonts->BlitText(190, 120, 0, std::to_string(App->scene_intro->player->velocity.y).c_str());

		App->fonts->BlitText(5, 140, 0, "TEST ACC X");
		App->fonts->BlitText(190, 140, 0, std::to_string(App->scene_intro->player->acceleration.x).c_str());
		App->fonts->BlitText(5, 150, 0, "TEST ACC Y");
		App->fonts->BlitText(190, 150, 0, std::to_string(App->scene_intro->player->acceleration.y).c_str());

		App->fonts->BlitText(5, 170, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::NONE:
		break;
	default:
		break;
	}
}