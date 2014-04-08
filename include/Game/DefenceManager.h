///header file for defence manager class///
///////////////////////////////////////////
/*
Manages the state of the defence blocks
protecting the player.
*/
///////////////////////////////////////////

#ifndef DEFENCE_MANAGER_H_
#define DEFENCE_MANAGER_H_

#include <Game/DefenceBlock.h>
#include <Game/AmmoManager.h>
#include <vector>
#include <memory>

class DefenceManager final 
	: public sf::Drawable,
	private sf::NonCopyable
{
public:
	DefenceManager(const sf::Vector2f& viewSize,AmmoManager& ammoManager);
	void Update(float dt);
	void Reset();

private:
	std::vector< std::unique_ptr<DefenceBlock> >m_blocks;
	sf::Vector2f m_viewSize;

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};

#endif