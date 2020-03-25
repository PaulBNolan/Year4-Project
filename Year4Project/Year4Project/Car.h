#ifndef CAR
#define CAR
#include "SFML/Graphics.hpp"
#include "Town.h"
class Car
{
public:
	Car(std::vector<Town*> &t_path);

	void update();
	void render(sf::RenderWindow& t_window);

	sf::Vector2f getPosition()
	{
		return m_position;
	};

	int getWidth();

	float getDistance(sf::Vector2f t_current, sf::Vector2f t_dest);
private:
	sf::CircleShape m_body;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;

	float m_angle;

	std::vector<Town*> &m_path;

	int m_widthValue;
};

#endif