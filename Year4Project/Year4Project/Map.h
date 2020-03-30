#ifndef MAP
#define MAP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "Town.h"
#include "Road.h"
#include "LoadMap.h"
class Map
{
public:
	Map(MapData t_map);

	void generatePath(int t_startId, int t_endId);
	void processLeftMouseKey(sf::Vector2f t_carPos, sf::Vector2i t_pos);
	void render(sf::RenderWindow& t_window);

	std::vector<Town*> getPath()
	{
		return m_path;
	}
	std::vector<Town*> m_path;
private:
	std::vector<Town*> m_townList;
	std::vector<Road*> m_roadList;

	int m_startEndIds[2];

	float getDistance(sf::Vector2f t_current, sf::Vector2f t_dest);

	float m_maximumCost;
	//auto m_checkTimeStart;
	//float m_checkTimeEnd;
};

#endif // !1
