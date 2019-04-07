#include "InvaderEntity.h"
#include <string>

using namespace std;
using namespace sf;

namespace {
	const string textureFilename = "Invader.psd";
	const float RADIUS = 32.0f;
	const float FIRE_RATE = 2.0f;
	const float VELOCITY = 200.0f;
}

InvaderEntity::InvaderEntity(Game *game, Vector2f position, Vector2f direction) :
	Entity(game),
	mSprite(game->createSprite(textureFilename, position)),
	mRadius(RADIUS),
	mFireDelta(1.0 / FIRE_RATE),
	mFireTime(0.0f),
	mDirection(direction),
	mVelocity(VELOCITY),
	mIsOld(false){
}


float InvaderEntity::getRadius() {
	return mRadius;
}

Vector2f InvaderEntity::getPosition() {
	return mSprite.getPosition();
}

EntityType InvaderEntity::getType() {
	return EntityType::SHIP;
}

EntityFaction InvaderEntity::getFaction() {
	return EntityFaction::ENEMY;
}

void InvaderEntity::collide(Entity*otherEntity) {
	if (otherEntity->getFaction() == EntityFaction::FRIEND) {
		mIsOld = true;
		Vector2f position = mSprite.getPosition();
		mGame->add(new ExplosionEntity(mGame, position));
		mGame->remove(this);
	}
}

void InvaderEntity::draw() {
	mGame->draw(mSprite);
}

void InvaderEntity::handleMovement(float deltaTime) {
	float minX = mRadius;
	float maxX = mGame->getRenderWindow().getSize().x - mRadius;
	Vector2f position = mSprite.getPosition();
	position += deltaTime * mVelocity* mDirection;
	if (position.x < minX) {
		position.x = minX;
		mDirection.x = -mDirection.x;
	}
	else if (maxX < position.x) {
		position.x = maxX;
		mDirection.x = -mDirection.x;
	}
	mSprite.setPosition(position);
}

void InvaderEntity::handleFire(float deltaTime) {
	mFireTime += deltaTime;
	if (mFireDelta < mFireTime && mGame->isVisible(this)) {
		EntityFaction category = EntityFaction::ENEMY;
		Vector2f position = mSprite.getPosition();
		Vector2f direction = Vector2f(0.0f, 1.0f);
		mGame->add(new BulletEntity(mGame, category, position, direction));
		mFireTime = 0.0f;
	}
}

void InvaderEntity::update(float deltaTime) {
	handleMovement(deltaTime);
	handleFire(deltaTime);
}

bool InvaderEntity::isOld() {
	return mIsOld;
}