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

///source for player entity
#include <Game/Player.h>

//ctor
Player::Player(const sf::Vector2f& startPosition, const sf::Vector2f viewSize, AmmoManager& ammoManager)
	: BaseEntity		(startPosition),
	m_viewSize			(viewSize),
	m_ammoManager		(ammoManager),
	m_initialSize		(160.f, 60.f),
	m_initalPosition	(startPosition),
	m_damage			(15.f) //how much damage to take when hit
{
	//set player appearance and movement properties
	m_shape.setSize(m_initialSize);
	m_shape.setFillColor(sf::Color::Red);
	m_moveSpeed = 800.f;
	m_velocity.x = 1.f;
	m_type = BaseEntity::Type::Player;
}

//public
void Player::Update(float dt)
{
	//update position
	m_shape.move(m_velocity * m_moveSpeed * dt);
	
	//check bounds
	if(m_shape.getPosition().x  > (m_viewSize.x - m_shape.getSize().x)
		|| m_shape.getPosition().x < 0.f)
	{
		m_shape.move(-m_velocity * m_moveSpeed * dt);
		m_velocity.x =- m_velocity.x;
	}

	//check collision with ammo
	if(m_ammoManager.Collides(m_shape.getGlobalBounds(), m_type))
	{
		m_shape.setSize(sf::Vector2f(m_shape.getSize().x, m_shape.getSize().y - m_damage));
		m_shape.move(0.f, m_damage);
		if(m_shape.getSize().y < 1.f) m_state = State::Dead;
	}
}

void Player::Fire()
{
	m_ammoManager.Fire(m_type, m_shape.getPosition() + (m_shape.getSize() / 2.f));
}

void Player::Reset()
{
	m_shape.setSize(m_initialSize);
	m_shape.setPosition(m_initalPosition);
	m_state = State::Alive;
	m_score = 0u;
	m_rounds = 1u;
}

//private