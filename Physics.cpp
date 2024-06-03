#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Particle.cpp"

using namespace sf;


class Cell
{
public:
	std::vector<Particle*> contained;

	Cell() = default;

	void clearCell()
	{
		contained.clear();
	}
};


class Solver
{
public:
	std::vector<Particle*> particleArray;
	std::vector<std::vector<Cell*>> collisionGrid;

	float cellSize;
	
	float boundaryRadius;//for circular boundary
	bool circularBoundary;

	Vector2f windowSize;

	float boundaryWidth;
	float boundaryHeight;

	Vector2f centre;

	int substeps;

	float dampingCoeff;


	Solver(float r, Vector2f winSize, float damp, int steps = 1, float cellSize)
	{
		boundaryRadius = r;
		circularBoundary = true;

		centre = winSize / 2.f;

		initSolver(winSize, damp, steps);
		initGrid(cellSize);
	}

	Solver(float w, float h, Vector2f winSize, float damp, int steps = 1, float cellSize)
	{
		boundaryWidth = w;
		boundaryHeight = h;
		circularBoundary = false;

		initSolver(winSize, damp, steps);
		initGrid(cellSize);
	}



	Particle* addParticle(Vector2f _pos, float r, Color color)
	{
		Particle* particle = new Particle(_pos, r, color);

		particleArray.push_back(particle);

		return particle;
	}


	void updateGrid()
	{

	}


	void clearGrid()
	{
		for (auto row : collisionGrid)
		{
			for (auto cell : row)
			{
				cell->clearCell();
			}
		}
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

	void resolveCollisions(Particle* particle, Particle* toCheck, float dist, float sumRadius)
	{
		Vector2f dir = unitVector(particle->pos - toCheck->pos);	//towards partcile from toCheck
		float move = (sumRadius - dist) / 2.f;

		float mass_ratio_1 = toCheck->radius / sumRadius;
		float mass_ratio_2 = particle->radius / sumRadius;

		toCheck->pos -= move * mass_ratio_2 * dir;
		particle->pos += move * mass_ratio_1 * dir;
	}


	void checkCollisions(Particle* toCheck)
	{
		/*for (auto particle : particleArray)
		{
			if (toCheck == particle)
			{
				continue;
			}

			float dist = calcDistance(particle->pos, toCheck->pos);

			float sumRadius = particle->radius + toCheck->radius;

			if (dist < sumRadius)
			{
				resolveCollisions(particle, toCheck, dist, sumRadius);
			}
		}*/



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




private:

	void initSolver(Vector2f winSize, float damp, int steps = 1)
	{
		dampingCoeff = damp;
		substeps = steps;
		windowSize = winSize;
	}

	//to initialize the collision grid
	void initGrid(float cellSize)
	{
		for (int i = 0; i < windowSize.x / cellSize; i++)
		{
			std::vector<Cell*> row;

			for (int j = 0; j < windowSize.y / cellSize; j++)
			{
				Cell* cell = new Cell();

				row.push_back(cell);
			}

			collisionGrid.push_back(row);
		}
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