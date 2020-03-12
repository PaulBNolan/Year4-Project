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
	float getWeight();
private:
	int m_connectedTownIds[2];
	sf::Vector2f m_position[2];
	sf::Vertex m_road[2];
	std::vector<sf::RectangleShape> m_roadTiles;
	int m_id;
	float m_weight;
	bool m_checked;
	float m_length;
	float m_width;
	float m_angle;
	sf::Vector2f m_tileColumnDisplacement;
};

#endif