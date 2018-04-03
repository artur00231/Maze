#pragma once

#include "SFML\Graphics.hpp"

class LoadingScreen
{
public:
	LoadingScreen();

	void drawLoadingScreen(sf::RenderTarget & target);

	void setString(const sf::String & string);

private:
	void loadResource();
	void drawLoadingCircle(sf::RenderTarget & target);

	sf::Font font;
	sf::Text text;
	std::size_t step{ 0 };
};