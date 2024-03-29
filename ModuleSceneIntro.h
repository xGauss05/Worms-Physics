#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "ModulePhysics.h"

#define BOUNCER_TIME 200

class ModuleSceneIntro;

class ModuleSceneIntro : public Module
{
public:

	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	bool win = false;
	bool lose = false;

	SDL_Texture* winScreen;
	SDL_Texture* loseScreen;

	Body* clown[10];
	Body* player = nullptr;
	Trampoline* trampoline1 = nullptr;
	Trampoline* trampoline2 = nullptr;

	p2Point<float> balloonPos[10];
	Balloon* balloons[10];
	SDL_Texture* glider = nullptr;

	Body* ground1 = nullptr;
	Body* ground2 = nullptr;
	Body* ground3 = nullptr;
	Body* ground4 = nullptr;
	Body* ground5 = nullptr;
	Body* ground6 = nullptr;
	Body* ground7 = nullptr;
	Body* ground8 = nullptr;
	Body* ground9 = nullptr;
	Body* ground10 = nullptr;
	Body* ground11 = nullptr;
	Body* ground12 = nullptr;
	Body* ground13 = nullptr;
	Body* ground14 = nullptr;
	Body* ground15 = nullptr;

	SDL_Texture* background;

};
