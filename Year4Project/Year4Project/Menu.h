#ifndef MENU
#define MENU
#include "Car.h"
#include "SFML/Graphics.hpp"
class Menu
{
public:
	Menu(sf::Font t_font);

	bool processMouseClick(sf::Vector2i t_mousePosition, Car* t_car);

	void render(sf::RenderWindow& t_window);
private:
	sf::RectangleShape m_typeButtons[3];
	sf::Text m_typeText[3];
	sf::Font m_font;
};

#endif