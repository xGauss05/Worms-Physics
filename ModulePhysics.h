#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2List.h"

#define PIXELS_PER_METER 50.0f
#define METER_PER_PIXEL 0.02f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum IntegrationMethod {
	FORWARD_EULER,
	BACKWARD_EULER,
	VERLET
};

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

	p2Point<float> GetPosition() const;

	float GetMass() const;

	void ApplyExternalForce(p2Point<float> f);

	//float GetRotation() const;
	//bool Contains(int x, int y) const;
	
public:
	p2Point<float> position;
	int width, height;

	SDL_Rect* body;
	BodyType type;

	p2Point<float> externalForce;
	float dragSurface;

private:
	float mass;
	
};


class World {

public: 
	World();
	~World();

	void Step();


private: 
	
	p2Point<float> CalculateGravityForce(Body b);
	p2Point<float> CalculateLiftForce(Body b);
	p2Point<float> CalculateDragForce(Body b);
	// Impulsive is externalForce / artificialForce

	void Integrate(Body &b);

private:
	
	p2List<Body> bodies;

	IntegrationMethod integrationMethod = VERLET;
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
};