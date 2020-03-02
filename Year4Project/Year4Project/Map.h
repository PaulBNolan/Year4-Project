#ifndef MAP
#define MAP
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Town.h"
#include "Road.h"
class Map
{
public:
	Map();

	void render(sf::RenderWindow& t_window);
private:
	std::vector<Town*> m_townList;
	std::vector<Road*> m_roadList;
};

#endif // !1
