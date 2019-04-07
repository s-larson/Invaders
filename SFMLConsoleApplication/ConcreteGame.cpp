#include "ConcreteGame.h"
#include <string>

#include <iostream> //fels�kning. Ta bort detta

using namespace std;
using namespace sf;

namespace {

	//Inst�llningar f�r f�nstret
	const string windowTitle = "Asteroids"; //Titel p� spelf�nstret
	const VideoMode windowSize = VideoMode(768, 1024); //Storlek p� spelf�nstret
	const Color backgroundColor = Color::Black; //Bakgrundsf�rg f�r spelf�nstret
	const unsigned int FRAMERATE_LIMIT = 60; //H�gst antal uppdateringar per sekund. Beh�vs denna?

	//Inst�llningar f�r invaders
	const unsigned int SPAWN_DELTA = 2;
	const unsigned int SPAWN_BASE = 1;
	const unsigned int SPAWN_INCREMENT = 0.5f;


}

ConcreteGame::ConcreteGame() : //Konstruktor
	mRenderWindow(windowSize, windowTitle, Style::Titlebar | Style::Close),
	mGameOver(false),
	mTextureResources(),
	mEntities(),
	mOldEntities(),
	mNewEntities(),
	mTime(0.0f),
	mSpawnTime(0.0f)
{
	mEntities.push_back(new ShipEntity(this)); //Skapar skeppet och skickar med pekare till spelet
}

void ConcreteGame::run() { //Huvudloop
	Clock clock;
	while (mRenderWindow.isOpen() && !mGameOver) {
		float deltaTime = clock.restart().asSeconds();
		handleWindowEvents();
		clearWindow();
		updateTime(deltaTime);
		updateEntities(deltaTime);
		spawnInvaders(deltaTime);
		collideEntitites();
		destroyDistantEntities();
		destroyOldEntities();
		integrateNewEntities();
		drawEntities();
		displayWindow();
	}
}


/////// F�NSTERFUNKTIONER ////////

RenderWindow& ConcreteGame::getRenderWindow() { //varf�r finns denna?
	return mRenderWindow;
}

void ConcreteGame::clearWindow() { //Rensar f�nstret
	mRenderWindow.clear(backgroundColor);
}

void ConcreteGame::displayWindow() { //Ritar ut inneh�llet p� sk�rmen. St�nger mRenderWindow om f�nstret st�ngs
	mRenderWindow.display();
}

void ConcreteGame::handleWindowEvents() { //St�nger f�nstret om anv�ndaren kryssar rutan
	Event event;
	while (mRenderWindow.pollEvent(event)) {
		if (event.type == Event::Closed) {
			mRenderWindow.close();
		}
	}
}

//////// SLUMPFUNKTIONER ////////

float ConcreteGame::getRandomFloat(float min, float max) {
	uniform_real_distribution<float> distribution(min, max);
	return distribution(mGenerator);
}

bool ConcreteGame::getRandomBoolean(float probability) {
	float value = getRandomFloat(0.0f, 1.0f);
	if (value < probability) {
		return true;
	}
	else {
		return false;
	}
}

mt19937 ConcreteGame::createGenerator() {
	random_device randomDevice;
	mt19937 generator(randomDevice());
	return generator;
}

Vector2f ConcreteGame::getSpawnPosition() {
	Vector2f position;
	position.x = getRandomFloat(0.0f, mRenderWindow.getSize().x);
	position.y = getRandomFloat(-400.0f, -50.0f);
	return position;
}
Vector2f ConcreteGame::getSpawnDirection() {
	Vector2f direction;
	direction.y = 1.0f;
	if (getRandomBoolean(0.5f)) {
		direction.x = 1.0f;
	}
	else
	{
		direction.x = -1.0f;
	}

	return direction;
}

//////// TEXTURES & SPRITES ////////


Texture& ConcreteGame::getTexture(string filename) { //Huvudfunktion f�r att l�sa in texturer. Anv�nder funktionen getTexture i TextureResource.cpp
	for (TextureResources::size_type i = 0; i < mTextureResources.size(); i++) {
		if (filename == mTextureResources[i]->getFilename()) {
			return mTextureResources[i]->getTexture();
		}
	}
	Texture texture;
	texture.loadFromFile(filename);
	TextureResource *textureResource = new TextureResource(filename, texture);
	mTextureResources.push_back(textureResource);
	return textureResource->getTexture();
}

Sprite ConcreteGame::createSprite(string filename, Vector2f position) { //skapar en Sprite med centrerad position
	Sprite sprite(getTexture(filename));
	float originX = 0.5f * sprite.getLocalBounds().width;
	float originY = 0.5f * sprite.getLocalBounds().height;
	Vector2f origin(originX, originY);
	sprite.setOrigin(origin);
	sprite.setPosition(position);
	return sprite;
}

//////// UTRITNING / UPPDATERING ////////

void ConcreteGame::draw(Sprite &sprite) { //Anropas av entiteterna (deras draw-funktion)
	mRenderWindow.draw(sprite);
}

void ConcreteGame::drawEntities(EntityType type) { //F�rsta steget i utritning. G�r igenom vectorn med entiteter och ritar ut samtliga av samma typ
	for (EntityVector::size_type i = 0; i < mEntities.size(); i++) {
		Entity*entity = mEntities[i];
		if (entity->getType() == type) {
			entity->draw();
		}
	}
}

