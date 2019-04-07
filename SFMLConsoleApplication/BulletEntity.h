#ifndef INCLUDED_BULLET
#define INCLUDED_BULLET

#include "Entity.h"
#include "Game.h"

#include "SFML/Graphics.hpp"

class BulletEntity : public Entity {
public:
	BulletEntity(Game *game, EntityFaction category, sf::Vector2f position, sf::Vector2f direction);

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
	float mVelocity;
	sf::Vector2f mDirection;
	EntityFaction mFaction;
	bool mIsOld;
	void distantBullet();
};

#endif // !INCLUDED_BULLET
