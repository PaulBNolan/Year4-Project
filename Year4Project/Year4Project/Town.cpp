#include "Town.h"

Town::Town(sf::Vector2f t_position, float t_value, int t_id)
{
	float radius = 12.5;

	m_position = t_position;
	m_town.setPosition(m_position);
	m_town.setRadius(radius);

	m_town.setOutlineThickness(2.0f);

	m_center = sf::Vector2f(t_position.x + radius, t_position.y + radius);
	m_townId = t_id;
	m_accumulatedCost = 0;
	m_IdText.setString(std::to_string(m_townId));
	m_IdText.setPosition(m_position);
	m_IdText.setCharacterSize(radius * 2);
	m_IdText.setFillColor(sf::Color::Red);

	m_font.loadFromFile("ASSETS/FONTS/arial.ttf");

	m_IdText.setFont(m_font);


	setOriginalFuel(t_value);
	m_fuelValue = t_value;
}

void Town::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_town);
	t_window.draw(m_IdText);
}

void Town::setRelatedIds(int t_id)
{
	m_connectedRoadIds.push_back(t_id);
}

void Town::setHeuristic(sf::Vector2f t_dest)
{
	m_heuristic = sqrt(pow(m_center.x - t_dest.x,2) + pow(m_center.y - t_dest.y, 2));
}

void Town::setAccumaltedCost(float t_roadWeight, float t_prevCost)
{
	m_accumulatedCost = t_prevCost + t_roadWeight;
}

void Town::pushFrontPrevIds(int t_id)
{
	m_prevIds.insert(m_prevIds.begin(),t_id);
}

void Town::setPrevIds(std::vector<int> t_currentPrev, int t_id)
{
	m_prevIds = t_currentPrev;
	m_prevIds.push_back(t_id);
}

void Town::popBackPrevIds() {
	m_prevIds.pop_back();
}

void Town::popFrontPrevIds()
{
	m_prevIds.erase(m_prevIds.begin());
}

void Town::clearPrevIds()
{
	m_prevIds.clear();
}

void Town::setPrevId(int t_id)
{
	m_prevId = t_id;
}

void Town::setChecked(bool t_checked)
{
	m_checked = t_checked;
}

void Town::setCurrentFuel(float t_fuel)
{
	m_currentFuel = t_fuel;
}
void Town::setOriginalFuel(float t_fuel)
{
	m_originalFuelValue = t_fuel;

	if (m_originalFuelValue == 0)
	{
		m_town.setFillColor(sf::Color::Yellow);
		m_town.setOutlineColor(sf::Color::Blue);
	}
	else
	{
		m_town.setFillColor(sf::Color::Cyan);
		m_town.setOutlineColor(sf::Color::Magenta);
	}
}

void Town::useFuelValue()
{
	m_fuelValue = 0;
}
void Town::resetFuelValue()
{
	m_fuelValue = m_originalFuelValue;
}

void Town::setColor(sf::Color t_color)
{
	m_town.setFillColor(t_color);
}

sf::Vector2f Town::getPosition()
{
	return m_position;
}

sf::Vector2f Town::getCenter()
{
	return m_center;
}

int Town::getID()
{
	return m_townId;
}

std::vector<int> Town::getPrevIds()
{
	return m_prevIds;
}

int Town::getPrevId()
{
	return m_prevId;
}

float Town::getHeuristic()
{
	return m_heuristic;
}

float Town::getAccumaltedCost()
{
	return m_accumulatedCost;
}

bool Town::getChecked()
{
	return m_checked;
}

std::vector<int> Town::getRelatedIds()
{
	return m_connectedRoadIds;
}

float Town::getCurrentFuel()
{
	return m_currentFuel;
}

float Town::getFuelValue()
{
	return m_fuelValue;
}

void Town::setNumberOfPasses(int t_value)
{
	m_numberOfStationsPassed = t_value;
}
int Town::getNumberOfPasses()
{
	return m_numberOfStationsPassed;
}