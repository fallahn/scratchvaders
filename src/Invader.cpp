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

///source for invader entity///
#include <Game/Invader.h>
#include <Helpers.h>

//ctor
Invader::Invader(const sf::Vector2f& startPosition, AmmoManager& ammoManager, Player& player)
	: BaseEntity		(startPosition), 
	m_ammoManager		(ammoManager),
	m_player			(player),
	m_minShootTime		(10.f),
	m_maxShootTime		(39.f),
	m_shootTime			(Helpers::Random::Float(1.f, 30.f)),
	m_initialPosition	(startPosition)
{
	//set type and appearance of entity
	m_type = BaseEntity::Type::Invader;

	m_shape.setSize(sf::Vector2f(60.f, 50.f));
	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOrigin(40.f, 0.f);
}

//public
void Invader::Update(float dt)
{
	if(m_state == State::Alive)
	{
		//fire weapon at intervals
		if(m_shootClock.getElapsedTime().asSeconds() > m_shootTime)
		{
			Fire();
			m_shootClock.restart();
			m_shootTime = Helpers::Random::Float(m_minShootTime, m_maxShootTime);
		}

		//check for collision with ammo
		if(m_ammoManager.Collides(m_shape.getGlobalBounds(), m_type))
		{
			m_state = State::Dead;
			m_player.AddScore(25u);
		}

		//check for collision with player
		if(m_player.GetShape().getGlobalBounds().intersects(m_shape.getGlobalBounds()))
		{
			m_player.SetState(BaseEntity::State::Dead);
		}
	}
}

void Invader::Fire()
{
	//call the fire function of the ammo manager setting the owner to this entity's type
	m_ammoManager.Fire(m_type, m_shape.getPosition() + (m_shape.getSize() / 2.f));
}

void Invader::Reset()
{
	m_shape.setPosition(m_initialPosition);
	m_state = BaseEntity::State::Alive;
	m_shootClock.restart();
}
//private