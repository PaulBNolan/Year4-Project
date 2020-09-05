#pragma once
#include "Map.h"

/// <summary>
/// The planned purpose of this class was to allow the user to alter the fuel values of the nodes in order to allow for better testing
/// </summary>
class TownControlPanel
{
	TownControlPanel(std::vector<Town*> t_townList);

	void processClickEvents(sf::Vector2f t_mousePosition);
	void render(sf::RenderWindow& t_window);


	//These variables are used to represent the node list and to ensure it affects the class
	std::vector<Town*> m_townList;
	std::vector<sf::CircleShape> m_nodes;
	std::vector<sf::RectangleShape> m_emptyButtons;
	std::vector<sf::RectangleShape> m_fillButtons;
};

