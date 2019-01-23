#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Direction.h"

class Bullet : public Entity
{
public:
	Bullet(const sf::FloatRect& area, const Vector2f& position, const Direction& direction);
	~Bullet();

	void update();

	bool mustBeDestroyed() const;

private:
	Direction m_direction;

	bool m_mustBeDestroyed = false;

	virtual void draw(RenderTarget& target, RenderStates states) const override;
};