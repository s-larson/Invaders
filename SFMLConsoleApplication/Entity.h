#ifndef INCLUDED_ENTITY
#define INCLUDED_ENTITY

class Game;
#include "SFML/Graphics.hpp"

enum class EntityType{PROJECTILE, SHIP, EFFECT};
enum class EntityFaction{FRIEND, ENEMY, NEUTRAL};

class Entity {
public:
	Entity(Game *game); //Initieras med en pekare till spelet
	virtual ~Entity(); //Destruktor

	//Funktioner för information om entiteten
	virtual float getRadius() = 0; 
	virtual sf::Vector2f getPosition() = 0;
	virtual EntityType getType() = 0;
	virtual EntityFaction getFaction() = 0;

	virtual void update(float deltaTime) = 0;
	virtual void collide(Entity*otherEntity) = 0; //anropas VID kollision
	virtual void draw() = 0;

	virtual bool isOld() = 0;
	
protected: // <-- objektet kan användas av subklasserna
	Game *mGame;
};

#endif // !INCLUDED_ENTITY
