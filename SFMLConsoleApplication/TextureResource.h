#ifndef INCLUDED_TEXTURE_RESOURCE
#define INCLUDED_TEXTURE_RESOURCE
#include "SFML/Graphics.hpp"
#include <string>

class TextureResource{
	public:
		TextureResource(std::string filename, sf::Texture texture);
		std::string getFilename();
		sf::Texture& getTexture();
	private:
		std::string mFilename;
		sf::Texture mTexture;
};
#endif