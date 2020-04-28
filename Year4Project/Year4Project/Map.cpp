#include "Map.h"

Map::Map(MapData t_map)
{
	for (int i = 0; i < t_map.m_towns.size(); i++)
	{
		m_townList.push_back(new Town(t_map.m_towns[i].m_position, t_map.m_towns[i].m_fuelValue,i));
	}
	for (int i = 0; i < t_map.m_roads.size(); i++)
	{
		m_roadList.push_back(new Road(i,t_map.m_roads[i].m_width));

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

void Map::generatePath(int t_startId, int t_targetId)
{
	Town* m_startTown = m_townList[t_startId];
	Town* m_endTown = m_townList[t_targetId];

	std::cout << "Start " << t_startId << std::endl;
	std::cout << "End " << t_targetId << std::endl;

	for (int i = 0; i < m_townList.size(); i++)
	{
		m_townList[i]->setHeuristic(m_endTown->getCenter());
		m_townList[i]->clearPrevIds();
		if (m_townList[i] != m_startTown)
		{
			m_townList[i]->setAccumaltedCost(std::numeric_limits<int>::max() - 10000,0);
			m_townList[i]->setChecked(false);
		}

		if (m_townList[i]->getFuelValue() == 0)
		{
			m_townList[i]->setColor(sf::Color::Yellow);
		}
		else
		{
			m_townList[i]->setColor(sf::Color::Cyan);
		}
		m_townList[i]->setCurrentFuel(0);
	}

	for (int i = 0; i < m_roadList.size(); i++)
	{
		if (m_roadList[i]->getWidth() > 1)
		{
			m_roadList[i]->setActive(true);
		}
		else
		{
			m_roadList[i]->setActive(false);
		}
	}


	std::vector<Town*> m_searchQue;

	m_searchQue.push_back(m_startTown);

	float m_fuel = 450;

	m_startTown->setChecked(true);
	m_startTown->setCurrentFuel(m_fuel);

	m_townList[t_startId]->setAccumaltedCost(0, 0);
	m_townList[t_startId]->setChecked(true);
	std::vector<int> t_startList;
	m_townList[t_startId]->setPrevIds(t_startList ,-66);

	auto m_checkTimeStart = std::chrono::high_resolution_clock::now();

	while (m_searchQue.size() != 0 && m_searchQue.back()->getID() != t_targetId)
	{
		std::vector<Town*> searchedTowns;
		int m_currentTownId = m_searchQue.back()->getID();
		for (int i = 0; i < m_searchQue.back()->getRelatedIds().size(); i++)
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

			std::cout << "Size: " << m_searchQue.back()->getRelatedIds().size() << std::endl;
			std::cout << m_currentTownId <<" Checking Town: " << m_townIndex << std::endl;
			if (m_roadList[m_roadIndex]->getActive())
			{
				int pathCost = m_townList[m_currentTownId]->getAccumaltedCost();

				std::vector<int> m_prevIds = m_townList[m_currentTownId]->getPrevIds();

				float m_estimatedFuelCost = m_townList[m_currentTownId]->getCurrentFuel() - m_roadList[m_roadIndex]->getWeight();
				//std::cout << m_estimatedFuelCost << std::endl;
				if (m_townIndex != m_prevIds.back() && m_estimatedFuelCost >= 0)
				{
					//std::cout << m_townList[m_currentTownId]->getCurrentFuel() << " " << m_roadList[m_roadIndex]->getWeight() << std::endl;
					//std::cout << m_townList[m_townIndex]->getCurrentFuel() << std::endl;

					int dist = m_roadList[m_roadIndex]->getWeight() + pathCost + m_townList[m_townIndex]->getHeuristic();
					int m_searchedNode = m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic();

					///The first half of the below situation is to check for the shortest path to the destination while the second half is used
					///when a route encounters a shorter route with the less fuel ie the shorter route will be created first but if it encounters 
					///a longer route with more fuel the longer route takes priority
					if (dist < m_searchedNode || (m_estimatedFuelCost > m_townList[m_townIndex]->getCurrentFuel() && m_townList[m_townIndex]->getChecked()))
					{

						if (m_estimatedFuelCost > m_townList[m_townIndex]->getCurrentFuel())
						{
							m_townList[m_townIndex]->setChecked(false);
						}

						m_townList[m_townIndex]->setCurrentFuel(m_estimatedFuelCost + m_townList[m_townIndex]->getFuelValue());
						m_townList[m_townIndex]->useFuelValue();
						m_townList[m_townIndex]->setPrevIds(m_prevIds,m_townList[m_currentTownId]->getID());
						m_townList[m_townIndex]->setAccumaltedCost(m_roadList[m_roadIndex]->getWeight(), m_townList[m_currentTownId]->getAccumaltedCost());

					}
					else
					{
						std::cout << "Failed Dist " << dist << " " << m_searchedNode <<std::endl;
					}

					if (m_townList[m_townIndex]->getChecked() == false)
					{
						m_townList[m_townIndex]->setChecked(true);
		//		m_searchQue.push_back(m_townList[m_townIndex]);
						//std::cout << m_townIndex << std::endl;
						if (searchedTowns.size() == 0)
						{
							searchedTowns.push_back(m_townList[m_townIndex]);
						}
						else
						{
							int m_size = searchedTowns.size();
							
							for (int i = 0; i < m_size; i++)
							{
								//std::cout << i << std::endl;
								if(m_townList[m_townIndex]->getHeuristic() < searchedTowns.back()->getHeuristic())
								{
									searchedTowns.push_back(m_townList[m_townIndex]);
								}
								else if (m_townList[m_townIndex]->getHeuristic() > searchedTowns.front()->getHeuristic())
								{
									searchedTowns.insert(searchedTowns.begin(),m_townList[m_townIndex]);
								}
								else if(i > 0)
								{
									if (m_townList[m_townIndex]->getHeuristic() > searchedTowns[i]->getHeuristic()
										&&
										m_townList[m_townIndex]->getHeuristic() < searchedTowns[i - 1]->getHeuristic())
									{
										searchedTowns.insert(searchedTowns.begin() + i, m_townList[m_townIndex]);
									}
								}
							}

						}

						if (m_townIndex == t_targetId)
						{
							break;
						}
					}
				}
			}
		}
		//m_searchQue.erase(m_searchQue.begin());
		m_searchQue.pop_back();
		for (int i = 0; i < searchedTowns.size(); i++)
		{
			if (searchedTowns.size() > 0)
			{
				m_searchQue.push_back(searchedTowns[i]);
			}
		}
	}
	int m_currentIndex = t_targetId;
	m_roughPath.clear();
	//m_townList[t_targetId]->getPrevId().erase(m_townList[t_targetId]->getPrevId().begin());
	std::vector<int> m_pathIds = m_townList[t_targetId]->getPrevIds();

	m_pathIds.push_back(t_targetId);

	while (m_pathIds.size() != 1)
	{
		if (m_currentIndex == t_targetId && m_townList[t_targetId]->getPrevIds().size() == 0)
		{
			std::cout << "Not Enough Fuel";
		}
		else
		{
			//std::cout << m_currentIndex << std::endl;
			//std::cout << m_townList[m_currentIndex]->getCurrentFuel() << std::endl;
			m_roughPath.push_back(m_townList[m_pathIds.back()]);
			m_townList[m_pathIds.back()]->setColor(sf::Color::Green);
			m_pathIds.pop_back();
		}
	}

	m_path.clear();
	m_path.push_back(m_roughPath.front()->getCenter());
	int index = 1;
	int m_currentRoadIndex;
	while (m_path.back() != m_roughPath.back()->getCenter())
	{
		for (int i = 0; i < m_roughPath[index - 1]->getRelatedIds().size(); i++)
		{
			for (int j = 0; j < m_roughPath[index]->getRelatedIds().size(); j++)
			{
				if (m_roughPath[index - 1]->getRelatedIds()[i] == m_roughPath[index]->getRelatedIds()[j])
				{
					m_currentRoadIndex = m_roughPath[index]->getRelatedIds()[j];
				}
			}
		}

		if (getDistance(m_roadList[m_currentRoadIndex]->getTiles()[0][0]->getPosition(), m_roughPath[index - 1]->getPosition()) 
			< getDistance(m_roadList[m_currentRoadIndex]->getTiles()[0][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition(), 
				m_roughPath[index - 1]->getPosition()))
		{
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][0]->getPosition());
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition());
		}
		else
		{
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition());
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][0]->getPosition());
		}

		m_path.push_back(m_roughPath[index]->getCenter());
		index++;
	}


	auto m_checkTimeEnd = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time = m_checkTimeEnd - m_checkTimeStart;

	std::cout << time.count() << std::endl;

	for (int i = 0; i < 2; i++)
	{
		m_startEndIds[i] = -10;
	}
}

