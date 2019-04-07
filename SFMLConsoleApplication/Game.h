#ifndef INCLUDED_GAME
#define INCLUDED_GAME

class Entity;
#include "SFML/Graphics.hpp"
#include <string>

class Game {
	public:
		Game();
		virtual ~Game();

		virtual void run() = 0;
		virtual sf::RenderWindow& getRenderWindow() = 0; // Tillgång till fönstret
		
		virtual sf::Sprite createSprite(std::string Filename, sf::Vector2f position = sf::Vector2f()) = 0;
		virtual void draw(sf::Sprite &sprite) = 0;
		virtual void add(Entity *entity) = 0;
		virtual void remove(Entity *entity) = 0;
		virtual bool isVisible(Entity *entity) = 0;
		virtual void shipHit() = 0;
};

#endif // !INCLUDED_GAME
