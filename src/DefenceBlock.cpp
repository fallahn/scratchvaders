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

///source for defence block entity///
#include <Game/DefenceBlock.h>

//ctor
DefenceBlock::DefenceBlock(const sf::Vector2f& position, AmmoManager& ammoManager)
	: BaseEntity			(position),
	m_initialPosition		(position),
	m_initialSize			(200.f, 30.f),
	m_ammoManager			(ammoManager),
	m_damageAmount			(3.f)
{
	//set appearance
	m_shape.setSize(m_initialSize);
	m_shape.setFillColor(sf::Color::Blue);
	m_shape.setOrigin(m_initialSize / 2.f);
}

//public
void DefenceBlock::Update(float dt)
{
	if(m_state == State::Alive)
	{
		//state ourself as being type player so we only get damaged by enemy fire
		if(m_ammoManager.Collides(m_shape.getGlobalBounds(), Type::Player))
		{
			m_shape.setSize(sf::Vector2f(m_shape.getSize().x, m_shape.getSize().y - m_damageAmount));
			m_shape.move(0.f, m_damageAmount);
			if(m_shape.getSize().y < 1.f) m_state = State::Dead;
		}
	}
}

void  DefenceBlock::Reset()
{
	m_shape.setSize(m_initialSize);
	m_shape.setPosition(m_initialPosition);
	m_state = State::Alive;
}

//private