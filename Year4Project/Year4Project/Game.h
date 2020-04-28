/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Car.h"
#include "LoadMap.h"
class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processLeftMouseKey();
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	bool m_exitGame; // control exiting game
	Map* m_map;
	MapData m_mapData;
	Car* m_car;

	sf::RectangleShape m_multiObjectiveHudBox;
	sf::Text m_multiObjectiveText;
	sf::Text m_multiObjectiveTimeText;
	sf::Text m_multiObjectivePathText;

	sf::RectangleShape m_aStarHudBox;
	sf::Text m_aStarText;
	sf::Text m_aStarTimeText;
	sf::Text m_aStarPathText;
};

#endif // !GAME_HPP

