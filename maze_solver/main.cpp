#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <iterator>

#include "Maze.h"
#include "Maze_solver.h"

#include "SFML\Graphics.hpp"


#include "App.h"

using namespace std::literals::string_literals;

int main()
{
	/*
	const std::size_t maze_size = 21;
	
	sf::Vector2f screen{ 1280.0, 720.0 };
	sf::Vector2f block{ 20.0, 20.0 };

	sf::View view{ { screen.x / 2, screen.y / 2 }, screen };


	sf::RenderWindow window;
	window.setFramerateLimit(60);
	
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	window.create(sf::VideoMode{ 1280, 720 }, "Labirynt"s, sf::Style::Default, settings);
	window.setView(view);

	Maze maze;
	maze.makeNewMaze(maze_size, 2158851);


	sf::RenderTexture rtexture{};
	if (!rtexture.create(maze_size * block.x, maze_size * block.y))
	{
		std::cerr << "rtexture can't be created\n";
		abort();
	}

	sf::RectangleShape rec{ { block.x , block.y } };

	for (int i = 0; i < maze_size; i++)
	{
		for (int j = 0; j < maze_size; j++)
		{
			if (maze.getData()[i * maze_size + j] == ' ')
			{
				rec.setFillColor(sf::Color::White);
			}
			else
			{
				rec.setFillColor(sf::Color::Black);
			}

			rec.setPosition(j * block.x , i * block.y);

			rtexture.draw(rec);
		}
	}

	rtexture.display();

	sf::Sprite sprite{ rtexture.getTexture() };

	float factor = 1.0;

	sf::Vector2i last_mouse_position = sf::Mouse::getPosition();

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Left)
				{
					view.move({ -20, 0 });
				}

				if (event.key.code == sf::Keyboard::Key::Right)
				{
					view.move({ 20, 0 });
				}

				if (event.key.code == sf::Keyboard::Key::Up)
				{
					view.move({ 0, -20 });
				}

				if (event.key.code == sf::Keyboard::Key::Down)
				{
					view.move({ 0, 20 });
				}
			}


			if (event.type == sf::Event::MouseMoved)
			{
				sf::Vector2i mouse_position = sf::Mouse::getPosition();

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					view.move((last_mouse_position.x - mouse_position.x), (last_mouse_position.y - mouse_position.y));
				}

				last_mouse_position = mouse_position;
			}

			if (event.type == sf::Event::MouseWheelMoved)
			{
				factor = factor + event.mouseWheel.delta / 50.0f;

				if (factor < 0.02)
				{
					factor = 0.02f;
				}
			}
		}

		window.clear(sf::Color::White);
		window.setView(view);
		sprite.setScale(factor, factor);
		window.draw(sprite);
		window.display();

	}*/

	App app;
	app.start();


	return 0;
}
