#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Particle.cpp"

using namespace sf;


class Solver
{
public:
	std::vector<Particle*> particleArray;
	
	float boundaryRadius;//for circular boundary
	bool circularBoundary;

	Vector2f windowSize;

	float boundaryWidth;
	float boundaryHeight;

	Vector2f centre;

	int substeps;

	float dampingCoeff;


	Solver(float r, Vector2f winSize, float damp, int steps = 1)
	{
		boundaryRadius = r;
		circularBoundary = true;

		centre = winSize / 2.f;

		dampingCoeff = damp;

		substeps = steps;

		windowSize = winSize;
	}

	Solver(float w, float h, Vector2f winSize, float damp, int steps = 1)
	{
		boundaryWidth = w;
		boundaryHeight = h;
		circularBoundary = false;

		dampingCoeff = damp;

		substeps = steps;

		windowSize = winSize;
	}

	Particle* addParticle(Vector2f _pos, float r, Color color)
	{
		Particle* particle = new Particle(_pos, r, color);

		particleArray.push_back(particle);

		return particle;
	}

	void checkBoundaries(Particle* particle)
	{
		if (circularBoundary)
		{
			float diff = calcDistance(particle->pos, centre) + particle->radius - boundaryRadius;
			if (diff > 0)
			{
				Vector2f dir = unitVector(particle->pos  - centre);

				particle->pos -= diff * dir;
			}
		}
		else
		{
			if (particle->pos.x > windowSize.x - boundaryWidth - particle->radius)
			{
				particle->pos.x = windowSize.x - boundaryWidth - particle->radius;
			}			
			else if (particle->pos.x < boundaryWidth + particle->radius)
			{
				particle->pos.x = boundaryWidth + particle->radius;
			}
			else if (particle->pos.y > windowSize.y - boundaryHeight - particle->radius)
			{
				particle->pos.y = windowSize.y - boundaryHeight - particle->radius;
			}
			else if (particle->pos.y < boundaryHeight + particle->radius)
			{
				particle->pos.y = boundaryHeight + particle->radius;
			}
		}
	}

	void checkCollisions(Particle* toCheck)
	{
		for (auto particle : particleArray)
		{
			if (toCheck == particle)
			{
				continue;
			}

			float dist = calcDistance(particle->pos, toCheck->pos);

			float sumRadius = particle->radius + toCheck->radius;

			if (dist < sumRadius)
			{
				Vector2f dir = unitVector(particle->pos - toCheck->pos);	//towards partcile from toCheck
				float move = (sumRadius - dist) / 2.f;

				float mass_ratio_1 = toCheck->radius / sumRadius;
				float mass_ratio_2 = particle->radius / sumRadius;

				toCheck->pos -= move * mass_ratio_2 * dir;
				particle->pos += move * mass_ratio_1 * dir;
			}
		}
	}

	void updateParticles(float dt, Vector2f GRAVITY)
	{
		float step_dt = dt / substeps;
		for (int step = 0; step < substeps; step++)
		{
			for (auto particle : particleArray)
			{
				particle->updatePos(step_dt, GRAVITY, dampingCoeff);
				checkCollisions(particle);
				checkBoundaries(particle);
			}
		}
	}


	void setParticleVelocity(Particle* particle, float dt, Vector2f velocity)
	{
		float step_dt = dt / substeps;

		particle->setVelocity(step_dt, velocity);
	}



	//math functions

	float calcDistance(Vector2f pos1, Vector2f pos2)
	{
		return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
	}

	Vector2f unitVector(Vector2f v)
	{
		float magnitude = sqrt(v.x * v.x + v.y * v.y);

		return v / magnitude;
	}
};