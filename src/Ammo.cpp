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

///source for ammo entity///
#include <Game/BaseEntity.h>

//ctor
Ammo::Ammo(const sf::Vector2f& viewSize)
	: m_viewSize		(viewSize)
{
	m_state = State::Dead;
	m_moveSpeed = 1000.f;
	m_shape.setSize(sf::Vector2f(10.f, 20.f));
	
}

//public
void Ammo::Update(float dt)
{
	if(m_state == State::Alive)
	{
		//move ammo
		m_shape.move(m_velocity * m_moveSpeed * dt);
		//kill if it leaves view area
		if(m_shape.getPosition().y < 0.f ||
			m_shape.getPosition().y > m_viewSize.y)
			m_state = State::Dead;
	}
}

void Ammo::Fire(BaseEntity::Type owner, const sf::Vector2f& position)
{
	m_shape.setPosition(position);
	m_owner = owner;
	//set direction based on owner
	if(m_owner == BaseEntity::Type::Player)
	{
		m_shape.setFillColor(sf::Color(255u, 127u, 0u));
		m_velocity.y = -1.f;
	}
	else
	{
		m_shape.setFillColor(sf::Color(0u, 127u, 255u));
		m_velocity.y = 1.f;
	}
	m_state = State::Alive;
}

