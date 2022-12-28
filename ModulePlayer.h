#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"


class ModulePlayer : public Module
{
public:

	ModulePlayer(bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	Body* body = nullptr;

	// SFX indices
	uint dieFx;
	int jumps;

private:

	float movementForce = 10.0f;
	float jumpForce = 300.f;
	float movementDampen = 20.0f;
	float speedCap = 12.0;
	float idleDampenMultiplier = 3.0f;

};