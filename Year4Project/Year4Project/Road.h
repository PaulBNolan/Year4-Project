#ifndef ROAD
#define ROAD
#include "Town.h"
#include <SFML/Graphics.hpp>
#include <iostream>
class Road
{
public:
	Road(int t_id, int t_width);

	void render(sf::RenderWindow& t_window);
	void setRelatedIds(int t_id);
	void setPositions(sf::Vector2f t_position);
	void setActive(bool t_state);
	int getRelatedId(int t_i);
	int getId();
	int getWidth();
	float getWeight();
	bool getActive();
private:
	int m_connectedTownIds[2];
	sf::Vector2f m_position[2];
	sf::Vertex m_road[2];
	std::vector<std::vector<sf::RectangleShape>> m_roadTiles;

	int m_id;
	int m_widthValue;
	float m_weight;

	bool m_checked;
	bool m_active;

	float m_length;
	float m_width;
	float m_angle;

	sf::Vector2f m_tileColumnDisplacement;
	sf::Vector2f m_tileRowDisplacement;
};

#endif