#include "Entity.h"


Entity::Entity(Vector2f* area) :
	m_area(area), m_vertices(PrimitiveType::Quads)
{

}

Entity::~Entity()
{

}

const Vector2f& Entity::getPosition() const
{
	return Transformable::getPosition();
}

const FloatRect Entity::getGlobalBounds() const
{
	return getTransform().transformRect(m_bounds);
}

bool Entity::collides(const FloatRect& rect)
{
	for (size_t i = 0; i < m_hitboxes.size(); ++i)
		if (getTransform().transformRect(m_hitboxes[i]).intersects(rect))
			return true;

	return false;
}

void Entity::setPosition(const Vector2f& position)
{
	float x = position.x;
	float y = position.y;

	if (x < m_area.left)
		x = m_area.left;

	else if (x + m_bounds.width > m_area.left + m_area.width)
		x = m_area.x - m_bounds.width;

	if (y < m_area.top)
		y = m_area.top;

	else if (y + m_bounds.height > m_area.top + m_area.height)
		y = m_area.y - m_bounds.height;

	Transformable::setPosition(Vector2f(x, y));
}

void Entity::move(const Vector2f& direction)
{
	Entity::setPosition(m_velocity * direction + Transformable::getPosition());
}
