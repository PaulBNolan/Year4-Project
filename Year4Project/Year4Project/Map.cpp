#include "Map.h"

Map::Map(MapData t_map)
{
	for (int i = 0; i < t_map.m_towns.size(); i++)
	{
		m_townList.push_back(new Town(t_map.m_towns[i].m_position,i));
	}
	for (int i = 0; i < t_map.m_roads.size(); i++)
	{
		m_roadList.push_back(new Road(i));

		m_roadList[i]->setPositions(m_townList[t_map.m_roads[i].m_relatedId1]->getCenter());
		m_roadList[i]->setPositions(m_townList[t_map.m_roads[i].m_relatedId2]->getCenter());
		m_roadList[i]->setRelatedIds(t_map.m_roads[i].m_relatedId1);
		m_roadList[i]->setRelatedIds(t_map.m_roads[i].m_relatedId2);
	}

	for (int i = 0; i < m_townList.size(); i++)
	{
		for (int j = 0; j < m_roadList.size(); j++)
		{

			for (int z = 0; z < 2; z++)
			{
				if (m_townList[i]->getID() == m_roadList[j]->getRelatedId(z))
				{
					m_townList[i]->setRelatedIds(m_roadList[j]->getId());
				}
			}
		}
	}
	for (int i = 0; i < 2; i++)
	{
		m_startEndIds[i] = -10;
	}
}

void Map::generatePath(int t_startId, int t_endId)
{
	Town* m_startTown = m_townList[t_startId];
	Town* m_endTown = m_townList[t_endId];

	std::cout << "Start " << t_startId << std::endl;
	std::cout << "End " << t_endId << std::endl;

	for (int i = 0; i < m_townList.size(); i++)
	{
		m_townList[i]->setHeuristic(m_endTown->getCenter());
		m_townList[i]->setPrevId(-666);
		if (m_townList[i] != m_startTown)
		{
			m_townList[i]->setAccumaltedCost(std::numeric_limits<int>::max() - 10000,0);
			m_townList[i]->setChecked(false);
		}
		m_townList[i]->setColor(sf::Color::Blue);
	}

	std::vector<Town*> m_searchQue;

	m_searchQue.push_back(m_startTown);

	m_startTown->setChecked(true);

	m_townList[t_startId]->setAccumaltedCost(0, 0);
	m_townList[t_startId]->setChecked(true);
	while (m_searchQue.size() != 0)
	{
		int m_currentTownId = m_searchQue.front()->getID();
		for (int i = 0; i < m_searchQue.front()->getRelatedIds().size(); i++)
		{ 
			int m_roadIndex = m_townList[m_currentTownId]->getRelatedIds()[i];
			int m_townIndex;
			
			for (int z = 0; z < 2; z++)
			{
				if (m_currentTownId != m_roadList[m_roadIndex]->getRelatedId(z))
				{
					m_townIndex = m_roadList[m_roadIndex]->getRelatedId(z);
				}
			}
			std::cout << "Size: " << m_searchQue.front()->getRelatedIds().size() << std::endl;
			std::cout << m_currentTownId <<" Checking Town: " << m_townIndex << std::endl;
			int pathCost = m_townList[m_currentTownId]->getAccumaltedCost();

			int m_prevId = m_townList[m_currentTownId]->getPrevId();

			if (m_townIndex != m_prevId)
			{
				int dist = m_roadList[m_roadIndex]->getWeight() + pathCost + m_townList[m_townIndex]->getHeuristic();
				int m_searchedNode = m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic();
				//std::cout << dist << std::endl;
				//std::cout << m_searchedNode << std::endl;
				if (dist < m_searchedNode)
				{
					m_townList[m_townIndex]->setPrevId(m_townList[m_currentTownId]->getID());
					m_townList[m_townIndex]->setAccumaltedCost(m_roadList[m_roadIndex]->getWeight(),m_townList[m_currentTownId]->getAccumaltedCost());
				}


				if (m_townList[m_townIndex]->getChecked() == false)
				{
					m_townList[m_townIndex]->setChecked(true);
					m_searchQue.push_back(m_townList[m_townIndex]);
				}
			}
		}
		m_searchQue.erase(m_searchQue.begin());
	}
	int m_currentIndex = t_endId;
	m_path.clear();
	std::cout << "Path Start" << std::endl;
	while (m_currentIndex != -666)
	{
		std::cout << m_currentIndex << std::endl;
		m_path.push_back(m_townList[m_currentIndex]);
		m_townList[m_currentIndex]->setColor(sf::Color::Green);
		m_currentIndex = m_townList[m_currentIndex]->getPrevId();
	}
	std::cout << "Path End" << std::endl;

	for (int i = 0; i < 2; i++)
	{
		m_startEndIds[i] = -10;
	}
}

void Map::processLeftMouseKey(sf::Vector2i t_pos)
{
	int dist;
	for (int i = 0; i < m_townList.size(); i++)
	{
		dist = sqrt(pow(m_townList[i]->getCenter().x - t_pos.x,2) + pow(m_townList[i]->getCenter().y - t_pos.y,2));
		if (dist < 25)
		{
			if (m_startEndIds[0] == -10)
			{
				m_startEndIds[0] = i;
			}
			else if(i != m_startEndIds[0])
			{
				m_startEndIds[1] = i;
				generatePath(m_startEndIds[0], m_startEndIds[1]);
			}
		}
	}
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