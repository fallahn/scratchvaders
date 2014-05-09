//basic particle style starfield for menu background//
#ifndef STARFIELD_H_
#define STARFIELD_H_

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <Helpers.h>

#include <memory>

class StarField final
	: public sf::Drawable,
	private sf::NonCopyable
{
public:
	StarField();

	void Update(float dt, const sf::FloatRect& viewArea);

private:
	struct Star
	{
		Star(float minSpeed = 200.f, float maxSpeed = 800.f);
		void Update(float dt, const sf::Vector2f& velocity);
		sf::RectangleShape Shape;
		sf::Color Colour;

	private:
		const float m_minSpeed;
		const float m_maxSpeed;
		const float m_minSize;
		const float m_maxSize;
		float m_speed, m_size;
		bool m_grow;
		void m_Reset(void);
	};

	std::vector< std::unique_ptr<Star> > m_nearStars;
	std::vector< std::unique_ptr<Star> > m_farStars;
	sf::Vector2f m_velocity;
	sf::Clock m_randClock;

	float m_acceleration, m_randTime;

	//planet in background
	sf::Texture m_planetDiffuse, m_planetCraterNormal, m_planetGlobeNormal;
	sf::Sprite m_planetSprite;
	sf::Shader m_planetShader;
	sf::Vector2f m_planetResolution, m_planetPosition;

	//star texture
	sf::Texture m_starTexture;
	sf::Vector2f m_texCoords[4];

	void m_boundStar(Star& star, const sf::FloatRect& viewArea);
	void draw(sf::RenderTarget& rt, sf::RenderStates states)const override;
};



#endif //STARFIELD_H_