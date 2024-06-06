#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h>
#include "Physics.cpp"
#include "Renderer.cpp"

using namespace sf;
using namespace std;

int FPS = 240;
int PARTICLE_NUMBER = 3000;


int WINDOW_WIDTH = 1980;
int WINDOW_HEIGHT = 1080;

float BOUNDARY_RADIUS = 500.f;
float BOUNDARY_WIDTH = 25.f;
float BOUNDARY_HEIGHT = 25.f;

float RADIUS = 5;
float MAX_RADIUS = 12;
float MIN_RADIUS = 5;

float TIME_GAP = 0.01f;
float SPAWN_SPEED = 1700.f;
Vector2f SPAWN_POS;
float PI = 3.14159274101257324219;

//degrees
float minAngle = 30;	
float maxAngle = 150;	
float currAngle = 150;
int multiplier = -2;

float currColor = 0;

int SUBSTEPS = 12;
Vector2f GRAVITY = { 0.f, 1200.f };
float DAMPING_COEFF = 200.f;


void spawnRandomParticle(Solver* solver, float dt)
{
	float r = rand() % (int)(MAX_RADIUS - MIN_RADIUS + 1) + MIN_RADIUS;
	Color col = Color(rand() % 255, rand() % 255, rand() % 255);

	Particle* particle = solver->addParticle(SPAWN_POS, r, col);

	currAngle += multiplier;
	if (currAngle >= maxAngle)
	{
		multiplier *= -1;
	}
	else if (currAngle <= minAngle)
	{
		multiplier *= -1;
	}

	Vector2f dir = {cos(currAngle * PI / 180.f), sin(currAngle * PI / 180.f)};

	solver->setParticleVelocity(particle, dt, SPAWN_SPEED * dir);
}



int main()
{
	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = 1;

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "bruh", Style::Default, settings);
	Solver solver(BOUNDARY_RADIUS, { (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT }, DAMPING_COEFF, SUBSTEPS, MAX_RADIUS * 2.f);
	//Solver solver(BOUNDARY_WIDTH, BOUNDARY_HEIGHT, { (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT }, DAMPING_COEFF, SUBSTEPS, MAX_RADIUS * 2.f);
	RendererObject renderer;

	SPAWN_POS = { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 8.f };
	


	//FPS
	Clock clock;
	Clock timer;

	window.setFramerateLimit(FPS);

	Font font;
	font.loadFromFile("Fonts/comicsans.ttf");
	
	Text showFPS;
	showFPS.setFont(font);
	showFPS.setCharacterSize(24);


	int spawned = 0;
	unsigned int time;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}

		float dt = clock.restart().asSeconds();
		showFPS.setString(to_string((int)(1 / dt)));


		if (spawned < PARTICLE_NUMBER && timer.getElapsedTime().asSeconds() > TIME_GAP)
		{
			timer.restart();

			spawnRandomParticle(&solver, dt);
			spawned++;
		}

		solver.updateParticles(dt, GRAVITY);


		// render
		window.clear(Color(100, 100, 100, 75));
		window.draw(showFPS);

		renderer.renderBoundaries(&window, &solver);
		renderer.renderParticles(&window, &solver);

		window.display();
	}



    return 0;
}