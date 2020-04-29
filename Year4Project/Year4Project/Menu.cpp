#include "Menu.h"

Menu::Menu(sf::Font t_font)
{
	m_font = t_font;

	m_typeText[0].setString("Small");
	m_typeText[1].setString("Medium");
	m_typeText[2].setString("Large");
	for (int i = 0; i < 3; i++)
	{
		m_typeButtons[i].setSize(sf::Vector2f(1350 * 0.5,100));
		m_typeButtons[i].setPosition(sf::Vector2f(1350 * 0.25, 150 + i * 200));
		m_typeButtons[i].setFillColor(sf::Color::Red);

		m_typeText[i].setFont(m_font);
		m_typeText[i].setFillColor(sf::Color::Black);
		m_typeText[i].setCharacterSize(40);
		m_typeText[i].setPosition(m_typeButtons[i].getPosition().x + m_typeButtons[i].getGlobalBounds().width / 2 - m_typeText[i].getGlobalBounds().width / 2,
			m_typeButtons[i].getPosition().y + m_typeButtons[i].getGlobalBounds().height / 2 - m_typeText[i].getGlobalBounds().height / 2);
	}
}

bool Menu::processMouseClick(sf::Vector2i t_mousePosition, Car* t_car)
{
	for (int i = 0; i < 3; i++)
	{
		if (t_mousePosition.x >= m_typeButtons[i].getPosition().x && t_mousePosition.x <= m_typeButtons[i].getPosition().x + m_typeButtons[i].getGlobalBounds().width
			&&
			t_mousePosition.y >= m_typeButtons[i].getPosition().y && t_mousePosition.y <= m_typeButtons[i].getPosition().y + m_typeButtons[i].getGlobalBounds().height)
		{
			switch (i)
			{
			case 0:
				t_car->setType(CarType::Small);
				break;
			case 1:
				t_car->setType(CarType::Medium);
				break;
			case 2:
				t_car->setType(CarType::Large);
				break;
			}

			return true;
		}
	}

	return false;
}

void Menu::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < 3; i++)
	{
		t_window.draw(m_typeButtons[i]);
		t_window.draw(m_typeText[i]);
	}
}