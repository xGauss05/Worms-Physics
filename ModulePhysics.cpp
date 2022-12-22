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

World::World() {
	gravity.x = 0.0f;
	gravity.y = 9.81f;

	density = 1.293f;
}

World::World(p2Point<float> g) {
	gravity.x = g.x;
	gravity.y = g.y;
}

World::~World() {
	bodies.clear();
}

void World::Step() {
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

	for (p2List_item<Body>* b = bodies.getFirst(); b; b = b->next) {
		p2Point<float> total;
		total.SetToZero();

		// calculate all forces
		total += CalculateGravityForce(b->data);
		total += CalculateDragForce(b->data);
		total += CalculateLiftForce(b->data);
		total += b->data.externalForce;

		// integrator
		Integrate(b->data, total);

		// collisions
		Body b2Test;
		SolveCollisions(b->data,b2Test);
	}

}

p2Point<float> World::CalculateGravityForce(Body b) {

	p2Point<float> force;
	force.x = gravity.x * b.GetMass();
	force.y = gravity.y * b.GetMass();

	return force;
}

p2Point<float> World::CalculateLiftForce(Body b) {

	p2Point<float> force;
	
	float constantLift = 2.0f;

	// perpendicular to v
	force.x = -0.5f * density * b.velocity.y * b.velocity.y * b.dragSurface * constantLift;
	force.y = -0.5f * density * b.velocity.x * b.velocity.x * b.dragSurface * constantLift;

	return force;
}

p2Point<float> World::CalculateDragForce(Body b) {

	p2Point<float> force;

	float constantDrag = 5.0f;

	// parallel to v
	force.x = -0.5f * density * b.velocity.x * b.velocity.x * b.dragSurface * constantDrag;
	force.y = -0.5f * density * b.velocity.y * b.velocity.y * b.dragSurface * constantDrag;

	return force;
}

void World::Integrate(Body& body, p2Point<float> force) {


	// 3: newton's 2nd law
	// F = m * a --> a = F / m
	p2Point<float> acceleration;
	acceleration.x = force.x / body.GetMass();
	acceleration.y = force.y / body.GetMass();

	// m != 0

	//// 4.1: backwards euler
	//x = x + v * dt;
	//v = v + a * dt;

	//// 4.2: forwards euler
	//v = v + a * dt;
	//x = x + v * dt;

	//// 4.3: verlet
	//x = x + v * dt + 0.5 * a * dt * dt;
	//v = v + a * dt;


	switch (integrationMethod)
	{
	case FORWARD_EULER:
		body.velocity.x = body.velocity.x + acceleration.x * App->dt;
		body.velocity.y = body.velocity.y + acceleration.y * App->dt;

		body.position.x = body.position.x + body.velocity.x * App->dt;
		body.position.y = body.position.y + body.velocity.y * App->dt;

		break;
	case BACKWARD_EULER:
		body.position.x = body.position.x + body.velocity.x * App->dt;
		body.position.y = body.position.y + body.velocity.y * App->dt;

		body.velocity.x = body.velocity.x + acceleration.x * App->dt;
		body.velocity.y = body.velocity.y + acceleration.y * App->dt;

		break;
	case VERLET:
		body.position.x = body.position.x + body.velocity.x * App->dt + 0.5f * acceleration.x * App->dt * App->dt;
		body.position.y = body.position.y + body.velocity.y * App->dt + 0.5f * acceleration.y * App->dt * App->dt;

		body.velocity.x = body.velocity.x + acceleration.x * App->dt;
		body.velocity.y = body.velocity.y + acceleration.y * App->dt;

		break;
	default:
		break;
	}


}

void World::SolveCollisions(Body bodyA, Body bodyB)
{
	if (bodyA.type == CIRCLE && bodyB.type == RECTANGLE)
	{
		p2Point<float> rectClosest;
		rectClosest.x = max(bodyB.GetPosition().x, min(bodyA.GetPosition().x, bodyB.GetPosition().x + bodyB.width));
		rectClosest.y = max(bodyB.GetPosition().y, min(bodyA.GetPosition().y, bodyB.GetPosition().y + bodyB.height));

		float distance = bodyA.GetPosition().DistanceTo(rectClosest);
		if (distance < bodyA.width /*this "width" parameter would be the radius*/)
		{
			LOG("Collision detected");
		}
	}
}

// ------------------------------------

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled) {
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics() {
}

bool ModulePhysics::Start() {
	LOG("Creating Physics 2D environment");

	world = new World();

	return true;
}

// 
update_status ModulePhysics::PreUpdate() {
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate() {
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp() {
	LOG("Destroying physics world");

	return true;
}

