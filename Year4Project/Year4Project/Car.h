#ifndef CAR
#define CAR
#include "SFML/Graphics.hpp"
#include "Town.h"
class Car
{
public:
	Car(std::vector<sf::Vector2f> &t_path);

	void update();
	void render(sf::RenderWindow& t_window);

	sf::Vector2f getPosition()
	{
		return m_position;
	};

	int getWidth();

	float m_maximumFuel;

	float getDistance(sf::Vector2f t_current, sf::Vector2f t_dest);
private:
	sf::CircleShape m_body;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;

	float m_angle;

	std::vector<sf::Vector2f> &m_roughPath;

	int m_widthValue;
};

#endif