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

///source for ammo manager class///
#include <Game/AmmoManager.h>

//ctor
AmmoManager::AmmoManager(const sf::Vector2f& viewSize)
	: m_fireLimit		(0.5f)
{
	//TODO decide on ammo count
	for(auto i = 0u; i < 20u; i++)
	{
		m_ammos.push_back(std::unique_ptr<Ammo>(new Ammo(viewSize)));
	}
}


//public
void AmmoManager::Update(float dt)
{
	for(auto&& ammo : m_ammos) ammo->Update(dt);
}

void AmmoManager::Fire(BaseEntity::Type owner, const sf::Vector2f& position)
{
	//don't fire anything if time is too small
	//if(m_fireClock.getElapsedTime().asSeconds() < m_fireLimit)
	//	return;
	//else
	//	m_fireClock.restart();
	
	//find a dead ammo
	for(auto&& ammo : m_ammos)
	{
		if(ammo->GetState() == BaseEntity::State::Dead)
		{
			//fire first ammo not already alive
			ammo->Fire(owner, position);
			//and quit loop
			break;
		}
	}
}

bool AmmoManager::Collides(const sf::FloatRect& entityBounds, BaseEntity::Type entityType)
{
	for(auto&& ammo : m_ammos)
	{
		//the order in which conditions are tested is important to prevent
		//over testing of unneccessary entities
		if(ammo->GetState() == BaseEntity::State::Alive &&
			ammo->GetOwner() != entityType &&
			ammo->GetShape().getGlobalBounds().intersects(entityBounds))
		{
			//kill ammo
			ammo->SetState(BaseEntity::State::Dead);
			return true;
		}
	}
	
	return false; //if we made it this far we didn't hit anything
}
//private
void AmmoManager::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	for(auto&& ammo : m_ammos)
	{
		//draw all living ammo to render target
		if(ammo->GetState() == BaseEntity::State::Alive)
			rt.draw(ammo->GetShape(), states);
	}
}

