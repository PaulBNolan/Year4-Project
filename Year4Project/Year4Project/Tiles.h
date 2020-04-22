#ifndef TILES
#define TILES

#include "SFML/Graphics.hpp"

enum class TileType {Path,Wall,Entrance, Exit};

class Tiles
{
public:

	Tiles(sf::Vector2f t_dimensions,float t_angle, sf::Vector2f t_position);

	void render(sf::RenderWindow& t_window);

	void setType(TileType t_type);
	void setPosition(sf::Vector2f t_position);

	sf::Vector2f getPosition();
private:

	TileType m_type;

	sf::RectangleShape m_body;
	sf::Vector2f m_position;
};

#endif