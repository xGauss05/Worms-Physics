#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "p2Point.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{

}

ModulePlayer::~ModulePlayer()
{

}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	body = App->scene_intro->player;
	
	ballTexture = App->textures->Load("Assets/Textures/objects.png");
	
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	//App->textures->Unload(texture);

	return true;
}

update_status ModulePlayer::Update()
{
	// Dampening
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_IDLE) 
	{
		if (body->velocity.x > 0.5f || body->velocity.x < -0.5f) 
		{
			p2Point<float> linVel;
			linVel.x = -body->velocity.x * idleDampenMultiplier;
			linVel.y = 0.0f;
			body->ApplyExternalForce(linVel);
		}
	}

	// Move right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) 
	{
		if (body->velocity.x < -0.5f)
		{
			p2Point<float> linVel;
			linVel.x = movementDampen;
			linVel.y = 0.0f;
			//Opposite direction dampening
			body->ApplyExternalForce(linVel);
		}
		else
		{
			if (body->velocity.x < speedCap) 
			{
				p2Point<float> linVel;
				linVel.x = movementForce;
				linVel.y = 0.0f;
				//Opposite direction dampening
				body->ApplyExternalForce(linVel);
			}
		}
	}

	// Move left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) 
	{
		if (body->velocity.x > 0.5f)
		{
			p2Point<float> linVel;
			linVel.x = -movementDampen;
			linVel.y = 0.0f;
			body->ApplyExternalForce(linVel);
		}
		else
		{
			if (body->velocity.x > -speedCap) 
			{
				p2Point<float> linVel;
				linVel.x = -movementForce;
				linVel.y = 0.0f;
				body->ApplyExternalForce(linVel);
			}
		}
	}

	// Shoot
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_DOWN) 
	{	
		Body* newBall = new Body(PIXEL_TO_METERS(300), PIXEL_TO_METERS(300), CIRCLE, PIXEL_TO_METERS(8), DYNAMIC, 1.0f, 2.0f, 2.0f);
		newBall->texture = ballTexture;
		Projectile* newProjectile = new Projectile();
		newProjectile->body = newBall;

		p2Point<float> force, position;
		force.SetToZero();
		position.SetToZero();

		// Shoot left
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) 
		{
			force.x = -450.0f;
			force.y = -200.0f;

			position.x = body->position.x - PIXEL_TO_METERS(30);
			position.y = body->position.y - PIXEL_TO_METERS(30);

			App->physics->world->AddProjectile(newProjectile, position);
			newBall->ApplyExternalForce(force);
		}

		// Shoot right
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) 
		{
			force.x = 450.0f;
			force.y = -200.0f;

			position.x = body->position.x + PIXEL_TO_METERS(30);
			position.y = body->position.y - PIXEL_TO_METERS(30);

			App->physics->world->AddProjectile(newProjectile, position);
			newBall->ApplyExternalForce(force);
		}

		// Shoot idle
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_IDLE &&
			App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_IDLE) {
			force.x = 0.0f;
			force.y = -450.0f;

			position.x = body->position.x;
			position.y = body->position.y - PIXEL_TO_METERS(30);

			App->physics->world->AddProjectile(newProjectile, position);
			newBall->ApplyExternalForce(force);
		}
	}

	// Jump
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN) 
	{
		p2Point<float> linVel;
		linVel.x = 0.0f;
		linVel.y = -jumpForce;
		body->ApplyExternalForce(linVel);
	}

	// drag surface change
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_STATE::KEY_DOWN) 
	{
		if (withGlider) 
		{
			body->SetDragSurface(2.0f, 2.0f);
			withGlider = false;
		}
		else 
		{
			body->SetDragSurface(2.0f, 200.0f);
			withGlider = true;
		}
	}

	return UPDATE_CONTINUE;
}

