#include "Map.h"

Map::Map(MapData t_map, sf::Font t_font, Car *& t_car, sf::Vector2f t_dimensions):
	m_car(t_car)
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

	m_font = t_font;

	m_mapDimensions = t_dimensions;
	setHud();
}

void Map::setHud()
{
	m_multiObjectiveHudBox.setPosition(sf::Vector2f(0, m_mapDimensions.y * 0.8));
	m_multiObjectiveHudBox.setSize(sf::Vector2f(m_mapDimensions.x * 0.5, m_mapDimensions.y * 0.1));
	m_multiObjectiveHudBox.setFillColor(sf::Color::Red);

	m_aStarHudBox.setPosition(sf::Vector2f(0, m_mapDimensions.y * 0.9));
	m_aStarHudBox.setSize(sf::Vector2f(m_mapDimensions.x * 0.5, m_mapDimensions.y * 0.1));
	m_aStarHudBox.setFillColor(sf::Color::Blue);

	m_multiObjectiveText.setFont(m_font);
	m_multiObjectiveText.setCharacterSize(m_mapDimensions.y * 0.05);
	m_multiObjectiveText.setFillColor(sf::Color::Black);
	m_multiObjectiveText.setPosition(sf::Vector2f(2.5, m_mapDimensions.y * 0.8));
	m_multiObjectiveText.setString("Fuel Algorithm");

	m_multiObjectiveFuelText.setFont(m_font);
	m_multiObjectiveFuelText.setCharacterSize(m_mapDimensions.y * 0.05);
	m_multiObjectiveFuelText.setFillColor(sf::Color::Black);
	m_multiObjectiveFuelText.setPosition(sf::Vector2f(2.5, m_mapDimensions.y * 0.85));
	m_multiObjectiveFuelText.setString("Fuel: ");

	m_multiObjectiveTimeText.setFont(m_font);
	m_multiObjectiveTimeText.setCharacterSize(m_mapDimensions.y * 0.05);
	m_multiObjectiveTimeText.setFillColor(sf::Color::Black);
	m_multiObjectiveTimeText.setPosition(sf::Vector2f(m_mapDimensions.x * 0.25, m_mapDimensions.y * 0.8));
	m_multiObjectiveTimeText.setString("Time");

	m_multiObjectivePathText.setFont(m_font);
	m_multiObjectivePathText.setCharacterSize(m_mapDimensions.y * 0.05);
	m_multiObjectivePathText.setFillColor(sf::Color::Black);
	m_multiObjectivePathText.setPosition(sf::Vector2f(m_mapDimensions.x * 0.25, m_mapDimensions.y * 0.85));
	m_multiObjectivePathText.setString("Path: ");

	m_aStarText.setFont(m_font);
	m_aStarText.setCharacterSize(m_mapDimensions.y * 0.05);
	m_aStarText.setFillColor(sf::Color::Black);
	m_aStarText.setPosition(sf::Vector2f(2.5, m_mapDimensions.y * 0.9));
	m_aStarText.setString("A Star Algorithm");

	m_aStarTimeText.setFont(m_font);
	m_aStarTimeText.setCharacterSize(m_mapDimensions.y * 0.05);
	m_aStarTimeText.setFillColor(sf::Color::Black);
	m_aStarTimeText.setPosition(sf::Vector2f(m_mapDimensions.x * 0.25, m_mapDimensions.y * 0.9));
	m_aStarTimeText.setString("Time: ");

	m_aStarPathText.setFont(m_font);
	m_aStarPathText.setCharacterSize(m_mapDimensions.y * 0.05);
	m_aStarPathText.setFillColor(sf::Color::Black);
	m_aStarPathText.setPosition(sf::Vector2f(m_mapDimensions.x * 0.25, m_mapDimensions.y * 0.95));
	m_aStarPathText.setString("Path: ");
}

void Map::update()
{
	if (m_roughPath.size() != 0 && m_path.size() != 0)
	{
		if (m_roughPath.back()->getCenter() == m_path.back())
		{
			m_multiObjectiveFuelText.setString("Fuel: " + std::to_string(int(m_roughPath.back()->getCurrentFuel())));
			m_roughPath.pop_back();
		}
	}
}

