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
	m_window{ sf::VideoMode{ 1800, 1000}, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	m_window.setPosition(sf::Vector2i(0, 0));

	LoadMap::load(m_mapData);

	setupFontAndText(); // load font 
	setupSprite(); // load texture
	m_map = new Map(m_mapData,m_ArialBlackfont, m_car,sf::Vector2f(m_window.getSize()));
	m_car = new Car(m_map->m_path);
	m_menu = new Menu(m_ArialBlackfont);

	m_state = State::MenuSelect;
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
		processMouseClick(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processMouseClick(); // at least 60 fps
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
void Game::processMouseClick()
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
			switch (m_state)
			{
			case State::MenuSelect:
				if (m_menu->processMouseClick(sf::Mouse::getPosition(m_window), m_car))
				{
					m_state = State::Game;
				}
				
				break;
			case State::Game:
					m_map->processMouseClick(m_car->getPosition(), sf::Mouse::getPosition(m_window));
				break;
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
	switch (m_state)
	{
	case State::MenuSelect:
		if (sf::Keyboard::Escape == t_event.key.code)
		{
			m_exitGame = true;
		}
	case State::Game:
		if (sf::Keyboard::Escape == t_event.key.code)
		{
			m_state = State::MenuSelect;
		}
	}
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
		m_map->update();
		m_car->update();
	}

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear();

	switch (m_state)
	{
	case State::MenuSelect:
		m_menu->render(m_window);
		break;
	case State::Game:
		m_map->render(m_window);
		m_car->render(m_window);
		break;
	}
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

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
}
