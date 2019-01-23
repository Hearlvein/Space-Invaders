#pragma once

#include <SFML/Graphics.hpp>
#include "Direction.h"

class Entity : public Drawable, private Transformable
{
public:
	Entity(const sf::FloatRect& area);
	~Entity();

	void setPosition(const Vector2f& position);
	const Vector2f& getPosition() const;

	const FloatRect getGlobalBounds() const;

	bool collides(FloatRect& rect) const;
	void move(const Vector2f& direction);

protected:
	const sf::FloatRect& m_area;

	VertexArray m_vertices;
	const Texture* m_texture = nullptr;

	FloatRect m_bounds;
	vector<FloatRect> m_hitboxes;

	float m_velocity = 0.3f;

	virtual void draw(RenderTarget& target, RenderStates states) const override;
};