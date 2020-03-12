#include "LoadMap.h"

YAML::Node LoadMap::m_baseNode;

void operator >> (const YAML::Node& t_roadNode, RoadData& t_road)
{
	t_road.m_relatedId1 = t_roadNode["Id1"].as<int>();
	t_road.m_relatedId2 = t_roadNode["Id2"].as<int>();
}

void operator >> (const YAML::Node& t_townNode, TownData& t_town)
{
	t_town.m_name = t_townNode["Name"].as<std::string>();
	t_town.m_position.x = t_townNode["Position X"].as<float>();
	t_town.m_position.y = t_townNode["Position Y"].as<float>();
}

void operator >> (const YAML::Node& t_mapNode, MapData& t_map)
{
	const YAML::Node& townNode = t_mapNode["Towns"].as<YAML::Node>();

	////loop through all of the nodes in the scoresNode and load all of the data
	//auto ssize = townNode.size();
	for (unsigned i = 0; i < townNode.size(); ++i)
	{
		TownData m_town;
		townNode[i] >> m_town;
		t_map.m_towns.push_back(m_town);
	}

	const YAML::Node& roadNode = t_mapNode["Roads"].as<YAML::Node>();

	for (unsigned i = 0; i < roadNode.size(); ++i)
	{
		RoadData m_road;
		roadNode[i] >> m_road;
		t_map.m_roads.push_back(m_road);
	}

}

bool LoadMap::load(MapData& t_map)
{
	std::stringstream ss;
	ss << "Assets//Yaml//Map.yaml";

	try
	{
		m_baseNode = YAML::LoadFile(ss.str());
		if (m_baseNode.IsNull())
		{
			std::string msg("No file found :" + ss.str());
			throw std::exception(msg.c_str());
		}
		m_baseNode >> t_map;
	}
	catch (YAML::ParserException& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}

	return true;
}