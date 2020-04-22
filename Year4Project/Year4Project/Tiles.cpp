#include "Tiles.h"

Tiles::Tiles(sf::Vector2f t_dimensions,float t_angle, sf::Vector2f t_position)
{
	m_body.setOrigin(sf::Vector2f(t_dimensions.x / 2, t_dimensions.y / 2));
	m_body.setRotation(t_angle);
	m_body.setSize(t_dimensions);

	setPosition(t_position);
}

void Tiles::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

void Tiles::setType(TileType t_type)
{
	m_type = t_type;

	switch (m_type)
	{
	case TileType::Entrance:
		m_body.setFillColor(sf::Color::Green);
		break;
	case TileType::Exit:
		m_body.setFillColor(sf::Color::Red);
		break;
	case TileType::Path:
		m_body.setFillColor(sf::Color::Blue);
		break;
	case TileType::Wall:
		m_body.setFillColor(sf::Color::Cyan);
		break;
	}
}

void Tiles::setPosition(sf::Vector2f t_position)
{
	m_body.setPosition(t_position);
}

sf::Vector2f Tiles::getPosition()
{
	return m_position;
}