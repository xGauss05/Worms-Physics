#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

Body::Body() {

}

Body::~Body() {

}

p2Point<float> Body::GetPosition() const { 
	return position; 
}

float Body::GetMass() const {
	return mass; 
}

void Body::ApplyExternalForce(p2Point<float> f) {
	externalForce += f;
}

// --------------------------

World::World()
{
}

World::~World()
{
}

void World::Step()
{
	//	step() {
	//		for (bodies) {
	//			p2point<float> total = (0,0);
	//				CalculateForces(body) {
	//					total += CalculateGravityForce(body.mass);
	//					total += CalculateDragForce(body.surface);
	//					total += body.externalForce;
	//					body.externalForce = (0, 0);
	//				}
	//				integrator
	// 

}

p2Point<float> World::CalculateGravityForce(Body b)
{

	// total = bjfs +  fasidk;
	// return total;

	return p2Point<float>();
}

p2Point<float> World::CalculateLiftForce(Body b)
{
	return p2Point<float>();
}

p2Point<float> World::CalculateDragForce(Body b)
{
	return p2Point<float>();
}

void World::Integrate(Body& b)
{
}

// ------------------------------------

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}

