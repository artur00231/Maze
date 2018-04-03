#pragma once

#include "SFML\Graphics.hpp"
#include "Maze.h"

class App
{
public:
	App();
	~App();

	App(const App&) = delete;
	App(App&&) = delete;

	App& operator=(const App&) = delete;
	App& operator=(App&&) = delete;

	void start();

private:
	enum class AppState { Uninitialized, Running, Exiting, Loading };

	void gameLoop();
	void events(const sf::Event & event);

	bool isExiting() const;

	AppState app_state{ AppState::Uninitialized };

	sf::RenderWindow window;
	Maze maze;
	sf::Texture texture;
};

