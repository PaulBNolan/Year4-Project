#pragma once
#include "Map.h"

/// <summary>
/// The purpose of this class is to increase the speed of testing by allowing the user to affect the attributes of the maps towns
/// </summary>
class RoadControlPanel
{
	RoadControlPanel(std::vector<Road*> t_roadList);

	void processClickEvents(sf::Vector2f t_mousePosition);
	void render(sf::RenderWindow& t_window);


	//These variables are used to represent the node list and to ensure it affects the class
	std::vector<Road*> m_roadList;
	std::vector<sf::CircleShape> m_nodes;
	std::vector<sf::RectangleShape> m_widenButtons;
	std::vector<sf::RectangleShape> m_narrowButtons;
};
