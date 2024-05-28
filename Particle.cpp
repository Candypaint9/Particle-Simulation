#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

class Particle
{
public:
	Vector2f pos;
	Vector2f oldPos;
	float radius;
	Color color;

	Particle(Vector2f _pos, float _radius, Color _color)
	{
		pos = _pos;
		oldPos = pos;
		radius = _radius;
		color = _color;
	}

	void updatePos(float dt, Vector2f acceleration, float dampingCoeff)
	{
		Vector2f velocity = pos - oldPos;

		oldPos = pos;

		pos += velocity + (acceleration - velocity * dampingCoeff) * dt * dt;
	}

	void setVelocity(float dt, Vector2f velocity)
	{
		oldPos = pos - velocity * dt;
	}
};