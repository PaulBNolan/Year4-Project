#ifndef MAP
#define MAP
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Town.h"
#include "Road.h"
#include "LoadMap.h"
class Map
{
public:
	Map(MapData t_map);

	void generatePath(int t_startId, int t_endId);
	void processLeftMouseKey(sf::Vector2i t_pos);
	void render(sf::RenderWindow& t_window);
private:
	std::vector<Town*> m_townList;
	std::vector<Road*> m_roadList;

	std::vector<Town*> m_path;

	int m_startEndIds[2];
};

#endif // !1
