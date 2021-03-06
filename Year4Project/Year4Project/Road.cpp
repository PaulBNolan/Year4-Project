#include "Road.h"
Road::Road(int t_id, int t_width)
{
	m_id = t_id;
	m_widthValue = t_width;
	m_connectedTownIds[0] = -1;
}

void Road::processMouseClick(sf::Vector2i t_position)
{
	sf::Vector2f position = sf::Vector2f(t_position.x, t_position.y);
	for (int i = 0; i < 5; i++)
	{
		m_clickFound = false;
		for (int j = 0; j < rowNo; j++)
		{
			float distance = sqrt(pow(m_roadTiles[i][j]->getPosition().x - position.x, 2) +
				pow(m_roadTiles[i][j]->getPosition().y - position.y, 2));

			if (distance < m_width && m_blocked == false)
			{
				m_blocked = true;
				m_clickFound = true;
				break;
			}
			else if (distance < m_width && m_blocked == true)
			{
				m_blocked = false;
				m_clickFound = true;
				break;
			}
		}

		if (m_clickFound == true)
		{
			break;
		}
	}
}

void Road::render(sf::RenderWindow& t_window)
{
	if (m_blocked == false)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < rowNo; j++)
			{
				//t_window.draw(m_roadTiles[i][j]);
				m_roadTiles[i][j]->render(t_window);
			}
		}
	}
}

//This function is used to store the related town ids
void Road::setRelatedIds(int t_id)
{
	if (m_connectedTownIds[0] < 0)
	{
		m_connectedTownIds[0] = t_id;
	}
	else
	{
		m_connectedTownIds[1] = t_id;
	}
}

void Road::setActive(bool t_active)
{
	m_active = t_active;
}

//This function is used to set the relevent positions of the road and then construct a series of tiles based on them
void Road::setPositions(sf::Vector2f t_position)
{
	if (m_position[0].x == NULL)
	{
		m_position[0] = t_position;
	}
	else
	{
		m_position[1] = t_position;
		for (int i = 0; i < 2; i++)
		{
			m_road[i].color = sf::Color::Red;
			m_road[i].position = m_position[i];
		}
		m_weight = sqrt(pow(m_position[0].x - m_position[1].x, 2) + pow(m_position[0].y - m_position[1].y, 2));

		int columnNo = 5;
		rowNo = (m_weight) / m_widthValue;

		m_length = (m_weight) / rowNo;
		m_tileColumnDisplacement = sf::Vector2f((m_position[0].x - m_position[1].x) / rowNo,
			(m_position[0].y - m_position[1].y) / rowNo);
		m_width = m_widthValue;

		m_angle = atan2(m_position[0].x - m_position[1].x, m_position[1].y - m_position[0].y) * (180 / (22.0 / 7.0));

		m_tileRowDisplacement = sf::Vector2f(m_width * cos((m_angle) * ((22.0 / 7.0)/180)), 
			m_width * sin((m_angle) * ((22.0 / 7.0)/180)));

		for (int i = 0; i < 5; i++)
		{
			m_roadTiles.push_back(std::vector<Tiles*>());
			for (int j = 0; j < rowNo; j++)
			{
				sf::Vector2f position = sf::Vector2f(m_position[0].x + (m_tileColumnDisplacement.x * -j) + (m_tileRowDisplacement.x * (-i + 2.5)),
					m_position[0].y + (m_tileColumnDisplacement.y * -j) + (m_tileRowDisplacement.y * (-i + 2.5)));

				m_roadTiles[i].push_back(
					new Tiles(sf::Vector2f(m_width, m_length),
						m_angle,
						position));

				if (i != 0 && i != columnNo - 1 && i != (columnNo - 1) / 2 
					&& 
					(j > 0 && j < rowNo - 1))
				{
					m_roadTiles[i][j]->setType(TileType::Path);
				}
				else if ((j == 0 && i > 0 && i < (columnNo - 1) / 2)
					||
					(j == rowNo - 1 && i < columnNo - 1 && i > (columnNo - 1) / 2))
				{
					m_roadTiles[i][j]->setType(TileType::Entrance);
				}
				else if ((j == rowNo - 1 && i > 0 && i < (columnNo - 1) / 2)
					||
					(j == 0 && i < columnNo - 1 && i >(columnNo - 1) / 2))
				{
					m_roadTiles[i][j]->setType(TileType::Exit);
				}
				else
				{
					m_roadTiles[i][j]->setType(TileType::Wall);
				}
			}
		}
	}
}

void Road::setWidth(int t_value)
{
	m_widthValue += t_value;
}

/// <summary>
/// Below are the classes get functions used to get relevent data
/// </summary>
/// <param name="t_i"></param>
/// <returns></returns>

int Road::getRelatedId(int t_i)
{
	return m_connectedTownIds[t_i];
}

int Road::getId()
{
	return m_id;
}

int Road::getWidth()
{
	return m_widthValue;
}

float Road::getWeight()
{
	return m_weight;
}

bool Road::getActive()
{
	return m_active;
}

bool Road::getBlocked()
{
	return m_blocked;
}

bool Road::getClickFound()
{
	return m_clickFound;
}

std::vector<std::vector<Tiles*>> Road::getTiles()
{
	return m_roadTiles;
}