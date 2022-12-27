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

enum BodyShape {
	CIRCLE,
	RECTANGLE,
	//POINT
};

class Body
{
public:

	Body();
	~Body();

	p2Point<float> GetPosition() const;

	float GetMass() const;

	void ApplyExternalForce(p2Point<float> f);

	void Blit() const;
	void Blit(SDL_Rect section) const;

	
public:

	p2Point<float> position;
	p2Point<float> velocity;
	p2Point<float> acceleration;
	int width, height, radius;

	BodyShape shape;
	BodyType type;

	p2Point<float> externalForce;
	float dragSurface;

	SDL_Texture* texture = nullptr;

private:
	float mass = 1.0f;
	
};


class World {

public: 
	World();
	World(p2Point<float> g);

	~World();

	p2Point<float> GetGravity();
	void SetGravity(p2Point<float> g);
	void AddBody(Body* body);

	void Step();


private: 
	
	p2Point<float> CalculateGravityForce(Body* b);
	p2Point<float> CalculateLiftForce(Body* b);
	p2Point<float> CalculateDragForce(Body* b);
	// Impulsive is externalForce / artificialForce

	void Integrate(Body* body, p2Point<float> force);
	void SolveCollisions(Body* bodyA, Body* bodyB);

private:
	
	p2List<Body*> bodies;

	p2Point<float> gravity;

	float density;

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

public: 

	World* world = nullptr;

private:

	bool debug;
};