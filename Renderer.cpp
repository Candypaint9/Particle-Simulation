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
			boundary.setFillColor(Color(100, 100, 100, 75));

			window->draw(boundary);
		}
	}
};