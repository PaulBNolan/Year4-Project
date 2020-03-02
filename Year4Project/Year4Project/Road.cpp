#include "Road.h"
Road::Road(int t_id)
{
	m_id = t_id;
}

void Road::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_road,2,sf::Lines);
}

void Road::setRelatedIds(int t_id)
{
	if (m_connectedTownIds[0] < 0)
	{
		m_connectedTownIds[0] = t_id;
	}
	else
	{
		m_connectedTownIds[1] = t_id;
	}
}

void Road::setPositions(sf::Vector2f t_position)
{
	if (m_position[0].x == NULL)
	{
		m_position[0] = t_position;
	}
	else
	{
		m_position[1] = t_position;
		for (int i = 0; i < 2; i++)
		{
			m_road[i].color = sf::Color::Red;
			m_road[i].position = m_position[i];
		}
	}
}

int Road::getRelatedId(int t_i)
{
	return m_connectedTownIds[t_i];
}

int Road::getId()
{
	return m_id;
}