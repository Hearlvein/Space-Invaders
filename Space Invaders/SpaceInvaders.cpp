#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <ctime>
#include <list>

using namespace std;
using namespace sf;



class SpaceShip : public Entity
{
public:
	SpaceShip(Vector2f* area) : Entity(area)
	{
		m_hitboxes.resize(2);
		m_hitboxes[0] = FloatRect(Vector2f(1, 0), Vector2f(1, 1));
		m_hitboxes[1] = FloatRect(Vector2f(0, 1), Vector2f(3, 1));

		m_vertices.resize(8);
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
			m_vertices[i].color = Color::Black;
			m_vertices[i].texCoords = Vector2f(0.0f, 0.0f);
		}

		m_bounds = m_vertices.getBounds();

		// Requires valid bounds
		setPosition(Vector2f(m_area->x / 2 - m_bounds.width / 2, m_area->y - m_bounds.height));
	}

	~SpaceShip()
	{

	}

	void hasShot(bool has_shot = true)
	{
		if (has_shot)
			m_shootTimer.restart();
	}

	bool canShoot() const
	{
		return m_shootTimer.getElapsedTime() >= m_shootDelay;
	}

	void move(Direction dir)
	{
		if (dir == Entity::Direction::Left || dir == Entity::Direction::Right)
			Entity::move(dir);
	}

private:
	Clock m_shootTimer;
	Time m_shootDelay = milliseconds(200);
};

class Enemy : public Entity
{
public:
	Enemy(Vector2f* area, SpaceShip* target) : Entity(area), m_target(target)
	{
        m_velocity = 0.4f;
	}

	~Enemy()
	{

	}

	virtual void update() = 0;

protected:
	SpaceShip* m_target;
};

class Alien : public Enemy
{
public:
	Alien(Vector2f* area, SpaceShip* target) : Enemy(area, target)
	{
		m_hitboxes.resize(3);
		m_hitboxes[0] = FloatRect(Vector2f(0, 0), Vector2f(1, 1));
		m_hitboxes[1] = FloatRect(Vector2f(1, 1), Vector2f(1, 1));
		m_hitboxes[2] = FloatRect(Vector2f(2, 0), Vector2f(1, 1));

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
			m_vertices[i].color = Color::Yellow;
			m_vertices[i].texCoords = Vector2f(0.0f, 0.0f);
		}

		m_bounds = m_vertices.getBounds();

		// Requires valid bounds
		//setPosition(Vector2f(m_area->x / 2 - m_bounds.width / 2, 0.f));

		m_moveTimer.restart();
	}

	~Alien()
	{

	}

	void update()
	{
		if (m_bullets.size() > 0)
		{
			cout << "ball pos: " << m_bullets.back().getPosition().x << ";" << m_bullets.back().getPosition().y << endl;
		}

		if (Keyboard::isKeyPressed(Keyboard::F))
		// if (m_shootTimer.getElapsedTime() >= m_shootDelay)
		{
			cout << "shooting" << endl;

			m_bullets.push_back(Bullet(
				m_area,
				Vector2f(1.f, 1.f),
				// getPosition() / 20.f,
				Vector2f(
					0,
					0.5f
				)
			));

			m_shootTimer.restart();

			sleep(seconds(1));
		}

		for (auto it = m_bullets.begin(); it != m_bullets.end(); ++it)
			it->update();


		if (m_moveTimer.getElapsedTime() >= m_moveDuration)
		{
			if (m_currentDir == Right)
				m_currentDir = Left;
			else
				m_currentDir = Right;

			m_moveDuration = milliseconds(rand() % 1000);
			m_moveTimer.restart();
		}

		Entity::move(m_currentDir);
	}

private:
	Entity::Direction m_currentDir = Right;

	list<Bullet> m_bullets;
	Time m_shootDelay = seconds(1000);
	Clock m_shootTimer;

	Time m_moveDuration = seconds(1);
	Clock m_moveTimer;

	virtual void draw(RenderTarget& target, RenderStates states) const override
	{
		Entity::draw(target, states);

		for (auto it = m_bullets.begin(); it != m_bullets.end(); ++it)
			target.draw(*it);
	}
};

class SpaceInvaders : public Drawable
{
public:
	SpaceInvaders() : m_spaceShip(&m_area)
	{
		m_drawTransform.scale(m_scale, m_scale);

		addEnemy();
	}

	~SpaceInvaders()
	{
        for (size_t i = 0; i < m_enemies.size(); ++i)
            delete m_enemies[i];
        m_enemies.clear();
	}

	void update(Time elapsed)
	{
		// Enemies logic
		for (auto enemyIt = m_enemies.begin(); enemyIt != m_enemies.end(); ++enemyIt)
			(*enemyIt)->update();

		// Bullet logic
		for (auto bulletIt = m_bullets.begin(); bulletIt != m_bullets.end(); ++bulletIt)
		{
		    bool br = false;

			bulletIt->update();

			if (bulletIt->mustBeDestroyed())
			{
				m_bullets.erase(bulletIt);
				br = true;
			}

			if (br) return;

			for (auto enemyIt = m_enemies.begin(); enemyIt != m_enemies.end(); ++enemyIt)
			{
                FloatRect bulletBounds = bulletIt->getGlobalBounds();
				if ((*enemyIt)->collides(bulletBounds))
				{
					m_enemies.erase(enemyIt);
					m_bullets.erase(bulletIt);

					break;
				}
			}
		}
	}

	Vector2f getSurface() const
	{
		return m_scale * m_area;
	}

	void addEnemy()
	{
	    if (m_enemies.size() < 50)
            m_enemies.push_back(new Alien(&m_area, &m_spaceShip));
	}

	void moveSpaceShip(Entity::Direction dir)
	{
		m_spaceShip.move(dir);
	}

	void fire()
	{
	    if (m_spaceShip.canShoot())
	    {
	    	FloatRect shipBounds = m_spaceShip.getGlobalBounds();
			m_bullets.push_back(Bullet(&m_area, Vector2f(shipBounds.left + shipBounds.width / 2, shipBounds.top), Vector2f(0, -1)));

			m_spaceShip.hasShot(true);
	    }
	}

private:
	float m_scale = 20.f;
	Transform m_drawTransform;
	Vector2f m_area = Vector2f(40.f, 20.f);

	SpaceShip m_spaceShip;
	vector<Enemy*> m_enemies;
	vector<Bullet> m_bullets;

	virtual void draw(RenderTarget& target, RenderStates states) const override
	{
		states.transform = m_drawTransform;

		for (size_t i = 0; i < m_enemies.size(); ++i)
            target.draw(*m_enemies.at(i), m_drawTransform);

		target.draw(m_spaceShip, m_drawTransform);

		for (size_t i = 0; i < m_bullets.size(); ++i)
            target.draw(m_bullets.at(i), m_drawTransform);
	}
};

int main()
{
	srand(time(0));
	SpaceInvaders game;

	RenderWindow window(VideoMode(game.getSurface().x, game.getSurface().y), "Space Invaders");
	window.setFramerateLimit(60);

	Clock clock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if(event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
			game.moveSpaceShip(Entity::Direction::Left);

		else if (Keyboard::isKeyPressed(Keyboard::Right))
			game.moveSpaceShip(Entity::Direction::Right);

		else if (Keyboard::isKeyPressed(Keyboard::A))
			game.addEnemy();

		if (Keyboard::isKeyPressed(Keyboard::Space))
			game.fire();

		game.update(clock.restart());

		window.clear(Color::White);
		window.draw(game);
		window.display();
	}

	return 0;
}
