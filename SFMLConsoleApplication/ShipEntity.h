#ifndef INCLUDED_SHIPENTITY
#define INCLUDED_SHIPENTITY

#include "Entity.h"
#include "BulletEntity.h"
#include "Game.h"

#include "SFML/Graphics.hpp"

class ShipEntity : public Entity {
public:
	ShipEntity(Game *game);
	
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
	float mFireDelta; //tid mellan varje skott
	float mFireTime; //tid som har gått sedan föregående skott
	float mVelocity;
	void updatePosition(float deltaTime);
	void constrainPosition();
	void handleFire(float deltaTime);
	bool mIsOld;
};

#endif // !define INCLUDED_SHIPENTITY
