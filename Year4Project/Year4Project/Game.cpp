/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1350, 700}, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	m_window.setPosition(sf::Vector2i(0, 0));

	LoadMap::load(m_mapData);

	setupFontAndText(); // load font 
	setupSprite(); // load texture
	m_map = new Map(m_mapData);
	m_car = new Car(m_map->m_path);

	m_multiObjectiveHudBox.setPosition(sf::Vector2f(0, 550));
	m_multiObjectiveHudBox.setSize(sf::Vector2f(550,75));
	m_multiObjectiveHudBox.setFillColor(sf::Color::Red);

	m_aStarHudBox.setPosition(sf::Vector2f(0, 625));
	m_aStarHudBox.setSize(sf::Vector2f(550, 75));
	m_aStarHudBox.setFillColor(sf::Color::Blue);
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		else if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			if (m_map->getPath().size() == 0)
			{
				processLeftMouseKey();
			}
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::processLeftMouseKey()
{
	m_map->processLeftMouseKey(m_car->getPosition(),sf::Mouse::getPosition(m_window));
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	else
	{
		m_car->update();
	}

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear();
	//m_window.draw(m_welcomeMessage);
	//m_window.draw(m_logoSprite);
	m_map->render(m_window);
	m_car->render(m_window);

	m_window.draw(m_multiObjectiveHudBox);
	m_window.draw(m_multiObjectiveText);
	m_window.draw(m_multiObjectiveTimeText);
	m_window.draw(m_multiObjectivePathText);

	m_window.draw(m_aStarHudBox);
	m_window.draw(m_aStarText);
	m_window.draw(m_aStarTimeText);
	m_window.draw(m_aStarPathText);

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	/*m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);*/

	m_multiObjectiveText.setFont(m_ArialBlackfont);
	m_multiObjectiveText.setCharacterSize(20);
	m_multiObjectiveText.setFillColor(sf::Color::Black);
	m_multiObjectiveText.setPosition(sf::Vector2f(2.5, 577.5));
	m_multiObjectiveText.setString("Fuel Algorithm");

	m_multiObjectiveTimeText.setFont(m_ArialBlackfont);
	m_multiObjectiveTimeText.setCharacterSize(20);
	m_multiObjectiveTimeText.setFillColor(sf::Color::Black);
	m_multiObjectiveTimeText.setPosition(sf::Vector2f(200, 557.5));
	m_multiObjectiveTimeText.setString("Time");

	m_multiObjectivePathText.setFont(m_ArialBlackfont);
	m_multiObjectivePathText.setCharacterSize(20);
	m_multiObjectivePathText.setFillColor(sf::Color::Black);
	m_multiObjectivePathText.setPosition(sf::Vector2f(200, 582.5));
	m_multiObjectivePathText.setString("Path");

	m_aStarText.setFont(m_ArialBlackfont);
	m_aStarText.setCharacterSize(20);
	m_aStarText.setFillColor(sf::Color::Black);
	m_aStarText.setPosition(sf::Vector2f(2.5, 652.5));
	m_aStarText.setString("A Star Algorithm");

	m_aStarTimeText.setFont(m_ArialBlackfont);
	m_aStarTimeText.setCharacterSize(20);
	m_aStarTimeText.setFillColor(sf::Color::Black);
	m_aStarTimeText.setPosition(sf::Vector2f(200, 632.5));
	m_aStarTimeText.setString("Time");

	m_aStarPathText.setFont(m_ArialBlackfont);
	m_aStarPathText.setCharacterSize(20);
	m_aStarPathText.setFillColor(sf::Color::Black);
	m_aStarPathText.setPosition(sf::Vector2f(200, 657.5));
	m_aStarPathText.setString("Path");

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
}
