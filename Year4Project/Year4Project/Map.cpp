#include "Map.h"

Map::Map()
{
	for (int i = 0; i < 5; i++)
	{
		m_roadList.push_back(new Road(i));
	}

	m_townList.push_back(new Town(sf::Vector2f(50, 50),0));
	m_townList.push_back(new Town(sf::Vector2f(200, 50),1));
	m_townList.push_back(new Town(sf::Vector2f(200, 200),2));
	m_townList.push_back(new Town(sf::Vector2f(50, 200),3));
	m_townList.push_back(new Town(sf::Vector2f(400, 400),4));

	m_roadList[0]->setPositions(m_townList[0]->getCenter());
	m_roadList[0]->setPositions(m_townList[1]->getCenter());
	m_roadList[0]->setRelatedIds(0);
	m_roadList[0]->setRelatedIds(1);

	m_roadList[1]->setPositions(m_townList[1]->getCenter());
	m_roadList[1]->setPositions(m_townList[2]->getCenter());
	m_roadList[1]->setRelatedIds(1);
	m_roadList[1]->setRelatedIds(2);

	m_roadList[2]->setPositions(m_townList[2]->getCenter());
	m_roadList[2]->setPositions(m_townList[3]->getCenter());
	m_roadList[2]->setRelatedIds(2);
	m_roadList[2]->setRelatedIds(3);

	m_roadList[3]->setPositions(m_townList[3]->getCenter());
	m_roadList[3]->setPositions(m_townList[0]->getCenter());
	m_roadList[3]->setRelatedIds(3);
	m_roadList[3]->setRelatedIds(0);

	m_roadList[4]->setPositions(m_townList[2]->getCenter());
	m_roadList[4]->setPositions(m_townList[4]->getCenter());
	m_roadList[4]->setRelatedIds(2);
	m_roadList[4]->setRelatedIds(4);

	for (int i = 0; i < m_townList.size(); i++)
	{
		for (int j = 0; j < m_roadList.size(); j++)
		{
			for (int z = 0; z < 2; z++)
			{
				if (m_townList[i]->getID() == m_roadList[j]->getRelatedId(z))
				{
					std::cout << i << " " << j << std::endl;
					m_townList[i]->setRelatedIds(m_roadList[j]->getId());
				}
			}
		}
	}
	std::cout << "" << std::endl;
}

void Map::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_roadList.size(); i++)
	{
		m_roadList[i]->render(t_window);
	}

	for (int i = 0; i < m_townList.size(); i++)
	{
		m_townList[i]->render(t_window);
	}
}