void Map::generatePath(int t_startId, int t_targetId, int t_roadId)
{
	Town* m_startTown = NULL;
	Town* m_endTown = NULL;
	std::cout << t_roadId << std::endl;
	if (t_roadId == -1)
	{
		m_startTown = m_townList[t_startId];
		m_endTown = m_townList[t_targetId];
	}
	else
	{
		m_endTown = m_roughPath.back();

		if (m_roadList[t_roadId]->getBlocked() == true)
		{
			//A single for loop is used due to the two nodes being looked for should be adjacent on the list for there connecting path to affect it by being turned off
			for (int i = 0; i < m_roughPath.size() - 1; i++)
			{
				if (( m_roughPath[i]->getID() == m_roadList[t_roadId]->getRelatedId(0) && m_roughPath[i + 1]->getID() == m_roadList[t_roadId]->getRelatedId(1) )
					||
					(m_roughPath[i]->getID() == m_roadList[t_roadId]->getRelatedId(1) && m_roughPath[i + 1]->getID() == m_roadList[t_roadId]->getRelatedId(0)))
					{
						m_removedTowns = std::vector<Town*>(m_roughPath.begin() + i, m_roughPath.end());
						m_roughPath = std::vector<Town*>(m_roughPath.begin(), m_roughPath.begin() + i);
						m_startTown = m_roughPath.back();
					}
			}
		}
		else if (m_roadList[t_roadId]->getBlocked() == false)
		{
			//An a* derived algorithm is a directed algorithm so if a new possibility were to open up on a previously explored path theres a possibility it would create a more efficent path then the current
			if (m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getChecked() == true || m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getChecked() == true)
			{
				m_endTown = m_roughPath.back();
				int currentIndex;
				if (m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getChecked() == true && m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getChecked() == true)
				{
					std::cout << "Both nodes are retraced until the connected path node is found." << std::endl;
				}
				else
				{
					if (m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getChecked() == true)
					{
						currentIndex = m_roadList[t_roadId]->getRelatedId(0);

					}
					else if (m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getChecked() == true)
					{
						currentIndex = m_roadList[t_roadId]->getRelatedId(1);
					}

					for (int i = m_townList[currentIndex]->getPrevIds().size() - 1; i >= 0; i--)
					{
						for (int j = 0; j < m_roughPath.size(); j++)
						{
							if (m_townList[m_townList[currentIndex]->getPrevIds()[i]] == m_roughPath[j])
							{
								m_startTown = m_roughPath[j];
								break;
							}
						}
					}
					if (m_startTown == NULL)
					{
						m_startTown = m_roughPath.front();
					}
				}
			}
		}
	}

	//std::cout << "Start " << t_startId << std::endl;
	//std::cout << "End " << t_targetId << std::endl;

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
		if (m_roadList[i]->getBlocked() == false && m_roadList[i]->getWidth() >= m_car->getWidth())
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

	float m_fuelLimit = m_car->getFuel();

	m_startTown->setChecked(true);
	m_startTown->setCurrentFuel(m_fuelLimit);

	m_townList[t_startId]->setAccumaltedCost(0, 0);
	m_townList[t_startId]->setChecked(true);
	std::vector<int> t_startList;
	m_townList[t_startId]->setPrevIds(t_startList ,-66);

	auto m_checkTimeStart = std::chrono::high_resolution_clock::now();
	
	while (m_searchQue.size() != 0 && m_searchQue.back()->getID() != t_targetId)
	{
		std::vector<int> searchedTownsCosts;
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

			//std::cout << "Size: " << m_searchQue.back()->getRelatedIds().size() << std::endl;
			//std::cout << m_currentTownId <<" Checking Town: " << m_townIndex << std::endl;
			if (m_roadList[m_roadIndex]->getActive())
			{
				int pathCost = m_townList[m_currentTownId]->getAccumaltedCost();
				std::vector<int> m_prevIds = m_townList[m_currentTownId]->getPrevIds();

				float m_estimatedFuelCost = m_townList[m_currentTownId]->getCurrentFuel() - m_roadList[m_roadIndex]->getWeight();
				if ((m_townIndex != m_prevIds.back() || m_estimatedFuelCost > m_townList[m_townIndex]->getCurrentFuel()) && m_estimatedFuelCost >= 0)
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
						//std::cout << "Failed Dist " << dist << " " << m_searchedNode <<std::endl;
					}

					if (m_townList[m_townIndex]->getChecked() == false)
					{
						m_townList[m_townIndex]->setChecked(true);


						//Method used to sort checked roads in order of highest to lowest
						//Bug found that causes two different paths being created between two points
						if (searchedTownsCosts.size() == 0)
						{
							searchedTowns.push_back(m_townList[m_townIndex]);
							searchedTownsCosts.push_back(dist);
						}
						else
						{
							/*int m_size = searchedTowns.size();
							
							for (int i = 0; i < m_size; i++)
							{
								if(dist < searchedTownsCosts.back())
								{
									searchedTowns.push_back(m_townList[m_townIndex]);
									searchedTownsCosts.push_back(dist);
								}
								else if (dist > searchedTownsCosts.front())
								{
									searchedTowns.insert(searchedTowns.begin(),m_townList[m_townIndex]);
									searchedTownsCosts.insert(searchedTownsCosts.begin(),dist);
								}
								else if(i > 0)
								{
									if (dist > searchedTownsCosts[i]
										&&
										dist < searchedTownsCosts[i - 1])
									{
										searchedTowns.insert(searchedTowns.begin() + i, m_townList[m_townIndex]);
										searchedTownsCosts.insert(searchedTownsCosts.begin() + i ,dist);
									}
								}
							}*/
							int m_size = searchedTowns.size();

							for (int i = 0; i < m_size; i++)
							{
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

						if (m_townIndex == t_targetId)
						{
							break;
						}
					}
				}
			}
		}

		m_searchQue.pop_back();
		for (int i = 0; i < searchedTowns.size(); i++)
		{
			if (searchedTowns.size() > 0)
			{
				m_searchQue.push_back(searchedTowns[i]);
			}
		}
	}

	if (m_townList[t_targetId]->getPrevIds().size() != 0)
	{
		int m_currentIndex = t_targetId;
		m_roughPath.clear();
		//m_townList[t_targetId]->getPrevId().erase(m_townList[t_targetId]->getPrevId().begin());
		std::vector<int> m_pathIds = m_townList[t_targetId]->getPrevIds();

		m_pathIds.push_back(t_targetId);

		while (m_pathIds.size() != 1)
		{
			if (m_currentIndex == t_targetId && m_townList[t_targetId]->getPrevIds().size() == 0)
			{
				//std::cout << "Not Enough Fuel";
			}
			else
			{
				//std::cout << m_currentIndex << std::endl;
				//std::cout << m_townList[m_pathIds.back()]->getCurrentFuel() << std::endl;
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

	}
	else
	{
		//std::cout << "Path Failed" << std::endl;
	}
	auto m_checkTimeEnd = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time = m_checkTimeEnd - m_checkTimeStart;

	std::cout << time.count() << std::endl;
	m_multiObjectiveTimeText.setString("Time: " + std::to_string(time.count()));

	float m_accumWidth = m_multiObjectivePathText.getGlobalBounds().width;
	float m_accumHeight = 0;

	m_nodeText.clear();
	for (int i = m_roughPath.size() - 1; i >= 0; i--)
	{
		m_nodeText.push_back(sf::Text());
		if (i > 0)
		{
			m_nodeText[m_roughPath.size() - 1 - i].setString(std::to_string(m_roughPath[i]->getID()) + ", ");
		}
		else
		{
			m_nodeText[m_roughPath.size() - 1 - i].setString(std::to_string(m_roughPath[i]->getID()));
		}
		m_nodeText[m_roughPath.size() - 1 - i].setPosition(sf::Vector2f(m_multiObjectivePathText.getPosition().x + m_accumWidth, m_multiObjectivePathText.getPosition().y));
		m_nodeText[m_roughPath.size() - 1 - i].setFont(m_font);
		m_nodeText[m_roughPath.size() - 1 - i].setFillColor(sf::Color::Black);
		m_nodeText[m_roughPath.size() - 1 - i].setCharacterSize(m_mapDimensions.y * 0.015);

		m_accumWidth += m_nodeText[m_roughPath.size() - 1 - i].getGlobalBounds().width;
	}
}

void Map::generatePathAStar(int t_startId, int t_endId)
{
	Town* m_startTown = m_townList[t_startId];
	Town* m_endTown = m_townList[t_endId];

	//std::cout << "Start " << t_startId << std::endl;
	//std::cout << "End " << t_endId << std::endl;

	for (int i = 0; i < m_townList.size(); i++)
	{
		m_townList[i]->setHeuristic(m_endTown->getCenter());
		m_townList[i]->clearPrevIds();
		if (m_townList[i] != m_startTown)
		{
			m_townList[i]->setAccumaltedCost(std::numeric_limits<int>::max() - 10000, 0);
			m_townList[i]->setChecked(false);
		}
		m_townList[i]->setCurrentFuel(0);
	}

	std::vector<Town*> m_searchQue;

	m_searchQue.push_back(m_startTown);

	float m_fuelLimit = m_car->getFuel();

	m_startTown->setChecked(true);
	m_startTown->setCurrentFuel(m_fuelLimit);

	m_townList[t_startId]->setAccumaltedCost(0, 0);
	m_townList[t_startId]->setChecked(true);

	m_townList[t_startId]->setPrevId(-66);

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

			//std::cout << "Size: " << m_searchQue.back()->getRelatedIds().size() << std::endl;
			//std::cout << m_currentTownId << " Checking Town: " << m_townIndex << std::endl;
			if (m_roadList[m_roadIndex]->getActive())
			{
				int pathCost = m_townList[m_currentTownId]->getAccumaltedCost();

				float m_estimatedFuelCost = m_townList[m_currentTownId]->getCurrentFuel() - m_roadList[m_roadIndex]->getWeight();

				if (m_townIndex != m_townList[m_currentTownId]->getPrevId())
				{
					int dist = m_roadList[m_roadIndex]->getWeight() + pathCost + m_townList[m_townIndex]->getHeuristic();
					int m_searchedNode = m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic();

					if (dist < m_searchedNode)
					{
						m_townList[m_townIndex]->setCurrentFuel(m_estimatedFuelCost + m_townList[m_townIndex]->getFuelValue());
						m_townList[m_townIndex]->setPrevId(m_townList[m_currentTownId]->getID());
						m_townList[m_townIndex]->setAccumaltedCost(m_roadList[m_roadIndex]->getWeight(), m_townList[m_currentTownId]->getAccumaltedCost());

					}
					else
					{
						//std::cout << "Failed Dist " << dist << " " << m_searchedNode << std::endl;
					}

					if (m_townList[m_townIndex]->getChecked() == false)
					{
						m_townList[m_townIndex]->setChecked(true);

						if (searchedTowns.size() == 0)
						{
							searchedTowns.push_back(m_townList[m_townIndex]);
						}
						else
						{
							int m_size = searchedTowns.size();

							for (int i = 0; i < m_size; i++)
							{
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
	m_aStarRoughPath.clear();
	//m_townList[t_targetId]->getPrevId().erase(m_townList[t_targetId]->getPrevId().begin());
	int m_pathId = t_endId;

	while (m_pathId > -1)
	{
			//std::cout << m_currentIndex << std::endl;
			//std::cout << m_townList[m_pathId]->getCurrentFuel() << std::endl;
		m_aStarRoughPath.push_back(m_townList[m_pathId]);

			m_pathId = m_townList[m_pathId]->getPrevId();
	}

	m_aStarPath.clear();
	m_aStarPath.push_back(m_aStarRoughPath.front()->getCenter());
	int index = 1;
	int m_currentRoadIndex;
	while (m_aStarPath.back() != m_aStarRoughPath.back()->getCenter())
	{
		for (int i = 0; i < m_aStarRoughPath[index - 1]->getRelatedIds().size(); i++)
		{
			for (int j = 0; j < m_aStarRoughPath[index]->getRelatedIds().size(); j++)
			{
				if (m_aStarRoughPath[index - 1]->getRelatedIds()[i] == m_aStarRoughPath[index]->getRelatedIds()[j])
				{
					m_currentRoadIndex = m_aStarRoughPath[index]->getRelatedIds()[j];
				}
			}
		}

		if (getDistance(m_roadList[m_currentRoadIndex]->getTiles()[0][0]->getPosition(), m_aStarRoughPath[index - 1]->getPosition())
			< getDistance(m_roadList[m_currentRoadIndex]->getTiles()[0][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition(),
				m_aStarRoughPath[index - 1]->getPosition()))
		{
			m_aStarPath.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][0]->getPosition());
			m_aStarPath.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition());
		}
		else
		{
			m_aStarPath.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][m_roadList[m_currentRoadIndex]->getTiles().size() - 1]->getPosition());
			m_aStarPath.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][0]->getPosition());
		}

		m_aStarPath.push_back(m_aStarRoughPath[index]->getCenter());
		index++;
	}


	auto m_checkTimeEnd = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time = m_checkTimeEnd - m_checkTimeStart;

	std::cout << time.count() << std::endl;
	m_aStarTimeText.setString("Time: " + std::to_string(time.count()));

	float m_accumWidth = m_aStarPathText.getGlobalBounds().width;
	float m_accumHeight = 0;

	m_AStarNodeText.clear();
	for (int i = m_aStarRoughPath.size() - 1; i >= 0; i--)
	{
		m_AStarNodeText.push_back(sf::Text());
		if (i > 0)
		{
			m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].setString(std::to_string(m_aStarRoughPath[i]->getID()) + ", ");
		}
		else
		{
			m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].setString(std::to_string(m_aStarRoughPath[i]->getID()));
		}
		m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].setPosition(sf::Vector2f(m_aStarPathText.getPosition().x + m_accumWidth, m_aStarPathText.getPosition().y));
		m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].setFont(m_font);

		if (m_aStarRoughPath[i]->getCurrentFuel() > 0)
		{
			m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].setFillColor(sf::Color::Green);
		}
		else
		{
			m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].setFillColor(sf::Color::Red);
		}
		m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].setCharacterSize(m_mapDimensions.y * 0.015);

		m_accumWidth += m_AStarNodeText[m_aStarRoughPath.size() - 1 - i].getGlobalBounds().width;
	}
}

