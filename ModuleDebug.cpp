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
	font = App->fonts->Load("Assets/Textures/font2.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,&!-(): ", 1);
	
	return true;
}

update_status ModuleDebug::Update() 
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) debug = !debug;

	if (debug) 
	{
		#pragma region Menu navigation

		//Screen input management
		if (timeScreen == true)
		{
			currentScreen = Screen::TIME;

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && targetFPS < 120)
				targetFPS += 10;

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && targetFPS > 10)
				targetFPS -= 10;

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			{
				if (timeScheme == DeltaTimeScheme::FIXED)
				{
					timeScheme = DeltaTimeScheme::SEMIFIXED;
				}
				else if (timeScheme == DeltaTimeScheme::SEMIFIXED)
				{
					timeScheme = DeltaTimeScheme::VARIABLE;
				}
				else if (timeScheme == DeltaTimeScheme::VARIABLE)
				{
					timeScheme = DeltaTimeScheme::FIXED;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			{
				if (timeScheme == DeltaTimeScheme::FIXED)
				{
					timeScheme = DeltaTimeScheme::VARIABLE;
				}
				else if (timeScheme == DeltaTimeScheme::SEMIFIXED)
				{
					timeScheme = DeltaTimeScheme::FIXED;
				}
				else if (timeScheme == DeltaTimeScheme::VARIABLE)
				{
					timeScheme = DeltaTimeScheme::SEMIFIXED;
				}
			}
		}

		if (gravity == true)
		{
			currentScreen = Screen::GRAVITY;

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && App->physics->world->GetGravity().y < 20.0f)
			{
				if (abs(App->physics->world->gravity.y - (9.805f)) < 0.1f)
				{
					App->physics->world->gravity.y = 10.0f;
				}
				else if (abs(App->physics->world->gravity.y - (8.905f)) < 0.1f)
				{
					App->physics->world->gravity.y = 9.81f;
				}
				else
				{
					App->physics->world->gravity.y += 1.0f;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && App->physics->world->GetGravity().y > -20.0f)
			{
				if (abs(App->physics->world->gravity.y - (9.805f)) < 0.1f)
				{
					App->physics->world->gravity.y = 9.0f;
				}
				else if (abs(App->physics->world->gravity.y - (9.905f)) < 0.1f)
				{
					App->physics->world->gravity.y = 9.81f;
				}
				else
				{
					App->physics->world->gravity.y -= 1.0f;
				}
			}
		}

		if (coefficients == true)
		{
			//This one has the screen activation at the end due to input difficulties

			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				worldRest = true;
				coefficients = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				trampolineRest = true;
				coefficients = false;
			}

			currentScreen = Screen::COEFFICIENTS;
		}

		if (integration == true)
		{
			currentScreen = Screen::INTEGRATION;

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			{
				if (App->physics->world->integrationMethod == FORWARD_EULER)
				{
					App->physics->world->integrationMethod = VERLET;
				}
				else if (App->physics->world->integrationMethod == BACKWARD_EULER)
				{
					App->physics->world->integrationMethod = FORWARD_EULER;
				}
				else if (App->physics->world->integrationMethod == VERLET)
				{
					App->physics->world->integrationMethod = BACKWARD_EULER;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			{
				if (App->physics->world->integrationMethod == FORWARD_EULER)
				{
					App->physics->world->integrationMethod = BACKWARD_EULER;
				}
				else if (App->physics->world->integrationMethod == BACKWARD_EULER)
				{
					App->physics->world->integrationMethod = VERLET;
				}
				else if (App->physics->world->integrationMethod == VERLET)
				{
					App->physics->world->integrationMethod = FORWARD_EULER;
				}
			}
		}

		if (forces == true)
		{
			currentScreen = Screen::FORCES;

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
				App->physics->world->gravityOn = !App->physics->world->gravityOn;

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
				App->physics->world->liftOn = !App->physics->world->liftOn;

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
				App->physics->world->dragOn = !App->physics->world->dragOn;
		}
		
		if (variables == true) currentScreen = Screen::VARIABLES;
		


		//Coefficients submenus
		if (worldRest == true)
		{
			currentScreen = Screen::WORLD_RESTITUTION;

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && App->physics->world->globalRestitution < 2.0f)
				App->physics->world->globalRestitution += 0.1f;

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && App->physics->world->globalRestitution > 0.1f)
				App->physics->world->globalRestitution -= 0.1f;
		}
		if (trampolineRest == true)
		{
			currentScreen = Screen::TRAMP_RESTITUTION;

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && App->physics->world->trampolines.getFirst()->data->body->GetLocalRestitution() < 7.0f)
			{
				for (p2List_item<Trampoline*>* b = App->physics->world->trampolines.getFirst(); b; b = b->next)
				{
					b->data->body->SetLocalRestitution(App->physics->world->trampolines.getFirst()->data->body->GetLocalRestitution() + 0.5f);
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && App->physics->world->trampolines.getFirst()->data->body->GetLocalRestitution() > 0.5f)
			{
				for (p2List_item<Trampoline*>* b = App->physics->world->trampolines.getFirst(); b; b = b->next)
				{
					b->data->body->SetLocalRestitution(App->physics->world->trampolines.getFirst()->data->body->GetLocalRestitution() - 0.5f);
				}
			}
		}


		//Going back
		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
		{
			if (currentScreen == Screen::WORLD_RESTITUTION || currentScreen == Screen::TRAMP_RESTITUTION)
			{
				currentScreen = Screen::COEFFICIENTS;
				worldRest = false; trampolineRest = false;
				coefficients = true;
			}
			else if (currentScreen == Screen::HOME)
			{
				debug = false;
			}
			else
			{
				currentScreen = Screen::HOME;
				timeScreen = false; gravity = false; coefficients = false; integration = false; forces = false; variables = false;
			}
		}

		//Home screen
		if (currentScreen == Screen::HOME)
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) timeScreen = true;

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) gravity = true;

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) coefficients = true;

			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) integration = true;

			if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) forces = true;

			if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) variables = true;
		}

		#pragma endregion
	}

	return UPDATE_CONTINUE;
}

