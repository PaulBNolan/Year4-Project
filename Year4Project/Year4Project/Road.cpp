#include "Road.h"
Road::Road(int t_id)
{
	m_id = t_id;
}

void Road::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_road,2,sf::Lines);
	for (int i = 0; i < 3; i++)
	{
		t_window.draw(m_roadTiles[i]);
	}
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
		m_weight = sqrt(pow(m_position[0].x - m_position[1].x, 2) + pow(m_position[0].y - m_position[1].y, 2));
		m_length = m_weight / 3;
		m_tileColumnDisplacement = sf::Vector2f((m_position[0].x - m_position[1].x) / 3, (m_position[0].y - m_position[1].y) / 3);
		m_width = 15;

		m_angle = atan2(m_position[0].x - m_position[1].x, m_position[1].y - m_position[0].y) * (180 / (22.0 / 7.0));

		for (int i = 0; i < 3; i++)
		{
			m_roadTiles.push_back(sf::RectangleShape());

			m_roadTiles[i].setRotation(m_angle);
			m_roadTiles[i].setFillColor(sf::Color::Red);
			m_roadTiles[i].setOutlineColor(sf::Color::Blue);
			m_roadTiles[i].setOutlineThickness(2);
			m_roadTiles[i].setSize(sf::Vector2f(m_width, m_length));
			m_roadTiles[i].setPosition(sf::Vector2f(m_position[0].x + (m_tileColumnDisplacement.x * -i), m_position[0].y + (m_tileColumnDisplacement.y * -i)));
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

float Road::getWeight()
{
	return m_weight;
}