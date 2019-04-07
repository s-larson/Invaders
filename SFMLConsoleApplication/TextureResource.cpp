#include "TextureResource.h"

TextureResource::TextureResource(std::string filename, sf::Texture texture) :
	mFilename(filename),
	mTexture(texture)
{}

sf::Texture &TextureResource::getTexture() {
	return mTexture;
}

std::string TextureResource::getFilename() {
	return mFilename;
}