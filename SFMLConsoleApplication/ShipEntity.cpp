#include "ShipEntity.h"
#include <string>

using namespace std;
using namespace sf;

namespace {
	const string textureFilename = "Ship.psd";
	const float RADIUS = 32.0f;
	const float FIRE_RATE = 3.0f;
	const float VELOCITY = 400.0f;
	Vector2f start_pos = Vector2f(384.0f, 768.0f); //halva bredden, 75% av höjden
	
}

//Template för att multiplicera vectorer (behövs i updatePosition()?? Eller ej?)
template <typename T>
Vector2<T> operator *(const Vector2<T>& left, const Vector2<T>& right)
{
	T X = left.x * right.x;
	T Y = left.y * right.y;
	return Vector2<T>(X, Y);
}

Vector2f toFloat(Vector2u ints) { //Konverterar int till float. Används inte??
	Vector2f floats;
	floats.x = static_cast <float> (ints.x);
	floats.y = static_cast <float> (ints.y);
	return floats;
}

ShipEntity::ShipEntity(Game *game) :
	Entity(game),
	mSprite(game->createSprite(textureFilename, start_pos)),
	mRadius(RADIUS),
	mFireDelta(1.0f / FIRE_RATE),
	mFireTime(0.0f),
	mVelocity(VELOCITY),
	mIsOld(false)
{
}

float ShipEntity::getRadius() {
	return mRadius;
}

sf::Vector2f ShipEntity::getPosition() {
	return mSprite.getPosition();
}

EntityType ShipEntity::getType() {
	return EntityType::SHIP;
}

EntityFaction ShipEntity::getFaction() {
	return EntityFaction::FRIEND;
}

void ShipEntity::updatePosition(float deltaTime) {
	float directionX = 0.0f;
	float directionY = 0.0f;

	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		directionX -= 1.0f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		directionX += 1.0f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		directionY -= 1.0f;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		directionY += 1.0f;
	}
	Vector2f direction(directionX, directionY);
	mSprite.move(deltaTime * mVelocity * direction);
}

void ShipEntity::constrainPosition() {
	float minX = mRadius;
	float minY = mRadius;
	float maxX = mGame->getRenderWindow().getSize().x - mRadius;
	float maxY = mGame->getRenderWindow().getSize().y - mRadius;
	float shipX = mSprite.getPosition().x;
	float shipY = mSprite.getPosition().y;
	if (shipX < minX) {
		shipX = minX;
	}
	else if (maxX < shipX) {
		shipX = maxX;
	}
	if (shipY < minY) {
		shipY = minY;
	}
	else if (maxY < shipY) {
		shipY = maxY;
	}
	Vector2f shipPosition(shipX, shipY);
	mSprite.setPosition(shipPosition);
}

void ShipEntity::collide(Entity *otherEntity) {
	if (otherEntity->getFaction() == EntityFaction::ENEMY) {
		mIsOld = true;
		mGame->remove(this);
		mGame->shipHit();
	}
}

bool ShipEntity::isOld() {
	return mIsOld;
}

void ShipEntity::draw() {
	mGame->draw(mSprite);
}

void ShipEntity::update(float deltaTime) {
	updatePosition(deltaTime);
	constrainPosition();
	handleFire(deltaTime);
}

void ShipEntity::handleFire(float deltaTime) {
	mFireTime += deltaTime;
	if (Keyboard::isKeyPressed(Keyboard::Space) && mFireDelta < mFireTime) {
			EntityFaction category = EntityFaction::FRIEND;
			Vector2f position = mSprite.getPosition();
			Vector2f direction1 = Vector2f(-1.0f, -1.0f); //Vänster
			Vector2f direction2 = Vector2f(0.0f, -1.0f); //Mitt
			Vector2f direction3 = Vector2f(1.0f, -1.0f); //Höger
			mGame->add(new BulletEntity(mGame, category, position, direction1));
			mGame->add(new BulletEntity(mGame, category, position, direction2));
			mGame->add(new BulletEntity(mGame, category, position, direction3));
			mFireTime = 0.0f;
	}
}