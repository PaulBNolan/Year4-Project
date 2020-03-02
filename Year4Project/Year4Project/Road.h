#ifndef ROAD
#define ROAD
#include "Town.h"
#include <SFML/Graphics.hpp>
class Road
{
public:
	Road(int t_id);

	void render(sf::RenderWindow& t_window);
	void setRelatedIds(int t_id);
	void setPositions(sf::Vector2f t_position);
	int getRelatedId(int t_i);
	int getId();
private:
	int m_connectedTownIds[2];
	sf::Vector2f m_position[2];
	sf::Vertex m_road[2];
	int m_id;
};

#endif