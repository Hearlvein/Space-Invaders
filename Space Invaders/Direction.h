#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

class Direction
{
public:
	Direction();
	Direction(const Vector2f& coordinates);

	~Direction();

	const Vector2f& getVec() const;

private:
	sf::Vector2f m_direction;
};

Vector2f getVecDir(float angle);
Vector2f getVecDir(const sf::Vector2f& coordinates);
Vector2f getVecDir(const std::string& direction);