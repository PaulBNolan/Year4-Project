#ifndef TOWN
#define TOWN
#include <SFML/Graphics.hpp>
#include "Road.h"

class Town
{
public:
	Town(sf::Vector2f t_position, float t_value,int t_id);

	void render(sf::RenderWindow& t_window);
	void setRelatedIds(int t_id);
	sf::Vector2f getPosition();
	sf::Vector2f getCenter();
	int getID();

	void setHeuristic(sf::Vector2f t_dest);
	float getHeuristic();

	void setAccumaltedCost(float t_roadWeight, float t_prevWeight);
	float getAccumaltedCost();

	void setCurrentFuel(float t_fuel);
	void setOriginalFuel(float t_fuel);
	float getCurrentFuel();

	void useFuelValue();
	void resetFuelValue();
	float getFuelValue();

	void setPrevIds(std::vector<int> t_currentPrev, int t_id);
	void pushFrontPrevIds(int t_id);
	void popBackPrevIds();
	void popFrontPrevIds();
	void clearPrevIds();
	std::vector<int> getPrevIds();

	void setPrevId(int t_id);
	int getPrevId();

	void setColor(sf::Color t_color);

	void setChecked(bool t_checked);
	bool getChecked();

	std::vector<int> getRelatedIds();

	void setNumberOfPasses(int t_value);
	int getNumberOfPasses();
private:
	std::vector<int> m_connectedRoadIds;
	int m_townId;

	sf::CircleShape m_town;
	sf::Vector2f m_position;
	sf::Vector2f m_center;

	float m_heuristic;
	float m_accumulatedCost;

	float m_originalFuelValue;
	float m_fuelValue;
	float m_currentFuel;


	std::vector<int> m_prevIds;
	int m_prevId;

	//Used for when nodes with lower values are compared agaisnt nodes with higher fuel values and more stations visited
	int m_numberOfStationsPassed;

	bool m_checked;
	sf::Text m_IdText;
	sf::Font m_font;
};

#endif