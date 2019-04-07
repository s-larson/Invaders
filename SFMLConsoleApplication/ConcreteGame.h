#ifndef INCLUDED_CONCRETE
#define INCLUDED_CONCRETE

#include "Game.h"
#include "ShipEntity.h"
#include "InvaderEntity.h"
#include "TextureResource.h"
#include <random>


class ConcreteGame : public Game {
public: 
	ConcreteGame();
	virtual void run() override;

	//Fönsterfunktioner
	virtual sf::RenderWindow& getRenderWindow() override;
	void clearWindow();
	void displayWindow();
	void handleWindowEvents();

	//Texturfunktioner
	sf::Texture& getTexture(std::string filename);
	virtual sf::Sprite createSprite(std::string filename, sf::Vector2f position = sf::Vector2f()) override;
	virtual void draw(sf::Sprite &sprite) override; //När ska denna användas?

	//Utritning / uppdatering
	void drawEntities(EntityType type);
	void drawEntities();
	void updateTime(float deltaTime);
	void updateEntities(float deltaTime);

	virtual void add(Entity *entity) override; //anropas när skepp träffas av skott och när spelaren/fienden skjuter
	virtual void remove(Entity *entity) override;
	virtual bool isVisible(Entity *entity) override;

	void spawnInvader(); //Huvudfunktion för att skapa invaders
	void spawnInvaders(float deltaTime);
	void integrateNewEntities();
	void destroyOldEntities();
	void destroyDistantEntities();
	void collideEntitites();
	bool overlap(Entity *entity0, Entity *entity1);
	virtual void shipHit() override;

private:
	typedef std::vector<TextureResource*> TextureResources;
	typedef std::vector<Entity*> EntityVector;
	EntityVector mEntities;
	EntityVector mNewEntities;
	EntityVector mOldEntities;
	TextureResources mTextureResources;
	sf::RenderWindow mRenderWindow;
	
	//Spelregler
	bool mGameOver;
	float mTime;
	float mSpawnTime;

	//Slumpfunktioner
	std::mt19937 mGenerator;
	float getRandomFloat(float min, float max);
	bool getRandomBoolean(float probability);
	std::mt19937 createGenerator();
	sf::Vector2f getSpawnPosition();
	sf::Vector2f getSpawnDirection();
};

#endif