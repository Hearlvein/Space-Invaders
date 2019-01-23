#include "Direction.h"

Direction::Direction() :
	m_direction(Vector2f(0, 0))
{}

Direction(const Vector2f& coordinates);
{
	m_direction = getDirection(coordinates);
}

~Direction()
{}

const Vector2f& Direction::getVec() const
{
	return m_direction;
}


Vector2f getVecDir(float angle)
{
	return Vector2f(std::sin(angle), std::cos(angle));
}

Vector2f getVecDir(const sf::Vector2f& coordinates)
{
	float great_norm = std::sqrt(coordinates.x * coordinates.x + coordinates.y * coordinates.y);

	return Vector2f(
		coordinates.x / great_norm;
		coordinates.y / great_norm;
	);
}

Vector2f getVecDir(const std::string& direction)
{
	switch (direction)
	{
		case "Left":
			m_direction = Vector2f(-1, 0);
			break;

		case "Up":
			m_direction = Vector2f(0, -1);
			break;

		case "Right":
			m_direction = Vector2f(1, 0);
			break;

		case "Down":
			m_direction = Vector2f(0, 1);
			break;
		
		default:
			break;
	}
}
