#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"


class ModuleTrampoline : public Module
{
public:

	ModuleTrampoline(bool start_enabled = true);
	virtual ~ModuleTrampoline();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	Body* body = nullptr;

	// SFX indices
	uint boingFx;

private:

	float movementForce = 10.0f;
};