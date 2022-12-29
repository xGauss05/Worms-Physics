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
	Body(float positionX, float positionY, BodyShape shape, float width, float height, BodyType type, float mass = 1.0f, float dragSurfaceX = 1.0f, float dragSurfaceY = 1.0f);
	Body(float positionX, float positionY, BodyShape shape, float radius, BodyType type, float mass = 1.0f, float dragSurfaceX = 1.0f, float dragSurfaceY = 1.0f);
	~Body();

	p2Point<float> GetPosition() const;

	float GetWidth() const;
	float GetHeight() const;
	float GetRadius() const;

	float GetMass() const;
	p2Point<float> GetDragSurface() const;
	float GetLocalRestitution() const;

	BodyShape GetShape() const;
	BodyType GetType() const;

	void ApplyExternalForce(p2Point<float> f);

	void SetDragSurface(float dragSurfaceX, float dragSurfaceY);
	void SetDragSurface(p2Point<float> dragSurface);

	void SetLocalRestitution(float restitutionCoeff);

	void Blit() const;
	void Blit(SDL_Rect section) const;

	void OnCollision(Body* otherBody);

public:

	p2Point<float> position;
	p2Point<float> velocity;
	p2Point<float> acceleration;

	p2Point<float> externalForce;

	SDL_Texture* texture = nullptr;

private:

	float width, height, radius;

	float mass;
	p2Point<float> dragSurface;

	float localRestitution;

	BodyShape shape;
	BodyType type;
};

struct Projectile {
	Body* body;
	int lifetime;
	bool isAlive;
	SDL_Rect section;

	Projectile(int type1, int type2) : body(nullptr), lifetime(100), isAlive(true)
	{
		
		section = { 0 + 16 * type1,32 + 16 * type2,16,16 };
	}

	~Projectile()
	{
		delete body;
		body = nullptr;
	}
};

class World {

public:
	World();
	World(p2Point<float> g);

	~World();

	p2Point<float> GetGravity();
	void SetGravity(p2Point<float> g);
	void AddBody(Body* body);
	void AddProjectile(Projectile* projectile, p2Point<float> position);
	void BlitProjectiles();
	void UpdateProjectiles();
	void Step();


private:

	p2Point<float> CalculateGravityForce(Body* b);
	p2Point<float> CalculateLiftForce(Body* b);
	p2Point<float> CalculateDragForce(Body* b);
	// Impulsive is externalForce / artificialForce

	void Integrate(Body* body, p2Point<float> force);
	void SolveCollisions(Body* bodyA, Body* bodyB);

	void SeparateCircleRect(Body* bodyC, Body* bodyR, p2Point<float> distance);
	void SeparateCircleCircle(Body* bodyA, Body* bodyB, p2Point<float> distance);
	void SeparateRectRect(Body* bodyA, Body* bodyB);

private:

	p2List<Body*> bodies;
	p2List<Projectile*> projectiles;
	p2Point<float> gravity;

	float density;

	float globalRestitution;

	IntegrationMethod integrationMethod = VERLET;

	float dt;

	friend class ModuleDebug;
};


class ModulePhysics : public Module
{
public:
	ModulePhysics(bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

public:

	World* world = nullptr;

};