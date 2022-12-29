#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "ModulePhysics.h"

#define BOUNCER_TIME 200

enum lightTypes
{
	tiny,
	medium,
	big
};

class ModuleSceneIntro;

struct Light
{
	Light() : texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);

	lightTypes type;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* tex_light_tiny;
	SDL_Texture* tex_light_medium;
	SDL_Texture* tex_light_big;
	
	uint fx_light_tiny;
	uint fx_light_medium;
	uint fx_light_big;

	p2DynArray<Light> lights;

	uint player_lose_fx;

	Body* test1 = nullptr;
	Body* player = nullptr;
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
