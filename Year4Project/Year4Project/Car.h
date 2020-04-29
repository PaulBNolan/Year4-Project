#ifndef CAR
#define CAR
#include "SFML/Graphics.hpp"
#include "Town.h"

enum class CarType { Small, Medium, Large };

class Car
{
public:
	Car(std::vector<sf::Vector2f> &t_path);

	void update();
	void render(sf::RenderWindow& t_window);

	void setType(CarType t_type);


	sf::Vector2f getPosition()
	{
		return m_position;
	};

	float getWidth();

	float m_maximumFuel;

	float getDistance(sf::Vector2f t_current, sf::Vector2f t_dest);
	float getFuel();
private:
	sf::CircleShape m_body;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;

	float m_angle;

	std::vector<sf::Vector2f> &m_roughPath;

	int m_widthValue;

	CarType m_carType;
};

#endif