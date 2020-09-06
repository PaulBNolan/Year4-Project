#include "Map.h"
/// <summary>
/// The construction of map based on loaded data
/// </summary>
/// <param name="t_map"></param>
/// <param name="t_font"></param>
/// <param name="t_car"></param>
/// <param name="t_dimensions"></param>
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
/// <summary>
/// Hud set up
/// </summary>
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
			//m_multiObjectiveFuelText.setString("Fuel: " + std::to_string(int(m_roughPath.back()->getCurrentFuel())));
			std::cout << m_roughPath.back()->getID() << " Popped" << std::endl;
			m_passedIds.push_back(m_roughPath.back()->getID());
			m_roughPath.pop_back();
			m_nodeQue.erase(m_nodeQue.begin());
		}
	}
}


/// <summary>
/// The main pathfinding algorithm
/// </summary>
/// <param name="t_startId"></param>
/// <param name="t_targetId"></param>
/// <param name="t_roadId"></param>
void Map::generatePath(int t_startId, int t_targetId, int t_roadId)
{

	Town* m_startTown = NULL;
	Town* m_endTown = NULL;

	bool m_newPath = false;
	bool m_oldPath = false;

	//Set up for instances with no paths
	if (t_roadId == -1)
	{
		m_passedIds.clear();
		m_newPath = true;
		m_currentPathMinimumCost = std::numeric_limits<int>::max() - 10000;
		m_roughPath.clear();
		m_startTown = m_townList[t_startId];
		m_endTown = m_townList[t_targetId];
	}
	else
	{
		//Set up for instances with a path already present
		//Both of these are set to the end and start of the current rough path with the id of the closest being found to determine the best location on the path to start off with
		m_endTown = m_roughPath.front();
		t_targetId = m_endTown->getID();

		m_startTown = m_roughPath.back();
		t_startId = m_startTown->getID();

		int m_breakOffPoint = -1;

		std::vector<int> m_previousIds = m_endTown->getPrevIds();
		m_previousIds.pop_back();
		for (int i = 1; i < m_roughPath.size(); i++)
		{
			m_roughPath[i]->setPrevIds(m_previousIds, 1);
			m_roughPath[i]->popBackPrevIds();
			m_previousIds.pop_back();
		}

		/// <summary>
		/// Both of the below situations are used to answer different situations
		/// The blocked situation has a easy to identify point of interference and includes a broken path so the currentpathcost has to be found again
		/// The unblocked situation has a harder to determine point of interference due to it not being part of the path but the path itself is unaffected so its pathcost can still be used as a limiter
		/// after being updated to take into account progress made
		/// </summary>

		if (m_roadList[t_roadId]->getBlocked() == true)
		{
			std::cout << "Path Turned off" << std::endl;
			//Since the path is broken the previous path cost no longer applies
			m_currentPathMinimumCost = std::numeric_limits<int>::max() - 10000;
			//A single for loop is used due to the two nodes being looked for should be adjacent on the list for there connecting path to affect it by being turned off
			for (int i = 1; i < m_nodeQue.size(); i++)
			{
				if (( m_nodeQue[i].m_id == m_roadList[t_roadId]->getRelatedId(0) && m_nodeQue[i - 1].m_id == m_roadList[t_roadId]->getRelatedId(1) )
					||
					(m_nodeQue[i].m_id == m_roadList[t_roadId]->getRelatedId(1) && m_nodeQue[i - 1].m_id == m_roadList[t_roadId]->getRelatedId(0)))
					{
						//m_removedTowns = std::vector<Town*>(m_roughPath.begin() + i, m_roughPath.end());
						//m_roughPath = std::vector<Town*>(m_roughPath.begin(), m_roughPath.begin() + i);
					m_breakOffPoint = i - 1;
					m_oldPath = true;
					}
			}
		}
		else if (m_roadList[t_roadId]->getBlocked() == false)
		{
			std::cout << "Path Turned on" << std::endl;

			if (m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getChecked() == true || m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getChecked() == true)
			{
				float m_estimatedMinimumCost;
				float minimumDistanceToTarget;
				float minimumDistanceToStart;

				int m_closestToStartId;

				//The below two conditions check if the path could be viable
				if (m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getHeuristic() < m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getHeuristic())
				{
					minimumDistanceToTarget = m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getHeuristic();
					minimumDistanceToStart = getDistance(m_roughPath.back()->getCenter(), m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getCenter());

					m_closestToStartId = m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getID();
				}
				else
				{
					minimumDistanceToTarget = m_townList[m_roadList[t_roadId]->getRelatedId(1)]->getHeuristic();
					minimumDistanceToStart = getDistance(m_roughPath.back()->getCenter(), m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getCenter());

					m_closestToStartId = m_townList[m_roadList[t_roadId]->getRelatedId(0)]->getID();
				}

				m_estimatedMinimumCost = minimumDistanceToStart + m_roadList[t_roadId]->getWeight() + minimumDistanceToTarget;

				//The currentpathcost will have to have its value reduced by what has so far been traveled
				m_currentPathMinimumCost -= m_roughPath.back()->getAccumaltedCost();

				if (m_currentPathMinimumCost > m_estimatedMinimumCost)
				{
					m_oldPath = true;
					for (int i = 0; i < m_nodeQue.size(); i++)
					{
						if (m_nodeQue[i].m_id == m_roadList[t_roadId]->getRelatedId(0) || m_nodeQue[i].m_id == m_roadList[t_roadId]->getRelatedId(1))
						{
							m_breakOffPoint = i;
							break;
						}
					}

					//If the above fails to find the point the below will be used to find the most viable one
					if (m_breakOffPoint == -1)
					{
						float m_currentLowest = 10000;

						for (int i = 0; i < m_nodeQue.size(); i++)
						{
							if (getDistance(m_townList[m_nodeQue[i].m_id]->getPosition(), m_townList[m_closestToStartId]->getPosition()) < m_currentLowest)
							{
								m_breakOffPoint = i;
								m_currentLowest = getDistance(m_townList[m_nodeQue[i].m_id]->getPosition(), m_townList[m_closestToStartId]->getPosition());
							}
						}
					}
				}
			}
		}

		/// <summary>
		/// Construction and set up of path
		/// </summary>
		/// <param name="t_startId"></param>
		/// <param name="t_targetId"></param>
		/// <param name="t_roadId"></param>
		if (m_breakOffPoint != -1)
		{
			for (int i = m_roughPath.size() - 1; i >= 0; i--)
			{
				std::cout << "Id: " << m_roughPath[i]->getID() << " Previous List BEFORE";
				for (int j = 0; j < m_roughPath[i]->getPrevIds().size(); j++)
				{
					std::cout << m_roughPath[i]->getPrevIds()[j] << " ";
				}
				std::cout << " " << std::endl;
			}
			m_nodeQue.front().m_previousId = -66;

			for (int i = m_nodeQue.size() - 1; i >= 0; i--)
			{
				m_nodeQue[i].m_accumaltedCost -= m_nodeQue.front().m_accumaltedCost;
			}

			while (m_roughPath.front() != m_townList[m_nodeQue[m_breakOffPoint].m_id])
			{
				//std::cout << m_roughPath.front()->getID() << std::endl;

				m_roughPath.erase(m_roughPath.begin());
				m_removedQue.insert(m_removedQue.begin(), m_nodeQue.back());
				m_nodeQue.pop_back();
			}


			for (int i = m_removedQue.size() - 1; i >= 1; i--)
			{
				m_removedQue[i].m_accumaltedCost -= m_removedQue[i - 1].m_accumaltedCost;
				m_removedQue[i].m_currentFuel -= (m_removedQue[i - 1].m_currentFuel - m_townList[m_nodeQue.back().m_id]->getFuelValue());
			}

			while (m_roughPath.back()->getPrevIds().size() != 0)
			{
				m_roughPath.back()->popBackPrevIds();
				//std::cout << m_passedIds.back() << std::endl;
			}
			for (int i = m_roughPath.size() - 1; i >= 0; i--)
			{
				m_roughPath[i]->clearPrevIds();
				//while (m_roughPath[i]->getPrevIds().size() > (m_roughPath.size() - (i + i)))
				//{
					//std::cout << "Id: " << i << "Current Size: " << m_roughPath[i]->getPrevIds().size() << " Intended Size: " << m_roughPath.size() - i << std::endl;
				//	m_roughPath[i]->popFrontPrevIds();
				//}
				for (int j = 0; j < m_nodeQue.size() - i; j++)
				{
					m_roughPath[i]->setPrevIds(m_roughPath[i]->getPrevIds(), m_nodeQue[j].m_previousId);
				}

				std::cout << "Id: " << m_roughPath[i]->getID() << " Previous List AFTER";
				for (int j = 0; j < m_roughPath[i]->getPrevIds().size(); j++)
				{
					std::cout << m_roughPath[i]->getPrevIds()[j] << " ";
			    }
			 std::cout << " " << std::endl;
			}
			std::cout << m_breakOffPoint;
		}
	}

	std::cout << "Start " << t_startId << std::endl;
	std::cout << "End " << t_targetId << std::endl;

	if (m_newPath == true || m_oldPath == true)
	{
		//Clearing and setting up of previous town and road data
		for (int i = 0; i < m_townList.size(); i++)
		{
			m_townList[i]->setHeuristic(m_endTown->getCenter());
			if (m_roughPath.size() == 0)
			{
				m_townList[i]->clearPrevIds();
				if (m_townList[i] != m_startTown)
				{
					m_townList[i]->setChecked(false);
					m_townList[i]->setAccumaltedCost(std::numeric_limits<int>::max() - 10000, 0);
				}
				m_townList[i]->setCurrentFuel(0);
				m_townList[i]->setNumberOfPasses(0);

			}
			else
			{
				bool clear = true;
				int queId;
				for (int j = 0; j < m_roughPath.size(); j++)
				{
					if (m_roughPath[j] == m_townList[i])
					{
						clear = false;
						queId = m_nodeQue.size() - j - 1;
					}
				}
				if (clear == true)
				{
					std::cout << "Cleared: " << i << std::endl;
					m_townList[i]->setChecked(false);
					m_townList[i]->clearPrevIds();
					m_townList[i]->setCurrentFuel(0);
					m_townList[i]->setAccumaltedCost(std::numeric_limits<int>::max() - 10000, 0);
					m_townList[i]->setNumberOfPasses(0);
				}
				else
				{
					m_townList[i]->setChecked(true);
					m_townList[i]->setCurrentFuel(m_nodeQue[queId].m_currentFuel);
				}
			}

			if (m_townList[i]->getFuelValue() == 0)
			{
				m_townList[i]->setColor(sf::Color::Yellow);
			}
			else
			{
				m_townList[i]->setColor(sf::Color::Cyan);
			}
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



		float m_fuelLimit = m_car->getFuel();

		m_startTown->setChecked(true);
		m_startTown->setCurrentFuel(m_fuelLimit);

		m_townList[t_startId]->setAccumaltedCost(0, 0);
		m_townList[t_startId]->setChecked(true);

		if (m_roughPath.size() == 0)
		{

			m_startTown->setCurrentFuel(m_fuelLimit);
			//m_searchQue.push_back(m_startTown);
			m_nodeQue.clear();
			m_nodeQue.push_back(NodeData(t_startId, -66, m_townList[t_startId]->getAccumaltedCost(), m_townList[t_startId]->getCurrentFuel(), 0));
		}
		if (m_roughPath.size() != 0)
		{
			m_roughPath.clear();
		}
		std::vector<int> t_startList;

		if (t_roadId == -1)
		{
			m_townList[t_startId]->setPrevIds(t_startList, -66);
		}
		auto m_checkTimeStart = std::chrono::high_resolution_clock::now();

		//Variable used to prevent fuel points from being readded to the same path. This is used to prevent loops
		bool m_stationPassed;

		//Used to check if the current node can overwrite the next one
		bool m_moreEfficent;


		//std::vector<NodeData> m_nodeQue;
		//m_nodeQue.push_back(NodeData(t_startId,-66, m_townList[t_startId]->getAccumaltedCost(), m_townList[t_startId]->getCurrentFuel(),0));
		////Algorithm 
		while (m_nodeQue.size() != 0)
		{
			//std::vector<int> searchedTownsCosts;
			std::vector<Town*> searchedTowns;

			int m_currentTownId = m_nodeQue.back().m_id;

			//bool m_onMissingPath = false;
			//int m_pointOnMissingPath;

			//if (m_removedQue.size() > 2 && m_currentPathMinimumCost == std::numeric_limits<int>::max() - 10000, 0)
			//{
			//	for (int i = 0; i < m_removedQue.size() - 2; i++)
			//	{
			//		if (m_currentTownId == m_removedQue[i].m_id)
			//		{
			//			m_pointOnMissingPath = i;
			//			m_onMissingPath = true;
			//			break;
			//		}
			//	}
			//
			//	if (m_onMissingPath == true)
			//	{
			//		for (int i = m_pointOnMissingPath + i; i < m_removedQue.size() - 2; i++)
			//		{
			//			if (m_nodeQue.back().m_currentFuel + m_removedQue[i].m_currentFuel > 0)
			//			{
							//m_townList[m_currentTownId]->setPrevIds(,)
			//			}
			//		}
			//	}
			//}

			//Any path that currently has a higher path cost then current when to the object will be removed. Additionally since the heuristic is set up by finding the distance between the nodes and the object
			//any node with a heuristic higher then the current minimum cost will be skipped over


			if (m_nodeQue.back().m_accumaltedCost + m_townList[m_nodeQue.back().m_id]->getHeuristic() <= m_currentPathMinimumCost)
			{
				//Solution to issue caused by prev ids list beforehand. A prev id is set up first then when it comes to check the nodes neighbours the list is updated to prevent confusion
				if (m_currentTownId != t_startId && m_currentTownId != t_targetId)
				{
					//std::cout << m_townList[m_townList[m_currentTownId]->getPrevId()]->getPrevIds().size() << " " << m_townList[m_currentTownId]->getPrevId() << std::endl;
					m_townList[m_currentTownId]->setPrevIds(m_townList[m_nodeQue.back().m_previousId]->getPrevIds(), m_nodeQue.back().m_previousId);
				}
				//for (int i = 0; i < m_searchQue.back()->getRelatedIds().size(); i++)
				for (int i = 0; i < m_townList[m_nodeQue.back().m_id]->getRelatedIds().size(); i++)
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
					//This if situation is for preventing the checking of unnconnected towns and paths which can already be determined to be unviable


					if (m_roadList[m_roadIndex]->getActive() && (m_roadList[m_roadIndex]->getWeight() + m_nodeQue.back().m_accumaltedCost + m_townList[m_townIndex]->getHeuristic() <= m_currentPathMinimumCost))
					{
						int pathCost = m_nodeQue.back().m_accumaltedCost;
						std::vector<int> m_prevIds = m_townList[m_currentTownId]->getPrevIds();

						float m_estimatedFuelCost = m_nodeQue.back().m_currentFuel - m_roadList[m_roadIndex]->getWeight();

						m_moreEfficent = false;
						
						//The below two situations are used to determine if a situation if more fuel efficent then one of its predecessors
						if (m_estimatedFuelCost > m_townList[m_townIndex]->getCurrentFuel())
						{
							m_moreEfficent = true;
						}
						else if (m_estimatedFuelCost >= m_townList[m_townIndex]->getCurrentFuel() - (1000 * (m_townList[m_townIndex]->getNumberOfPasses() - m_nodeQue.back().m_numberOfPasses))
							&&
							m_townList[m_townIndex]->getNumberOfPasses() > m_nodeQue.back().m_numberOfPasses)
						{
							m_moreEfficent = true;
						}


						if ((m_townIndex != m_prevIds.back() || m_moreEfficent == true) && m_estimatedFuelCost >= 0)
						{
							m_stationPassed = false;
							if (m_nodeQue.back().m_numberOfPasses > 0)
							{
								int stationsChecked = 0;
								for (int i = 1; i < m_townList[m_currentTownId]->getPrevIds().size(); i++)
								{
									//Checks for any occurence of a fuel point being already present on the current path
									if (m_townList[m_currentTownId]->getPrevIds()[i] == m_townIndex && m_townList[m_townList[m_currentTownId]->getPrevIds()[i]]->getFuelValue() > 0)
									{
										m_stationPassed = true;
										break;
									}
									else if (m_townList[m_townList[m_currentTownId]->getPrevIds()[i]]->getFuelValue() > 0)
									{
										stationsChecked++;
										//Every station has been checked and the next station node isnt among them so the loop is broken early
										if (stationsChecked == m_nodeQue.back().m_numberOfPasses)
										{
											break;
										}
									}
								}
							}

							int dist = m_roadList[m_roadIndex]->getWeight() + pathCost + m_townList[m_townIndex]->getHeuristic();
							int m_searchedNode = m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic();

							///The first half of the below situation is to check for the shortest path to the destination while the second half is used
							///when a route encounters a shorter route with the less fuel ie the shorter route will be created first but if it encounters 
							///a longer route with more fuel the longer route takes priority with the exception of the target node. The higher fuel value is to ensure a path can reach the target
							/// so instances were the target node is checked but has taken more time are ignored		

							if (dist < m_searchedNode || (m_moreEfficent == true && m_stationPassed == false && m_townList[m_townIndex]->getChecked() && m_townIndex != t_targetId))
							{
								if (m_moreEfficent == true)
								{
									m_townList[m_townIndex]->setChecked(false);
								}

								if (m_townList[m_townIndex]->getFuelValue() > 0)
								{
									m_townList[m_townIndex]->setNumberOfPasses(m_nodeQue.back().m_numberOfPasses + 1);
								}
								else
								{
									m_townList[m_townIndex]->setNumberOfPasses(m_nodeQue.back().m_numberOfPasses);
								}

								m_townList[m_townIndex]->setCurrentFuel(m_estimatedFuelCost + m_townList[m_townIndex]->getFuelValue());

								m_townList[m_townIndex]->setAccumaltedCost(m_roadList[m_roadIndex]->getWeight(), m_nodeQue.back().m_accumaltedCost);

								if (m_townIndex == t_targetId)
								{
									m_townList[m_townIndex]->setPrevIds(m_townList[m_currentTownId]->getPrevIds(), m_townList[m_currentTownId]->getID());
									m_currentPathMinimumCost = m_townList[m_townIndex]->getAccumaltedCost();
								}
								else
								{
									m_townList[m_townIndex]->setPrevId(m_townList[m_currentTownId]->getID());
								}

							}

							if (m_townList[m_townIndex]->getChecked() == false)
							{
								m_townList[m_townIndex]->setChecked(true);

								//This is the sorting of the nodes searched
								if (searchedTowns.size() == 0)
								{
									searchedTowns.push_back(m_townList[m_townIndex]);
								}
								else
								{
									int m_size = searchedTowns.size();

									for (int i = 0; i < m_size; i++)
									{
										if (m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic() <= searchedTowns.back()->getAccumaltedCost() + searchedTowns.back()->getHeuristic())
										{
											searchedTowns.push_back(m_townList[m_townIndex]);
										}
										else if (m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic() >= searchedTowns.front()->getAccumaltedCost() + searchedTowns.back()->getHeuristic())
										{
											searchedTowns.insert(searchedTowns.begin(), m_townList[m_townIndex]);
										}
										else if (i > 0)
										{
											if (m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic() >= searchedTowns[i]->getAccumaltedCost() + searchedTowns.back()->getHeuristic()
												&&
												m_townList[m_townIndex]->getAccumaltedCost() + m_townList[m_townIndex]->getHeuristic() <= searchedTowns[i - 1]->getAccumaltedCost() + searchedTowns.back()->getHeuristic())
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


				//std::cout << "Current Node" << m_searchQue.back()->getID() << std::endl;
			}
			m_nodeQue.pop_back();

			if (searchedTowns.size() > 0)
			{
				for (int i = 0; i < searchedTowns.size(); i++)
				{
					if (searchedTowns.size() > 0)
					{
						m_nodeQue.push_back(NodeData(searchedTowns[i]->getID(), searchedTowns[i]->getPrevId(), searchedTowns[i]->getAccumaltedCost(), searchedTowns[i]->getCurrentFuel(), searchedTowns[i]->getNumberOfPasses()));
			
					}
				}
			}
		}

		auto m_checkTimeEnd = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> time = m_checkTimeEnd - m_checkTimeStart;



		if (m_townList[t_targetId]->getPrevIds().size() != 0)
		{
			int m_currentIndex = t_targetId;
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
					m_roughPath.push_back(m_townList[m_pathIds.back()]);
					m_townList[m_pathIds.back()]->setColor(sf::Color::Green);
					m_pathIds.pop_back();
				}

			}


			//Refine Path and store data in the event of a dynamic event
			m_path.clear();
			m_path.push_back(m_roughPath.front()->getCenter());
			int index = 1;
			int m_currentRoadIndex;

			m_currentAccumlatedCost = m_roughPath[0]->getAccumaltedCost();
			m_currentFuel = m_roughPath[0]->getCurrentFuel();

			m_fuelValue = m_roughPath[0]->getFuelValue();
			m_previousId = m_roughPath[1]->getID();

			m_nodeQue.insert(m_nodeQue.begin(), NodeData(m_roughPath[0]->getID(), m_previousId, m_roughPath[0]->getAccumaltedCost(), m_roughPath[0]->getCurrentFuel(), m_roughPath[0]->getNumberOfPasses()));

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
					< getDistance(m_roadList[m_currentRoadIndex]->getTiles()[0][m_roadList[m_currentRoadIndex]->getTiles()[0].size() - 1]->getPosition(),
						m_roughPath[index - 1]->getPosition()))
				{
					m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][0]->getPosition());
					m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[1][m_roadList[m_currentRoadIndex]->getTiles()[1].size() - 1]->getPosition());
				}
				else
				{
					m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][m_roadList[m_currentRoadIndex]->getTiles()[3].size() - 1]->getPosition());
					m_path.push_back(m_roadList[m_currentRoadIndex]->getTiles()[3][0]->getPosition());
				}

				m_path.push_back(m_roughPath[index]->getCenter());


				m_currentAccumlatedCost -= m_roadList[m_currentRoadIndex]->getWeight();
				m_currentFuel += m_roadList[m_currentRoadIndex]->getWeight() - m_fuelValue;

				m_fuelValue = m_roughPath[index]->getFuelValue();

				if (index + 1 < m_roughPath.size())
				{
					m_previousId = m_roughPath[index + 1]->getID();
				}
				else
				{
					m_previousId = -66;
				}

				m_roughPath[index]->setAccumaltedCost(m_currentAccumlatedCost, 0);
				m_roughPath[index]->setCurrentFuel(m_currentFuel);

				m_nodeQue.insert(m_nodeQue.begin(), NodeData(m_roughPath[index]->getID(), m_previousId, m_roughPath[index]->getAccumaltedCost(), m_roughPath[index]->getCurrentFuel(), m_roughPath[index]->getNumberOfPasses()));

				index++;
			}

			m_nodeQue.front().m_accumaltedCost = 0;
			m_points.clear();

			sf::CircleShape t_circle;
			for (int i = 0; i < m_path.size(); i++)
			{
				t_circle.setPosition(m_path[i]);
				t_circle.setRadius(1);
				t_circle.setFillColor(sf::Color::White);

				m_points.push_back(t_circle);
			}

		}
		else
		{
			//std::cout << "Path Failed" << std::endl;
		}
		m_checkTimeEnd = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> time2 = m_checkTimeEnd - m_checkTimeStart;

		std::cout << time.count() << std::endl;
		std::cout << time2.count() << std::endl;
		m_multiObjectiveTimeText.setString("Time: " + std::to_string(time.count()));

		float m_accumWidth = m_multiObjectivePathText.getGlobalBounds().width;
		float m_accumHeight = 0;

		m_nodeText.clear();

		for (int i = 0; i < m_passedIds.size(); i++)
		{
			m_nodeText.push_back(sf::Text());

			m_nodeText[i].setString(std::to_string(m_passedIds[i]) + ", ");

			m_nodeText[i].setPosition(sf::Vector2f(m_multiObjectivePathText.getPosition().x + m_accumWidth, m_multiObjectivePathText.getPosition().y));
			m_nodeText[i].setFont(m_font);
			m_nodeText[i].setFillColor(sf::Color::Black);
			m_nodeText[ i].setCharacterSize(m_mapDimensions.y * 0.015);

			m_accumWidth += m_nodeText[i].getGlobalBounds().width;
		}

		for (int i = m_roughPath.size() - 1; i >= 0; i--)
		{
			m_nodeText.push_back(sf::Text());
			if (i > 0)
			{
				m_nodeText[m_passedIds.size() + m_roughPath.size() - 1 - i].setString(std::to_string(m_roughPath[i]->getID()) + ", ");
			}
			else
			{
				m_nodeText[m_passedIds.size() + m_roughPath.size() - 1 - i].setString(std::to_string(m_roughPath[i]->getID()));
			}
			m_nodeText[m_passedIds.size() + m_roughPath.size() - 1 - i].setPosition(sf::Vector2f(m_multiObjectivePathText.getPosition().x + m_accumWidth, m_multiObjectivePathText.getPosition().y));
			m_nodeText[m_passedIds.size() + m_roughPath.size() - 1 - i].setFont(m_font);
			m_nodeText[m_passedIds.size() + m_roughPath.size() - 1 - i].setFillColor(sf::Color::Black);
			m_nodeText[m_passedIds.size() + m_roughPath.size() - 1 - i].setCharacterSize(m_mapDimensions.y * 0.015);

			m_accumWidth += m_nodeText[m_passedIds.size() + m_roughPath.size() - 1 - i].getGlobalBounds().width;
		}
	}
	else
	{
		std::cout << "Algorithm not needed" << std::endl;
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
		if (m_townList[i] != m_startTown)
		{

			m_townList[i]->setAccumaltedCost(std::numeric_limits<int>::max() - 10000, 0);
			m_townList[i]->setChecked(false);
			m_townList[i]->setHeuristic(m_endTown->getCenter());
		}
		//m_townList[i]->clearPrevIds
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

						m_searchQue.insert(m_searchQue.begin(), m_townList[m_townIndex]);
					}
				}
			}
		}

		m_searchQue.pop_back();
		//std::cout << "2";
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


	auto m_checkTimeEnd = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time = m_checkTimeEnd - m_checkTimeStart;

	std::cout << time.count() << std::endl;
	m_aStarTimeText.setString("Time: " + std::to_string(time.count()));

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

	if (nodeClick == false)
	{
		for (int i = 0; i < m_roadList.size(); i++)
		{
			m_roadList[i]->processMouseClick(t_mousePos);

			if (m_roadList[i]->getClickFound())
			{
				if (m_roughPath.size() > 0)
				{
					generatePath(NULL, NULL, i);
				}
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
	t_window.draw(m_multiObjectiveFuelText);

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
	for (int i = 0; i < m_path.size(); i++)
	{
		t_window.draw(m_points[i]);
	}

}

float Map::getDistance(sf::Vector2f t_current, sf::Vector2f t_dest)
{
	return  sqrt(pow(t_current.x - t_dest.x, 2) + pow(t_current.y - t_dest.y, 2));
}