#include "Load.h"

#include <future>
#include <atomic>
using namespace std::literals::chrono_literals;

Load::Load()
{
}


Load::~Load()
{
}

std::tuple<bool, Maze, sf::Texture> Load::getResource(sf::RenderWindow & window, std::size_t size, std::size_t seed, std::size_t imperfection_factor)
{
	auto maze_f = std::async([this](std::size_t size, std::size_t seed, std::size_t imperfection_factor) {return this->generateMaze(size, seed, imperfection_factor); }, size, seed, imperfection_factor);
	std::future<sf::Texture> texture_f;
	std::atomic<bool> running = true;

	Maze maze{};
	sf::Texture texture{};

	loading_screen.setString("Generateing maze");

	while (true)
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Maze maze{};
				sf::Texture texture{};

				switch (step)
				{
				case Load::Steps::MAZE:
					maze_f._Abandon();	 // non standard
										// use with visual c++
					return { false, std::move(maze), std::move(texture) };
					break;
				case Load::Steps::TEXTURE:
					running = false;
					texture_f.get();
					return { false, std::move(maze), std::move(texture) };
					break;
				case Load::Steps::DONE:
					break;
				default:
					break;
				}
			}
		}

		switch (step)
		{
		case Load::Steps::MAZE:
			if (maze_f.wait_for(0s) == std::future_status::ready)
			{
				maze = maze_f.get();
				texture_f = std::async([this](const Maze & maze, std::atomic<bool> & running) {return this->generateTexture(maze, running); }, std::ref(maze), std::ref(running));
				loading_screen.setString("Drawing maze");
				step = Steps::TEXTURE;
			}
			break;
		case Load::Steps::TEXTURE:
			if (texture_f.wait_for(0s) == std::future_status::ready)
			{
				texture = texture_f.get();
				loading_screen.setString("Ending");
				step = Steps::DONE;
			}
			break;
		case Load::Steps::DONE:
			return { true, std::move(maze), std::move(texture) };
			break;
		default:
			break;
		}

		loading_screen.drawLoadingScreen(window);
		window.display();
	}
}

Maze Load::generateMaze(std::size_t size, std::size_t seed, std::size_t imperfection_factor)
{
	Maze maze{};
	maze.makeNewMaze(size, seed, imperfection_factor);

	return maze;
}

sf::Texture Load::generateTexture(const Maze & maze, std::atomic<bool> & running)
{
	sf::RenderTexture r_texture{};

	auto size = maze.getSize();
	auto max_size = sf::Texture::getMaximumSize() / 4;
	auto factor = max_size / static_cast<float>(size);

	sf::Vector2f base_size{ 1.0f, 1.0f };
	sf::RectangleShape rec{ { base_size.x * factor, base_size.y * factor } };
	
	r_texture.create(max_size, max_size);
	r_texture.clear(sf::Color::White);

	for (std::size_t i = 0; i < size; i++)
	{
		for (std::size_t j = 0; j < size; j++)
		{
			if (maze.getData()[i * size + j] == maze.getFloorAndWall().first)
			{
				rec.setFillColor(sf::Color::White);
			}
			else
			{
				rec.setFillColor(sf::Color::Black);
			}

			rec.setPosition(j * rec.getSize().x, i * rec.getSize().y);

			r_texture.draw(rec);

			if (!running)
			{
				return sf::Texture{};
			}
		}
	}
	
	r_texture.display();
	return r_texture.getTexture();
}
