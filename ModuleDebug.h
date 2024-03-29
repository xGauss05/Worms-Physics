#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__

#include "Globals.h"
#include "Module.h"
#include <chrono>
using namespace std::chrono;

enum class Screen
{
	HOME,
	TIME,
	GRAVITY,
	COEFFICIENTS,
	INTEGRATION,
	FORCES,
	VARIABLES,

	WORLD_RESTITUTION,
	TRAMP_RESTITUTION,

	NONE
};

enum class DeltaTimeScheme
{
	FIXED,
	SEMIFIXED,
	VARIABLE,
	NONE
};

class ModuleDebug : public Module {
public:
	// Constructor
	ModuleDebug(bool start_enabled = true);

	// Destructor
	~ModuleDebug();

	bool Start() override;

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	update_status Update();

	// Called at the end of the application loop
	// Draw all functionality (if debug mode is enabled)
	update_status PostUpdate();

	// Draws all existing colliders with some transparency
	void DebugDraw();

public:
	microseconds elapsedCycle;
	microseconds elapsedFrame;
	int targetFPS = 60;
	double FPS;
	DeltaTimeScheme timeScheme = DeltaTimeScheme::SEMIFIXED;

	// Simple debugging flag
	bool debug;

private:

	int font;

	Screen currentScreen = Screen::HOME;

	bool timeScreen = false;
	bool gravity = false;
	bool coefficients = false;
	bool integration = false;
	bool forces = false;
	bool variables = false;

	bool worldRest = false;
	bool trampolineRest = false;
};

#endif // __MODULE_DEBUG_H__
