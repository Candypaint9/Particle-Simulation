#include "Physics.cpp"

using namespace sf;

class RendererObject
{
public:
	RendererObject() = default;

	void renderParticles(RenderWindow* window, Solver* solver)
	{
		for (auto particle : solver->particleArray)
		{
			CircleShape circleShape(particle->radius, 30);

			//Drawing on screen
			circleShape.setFillColor(particle->color);
			circleShape.setPosition(particle->pos);
			circleShape.setOrigin(particle->radius, particle->radius);
			window->draw(circleShape);
		}
	}


	void renderBoundaries(RenderWindow* window, Solver* solver)
	{
		if (solver->circularBoundary)
		{
			CircleShape boundary(solver->boundaryRadius, 128);
			boundary.setPosition(solver->centre);
			boundary.setOrigin(solver->boundaryRadius, solver->boundaryRadius);
			boundary.setFillColor(Color(0, 0, 0, 255));

			window->draw(boundary);
		}
		else
		{
			RectangleShape boundary(solver->windowSize - 2.f * Vector2f{solver->boundaryWidth, solver->boundaryHeight});
			boundary.setPosition(solver->windowSize / 2.f + Vector2f{solver->boundaryWidth, solver->boundaryHeight});
			boundary.setOrigin(solver->windowSize / 2.f);
			boundary.setFillColor(Color(0, 0, 0, 255));

			window->draw(boundary);
		}
	}

	void showCollisionGrid(RenderWindow* window, Solver* solver)
	{
		for (int i = 0; i < solver->collisionGrid.size(); i++)
		{

			RectangleShape line(Vector2f{solver->windowSize.x, 2});
			
			line.setFillColor(Color::Cyan);
			line.setPosition(Vector2f{ 0, i* solver->cellSize });

			window->draw(line);
		}

		for (int j = 0; j < solver->collisionGrid[0].size(); j++)
		{
			RectangleShape line(Vector2f{ 2, solver->windowSize.y});

			line.setFillColor(Color::Cyan);
			line.setPosition(Vector2f{ j* solver->cellSize, 0 });

			window->draw(line);
		}
	}
};