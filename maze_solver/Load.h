#pragma once

#include <utility>
#include <memory>
#include <atomic> 

#include "Maze.h"
#include "LoadingScreen.h"
#include "SFML\Graphics.hpp"

class Load
{
public:
	Load();
	~Load();

	std::tuple<bool, Maze, sf::Texture> getResource(sf::RenderWindow & window, std::size_t size, std::size_t seed, std::size_t imperfection_factor);
	
private:
	enum class Steps {MAZE, TEXTURE, DONE};

	Maze generateMaze(std::size_t size, std::size_t seed, std::size_t imperfection_factor);
	sf::Texture generateTexture(const Maze & maze, std::atomic<bool> & running);

	LoadingScreen loading_screen{};
	Steps step{ Steps::MAZE };
};

