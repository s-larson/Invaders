#include "BulletEntity.h"

using namespace std;
using namespace sf;

#include <iostream>


namespace {
	const string textureFilename = "Bullet.psd";
	const float RADIUS = 32.0f;
	const float VELOCITY = 900.0f;
}

BulletEntity::BulletEntity(Game *game, EntityFaction category, sf::Vector2f position, sf::Vector2f direction) :
	Entity(game),
	mVelocity(VELOCITY),
	mRadius(RADIUS),
	mSprite(game->createSprite(textureFilename, position)),
	mDirection(direction),
	mFaction(category),
	mIsOld(false)
{
}

float BulletEntity::getRadius(){
	return mRadius;
}

Vector2f BulletEntity::getPosition() {
	return mSprite.getPosition();
}

EntityType BulletEntity::getType() {
	return EntityType::PROJECTILE;
}

EntityFaction BulletEntity::getFaction() {
	return mFaction;
}

void BulletEntity::update(float deltaTime) {
	mSprite.move(deltaTime * mVelocity * mDirection);
	distantBullet();
}

void BulletEntity::collide(Entity*otherEntity) { //markerar objektet för borttagning
	if (otherEntity->getFaction() != mFaction && otherEntity->getType() == EntityType::SHIP) {
		mIsOld = true;
		mGame->remove(this);
	}
}

void BulletEntity::draw() {
	mGame->draw(mSprite);
}

bool BulletEntity::isOld() {
	return mIsOld;
}

void BulletEntity::distantBullet() {
	RenderWindow &temp = mGame->getRenderWindow();
	if (mSprite.getPosition().y < 0.0f - mRadius || mSprite.getPosition().y > temp.getSize().y + mRadius) {
		mIsOld = true;
	}
}