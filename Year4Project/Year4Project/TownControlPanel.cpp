#include "TownControlPanel.h"

TownControlPanel::TownControlPanel(std::vector<Town*> t_townList)
{
	sf::Vector2f m_windowDimensions;
	for (int i = 0; i < m_townList.size(); i++)
	{
		m_nodes[i].setRadius(m_windowDimensions.x * 0.05);
		m_nodes[i].setPosition(sf::Vector2f(((m_windowDimensions.x * 0.05) * (i % 10)), ((m_windowDimensions.y * 0.15) * (i/10))));

		m_emptyButtons[i].setSize(sf::Vector2f(m_windowDimensions.x * 0.05, m_windowDimensions.y * 0.05));
		m_emptyButtons[i].setPosition(sf::Vector2f(((m_windowDimensions.x * 0.05) * (i % 10)), ((m_windowDimensions.y * 0.15) * (i / 10))));

		m_fillButtons[i].setSize(sf::Vector2f(m_windowDimensions.x * 0.05, m_windowDimensions.y * 0.05));
		m_emptyButtons[i].setPosition(sf::Vector2f(((m_windowDimensions.x * 0.05) * (i % 10)), ((m_windowDimensions.y * 0.15) * (i / 10))));
	}
}

void TownControlPanel::processClickEvents(sf::Vector2f t_mousePosition)
{
	for (int i = 0; i < m_townList.size(); i++)
	{
		if (m_fillButtons[i].getPosition().x <= t_mousePosition.x && m_fillButtons[i].getPosition().x + m_fillButtons[i].getGlobalBounds().width >= t_mousePosition.x
			&&
			m_fillButtons[i].getPosition().y <= t_mousePosition.y && m_fillButtons[i].getPosition().y + m_fillButtons[i].getGlobalBounds().height >= t_mousePosition.y)
		{
			m_townList[i]->setOriginalFuel(1000);
		}
		if (m_emptyButtons[i].getPosition().x <= t_mousePosition.x && m_emptyButtons[i].getPosition().x + m_emptyButtons[i].getGlobalBounds().width >= t_mousePosition.x
			&&
			m_emptyButtons[i].getPosition().y <= t_mousePosition.y && m_emptyButtons[i].getPosition().y + m_emptyButtons[i].getGlobalBounds().height >= t_mousePosition.y)
		{
			m_townList[i]->setOriginalFuel(0);
		}
	}
}
void TownControlPanel::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_townList.size(); i++)
	{
		t_window.draw(m_nodes[i]);
		t_window.draw(m_fillButtons[i]);
		t_window.draw(m_emptyButtons[i]);
	}
}