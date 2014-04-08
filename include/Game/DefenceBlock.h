///header file for defence block entity///
//////////////////////////////////////////
/*
Defence blocks provide shelter for the player
and can be destoyed after several hits from 
Invader ammo. Defence blocks also have their
own manager
*/
//////////////////////////////////////////
#ifndef DEFENCE_BLOCK_H_
#define DEFENCE_BLOCK_H_

#include <Game/BaseEntity.h>
#include <Game/AmmoManager.h>

class DefenceBlock final : public BaseEntity
{
public:
	DefenceBlock(const sf::Vector2f& position, AmmoManager& ammoManager);
	void Update(float dt) override;
	void Reset();

private:
	sf::Vector2f m_initialPosition;
	sf::Vector2f m_initialSize;
	AmmoManager& m_ammoManager;
	const float m_damageAmount; //amount to move / shrink by when damaged
};


#endif //DEFENCE_BLOCK_H_