void Map::generatePathAStar(int t_startId, int t_endId)
{
	Town* m_startTown = m_townList[t_startId];
	Town* m_endTown = m_townList[t_endId];

	std::cout << "Start " << t_startId << std::endl;
	std::cout << "End " << t_endId << std::endl;

	for (int i = 0; i < m_townList.size(); i++)
	{
		m_townList[i]->setHeuristic(m_endTown->getCenter());
		m_townList[i]->setPrevId(-66);
		if (m_townList[i] != m_startTown)
		{
			m_townList[i]->setAccumaltedCost(std::numeric_limits<int>::max() - 10000, 0);
			m_townList[i]->setChecked(false);
		}

		if (m_townList[i]->getFuelValue() == 0)
		{
			m_townList[i]->setColor(sf::Color::Yellow);
		}
		else
		{
			m_townList[i]->setColor(sf::Color::Cyan);
		}
		m_townList[i]->setCurrentFuel(0);
	}

	std::vector<Town*> m_searchQue;

	m_searchQue.push_back(m_startTown);

	m_startTown->setChecked(true);

	m_townList[t_startId]->setAccumaltedCost(0, 0);
	m_townList[t_startId]->setChecked(true);
	std::vector<int> t_startList;
	m_townList[t_startId]->setPrevIds(t_startList, -66);

	auto m_checkTimeStart = std::chrono::high_resolution_clock::now();

	while (m_searchQue.size() != 0 && m_searchQue.back()->getID() != t_endId)
	{
		std::vector<Town*> searchedTowns;
		int m_currentTownId = m_searchQue.back()->getID();
		for (int i = 0; i < m_searchQue.back()->getRelatedIds().size(); i++)
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

			std::cout << "Size: " << m_searchQue.back()->getRelatedIds().size() << std::endl;
			std::cout << m_currentTownId << " Checking Town: " << m_townIndex << std::endl;
			if (m_roadList[m_roadIndex]->getActive())
			{
				int pathCost = m_townList[m_currentTownId]->getAccumaltedCost();

				std::vector<int> m_prevIds = m_townList[m_currentTownId]->getPrevIds();

				//std::cout << m_estimatedFuelCost << std::endl;
				if (m_townIndex != m_prevIds.back())
				{
					//std::cout << m_townList[m_currentTownId]->getCurrentFuel() << " " << m_roadList[m_roadIndex]->getWeight() << std::endl;
					//std::cout << m_townList[m_townIndex]->getCurrentFuel() << std::endl;

					int dist = m_roadList[m_roadIndex]->getWeight() + pathCost + m_townList[m_townIndex]->getHeuristic();
					int m_searchedNode = m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic();

					///The first half of the below situation is to check for the shortest path to the destination while the second half is used
					///when a route encounters a shorter route with the less fuel ie the shorter route will be created first but if it encounters 
					///a longer route with more fuel the longer route takes priority
					if (dist < m_searchedNode)
					{
						m_townList[m_townIndex]->useFuelValue();
						m_townList[m_townIndex]->setPrevIds(m_prevIds, m_townList[m_currentTownId]->getID());
						m_townList[m_townIndex]->setAccumaltedCost(m_roadList[m_roadIndex]->getWeight(), m_townList[m_currentTownId]->getAccumaltedCost());

					}
					else
					{
						std::cout << "Failed Dist " << dist << " " << m_searchedNode << std::endl;
					}

					if (m_townList[m_townIndex]->getChecked() == false)
					{
						m_townList[m_townIndex]->setChecked(true);
						//		m_searchQue.push_back(m_townList[m_townIndex]);
										//std::cout << m_townIndex << std::endl;

						//The below situation is used for sorting the paths connected to a node based on their length
						if (searchedTowns.size() == 0)
						{
							searchedTowns.push_back(m_townList[m_townIndex]);
						}
						else
						{
							int m_size = searchedTowns.size();

							for (int i = 0; i < m_size; i++)
							{
								//std::cout << i << std::endl;
								if (m_townList[m_townIndex]->getHeuristic() < searchedTowns.back()->getHeuristic())
								{
									searchedTowns.push_back(m_townList[m_townIndex]);
								}
								else if (m_townList[m_townIndex]->getHeuristic() > searchedTowns.front()->getHeuristic())
								{
									searchedTowns.insert(searchedTowns.begin(), m_townList[m_townIndex]);
								}
								else if (i > 0)
								{
									if (m_townList[m_townIndex]->getHeuristic() > searchedTowns[i]->getHeuristic()
										&&
										m_townList[m_townIndex]->getHeuristic() < searchedTowns[i - 1]->getHeuristic())
									{
										searchedTowns.insert(searchedTowns.begin() + i, m_townList[m_townIndex]);
									}
								}
							}

						}

						if (m_townIndex == t_endId)
						{
							break;
						}
					}
				}
			}
		}
		//m_searchQue.erase(m_searchQue.begin());
		m_searchQue.pop_back();
		for (int i = 0; i < searchedTowns.size(); i++)
		{
			if (searchedTowns.size() > 0)
			{
				m_searchQue.push_back(searchedTowns[i]);
			}
		}
	}
	int m_currentIndex = t_endId;
	m_roughPath.clear();
	//m_townList[t_targetId]->getPrevId().erase(m_townList[t_targetId]->getPrevId().begin());
	std::vector<int> m_pathIds = m_townList[t_endId]->getPrevIds();

	m_pathIds.push_back(t_endId);

	while (m_pathIds.size() != 1)
	{
		if (m_currentIndex == t_endId && m_townList[t_endId]->getPrevIds().size() == 0)
		{
			std::cout << "Not Enough Fuel";
		}
		else
		{
			//std::cout << m_currentIndex << std::endl;
			//std::cout << m_townList[m_currentIndex]->getCurrentFuel() << std::endl;
			m_roughPath.push_back(m_townList[m_pathIds.back()]);
			m_townList[m_pathIds.back()]->setColor(sf::Color::Green);
			m_pathIds.pop_back();
		}
	}

	m_path.clear();
	m_path.push_back(m_roughPath.front()->getCenter());
	int index = 1;
	int m_currentRoadIndex;
	while (m_path.back() != m_roughPath.back()->getCenter())
	{
		for (int i = 0; i < m_roughPath[index - 1]->getRelatedIds().size(); i++)
		{
			for (int j = 0; j < m_roughPath[index]->getRelatedIds().size(); j++)
			{
				if (m_roughPath[index - 1]->getRelatedIds()[i] == m_roughPath[index]->getRelatedIds()[j])
				{
					m_currentRoadIndex = m_roughPath[index]->getRelatedIds()[j];
				}
			}
		}

		if (getDistance(m_roadList[m_currentRoadIndex]->getTiles()[0][0]->getPosition(), m_roughPath[index - 1]->getPosition())
			< getDistance(m_roadList[m_currentRoadIndex]->getTiles()[0][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition(),
				m_roughPath[index - 1]->getPosition()))
		{
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][0]->getPosition());
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition());
		}
		else
		{
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition());
			m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][0]->getPosition());
		}

		m_path.push_back(m_roughPath[index]->getCenter());
		index++;
	}


	auto m_checkTimeEnd = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time = m_checkTimeEnd - m_checkTimeStart;

	std::cout << time.count() << std::endl;

	for (int i = 0; i < 2; i++)
	{
		m_startEndIds[i] = -10;
	}
}

void Map::processLeftMouseKey(sf::Vector2f t_carPos,sf::Vector2i t_mousePos)
{
	float startDist;
	float currentStartDist = 0;
	int endDist;
	m_startEndIds[1] = -1;
	for (int i = 0; i < m_townList.size(); i++)
	{
		startDist = getDistance(m_townList[i]->getCenter(), t_carPos);
		endDist = getDistance(m_townList[i]->getCenter(),sf::Vector2f(t_mousePos));

		if (startDist < currentStartDist || currentStartDist == 0)
		{
			currentStartDist = startDist;
			m_startEndIds[0] = i;
		}

		if (endDist < 25)
		{
			m_startEndIds[1] = i;
		}
	}

	if (m_startEndIds[1] != -1)
	{
		generatePath(m_startEndIds[0], m_startEndIds[1]);
	}

	for (int i = 0; i < m_townList.size(); i++)
	{
		m_townList[i]->resetFuelValue();
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

float Map::getDistance(sf::Vector2f t_current, sf::Vector2f t_dest)
{
	return  sqrt(pow(t_current.x - t_dest.x, 2) + pow(t_current.y - t_dest.y, 2));
}