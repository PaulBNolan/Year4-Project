#ifndef LOADMAP
#define LOADMAP

#include "yaml-cpp\yaml.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <vector>
struct TownData
{
	std::string m_name;
	sf::Vector2f m_position;
};

struct RoadData
{
	int m_relatedId1;
	int m_relatedId2;
	int m_width;
};

struct MapData
{
	std::vector<TownData> m_towns;
	std::vector<RoadData> m_roads;
};

class LoadMap
{
public:
	LoadMap() = default;

	static bool load(MapData& t_map);

private:
	static YAML::Node m_baseNode;
};

#endif