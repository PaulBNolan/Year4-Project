#include "RoadControlPanel.h"

RoadControlPanel::RoadControlPanel(std::vector<Road*> t_townList)
{
	sf::Vector2f m_windowDimensions;
	for (int i = 0; i < m_roadList.size(); i++)
	{
		m_nodes[i].setRadius(m_windowDimensions.x * 0.05);
		m_nodes[i].setPosition(sf::Vector2f(((m_windowDimensions.x * 0.05) * (i % 10)), ((m_windowDimensions.y * 0.15) * (i / 10))));

		m_widenButtons[i].setSize(sf::Vector2f(m_windowDimensions.x * 0.05, m_windowDimensions.y * 0.05));
		m_widenButtons[i].setPosition(sf::Vector2f(((m_windowDimensions.x * 0.05) * (i % 10)), ((m_windowDimensions.y * 0.15) * (i / 10))));

		m_narrowButtons[i].setSize(sf::Vector2f(m_windowDimensions.x * 0.05, m_windowDimensions.y * 0.05));
		m_widenButtons[i].setPosition(sf::Vector2f(((m_windowDimensions.x * 0.05) * (i % 10)), ((m_windowDimensions.y * 0.15) * (i / 10))));
	}
}

void RoadControlPanel::processClickEvents(sf::Vector2f t_mousePosition)
{
	for (int i = 0; i < m_roadList.size(); i++)
	{
		if (m_narrowButtons[i].getPosition().x <= t_mousePosition.x && m_narrowButtons[i].getPosition().x + m_narrowButtons[i].getGlobalBounds().width >= t_mousePosition.x
			&&
			m_narrowButtons[i].getPosition().y <= t_mousePosition.y && m_narrowButtons[i].getPosition().y + m_narrowButtons[i].getGlobalBounds().height >= t_mousePosition.y)
		{
			m_roadList[i]->setWidth(-1);
		}
		if (m_widenButtons[i].getPosition().x <= t_mousePosition.x && m_widenButtons[i].getPosition().x + m_widenButtons[i].getGlobalBounds().width >= t_mousePosition.x
			&&
			m_widenButtons[i].getPosition().y <= t_mousePosition.y && m_widenButtons[i].getPosition().y + m_widenButtons[i].getGlobalBounds().height >= t_mousePosition.y)
		{
			m_roadList[i]->setWidth(1);
		}
	}
}
void RoadControlPanel::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_roadList.size(); i++)
	{
		t_window.draw(m_nodes[i]);
		t_window.draw(m_narrowButtons[i]);
		t_window.draw(m_widenButtons[i]);
	}
}