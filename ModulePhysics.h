#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#define PIXELS_PER_METER 50.0f
#define METER_PER_PIXEL 0.02f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum BodyType {
	DYNAMIC,
	KINEMATIC,
	STATIC
};

class Body
{
public:

	Body();
	~Body();

	void GetPosition(int& x, int& y) const;

	//float GetRotation() const;
	//bool Contains(int x, int y) const;
	
public:
	p2Point<float> position;
	int mass;
	int width, height;
	SDL_Rect* body;
	BodyType type;
	
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:

	bool debug;
	// List<Entities> entities;
};