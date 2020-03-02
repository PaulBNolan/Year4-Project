#ifndef TOWN
#define TOWN
#include <SFML/Graphics.hpp>
#include "Road.h"

class Town
{
public:
	Town(sf::Vector2f t_position, int t_id);

	void render(sf::RenderWindow& t_window);
	void setRelatedIds(int t_id);
	sf::Vector2f getPosition();
	sf::Vector2f getCenter();
	int getID();
private:
	std::vector<int> m_connectedRoadIds;
	int m_townId;

	sf::CircleShape m_town;
	sf::Vector2f m_position;
	sf::Vector2f m_center;

};

#endif