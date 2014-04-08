///base entity class///
///////////////////////////////////////
/*
BaseEntity is an ABC which defines basic
entity properties such as state, type,
shape and position. BaseEntity::Update()
is pure virtual and must be implemented
by derived entites to define custom behaviour.
*/
///////////////////////////////////////
#ifndef BASE_ENTITY_H_
#define BASE_ENTITY_H_

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Clock.hpp>

class BaseEntity : private sf::NonCopyable
{
public:
	//define type tags for derived types. Add
	//new entries here if defining new entity types
	enum class Type
	{
		Invader,
		Player,
		Ammo,
		None
	};

	enum class State
	{
		Alive,
		Dying,
		Dead
	};

	BaseEntity(sf::Vector2f startPosition = sf::Vector2f())
		: m_type	(Type::None),
		m_state		(State::Alive),
		m_moveSpeed	(0.f)
	{m_shape.setPosition(startPosition);};


	virtual ~BaseEntity(){};
	virtual void Update(float dt) = 0;
	const sf::RectangleShape& GetShape() const{return m_shape;};
	const Type GetType() const{return m_type;};
	const State GetState() const{return m_state;};
	void SetState(State state){m_state = state;};

protected:
	sf::RectangleShape m_shape;
	Type m_type;
	State m_state;
	float m_moveSpeed;
	sf::Vector2f m_velocity;
};

///////////////////////////////////////
/*
The Ammo class extends BaseEntity by
adding ownership properties and Fire()
Ammo entities are managed with the
AmmoManager class.
*/
///////////////////////////////////////
class Ammo final : public BaseEntity
{
public:
	Ammo(const sf::Vector2f& viewSize);
	void Update(float dt) override;
	BaseEntity::Type GetOwner() const{return m_owner;};
	void Fire(BaseEntity::Type owner, const sf::Vector2f& position);

private:
	BaseEntity::Type m_owner;
	sf::Vector2f m_viewSize;
};

#endif //BASE_ENTITY_H_