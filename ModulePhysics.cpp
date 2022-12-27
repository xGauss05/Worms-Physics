#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

Body::Body()
{

}

Body::~Body()
{
	if (texture != nullptr)
	{
		App->textures->Unload(texture);
	}
}

p2Point<float> Body::GetPosition() const
{
	return position;
}

float Body::GetMass() const
{
	return mass;
}

void Body::ApplyExternalForce(p2Point<float> f)
{
	externalForce += f;
}

void Body::Blit() const
{
	if (texture != nullptr)
	{
		switch (shape)
		{
		case CIRCLE:
			App->renderer->Blit(texture, position.x - radius, position.y - radius);
			break;
		case RECTANGLE:
			App->renderer->Blit(texture, position.x, position.y);
			break;
		default:
			break;
		}

	}
}

void Body::Blit(SDL_Rect section) const
{
	if (texture != nullptr)
	{
		switch (shape)
		{
		case CIRCLE:
			App->renderer->Blit(texture, position.x - radius, position.y - radius, &section);
			break;
		case RECTANGLE:
			App->renderer->Blit(texture, position.x, position.y, &section);
			break;
		default:
			break;
		}

	}

}

// --------------------------

World::World()
{
	gravity.x = 0.0f;
	gravity.y = 9.81f;

	density = 1.293f;

	dt = 0.0f;
}

World::World(p2Point<float> g)
{
	gravity.x = g.x;
	gravity.y = g.y;

	density = 1.293f;

	dt = 0.0f;
}

World::~World()
{
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

	dt = (float)App->debug->elapsedFrame.count() * 10E-7;
	LOG("Delta Time: %f", dt);

	for (p2List_item<Body*>* b = bodies.getFirst(); b; b = b->next) {
		p2Point<float> total;
		total.SetToZero();

		// calculate all forces
		total += CalculateGravityForce(b->data);
		//total += CalculateDragForce(b->data);
		//total += CalculateLiftForce(b->data);
		total += b->data->externalForce;
		b->data->externalForce.SetToZero();

		// integrator
		Integrate(b->data, total);

		for (p2List_item<Body*>* b2 = bodies.getFirst(); b2; b2 = b2->next) {

			if (b == b2) continue;

			SolveCollisions(b->data, b2->data);

		}

	}

}

p2Point<float> World::GetGravity()
{
	return gravity;
}

void World::SetGravity(p2Point<float> g)
{
	gravity = g;
}

void World::AddBody(Body* body)
{
	bodies.add(body);
}

p2Point<float> World::CalculateGravityForce(Body* b) {

	p2Point<float> force;
	force.x = gravity.x * b->GetMass();
	force.y = gravity.y * b->GetMass();

	return force;
}

p2Point<float> World::CalculateLiftForce(Body* b) {

	p2Point<float> force;

	float constantLift = 2.0f;

	// perpendicular to surface
	// always vertical
	force.x = 0;
	force.y = -0.5f * density * b->velocity.y * b->velocity.y * b->dragSurface * constantLift;

	return force;
}

p2Point<float> World::CalculateDragForce(Body* b) {

	p2Point<float> force;

	float constantDrag = 5.0f;

	// parallel to v
	force.x = -0.5f * density * b->velocity.x * b->velocity.x * b->dragSurface * constantDrag;
	force.y = -0.5f * density * b->velocity.y * b->velocity.y * b->dragSurface * constantDrag;

	return force;
}

void World::Integrate(Body* body, p2Point<float> force) {


	// 3: newton's 2nd law
	// F = m * a --> a = F / m

	body->acceleration.x = force.x / body->GetMass();
	body->acceleration.y = force.y / body->GetMass();


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
		body->velocity.x = body->velocity.x + body->acceleration.x * dt;
		body->velocity.y = body->velocity.y + body->acceleration.y * dt;

		body->position.x = body->position.x + body->velocity.x * dt;
		body->position.y = body->position.y + body->velocity.y * dt;

		break;
	case BACKWARD_EULER:
		body->position.x = body->position.x + body->velocity.x * dt;
		body->position.y = body->position.y + body->velocity.y * dt;

		body->velocity.x = body->velocity.x + body->acceleration.x * dt;
		body->velocity.y = body->velocity.y + body->acceleration.y * dt;

		break;
	case VERLET:
		body->position.x = body->position.x + body->velocity.x * dt + 0.5f * body->acceleration.x * dt * dt;
		body->position.y = body->position.y + body->velocity.y * dt + 0.5f * body->acceleration.y * dt * dt;

		body->velocity.x = body->velocity.x + body->acceleration.x * dt;
		body->velocity.y = body->velocity.y + body->acceleration.y * dt;

		break;
	default:
		break;
	}


}

void World::SolveCollisions(Body* bodyA, Body* bodyB)
{
	// separate the two bodies
	// invert speed in one axis depending on where they hit
	// activate on collisions

	if (bodyA->shape == RECTANGLE && bodyB->shape == RECTANGLE)
	{
		/*if (SDL_HasIntersection(bodyA.rect,bodyB.rect
		// SDL_HasIntersection( { bodyA.position.x, bodyA.position.y, bodyA.position.x + bodyA.width, bodyA.position.y + bodyA.height },
		{ bodyB.position.x, bodyB.position.y, bodyB.position.x + bodyB.width, bodyB.position.y + bodyB.height } ) {}))
		{
			LOG("Collision detected");
		}*/
	}
	else if (bodyA->shape == CIRCLE && bodyB->shape == CIRCLE)
	{
		float distance = bodyA->GetPosition().DistanceTo(bodyB->GetPosition());
		if (bodyA->radius + bodyB->radius < distance)
		{
			LOG("Collision detected");
		}

		//Lets separate
		//distanceToSeparateEveryCircle = (bodyA.radius + bodyB.radius - distance) / 2;

	}
	else if (bodyA->shape == CIRCLE && bodyB->shape == RECTANGLE)
	{
		p2Point<float> rectClosest;
		rectClosest.x = max(bodyB->GetPosition().x, min(bodyA->GetPosition().x, bodyB->GetPosition().x + bodyB->width));
		rectClosest.y = max(bodyB->GetPosition().y, min(bodyA->GetPosition().y, bodyB->GetPosition().y + bodyB->height));

		float distance = bodyA->GetPosition().DistanceTo(rectClosest);
		if (distance < bodyA->radius /*this "width" parameter would be the radius*/)
		{
			LOG("Collision detected");
		}
	}
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

	world = new World();

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step();

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return UPDATE_CONTINUE;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	delete world;

	return true;
}