update_status ModuleDebug::PostUpdate() 
{
	if (debug)
		DebugDraw();

	else
		App->fonts->BlitText(5, 10, 0, "PRESS F1 TO OPEN DEBUG MENU");

	return UPDATE_CONTINUE;
}

void ModuleDebug::DebugDraw() 
{
	#pragma region Colliders

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

	#pragma endregion

	//This draws all the debug menu screens
	SDL_Rect bg;

	//Menus
	if (timeScreen)				{ bg = { 2,38,280,142 }; }
	else if (gravity)			{ bg = { 2,38,280,82 }; }
	else if (coefficients)		{ bg = { 2,38,280,82 }; }
	else if (integration)		{ bg = { 2,38,280,82 }; }
	else if (forces)			{ bg = { 2,38,280,102 }; }
	else if (variables)			{ bg = { 2,38,280,162 }; }

	//Submenus
	else if (worldRest)			{ bg = { 2,38,280,82 }; }
	else if (trampolineRest)	{ bg = { 2,38,280,82 }; }

	//Main screen
	else						{ bg = { 2,38,280,102 }; }
	App->renderer->DrawQuad(bg, 0, 0, 0, 150, true);

	switch (currentScreen)
	{
	case Screen::HOME:
		App->fonts->BlitText(5, 40, 0, "PRESS A NUMBER TO OPEN ITS MENU");

		App->fonts->BlitText(5, 60, 0, "1. TIME OPTIONS");
		App->fonts->BlitText(5, 70, 0, "2. GRAVITY OPTIONS");
		App->fonts->BlitText(5, 80, 0, "3. COEFFICIENT OPTIONS");
		App->fonts->BlitText(5, 90, 0, "4. INTEGRATOR OPTIONS");
		App->fonts->BlitText(5, 100, 0, "5. FORCES OPTIONS");
		App->fonts->BlitText(5, 110, 0, "6. SHOW VARIABLES");

		App->fonts->BlitText(5, 130, 0, "PRESS F1 OR BACKSPACE TO CLOSE");
		break;
		
	case Screen::TIME:
		App->fonts->BlitText(5, 40, 0, "TIME OPTIONS");

		App->fonts->BlitText(5, 60, 0, "PRESS UP ARROW TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "DOWN ARROW TO DECREASE FPS VALUE");

		App->fonts->BlitText(5, 90, 0, "PRESS LEFT OR RIGHT ARROW TO");
		App->fonts->BlitText(5, 100, 0, "CHANGE DELTATIME TIME SCHEME");

		LOG("Elapsed Cycle:");
		LOG(std::to_string(elapsedCycle.count()).c_str());
		LOG("Elapsed Frame:");
		LOG(std::to_string(elapsedFrame.count()).c_str());

		App->fonts->BlitText(5, 120, 0, "TARGET FPS ");
		App->fonts->BlitText(100, 120, 0, std::to_string(targetFPS).c_str());
		App->fonts->BlitText(5, 130, 0, "CURRENT FPS ");
		App->fonts->BlitText(100, 130, 0, std::to_string(FPS).c_str());
		LOG("FPS:");
		LOG(std::to_string(FPS).c_str());

		App->fonts->BlitText(5, 150, 0, "DELTATIME SCHEME:");
		switch (timeScheme)
		{
		case DeltaTimeScheme::FIXED:
			App->fonts->BlitText(150, 150, 0, "FIXED");
			break;
		case DeltaTimeScheme::SEMIFIXED:
			App->fonts->BlitText(150, 150, 0, "SEMIFIXED");
			break;
		case DeltaTimeScheme::VARIABLE:
			App->fonts->BlitText(150, 150, 0, "VARIABLE");
			break;
		default:
			App->fonts->BlitText(150, 150, 0, "ERROR");
			break;
		}

		App->fonts->BlitText(5, 170, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::GRAVITY:
		App->fonts->BlitText(5, 40, 0, "GRAVITY OPTIONS");

		App->fonts->BlitText(5, 60, 0, "PRESS UP ARROW TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "DOWN ARROW TO DECREASE THE VALUE");

		App->fonts->BlitText(5, 90, 0, "GRAVITY ");
		App->fonts->BlitText(120, 90, 0, std::to_string(App->physics->world->GetGravity().y).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::COEFFICIENTS:
		App->fonts->BlitText(5, 40, 0, "COEFFICIENT OPTIONS");

		App->fonts->BlitText(5, 60, 0, "PRESS A NUMBER TO OPEN ITS MENU");

		App->fonts->BlitText(5, 80, 0, "1. WORLD RESTITUTION OPTIONS");
		App->fonts->BlitText(5, 90, 0, "2. TRAMPOLINES RESTITUTION OPTIONS");

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::INTEGRATION:
		App->fonts->BlitText(5, 40, 0, "INTEGRATOR OPTIONS");

		App->fonts->BlitText(5, 60, 0, "PRESS LEFT OR RIGHT ARROW TO");
		App->fonts->BlitText(5, 70, 0, "CHANGE INTEGRATION METHOD");

		App->fonts->BlitText(5, 90, 0, "CURRENT METHOD:");
		switch (App->physics->world->integrationMethod)
		{
		case VERLET:
			App->fonts->BlitText(130, 90, 0, "VERLET");
			break;
		case FORWARD_EULER:
			App->fonts->BlitText(130, 90, 0, "FORWARDS EULER");
			break;
		case BACKWARD_EULER:
			App->fonts->BlitText(130, 90, 0, "BACKWARDS EULER");
			break;
		default:
			App->fonts->BlitText(130, 90, 0, "ERROR");
			break;
		}

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::FORCES:
		App->fonts->BlitText(5, 40, 0, "INTEGRATOR OPTIONS");

		App->fonts->BlitText(5, 60, 0, "PRESS THE DIRECTIONAL ARROWS");
		App->fonts->BlitText(5, 70, 0, "TO TOGGLE THE DIFFERENT FORCES");

		App->fonts->BlitText(5, 90, 0, "GRAVITY (UP):");
		if (App->physics->world->gravityOn)	{ App->fonts->BlitText(130, 90, 0, "ON"); }
		else								{ App->fonts->BlitText(130, 90, 0, "OFF"); }

		App->fonts->BlitText(5, 100, 0, "LIFT (LEFT):");
		if (App->physics->world->liftOn)	{ App->fonts->BlitText(130, 100, 0, "ON"); }
		else								{ App->fonts->BlitText(130, 100, 0, "OFF"); }

		App->fonts->BlitText(5, 110, 0, "DRAG (RIGHT):");
		if (App->physics->world->dragOn)	{ App->fonts->BlitText(130, 110, 0, "ON"); }
		else								{ App->fonts->BlitText(130, 110, 0, "OFF"); }

		App->fonts->BlitText(5, 130, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::VARIABLES:
		App->fonts->BlitText(5, 40, 0, "VARIABLES");

		App->fonts->BlitText(5, 60, 0, "PLAYER");
		
		App->fonts->BlitText(5, 80, 0, "POSITION X (IN PIXELS)");
		App->fonts->BlitText(190, 80, 0, std::to_string(METERS_TO_PIXELS(App->scene_intro->player->position.x)).c_str());
		App->fonts->BlitText(5, 90, 0, "POSITION Y (IN PIXELS)");
		App->fonts->BlitText(190, 90, 0, std::to_string(METERS_TO_PIXELS(App->scene_intro->player->position.y)).c_str());
		App->fonts->BlitText(5, 100, 0, "POSITION X (IN METERS)");
		App->fonts->BlitText(190, 100, 0, std::to_string(App->scene_intro->player->position.x).c_str());
		App->fonts->BlitText(5, 110, 0, "POSITION Y (IN METERS)");
		App->fonts->BlitText(190, 110, 0, std::to_string(App->scene_intro->player->position.y).c_str());

		App->fonts->BlitText(5, 130, 0, "VELOCITY X");
		App->fonts->BlitText(190, 130, 0, std::to_string(App->scene_intro->player->velocity.x).c_str());
		App->fonts->BlitText(5, 140, 0, "VELOCITY Y");
		App->fonts->BlitText(190, 140, 0, std::to_string(App->scene_intro->player->velocity.y).c_str());

		App->fonts->BlitText(5, 160, 0, "ACCELERATION X");
		App->fonts->BlitText(190, 160, 0, std::to_string(App->scene_intro->player->acceleration.x).c_str());
		App->fonts->BlitText(5, 170, 0, "ACCELERATION Y");
		App->fonts->BlitText(190, 170, 0, std::to_string(App->scene_intro->player->acceleration.y).c_str());

		App->fonts->BlitText(5, 190, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::WORLD_RESTITUTION:
		App->fonts->BlitText(5, 40, 0, "WORLD RESTITUTION OPTIONS");

		App->fonts->BlitText(5, 60, 0, "PRESS UP ARROW TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "DOWN ARROW TO DECREASE THE VALUE");

		App->fonts->BlitText(5, 90, 0, "CURRENT COEFFICIENT");
		App->fonts->BlitText(180, 90, 0, std::to_string(App->physics->world->globalRestitution).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;
	case Screen::TRAMP_RESTITUTION:
		App->fonts->BlitText(5, 40, 0, "TRAMPOLINE RESTITUTION OPTIONS");

		App->fonts->BlitText(5, 60, 0, "PRESS UP ARROW TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "DOWN ARROW TO DECREASE THE VALUE");

		App->fonts->BlitText(5, 90, 0, "CURRENT COEFFICIENT");
		App->fonts->BlitText(180, 90, 0, std::to_string(App->physics->world->trampolines.getFirst()->data->body->GetLocalRestitution()).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::NONE:
		break;
	default:
		break;
	}
}