void ConcreteGame::drawEntities() { //Andra steget i utritning. Anv�nder drawEntities funktionen f�r att rita allt i r�tt ordning.
	drawEntities(EntityType::PROJECTILE);
	drawEntities(EntityType::SHIP);
	drawEntities(EntityType::EFFECT);
}

void ConcreteGame::updateTime(float deltaTime) {
	mTime += deltaTime;
}

void ConcreteGame::updateEntities(float deltaTime) { //G�r igenom vectorn av entiteter och anropar respektive update() funktion
	for (EntityVector::size_type i = 0; i < mEntities.size(); i++) {
		mEntities[i]->update(deltaTime);
	}
}

//////// L�GGA TILL / TA BORT ENTITETER ////////

void ConcreteGame::add(Entity *entity) { //L�gger till en entitet i spelet. Anropas hos objekten vid ex. skjutning.
	mNewEntities.push_back(entity);
}

void ConcreteGame:: integrateNewEntities() { //Kopierar �ver alla objekt i NewEntities till mEntities innan den rensas.
	for (EntityVector::size_type i = 0; i < mNewEntities.size(); i++) {
		mEntities.push_back(mNewEntities[i]);
	}
	mNewEntities.clear();
}

void ConcreteGame::remove(Entity *entity) { //Anropas i objektens Collide() funktion
	mOldEntities.push_back(entity);
}

void ConcreteGame::destroyOldEntities() { //Urskiljer vilka objekt som ska tas bort och ej
	//Skapar en ny vektor och s�ker igenom om objekten �r del av mOldEntities med hj�lp av isOld() funktionen �rvd fr�n basklassen
	EntityVector remainingEntities;
	for (EntityVector::size_type i = 0; i < mEntities.size(); i++) {
		Entity*entity = mEntities[i];
		if (mEntities[i]->isOld()){
			delete entity;
		}
		else {
			remainingEntities.push_back(entity);
		}
	}
	mOldEntities.clear();
	mEntities = remainingEntities;
}

void ConcreteGame::destroyDistantEntities() { //L�gger till objekt som flugit utanf�r sk�rmen i OldEntities vektorn
	for (EntityVector::size_type i = 0; i < mEntities.size(); i++) {
		Entity *temp = mEntities[i];
		if (temp->getPosition().y > mRenderWindow.getSize().y || temp->getPosition().y < -401.0f) {
			mOldEntities.push_back(temp);
		}
	}
}

bool ConcreteGame::isVisible(Entity *entity) { //Kollar om entiteten �r synlig i Y-led. (�ker aldrig utanf�r v�nster/h�ger kant)
	Vector2f position = entity->getPosition();
	float minY = entity->getRadius();
	float maxY = mRenderWindow.getSize().y - entity->getRadius();
	if (position.y < minY || position.y > maxY) {
		return false;
	}
	else {
		return true;
	}
}

void ConcreteGame::collideEntitites() { 
	//Sparar f�rst ner alla synliga entititer i en ny vektor. 
	EntityVector visibleEntities;
	for (EntityVector::size_type i = 0; i < mEntities.size(); i++) {
		Entity *entityX = mEntities[i];
		if (isVisible(entityX)) {
			visibleEntities.push_back(entityX);
		}
	}

	//J�mf�r objekt A med hela vektorn, g�r sedan �ver till B osv. Collide() anropas p� objekten i fr�ga, som i sin tur anropar remove() i ConcreteGame.
	for (EntityVector::size_type i = 0; i < visibleEntities.size(); i++) { 
		Entity *entity0 = visibleEntities[i];
		for (EntityVector::size_type j = i + 1; j < visibleEntities.size(); j++) {
			Entity*entity1 = visibleEntities[j];
			if (overlap(entity0, entity1)) {
				entity0->collide(entity1);
				entity1->collide(entity0);
			}
		}
	}
}

bool ConcreteGame::overlap(Entity *entity0, Entity *entity1) { //J�mf�r tv� objekt f�r kollision
	float deltaX = entity0->getPosition().x - entity1->getPosition().x;
	float deltaY = entity0->getPosition().y - entity1->getPosition().y;
	float radiusSum = entity0->getRadius() + entity1->getRadius();
	return deltaX * deltaX + deltaY * deltaY < radiusSum* radiusSum;
}

//////// INVADERS ////////

void ConcreteGame::spawnInvader() {
	Vector2f position = getSpawnPosition();
	Vector2f direction = getSpawnDirection();
	InvaderEntity *invader = new InvaderEntity(this, position, direction);
	mNewEntities.push_back(invader);
}

void ConcreteGame::spawnInvaders(float deltaTime) {
	mSpawnTime += deltaTime;
	if (SPAWN_DELTA < mSpawnTime) {
		int spawnCount = int(SPAWN_BASE + mTime * SPAWN_INCREMENT);
		for (int i = 0; i < spawnCount; i++) {
			spawnInvader();
		}
		mSpawnTime = 0.0f;
	}
}

void ConcreteGame::shipHit() {
	mGameOver = true;
}