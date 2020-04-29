#include "Car.h"

Car::Car(std::vector<sf::Vector2f> &t_path):
	m_roughPath(t_path)
{
}

void Car::update()
{
	
	if (m_roughPath.size() != 0)
	{
		if (m_velocity == sf::Vector2f(0, 0))
		{
			m_angle = atan2(m_position.y - m_roughPath.back().y, m_position.x - m_roughPath.back().x);
			m_velocity = sf::Vector2f(cos(m_angle), sin(m_angle));
		}

		if (getDistance(m_position, m_roughPath.back()) < 4)
		{
			//std::cout << "Fuel " << m_path.back()->getCurrentFuel() << std::endl;
			m_roughPath.pop_back();
			if (m_roughPath.size() > 0)
			{
				m_angle = atan2(m_position.y - m_roughPath.back().y, m_position.x - m_roughPath.back().x);
				m_velocity = sf::Vector2f(cos(m_angle), sin(m_angle));
			}
		}

		m_position -= m_velocity;

		m_body.setPosition(m_position);
	}
	else
	{
		m_velocity = sf::Vector2f(0, 0);
	}
}

void Car::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

void Car::setType(CarType t_type)
{
	m_position = sf::Vector2f(50, 50);

	switch (t_type)
	{
	case CarType::Small:
		m_body.setRadius(1);
		m_body.setOrigin(sf::Vector2f(1, 1));
		m_body.setPosition(m_position);
		m_body.setFillColor(sf::Color::Red);
		m_maximumFuel = 1000;
		break;

	case CarType::Medium:
		m_body.setRadius(1.5f);
		m_body.setOrigin(sf::Vector2f(1.5f, 1.5f));
		m_body.setPosition(m_position);
		m_body.setFillColor(sf::Color::White);
		m_maximumFuel = 1500;
		break;

	case CarType::Large:
		m_body.setRadius(2);
		m_body.setOrigin(sf::Vector2f(2, 2));
		m_body.setPosition(m_position);
		m_body.setFillColor(sf::Color::Yellow);
		m_maximumFuel = 2000;
		break;
	}
}

float Car::getWidth()
{
	return m_body.getRadius();
}

float Car::getFuel()
{
	return m_maximumFuel;
}

float Car::getDistance(sf::Vector2f t_current, sf::Vector2f t_dest)
{
	return  sqrt(pow(t_current.x - t_dest.x,2) + pow(t_current.y - t_dest.y, 2));
}