void Map::processMouseClick(sf::Vector2f t_carPos,sf::Vector2i t_mousePos)
{
	float startDist;
	float currentStartDist = 0;
	int endDist;
	m_startEndIds[1] = -1;

	bool nodeClick = false;

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
			nodeClick = true;
		}
	}

	for (int i = 0; i < m_roadList.size(); i++)
	{
		if (nodeClick == false)
		{
			m_roadList[i]->processMouseClick(t_mousePos);

			if (m_roadList[i]->getClickFound())
			{
				generatePath(NULL, NULL, i);
				break;
			}
		}
	}

	if (m_startEndIds[1] != -1)
	{
		generatePath(m_startEndIds[0], m_startEndIds[1]);

		for (int i = 0; i < m_townList.size(); i++)
		{
			m_townList[i]->resetFuelValue();
		}

		generatePathAStar(m_startEndIds[0], m_startEndIds[1]);

		for (int i = 0; i < m_townList.size(); i++)
		{
			m_townList[i]->resetFuelValue();
		}
	}

	for (int i = 0; i < 2; i++)
	{
		m_startEndIds[i] = -10;
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

	t_window.draw(m_multiObjectiveHudBox);
	t_window.draw(m_multiObjectiveText);
	t_window.draw(m_multiObjectiveTimeText);
	t_window.draw(m_multiObjectivePathText);
	//t_window.draw(m_multiObjectiveFuelText);

	for (int i = 0; i < m_nodeText.size(); i++)
	{
		t_window.draw(m_nodeText[i]);
	}

	t_window.draw(m_aStarHudBox);
	t_window.draw(m_aStarText);
	t_window.draw(m_aStarTimeText);
	t_window.draw(m_aStarPathText);

	for (int i = 0; i < m_AStarNodeText.size(); i++)
	{
		t_window.draw(m_AStarNodeText[i]);
	}
}

float Map::getDistance(sf::Vector2f t_current, sf::Vector2f t_dest)
{
	return  sqrt(pow(t_current.x - t_dest.x, 2) + pow(t_current.y - t_dest.y, 2));
}