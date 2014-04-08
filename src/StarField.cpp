/*********************************************************************
Zlib license.

Matt Marchant 2013 - 2014
http://trederia.blogspot.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

///source for starfield used in menu background///
#include <Game/StarField.h>

#include <Game/Shaders/Planet.h>

//star ctor
StarField::Star::Star(float minSpeed, float maxSpeed)
	: m_minSpeed	(minSpeed),
	m_maxSpeed		(maxSpeed),
	m_minSize		(m_minSpeed / 100.f),
	m_maxSize		(m_maxSpeed / 100.f),
	m_speed			(0.f),
	m_size			(0.f),
	m_grow			(false)
{
	m_Reset();
}

void StarField::Star::Update(float dt, const sf::Vector2f& velocity)
{
	Shape.move(velocity * m_speed * dt);
	float change = Helpers::Vectors::GetLength(velocity) * dt;

	if(m_grow)
	{
		m_size += change;
		if(m_size > m_maxSize) m_grow = false;
	}
	else
	{
		m_size -= change;
		if(m_size < m_minSize) m_grow = true;
	}

	m_speed = m_size * 100.f;
	Shape.setSize(sf::Vector2f(m_size, m_size));

	float max = 255.f / m_maxSpeed;
	sf::Uint8 c = static_cast<sf::Uint8>(m_speed * max);
	Colour = sf::Color(c, c, c, 255u);
}

void StarField::Star::m_Reset(void)
{
	m_speed = Helpers::Random::Float(m_minSpeed, m_maxSpeed);
	m_size = m_speed / 100.f;
	Shape.setSize(sf::Vector2f(m_size, m_size));
	Shape.setPosition(Helpers::Random::Float(0.f, 1920.f), Helpers::Random::Float(0.f, 1080.f));

	m_grow = ((Helpers::Random::Int(2, 3) % 2) == 1) ? true : false;
}

//----starfield-----//

//ctor
StarField::StarField()
	: m_acceleration	(0.4f),
	m_randTime			(Helpers::Random::Float(6.f, 15.f))
{
	for(auto i = 0u; i < 10u; i++)
		m_nearStars.push_back(std::shared_ptr<Star>(new Star(900.f, 1800.f)));

	for(auto i = 0u; i < 140u; ++i)
		m_farStars.push_back(std::shared_ptr<Star>(new Star(200.f, 750.f)));

	m_velocity.x = Helpers::Random::Float(-1.f, 1.f);
	m_velocity.y = Helpers::Random::Float(-1.f, 1.f);

	m_planetDiffuse.loadFromFile("assets/textures/environment/globe_diffuse.png");
	m_planetDiffuse.setRepeated(true);
	m_planetCraterNormal.loadFromFile("assets/textures/environment/crater_normal.jpg");
	m_planetCraterNormal.setRepeated(true);
	m_planetGlobeNormal.loadFromFile("assets/textures/environment/globe_normal.png");
	m_planetResolution.x = static_cast<float>(m_planetDiffuse.getSize().x);
	m_planetResolution.y = static_cast<float>(m_planetDiffuse.getSize().y);

	m_planetSprite.setTexture(m_planetDiffuse);
	m_planetSprite.setPosition(300.f, 250.f);

	m_planetShader.loadFromMemory(planetMap, sf::Shader::Fragment);
	m_planetShader.setParameter("normalTexture", m_planetCraterNormal);
	m_planetShader.setParameter("globeNormalTexture", m_planetGlobeNormal);

	m_starTexture.loadFromFile("assets/textures/environment/star.png");
	float texWidth = static_cast<float>(m_starTexture.getSize().x);
	m_texCoords[1] = sf::Vector2f(texWidth, 0.f);
	m_texCoords[2] = sf::Vector2f(texWidth, texWidth);
	m_texCoords[3] = sf::Vector2f(0.f, texWidth);
}

void StarField::Update(float dt, const sf::FloatRect& viewArea)
{
	//parse keyboard and set velocity
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_velocity.x = -1.f;
		m_acceleration = 1.f;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_velocity.x = 1.f;
		m_acceleration = 1.f;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_velocity.y = 1.f;
		m_acceleration = 1.f;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_velocity.y = -1.f;
		m_acceleration = 1.f;
	}
	
	m_velocity = Helpers::Vectors::Normalize(m_velocity) * m_acceleration;
	if(m_acceleration > 0.4f) m_acceleration -= (0.3f * dt);

	//randomise direction occasionally
	if(m_randClock.getElapsedTime().asSeconds() > m_randTime)
	{
		m_randClock.restart();
		m_randTime = Helpers::Random::Float(6.f, 15.f);

		float newX, newY;
		newX = Helpers::Random::Float(-0.5f, 0.6f);
		newY = Helpers::Random::Float(-0.6f, 0.7f);
		m_acceleration += std::abs((m_velocity.x - newX) + (m_velocity.y - newY)) / 2.f;

		m_velocity.x += newX;
		m_velocity.y += newY;
	}

	//update planet position
	m_planetPosition -= m_velocity * 200.f * dt;
	m_planetShader.setParameter("position", sf::Vector2f(m_planetPosition. x / m_planetResolution.x,1.f - ( m_planetPosition.y / m_planetResolution.y)));

	//update near stars
	for(auto star : m_nearStars)
	{
		star->Update(dt, m_velocity);
		m_boundStar(star, viewArea);
	}

	//update far stars
	for(auto star : m_farStars)
	{
		star->Update(dt, -m_velocity);
		m_boundStar(star, viewArea);
	}
}

//private
void StarField::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	sf::VertexArray farStars(sf::PrimitiveType::Quads);
	for(auto star : m_farStars)
	{
		for(auto i = 0u; i < star->Shape.getPointCount(); i++)
			farStars.append(sf::Vertex(star->Shape.getPoint(i) + star->Shape.getPosition(), star->Colour, m_texCoords[i]));
	}
	rt.draw(farStars, &m_starTexture);

	rt.draw(m_planetSprite, &m_planetShader);
	
	sf::VertexArray nearStars(sf::PrimitiveType::Quads);
	for(auto star : m_nearStars)
	{
		for(auto i = 0u; i < star->Shape.getPointCount(); i++)
			nearStars.append(sf::Vertex(star->Shape.getPoint(i) + star->Shape.getPosition(), star->Colour, m_texCoords[i]));
	}
	rt.draw(nearStars, &m_starTexture);
}

void StarField::m_boundStar(std::shared_ptr<Star>& star, const sf::FloatRect& viewArea)
{
		sf::Vector2f position = star->Shape.getPosition();
		if(position.x < 0) position.x += viewArea.width;
		else if(position.x > viewArea.width) position.x -= viewArea.width;

		if(position.y < 0) position.y += viewArea.height;
		else if(position.y > viewArea.height) position.y -= viewArea.height;

		star->Shape.setPosition(position);
}