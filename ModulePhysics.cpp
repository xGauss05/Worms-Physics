#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

Body::Body()
{

}

Body::Body(float positionX, float positionY, BodyShape shape, float width, float height, BodyType type, EntityType entityType, float mass, float dragSurfaceX, float dragSurfaceY)
{
	this->position.x = positionX;
	this->position.y = positionY;
	this->shape = shape;
	this->width = width;
	this->height = height;
	this->type = type;
	this->mass = mass;
	this->dragSurface.x = dragSurfaceX;
	this->dragSurface.y = dragSurfaceY;
	this->entityType = entityType;

	localRestitution = 1.0f;

	radius = 0.0f;

	velocity.SetToZero();
	acceleration.SetToZero();

	externalForce.SetToZero();
}

Body::Body(float positionX, float positionY, BodyShape shape, float radius, BodyType type, EntityType entityType, float mass, float dragSurfaceX, float dragSurfaceY)
{
	this->position.x = positionX;
	this->position.y = positionY;
	this->shape = shape;
	this->radius = radius;
	this->type = type;
	this->mass = mass;
	this->dragSurface.x = dragSurfaceX;
	this->dragSurface.y = dragSurfaceY;
	this->entityType = entityType;

	localRestitution = 1.0f;

	width = 0.0f;
	height = 0.0f;

	velocity.SetToZero();
	acceleration.SetToZero();

	externalForce.SetToZero();
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

float Body::GetWidth() const
{
	return width;
}

float Body::GetHeight() const
{
	return height;
}

float Body::GetRadius() const
{
	return radius;
}

float Body::GetMass() const
{
	return mass;
}

p2Point<float> Body::GetDragSurface() const
{
	return dragSurface;
}

float Body::GetLocalRestitution() const
{
	return localRestitution;
}

BodyShape Body::GetShape() const
{
	return shape;
}

BodyType Body::GetType() const
{
	return type;
}

void Body::ApplyExternalForce(p2Point<float> f)
{
	externalForce += f;
}

void Body::SetDragSurface(float dragSurfaceX, float dragSurfaceY)
{
	this->dragSurface.x = dragSurfaceX;
	this->dragSurface.y = dragSurfaceY;
}

void Body::SetDragSurface(p2Point<float> dragSurface)
{
	this->dragSurface = dragSurface;
}

void Body::SetLocalRestitution(float restitutionCoeff)
{
	localRestitution = restitutionCoeff;
}

void Body::Blit() const
{
	if (texture != nullptr)
	{
		switch (shape)
		{
		case CIRCLE:
			App->renderer->Blit(texture, METERS_TO_PIXELS(position.x) - METERS_TO_PIXELS(radius), METERS_TO_PIXELS(position.y) - METERS_TO_PIXELS(radius));
			break;
		case RECTANGLE:
			App->renderer->Blit(texture, METERS_TO_PIXELS(position.x), METERS_TO_PIXELS(position.y));
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
			App->renderer->Blit(texture, METERS_TO_PIXELS(position.x) - METERS_TO_PIXELS(radius), METERS_TO_PIXELS(position.y) - METERS_TO_PIXELS(radius), &section);
			break;
		case RECTANGLE:
			App->renderer->Blit(texture, METERS_TO_PIXELS(position.x), METERS_TO_PIXELS(position.y), &section);
			break;
		default:
			break;
		}
	}
}

void Body::OnCollision(Body* otherBody)
{

}

// --------------------------

World::World()
{
	gravity.x = 0.0f;
	gravity.y = 9.81f;

	density = 1.293f;

	globalRestitution = 0.8f;

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
	projectiles.clear();
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

		if (b->data->GetType() == BodyType::DYNAMIC)
		{
			// calculate all forces
			total += CalculateGravityForce(b->data);
			total += CalculateDragForce(b->data);
			total += CalculateLiftForce(b->data);
			total += b->data->externalForce;
			b->data->externalForce.SetToZero();

			if (total.x > 400.0f)
			{
				total.x = 400.0f;
			}
			if (total.x < -400.0f)
			{
				total.x = -400.0f;
			}
			if (total.y > 400.0f)
			{
				total.y = 400.0f;
			}
			if (total.y < -400.0f)
			{
				total.y = -400.0f;
			}

			// integrator
			Integrate(b->data, total);
		}

		for (p2List_item<Body*>* b2 = b->next; b2; b2 = b2->next)
		{
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

void World::AddProjectile(Projectile* projectile, p2Point<float> position)
{
	projectile->body->position = position;
	projectiles.add(projectile);
	bodies.add(projectile->body);
}

void World::BlitProjectiles()
{
	for (p2List_item<Projectile*>* b = projectiles.getFirst(); b; b = b->next)
	{
		b->data->body->Blit(b->data->section);
	}
}

void World::UpdateProjectiles()
{
	for (p2List_item<Projectile*>* b = projectiles.getFirst(); b; b = b->next)
	{
		if (projectiles.count() > 0) {
			if (b->data->isAlive == false) {
				bodies.del(bodies.findNode(b->data->body));
				projectiles.del(b);
				break;
			}
		}
	}

	for (p2List_item<Projectile*>* b = projectiles.getFirst(); b; b = b->next)
	{
		b->data->lifetime--;

		if (b->data->lifetime <= 0) {
			b->data->isAlive = false;
		}
	}
}

p2Point<float> World::CalculateGravityForce(Body* b)
{

	p2Point<float> force;
	force.x = gravity.x * b->GetMass();
	force.y = gravity.y * b->GetMass();

	return force;
}

p2Point<float> World::CalculateLiftForce(Body* b) {

	p2Point<float> force;

	float constantLift = 0.02f;

	// perpendicular to surface
	// always vertical

	float velSquaredY;
	velSquaredY = b->velocity.y * b->velocity.y;

	if (b->velocity.y > 0) {
		velSquaredY = -velSquaredY;
	}

	force.x = 0;
	force.y = 0.5f * density * velSquaredY * b->GetDragSurface().y * constantLift;

	return force;
}

p2Point<float> World::CalculateDragForce(Body* b) {

	p2Point<float> force;

	float constantDrag = 0.05f;

	// parallel to v

	p2Point<float> velSquared;
	velSquared.x = b->velocity.x * b->velocity.x;
	velSquared.y = b->velocity.y * b->velocity.y;

	if (b->velocity.x > 0) {
		velSquared.x = -velSquared.x;
	}
	if (b->velocity.y > 0) {
		velSquared.y = -velSquared.y;
	}

	force.x = 0.5f * density * velSquared.x * b->GetDragSurface().x * constantDrag;
	force.y = 0.5f * density * velSquared.y * b->GetDragSurface().y * constantDrag;

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

	if (bodyA->GetType() != BodyType::DYNAMIC && bodyB->GetType() != BodyType::DYNAMIC)
	{
		return;
	}

	if (bodyA->GetShape() == RECTANGLE && bodyB->GetShape() == RECTANGLE)
	{
		if (bodyA->position.x                        < bodyB->position.x + bodyB->GetWidth() &&
			bodyA->position.x + bodyA->GetWidth()    > bodyB->position.x &&
			bodyA->position.y                        < bodyB->position.y + bodyB->GetHeight() &&
			bodyA->position.y + bodyA->GetHeight() > bodyB->position.y
			)
		{

			LOG("Collision detected");
			LOG("RECT COLLIDING RECT");
			SeparateRectRect(bodyA, bodyB);
			bodyA->OnCollision(bodyB);
			bodyB->OnCollision(bodyA);
		}
	}
	else if (bodyA->GetShape() == CIRCLE && bodyB->GetShape() == CIRCLE)
	{
		float distance = bodyA->GetPosition().DistanceTo(bodyB->GetPosition());
		if (bodyA->GetRadius() + bodyB->GetRadius() > distance)
		{
			LOG("Collision detected");

			p2Point<float> distanceBalls;
			distanceBalls.x = (bodyA->GetRadius() + bodyB->GetRadius() - abs(bodyA->position.x - bodyB->position.x)) * 0.5f;
			distanceBalls.y = (bodyA->GetRadius() + bodyB->GetRadius() - abs(bodyA->position.y - bodyB->position.y)) * 0.5f;

			SeparateCircleCircle(bodyA, bodyB, distanceBalls);
			bodyA->OnCollision(bodyB);
			bodyB->OnCollision(bodyA);
		}

		//Lets separate
		//distanceToSeparateEveryCircle = (bodyA.radius + bodyB.radius - distance) / 2;

	}
	else if (bodyA->GetShape() == CIRCLE && bodyB->GetShape() == RECTANGLE)
	{
		p2Point<float> rectClosest;
		rectClosest.x = max(bodyB->GetPosition().x, min(bodyA->GetPosition().x, bodyB->GetPosition().x + bodyB->GetWidth()));
		rectClosest.y = max(bodyB->GetPosition().y, min(bodyA->GetPosition().y, bodyB->GetPosition().y + bodyB->GetHeight()));

		float distance = bodyA->GetPosition().DistanceTo(rectClosest);
		if (distance < bodyA->GetRadius())
		{

			LOG("Collision detected");
			SeparateCircleRect(bodyA, bodyB, rectClosest);
			bodyA->OnCollision(bodyB);
			bodyB->OnCollision(bodyA);
		}
	}
	else if (bodyA->GetShape() == RECTANGLE && bodyB->GetShape() == CIRCLE)
	{
		p2Point<float> rectClosest;
		rectClosest.x = max(bodyA->GetPosition().x, min(bodyB->GetPosition().x, bodyA->GetPosition().x + bodyA->GetWidth()));
		rectClosest.y = max(bodyA->GetPosition().y, min(bodyB->GetPosition().y, bodyA->GetPosition().y + bodyA->GetHeight()));

		float distance = bodyB->GetPosition().DistanceTo(rectClosest);
		if (distance < bodyB->GetRadius())
		{

			LOG("Collision detected");
			SeparateCircleRect(bodyB, bodyA, rectClosest);
			bodyA->OnCollision(bodyB);
			bodyB->OnCollision(bodyA);
		}
	}
}

void World::SeparateCircleRect(Body* bodyC, Body* bodyR, p2Point<float> rectClosest)
{

	if (bodyC->GetType() != BodyType::DYNAMIC)
	{

	}
	else if (bodyR->GetType() != BodyType::DYNAMIC)
	{
		p2Point<float> dist;
		dist.x = rectClosest.x - bodyC->position.x;
		dist.y = rectClosest.y - bodyC->position.y;

		if (abs(dist.x) > 0) {
			if (bodyC->position.y > bodyR->position.y &&
				bodyC->position.y < bodyR->position.y + bodyR->GetHeight())
			{
				if (dist.x > 0)
				{
					bodyC->position.x = bodyC->position.x - bodyC->GetRadius() + dist.x;
					bodyC->velocity.x = -bodyC->velocity.x * globalRestitution * bodyR->GetLocalRestitution();

				}
				else if (dist.x < 0)
				{
					bodyC->position.x = bodyC->position.x + bodyC->GetRadius() + dist.x;
					bodyC->velocity.x = -bodyC->velocity.x * globalRestitution * bodyR->GetLocalRestitution();

				}
			}
		}

		if (abs(dist.y) > 0) {
			if (bodyC->position.x > bodyR->position.x &&
				bodyC->position.x < bodyR->position.x + bodyR->GetWidth())
			{
				if (dist.y > 0)
				{
					bodyC->position.y = bodyC->position.y - bodyC->GetRadius() + dist.y;
					bodyC->velocity.y = -bodyC->velocity.y * globalRestitution * bodyR->GetLocalRestitution();
				}
				else if (dist.y < 0)
				{
					bodyC->position.y = bodyC->position.y + bodyC->GetRadius() + dist.y;
					bodyC->velocity.y = -bodyC->velocity.y * globalRestitution * bodyR->GetLocalRestitution();

				}
			}
		}
	}
	else
	{

	}

}

void World::SeparateCircleCircle(Body* bodyA, Body* bodyB, p2Point<float> distance)
{
	//distanceToSeparateEveryCircle = (bodyA.radius + bodyB.radius - distance) / 2;
	if (bodyA->GetType() != BodyType::DYNAMIC)
	{
		if (bodyA->position.x < bodyB->position.x) {
			bodyB->position.x = bodyB->position.x + distance.x;

			bodyB->velocity.x = -bodyB->velocity.x * globalRestitution * bodyA->GetLocalRestitution();
		}
		else if (bodyA->position.x > bodyB->position.x) {
			bodyB->position.x = bodyB->position.x - distance.x;

			bodyB->velocity.x = -bodyB->velocity.x * globalRestitution * bodyA->GetLocalRestitution();
		}

		if (bodyA->position.y < bodyB->position.y) {
			bodyB->position.y = bodyB->position.y + distance.y;

			bodyB->velocity.y = -bodyB->velocity.y * globalRestitution * bodyA->GetLocalRestitution();
		}
		else if (bodyA->position.y > bodyB->position.y) {
			bodyB->position.y = bodyB->position.y - distance.y;

			bodyB->velocity.y = -bodyB->velocity.y * globalRestitution * bodyA->GetLocalRestitution();
		}
	}
	else if (bodyB->GetType() != BodyType::DYNAMIC) {
		if (bodyA->position.x < bodyB->position.x) {
			bodyA->position.x = bodyA->position.x - distance.x;

			bodyA->velocity.x = -bodyA->velocity.x * globalRestitution * bodyB->GetLocalRestitution();
		}
		else if (bodyA->position.x > bodyB->position.x) {
			bodyA->position.x = bodyA->position.x + distance.x;

			bodyA->velocity.x = -bodyA->velocity.x * globalRestitution * bodyB->GetLocalRestitution();
		}

		if (bodyA->position.y < bodyB->position.y) {
			bodyA->position.y = bodyA->position.y - distance.y;

			bodyA->velocity.y = -bodyA->velocity.y * globalRestitution * bodyB->GetLocalRestitution();
		}
		else if (bodyA->position.y > bodyB->position.y) {
			bodyA->position.y = bodyA->position.y + distance.y;

			bodyA->velocity.y = -bodyA->velocity.y * globalRestitution * bodyB->GetLocalRestitution();
		}
	}
	else {

		if (bodyA->position.x < bodyB->position.x) {
			bodyA->position.x = bodyA->position.x - (distance.x * 0.5f);
			bodyB->position.x = bodyB->position.x + (distance.x * 0.5f);

			float totalVelocityX = abs(bodyA->velocity.x + bodyB->velocity.x);

			bodyA->velocity.x = -totalVelocityX * 0.5f;
			bodyB->velocity.x = totalVelocityX * 0.5f;

		}
		else if (bodyA->position.x > bodyB->position.x) {
			bodyA->position.x = bodyA->position.x + (distance.x * 0.5f);
			bodyB->position.x = bodyB->position.x - (distance.x * 0.5f);

			float totalVelocityX = abs(bodyA->velocity.x + bodyB->velocity.x);

			bodyA->velocity.x = totalVelocityX * 0.5f;
			bodyB->velocity.x = -totalVelocityX * 0.5f;
		}

		if (bodyA->position.y < bodyB->position.y) {
			bodyA->position.y = bodyA->position.y - (distance.y * 0.5f);
			bodyB->position.y = bodyB->position.y + (distance.y * 0.5f);

			float totalVelocityY = abs(bodyA->velocity.y + bodyB->velocity.y);

			bodyA->velocity.y = -totalVelocityY * 0.5f;
			bodyB->velocity.y = totalVelocityY * 0.5f;
		}
		else if (bodyA->position.y > bodyB->position.y) {
			bodyA->position.y = bodyA->position.y + (distance.y * 0.5f);
			bodyB->position.y = bodyB->position.y - (distance.y * 0.5f);

			float totalVelocityY = abs(bodyA->velocity.y + bodyB->velocity.y);

			bodyA->velocity.y = totalVelocityY * 0.5f;
			bodyB->velocity.y = -totalVelocityY * 0.5f;
		}



		//bodyA->position.x += distance.x * 0.5f;
		//bodyA->position.y += distance.y * 0.5f;
		//bodyB->position.x += distance.x * 0.5f;
		//bodyB->position.y += distance.y * 0.5f;
	}
}

void World::SeparateRectRect(Body* bodyA, Body* bodyB)
{
	if (bodyA->GetType() != DYNAMIC)
	{
		p2Point<float> rectClosest;
		rectClosest.x = max(bodyA->GetPosition().x, min(bodyB->GetPosition().x + bodyB->GetWidth() / 2, bodyA->GetPosition().x + bodyA->GetWidth()));
		rectClosest.y = max(bodyA->GetPosition().y, min(bodyB->GetPosition().y + bodyB->GetHeight() / 2, bodyA->GetPosition().y + bodyA->GetHeight()));

		p2Point<float> dist;
		dist.x = rectClosest.x - (bodyB->position.x + bodyB->GetWidth() / 2); //Point to center of the rectangle
		dist.y = rectClosest.y - (bodyB->position.y + bodyB->GetHeight() / 2); //Point to center of the rectangle

		if (abs(dist.x) > 0)
		{
			//From the left and is inside
			if (dist.x < 0 && (bodyB->position.x + bodyB->GetWidth() / 2) < bodyA->position.x + bodyA->GetWidth())
			{
				bodyB->position.x = bodyB->position.x - (bodyB->GetWidth() / 2 + dist.x);
				bodyB->velocity.x = -bodyB->velocity.x * globalRestitution * bodyA->GetLocalRestitution();
			}
			//From the left and is outside
			else if (dist.x > 0 && (bodyB->position.x + bodyB->GetWidth() / 2) < bodyA->position.x)
			{
				bodyB->position.x = bodyB->position.x - (bodyB->GetWidth() / 2 - dist.x);
				bodyB->velocity.x = -bodyB->velocity.x * globalRestitution * bodyA->GetLocalRestitution();
			}
			//From the right and outside
			else if (dist.x < 0 && (bodyB->position.x + bodyB->GetWidth() / 2) > bodyA->position.x + bodyA->GetWidth())
			{
				bodyB->position.x = bodyB->position.x + (bodyB->GetWidth() / 2 + dist.x);
				bodyB->velocity.x = -bodyB->velocity.x * globalRestitution * bodyA->GetLocalRestitution();
			}
			//From the right and inside
			else if (dist.x > 0 && (bodyB->position.x + bodyB->GetWidth() / 2) > bodyA->position.x)
			{
				bodyB->position.x = bodyB->position.x + (bodyB->GetWidth() / 2 + dist.x);
				bodyB->velocity.x = -bodyB->velocity.x * globalRestitution * bodyA->GetLocalRestitution();
			}
		}
		if (abs(dist.y) > 0)
		{
			//From the bottom and is inside
			if (dist.y > 0 && (bodyB->position.y + bodyB->GetHeight() / 2) > bodyA->position.y)
			{
				bodyB->position.y = bodyB->position.y + (bodyB->GetHeight() / 2 - dist.y);
				bodyB->velocity.y = -bodyB->velocity.y * globalRestitution * bodyA->GetLocalRestitution();
			}
			//From the top and is inside
			else if (dist.y < 0 && (bodyB->position.y + bodyB->GetHeight() / 2) < bodyA->position.y + bodyA->GetHeight())
			{
				bodyB->position.y = bodyB->position.y - (bodyB->GetHeight() / 2 + dist.y);
				bodyB->velocity.y = -bodyB->velocity.y * globalRestitution * bodyA->GetLocalRestitution();
			}
			//From the top and outside
			else if (dist.y > 0 && (bodyB->position.y + bodyB->GetHeight() / 2) < bodyA->position.y)
			{
				bodyB->position.y = bodyB->position.y - (bodyB->GetHeight() / 2 - dist.y);
				bodyB->velocity.y = -bodyB->velocity.y * globalRestitution * bodyA->GetLocalRestitution();
			}
			//From the bottom and is outside
			else if (dist.y < 0 && (bodyB->position.y + bodyB->GetHeight() / 2) > bodyA->position.y + bodyA->GetHeight())
			{
				bodyB->position.y = bodyB->position.y + (bodyB->GetHeight() / 2 + dist.y);
				bodyB->velocity.y = -bodyB->velocity.y * globalRestitution * bodyA->GetLocalRestitution();
			}
		}
	}
	else if (bodyB->GetType() != DYNAMIC)
	{
		p2Point<float> rectClosest;
		rectClosest.x = max(bodyB->GetPosition().x, min(bodyA->GetPosition().x + bodyA->GetWidth() / 2, bodyB->GetPosition().x + bodyB->GetWidth()));
		rectClosest.y = max(bodyB->GetPosition().y, min(bodyA->GetPosition().y + bodyA->GetHeight() / 2, bodyB->GetPosition().y + bodyB->GetHeight()));

		p2Point<float> dist;
		dist.x = rectClosest.x - (bodyA->position.x + bodyA->GetWidth() / 2); //Point to center of the rectangle
		dist.y = rectClosest.y - (bodyA->position.y + bodyA->GetHeight() / 2); //Point to center of the rectangle

		if (abs(dist.x) > 0)
		{
			//From the left and is inside
			if (dist.x < 0 && (bodyA->position.x + bodyA->GetWidth() / 2) < bodyB->position.x + bodyB->GetWidth())
			{
				bodyA->position.x = bodyA->position.x - (bodyA->GetWidth() / 2 + dist.x);
				bodyA->velocity.x = -bodyA->velocity.x * globalRestitution * bodyB->GetLocalRestitution();
			}
			//From the left and is outside
			else if (dist.x > 0 && (bodyA->position.x + bodyA->GetWidth() / 2) < bodyB->position.x)
			{
				bodyA->position.x = bodyA->position.x - (bodyA->GetWidth() / 2 - dist.x);
				bodyA->velocity.x = -bodyA->velocity.x * globalRestitution * bodyB->GetLocalRestitution();
			}
			//From the right and outside
			else if (dist.x < 0 && (bodyA->position.x + bodyA->GetWidth() / 2) > bodyB->position.x + bodyB->GetWidth())
			{
				bodyA->position.x = bodyA->position.x + (bodyA->GetWidth() / 2 + dist.x);
				bodyA->velocity.x = -bodyA->velocity.x * globalRestitution * bodyB->GetLocalRestitution();
			}
			//From the right and inside
			else if (dist.x > 0 && (bodyA->position.x + bodyA->GetWidth() / 2) > bodyB->position.x)
			{
				bodyA->position.x = bodyA->position.x + (bodyA->GetWidth() / 2 + dist.x);
				bodyA->velocity.x = -bodyA->velocity.x * globalRestitution * bodyB->GetLocalRestitution();
			}
		}
		if (abs(dist.y) > 0)
		{
			//From the bottom and is inside
			if (dist.y > 0 && (bodyA->position.y + bodyA->GetHeight() / 2) > bodyB->position.y)
			{
				bodyA->position.y = bodyA->position.y + (bodyA->GetHeight() / 2 - dist.y);
				bodyA->velocity.y = -bodyA->velocity.y * globalRestitution * bodyB->GetLocalRestitution();
			}
			//From the top and is inside
			else if (dist.y < 0 && (bodyA->position.y + bodyA->GetHeight() / 2) < bodyB->position.y + bodyB->GetHeight())
			{
				bodyA->position.y = bodyA->position.y - (bodyA->GetHeight() / 2 + dist.y);
				bodyA->velocity.y = -bodyA->velocity.y * globalRestitution * bodyB->GetLocalRestitution();
			}
			//From the top and outside
			else if (dist.y > 0 && (bodyA->position.y + bodyA->GetHeight() / 2) < bodyB->position.y)
			{
				bodyA->position.y = bodyA->position.y - (bodyA->GetHeight() / 2 - dist.y);
				bodyA->velocity.y = -bodyA->velocity.y * globalRestitution * bodyB->GetLocalRestitution();
			}
			//From the bottom and is outside
			else if (dist.y < 0 && (bodyA->position.y + bodyA->GetHeight() / 2) > bodyB->position.y + bodyB->GetHeight())
			{
				bodyA->position.y = bodyA->position.y + (bodyA->GetHeight() / 2 + dist.y);
				bodyA->velocity.y = -bodyA->velocity.y * globalRestitution * bodyB->GetLocalRestitution();
			}
		}
	}
	else
	{

	}
}

// ------------------------------------

ModulePhysics::ModulePhysics(bool start_enabled) : Module(start_enabled)
{

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
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	delete world;

	return true;
}
