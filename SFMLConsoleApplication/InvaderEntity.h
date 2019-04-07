#ifndef INCLUDED_INVADER
#define INCLUDED_INVADER

#include "Entity.h"
#include "Game.h"
#include "BulletEntity.h"
#include "ExplosionEntity.h"

#include "SFML/Graphics.hpp"

class InvaderEntity : public Entity {
public:
	InvaderEntity(Game *game, sf::Vector2f position, sf::Vector2f direction);

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
	float mFireDelta;
	float mFireTime; //Tid som passerat sedan senaste skottet
	sf::Vector2f mDirection;
	void handleMovement(float deltaTime);
	void handleFire(float deltaTime);
	bool mIsOld;
};

#endif // !INCLUDED_INVADER