///header for classes to manager entities///

////////////////////////////////////////////
/*
How it works: The AmmoManager creates a 'pool'
of ammo entities, pointers to which are stored
in m_ammos. Both invader and player entities
keep a reference to the AmmoManager so that
when they need to fire an ammo they can call
the AmmoManager::Fire() function directly.
The function takes and entity type and position
as a parameter. The entity type sets the 'owner'
(the entity type which fired it) so that it 
will only collide with entites of a different type.
The position parameter is used to set the initial
position of the ammo to that of the entity which
fired it. The Fire() function merely loops 
through the vector until it finds the first ammo
which isn't alive, and spawns it by calling its
own Fire() function.
*/
///////////////////////////////////////////

#ifndef AMMO_MANAGER_H_
#define AMMO_MANAGER_H_

#include <Game/BaseEntity.h>
#include <vector>
#include <memory>

class AmmoManager final 
	: public sf::Drawable,
	private sf::NonCopyable
{
public:
	AmmoManager(const sf::Vector2f& viewSize);
	void Update(float dt);
	void Fire(BaseEntity::Type owner, const sf::Vector2f& position);
	
	//checks each ammo with given bounds and returns true if there is a collision
	bool Collides(const sf::FloatRect& entityBounds, BaseEntity::Type entityType);
	void Reset(){for(auto&& ammo : m_ammos) ammo->SetState(BaseEntity::State::Dead);};
private:
	std::vector< std::unique_ptr<Ammo> > m_ammos;
	sf::Clock m_fireClock; //limits the number of shots fired when holding fire button
	const float m_fireLimit; //seconds before another shot can be fired

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};


#endif //AMMO_MAMNGER_H_