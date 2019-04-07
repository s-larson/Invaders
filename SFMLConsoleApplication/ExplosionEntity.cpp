#include "ExplosionEntity.h"

using namespace std;
using namespace sf;

namespace {
	const string textureFilename = "Explosion.psd";
	const float RADIUS = 22.0f;
	const float LIFE_TIME = 2.0f;
}

ExplosionEntity::ExplosionEntity(Game *game, sf::Vector2f position) :
	Entity(game),
	mRadius(RADIUS),
	mSprite(game->createSprite(textureFilename, position)),
	mFaction(EntityFaction::NEUTRAL),
	mIsOld(false)
{
}

void ExplosionEntity::draw() {
	mGame->draw(mSprite);
}

bool ExplosionEntity::isOld() {
	return mIsOld;
}

EntityType ExplosionEntity::getType() {
	return EntityType::EFFECT;
}

void ExplosionEntity::update(float deltaTime) { //Tar bort explosionen efter en viss tid
	float removeBullet = LIFE_TIME;
	while (removeBullet < 500)
	{
		removeBullet += deltaTime * 0.1f;
	}
	mIsOld = true;	
}
EntityFaction ExplosionEntity::getFaction() {
	return mFaction;
}


//Används ej
float ExplosionEntity::getRadius(){return mRadius;}
Vector2f ExplosionEntity::getPosition(){return mSprite.getPosition();}
void ExplosionEntity::collide(Entity*otherEntity) {}