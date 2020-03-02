#include "Town.h"

Town::Town(sf::Vector2f t_position, int t_id)
{
	m_position = t_position;
	m_town.setPosition(m_position);
	m_town.setRadius(25);
	m_town.setFillColor(sf::Color::Blue);

	m_center = sf::Vector2f(t_position.x + 25, t_position.y + 25);
	m_townId = t_id;
}

void Town::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_town);
}

void Town::setRelatedIds(int t_id)
{
	m_connectedRoadIds.push_back(t_id);
}

sf::Vector2f Town::getPosition()
{
	return m_position;
}

sf::Vector2f Town::getCenter()
{
	return m_center;
}

int Town::getID()
{
	return m_townId;
}