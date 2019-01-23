#include "Bullet.h"

Bullet(const sf::FloatRect& area, const Vector2f& position, const Direction& direction)
	: Entity(area), m_direction(direction)
{
	cout << "Constructor of bullet" << endl;
	m_hitboxes.resize(1);
	m_hitboxes[0] = FloatRect(Vector2f(0.f, 0.f), Vector2f(0.5f, 0.5f));

	m_vertices.resize(m_hitboxes.size() * 4);
	// Set vertices positions (unique for each)
	// Retrieved from m_hitboxes
	for (size_t i = 0; i < m_hitboxes.size(); ++i)
	{
		size_t index = i * 4;
		m_vertices[index++].position = Vector2f(m_hitboxes[i].left, m_hitboxes[i].top);
		m_vertices[index++].position = Vector2f(m_hitboxes[i].left + m_hitboxes[i].width, m_hitboxes[i].top);
		m_vertices[index++].position = Vector2f(m_hitboxes[i].left + m_hitboxes[i].width, m_hitboxes[i].top + m_hitboxes[i].height);
		m_vertices[index++].position = Vector2f(m_hitboxes[i].left, m_hitboxes[i].top + m_hitboxes[i].height);
	}
	// Set vertices color and texture (same for all)
	for (size_t i = 0; i < m_vertices.getVertexCount(); ++i)
	{
		m_vertices[i].color = Color::Red;
		m_vertices[i].texCoords = Vector2f(0.0f, 0.0f);
	}

	m_bounds = m_vertices.getBounds();

	Entity::setPosition(Vector2f(position.x - m_bounds.width / 2, position.y - m_bounds.height));

	m_velocity = 0.5f;
}

~Bullet()
{
	cout << "Destructor of bullet" << endl;
}

void update()
{
	move(m_direction.getVec());

	if (getPosition().x < 0.f || getPosition().x > m_area->x || getPosition().y < 0.f || getPosition().y > m_area->y);
		m_mustBeDestroyed = true;
}

bool mustBeDestroyed() const
{
	return m_mustBeDestroyed;
}