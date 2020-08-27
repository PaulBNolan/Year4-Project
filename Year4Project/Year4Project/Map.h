#ifndef MAP
#define MAP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "Town.h"
#include "Road.h"
#include "LoadMap.h"
#include "Car.h"


/// <summary>
/// Struct created to record necessary information on each node when it is searched
/// </summary>
struct NodeData
{
	int m_id;
	int m_previousId;
	float m_accumaltedCost;
	float m_currentFuel;
	int m_numberOfPasses;

	NodeData(int t_id, int t_previousId, float t_accumlatedCost, float t_currentFuel, int t_numverOfPasses) : 
		m_id(t_id),  
		m_previousId(t_previousId), 
		m_accumaltedCost(t_accumlatedCost), 
		m_currentFuel(t_currentFuel),
		m_numberOfPasses(t_numverOfPasses)
	{

	}
};

class Map
{
public:
	Map(MapData t_map,sf::Font t_font, Car * &t_car, sf::Vector2f t_dimensions);

	void setHud();

	void update();

	void generatePath(int t_startId = -1, int t_endId = -1, int t_roadId = -1);
	void generatePathAStar(int t_startId, int t_endId);
	void processMouseClick(sf::Vector2f t_carPos, sf::Vector2i t_pos);
	void render(sf::RenderWindow& t_window);

	std::vector<sf::Vector2f> getPath()
	{
		return m_path;
	}
	std::vector<Town*> m_roughPath;
	std::vector<sf::Vector2f> m_path;
	std::vector<Town*> m_removedTowns;

	std::vector<Town*> m_aStarRoughPath;

	std::vector<sf::Vector2f> m_aStarPath;
private:
	std::vector<Town*> m_townList;
	std::vector<Road*> m_roadList;

	int m_startEndIds[2];

	float getDistance(sf::Vector2f t_current, sf::Vector2f t_dest);

	float m_maximumCost;
	//auto m_checkTimeStart;
	//float m_checkTimeEnd;

	sf::Vector2f m_mapDimensions;

	sf::Font m_font;

	sf::RectangleShape m_multiObjectiveHudBox;
	sf::Text m_multiObjectiveText;
	sf::Text m_multiObjectiveTimeText;
	sf::Text m_multiObjectivePathText;
	sf::Text m_multiObjectiveFuelText;
	std::vector<sf::Text> m_nodeText;
	std::string m_multiObjectiveTimeString;
	std::string m_multiObjectivePathString;

	sf::RectangleShape m_aStarHudBox;
	sf::Text m_aStarText;
	sf::Text m_aStarTimeText;
	sf::Text m_aStarPathText;
	std::vector<sf::Text> m_AStarNodeText;
	std::string m_aStarTimeString;
	std::string m_aStarPathString;

	Car* &m_car;

	std::vector<sf::CircleShape> m_points;
};

#endif // !1
