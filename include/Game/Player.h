///header for player class///
/////////////////////////////
/*
The player entity, also derived
from BaseEntity differs only from
the Invader class by allowing
custom input and being managed directly
by the game class, as the single
instance negates the use of a
PlayerManager class (perhaps it would
be different for multiple players)
*/
/////////////////////////////
#ifndef PLAYER_H_
#define PLAYER_H_

#include <Game/AmmoManager.h>

class Player final : public BaseEntity
{
public:
	Player(const sf::Vector2f& startPosition, const sf::Vector2f viewSize, AmmoManager& ammoManager);
	void Update(float dt) override;
	void Fire();
	void Reset();
	void AddRound(){m_rounds++;};
	void AddScore(sf::Uint8 score){m_score += score;};
	sf::Uint8 GetRound() const{return m_rounds;};
	sf::Uint32 GetScore() const{return m_score;};
private:
	AmmoManager& m_ammoManager;
	sf::Vector2f m_initialSize;
	sf::Vector2f m_initalPosition;
	sf::Vector2f m_viewSize;
	const float m_damage;

	sf::Uint32 m_score;
	sf::Uint8 m_rounds;
};

#endif //PLAYER_H_