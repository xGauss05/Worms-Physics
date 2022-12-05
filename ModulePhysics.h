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

	float GetMass() {
		return mass;
	}

	void ApplyExternalForce(p2Point<float> f) {
		externalForce += f;
	}

	//float GetRotation() const;
	//bool Contains(int x, int y) const;
	
public:
	p2Point<float> position;
	int width, height;
	SDL_Rect* body;
	BodyType type;

	p2Point<float> externalForce;

private:
	float mass;
	
};


/*
class Force {
public:

	Force();
	~Force();

	virtual void CalculateForce(Body b);

	p2Point<float> GetMagnitude() {
		return magnitude;
	}

	Force operator+(Force F) {
		Force ret;

		ret.magnitude = magnitude + F.magnitude;

		return ret;
	}

	Force operator-(Force F) {
		Force ret;

		ret.magnitude = magnitude - F.magnitude;

		return ret;
	}

private:

	p2Point<float> magnitude;

};

class GravityForce : public Force {
public:
	GravityForce();
	~GravityForce();

	void CalculateForce(Body b) {
		// formula
	}

};
*/

// world
//	step() {
//		for (bodies) {
//			p2point<flaot> total = (0,0);
//				CalculateForces(body) {
//					total += CalculateGravityForce(body.mass);
//					total += CalculateDragForce(body.surface);
//					total += body.externalForce;
//					body.externalForce = (0, 0);
//				}


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