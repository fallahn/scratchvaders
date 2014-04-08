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

///source for invader manager class///
#include <Game/InvaderManager.h>

//ctor
InvaderManager::InvaderManager(const sf::Vector2f& viewSize, AmmoManager& ammoManager, Player& player)
	: m_viewSize			(viewSize),
	m_invaderCount			(0u),
	m_moveFrequency			(1.f),
	m_movementX				(20.f),
	m_movementY				(40.f)
{
	//set bounds of invaders
	m_invaderBounds.width = m_viewSize.x - 200.f;
	m_invaderBounds.height = m_viewSize.y / 4.f;
	m_invaderBounds.left = (m_viewSize.x - m_invaderBounds.width) / 2.f;
	
	//calc spacing - TODO kill magic numbers
	const float xCount = 14.f;
	const float yCount = 4.f;

	const float spaceX = m_invaderBounds.width / xCount;
	const float spaceY = m_invaderBounds.height / yCount;

	const float initY = 6.f;
	sf::Vector2f position(m_invaderBounds.left + spaceX / 2.f, initY);
	
	//create vector of invaders
	for(auto x = 0.f; x < xCount; x++)
	{
		for(auto y = 0.f; y < yCount; y++)
		{
			m_invaders.push_back(std::unique_ptr<Invader>(new Invader(position, ammoManager, player)));
			position.y += spaceY;
		}
		position.x += spaceX;
		position.y = initY;
	}
}

//public
void InvaderManager::Update(float dt)
{
	//update bounds movement
	if(m_movementClock.getElapsedTime().asSeconds() > 
		m_moveFrequency)
	{
		m_movementClock.restart();
		if(m_invaderBounds.left + m_movementX > 0 && 
			m_invaderBounds.left + m_invaderBounds.width + m_movementX < m_viewSize.x)
		{
			//move across screen
			m_invaderBounds.left += m_movementX;
			for(auto&& invader : m_invaders)
				invader->Move(m_movementX, 0.f);
		}
		else //invert X and move lower
		{
			m_movementX =- m_movementX;
			m_invaderBounds.top += m_movementY;
			for(auto&& invader : m_invaders)
				invader->Move(0.f, m_movementY);
		}
	}
	
	
	m_invaderCount = 0u;
	for(auto&& invader : m_invaders)
	{
		invader->Update(dt);
		//count living invaders
		if(invader->GetState() == BaseEntity::State::Alive)
			m_invaderCount++;
	}
}

void InvaderManager::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	for(const auto& invader : m_invaders)
	{
		//draw invaders currently alive
		if(invader->GetState() == BaseEntity::State::Alive)
			rt.draw(invader->GetShape());
	}
}

void InvaderManager::Reset()
{
	for(auto&& invader : m_invaders) invader->Reset();
	m_invaderBounds.top = 0.f;
	m_invaderBounds.left = (m_viewSize.x - m_invaderBounds.width) / 2.f;
}

//private