///header file for invader manager class///
///////////////////////////////////////////
/*
This works similarly to the AmmoManager class
by maintaining a vector of smart pointers to
Invaders, and calling all their Update and
Draw functions appropriately. When looping
through, the livingInvader count is updated 
so the number of alive Invaders can be tracked
and used to change the state of play. 
The manager is also responsible for moving
the Invaders on screen at timed intervals,
using a bounding rectangle to make sure the
Invaders don't move too far.
*/
///////////////////////////////////////////
#ifndef INVADER_MANAGER_H_
#define INVADER_MANAGER_H_

#include <Game/Invader.h>
#include <Game/AmmoManager.h>
#include <Game/Player.h>

class InvaderManager final 
	: public sf::Drawable,
	private sf::NonCopyable
{
public:
	InvaderManager(const sf::Vector2f& viewSize, AmmoManager& ammoManager, Player& player);
	void Update(float dt);
	sf::Uint8 GetInvaderCount() const{return m_invaderCount;};
	void Reset();
private:
	std::vector< std::unique_ptr<Invader> >m_invaders;
	sf::Uint8 m_invaderCount; //number of living Invaders after an update

	//movement properties
	sf::FloatRect m_invaderBounds;
	sf::Clock m_movementClock;
	const float m_moveFrequency; //how often invaders are moved in seconds
	float m_movementX; //updated to control horizontal movement
	const float m_movementY;
	sf::Vector2f m_viewSize;

	void draw(sf::RenderTarget& rt, sf::RenderStates)const override;
};

#endif //INVADER_MANAGER_H_