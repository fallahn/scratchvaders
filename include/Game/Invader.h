///header file for invader entity//
///////////////////////////////////
/*
Invader entities are the bad guys,
derived from BaseEntity. Each invader
holds a reference to the AmmoManager
so that it can fire ammo and check for
collision detection, and a reference
to the player for collision detection.
All Invaders are updated and moved
by InvaderManager.
*/
///////////////////////////////////
#ifndef INVADER_H_
#define INVADER_H_

#include <Game/AmmoManager.h>
#include <Game/Player.h>

class Invader final : public BaseEntity
{
public:
	Invader(const sf::Vector2f& startPosition, AmmoManager& ammoManager, Player& player);
	void Update(float dt) override;
	void Fire();
	void Move(float x, float y) {m_shape.move(x, y);};
	void Reset();
private:
	AmmoManager& m_ammoManager; //reference to AmmoManager
	Player& m_player; //reference to player for collision detection
	sf::Clock m_shootClock; //timer to control automated firing of weapons
	float m_shootTime;
	const float m_minShootTime;
	const float m_maxShootTime;
	sf::Vector2f m_initialPosition; //Invader gets reset to this position
};

#endif //INVADER_H_