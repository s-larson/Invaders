#ifndef INCLUDED_EXPLOSION
#define INCLUDED_EXPLOSION

#include "Entity.h"
#include "Game.h"

#include "SFML/Graphics.hpp"

class ExplosionEntity : public Entity {
public:
	ExplosionEntity(Game *game, sf::Vector2f position);

	virtual float getRadius() override;
	virtual sf::Vector2f getPosition() override;
	virtual EntityType getType() override;
	virtual EntityFaction getFaction() override;

	virtual void update(float deltaTime) override;
	virtual void collide(Entity*otherEntity) override; //anropas VID kollision
	virtual void draw() override;

	virtual bool isOld() override;

private:
	sf::Sprite mSprite;
	float mRadius;
	sf::Vector2f mDirection;
	EntityFaction mFaction;
	bool mIsOld;
};

#endif // !INCLUDED_BULLET
