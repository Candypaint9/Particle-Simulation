#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Particle.cpp"

using namespace sf;


class Cell
{
public:
	int numContained = 0;
	Particle* contained[4] = {};

	Cell() = default;

	void clearCell()
	{
		numContained = 0;
	}

	void addCell(Particle* particle)
	{
		contained[numContained] = particle;
		numContained++;
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


	Solver(float r, Vector2f winSize, float damp, int steps = 1, float cSize = 10.f)
	{
		boundaryRadius = r;
		circularBoundary = true;

		centre = winSize / 2.f;

		initSolver(winSize, damp, steps, cSize);
		initGrid();
	}

	Solver(float w, float h, Vector2f winSize, float damp, int steps = 1, float cSize = 10.f)
	{
		boundaryWidth = w;
		boundaryHeight = h;
		circularBoundary = false;

		initSolver(winSize, damp, steps, cSize);
		initGrid();
	}



	Particle* addParticle(Vector2f _pos, float r, Color color)
	{
		Particle* particle = new Particle(_pos, r, color);

		particleArray.push_back(particle);

		return particle;
	}


	void updateGrid()
	{
		for (auto particle : particleArray)
		{
			int x = floor(particle->pos.x / cellSize);
			int y = floor(particle->pos.y / cellSize);

			collisionGrid[y][x]->addCell(particle);
		}
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
				Vector2f dir = unitVector(particle->pos - centre);

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


	void checkCollisionsWithCellParticles(Cell* curr, Particle* p2)
	{
		for (int i = 0; i < curr->numContained; i++)
		{
			Particle* p1 = curr->contained[i];

			if (p1 == p2)
			{
				continue;
			}

			float dist = calcDistance(p2->pos, p1->pos);
			float sumRadius = p2->radius + p1->radius;

			if (dist < sumRadius)
			{
				resolveCollisions(p2, p1, dist, sumRadius);
			}
		}
	}


	void handleCollisions()
	{
		for (int i = 0; i < collisionGrid.size(); i++)
		{
			for (int j = 0; j < collisionGrid[0].size(); j++)
			{
				for (int k = 0; k < collisionGrid[i][j]->numContained; k++)
				{
					for (int x = -1; x <= 1; x++)
					{
						for (int y = -1; y <= 1; y++)
						{
							Vector2i coords = { x, y };

							if (coords == Vector2i{-1, -1} || coords == Vector2i{-1, 0} || coords == Vector2i{0, -1} || coords == Vector2i{1, -1})
							{
								continue;
							}

							if (i+x < 0 || i+x >= collisionGrid.size() || j+y < 0 || j+y >= collisionGrid[0].size())
							{
								continue;
							}

							checkCollisionsWithCellParticles(collisionGrid[i+x][j+y], collisionGrid[i][j]->contained[k]);
						}
					}
				}
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
				checkBoundaries(particle);
			}
			clearGrid();
			updateGrid();
			handleCollisions();
		}
	}


	void setParticleVelocity(Particle* particle, float dt, Vector2f velocity)
	{
		float step_dt = dt / substeps;

		particle->setVelocity(step_dt, velocity);
	}




private:

	void initSolver(Vector2f winSize, float damp, int steps = 1, float cSize = 10.f)
	{
		dampingCoeff = damp;
		substeps = steps;
		windowSize = winSize;
		cellSize = cSize;
	}

	//to initialize the collision grid
	void initGrid()
	{
		for (int i = 0; i < windowSize.y / cellSize; i++)
		{
			std::vector<Cell*> row;

			for (int j = 0; j < windowSize.x / cellSize; j++)
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