#ifndef TOWN
#define TOWN
#include <SFML/Graphics.hpp>
#include "Road.h"

class Town
{
public:
	Town(sf::Vector2f t_position, int t_id);

	void render(sf::RenderWindow& t_window);
	void setRelatedIds(int t_id);
	sf::Vector2f getPosition();
	sf::Vector2f getCenter();
	int getID();

	void setHeuristic(sf::Vector2f t_dest);
	float getHeuristic();

	void setAccumaltedCost(float t_roadWeight, float t_prevWeight);
	float getAccumaltedCost();

	void setPrevId(int t_id);
	int getPrevId();

	void setColor(sf::Color t_color);

	void setChecked(bool t_checked);
	bool getChecked();

	std::vector<int> getRelatedIds();
private:
	std::vector<int> m_connectedRoadIds;
	int m_townId;

	sf::CircleShape m_town;
	sf::Vector2f m_position;
	sf::Vector2f m_center;
	float m_heuristic;
	float m_accumulatedCost;
	int m_prevId;
	bool m_checked;
	sf::Text m_IdText;
	sf::Font m_font;
};

#endif