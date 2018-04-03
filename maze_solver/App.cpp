#include "App.h"

#include "Load.h"
#include "PathFinder.h"
#include <future>

App::App()
{
}

App::~App()
{
}

void App::start()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(sf::VideoMode{ 1280, 720 }, "Maze", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	app_state = AppState::Loading;

	while (!isExiting())
	{
		gameLoop();
	}
}

void App::gameLoop()
{
	window.clear(sf::Color::White);

	sf::Event event;

	while (window.pollEvent(event))
	{
		events(event);
	}

	switch (app_state)
	{
	case App::AppState::Running:
	{
		PathFinder path_finder{ std::move(maze), std::move(texture) };
		path_finder.display(window);

		app_state = AppState::Exiting;
	}
		break;
	case App::AppState::Loading:
	{
		Load load{};
		auto[success, maze, texture] = load.getResource(window, 251, std::chrono::system_clock::now().time_since_epoch().count(), 50);

		if (!success)
		{
			app_state = AppState::Exiting;
		}
		else
		{
			this->maze = std::move(maze);
			this->texture = std::move(texture);

			app_state = AppState::Running;
		}
	}
		break;
	default:
		break;
	}


	window.display();
}

void App::events(const sf::Event & event)
{
	if (event.type == sf::Event::Closed)
	{
		app_state = AppState::Exiting;
	}
}

bool App::isExiting() const
{
	return app_state == AppState::Exiting;
}
