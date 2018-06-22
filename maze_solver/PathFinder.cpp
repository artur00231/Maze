#include "PathFinder.h"

#include <iostream>

PathFinder::PathFinder(Maze && maze, sf::Texture && texture) : maze{ std::move(maze) }, texture{ texture }
{
	sprite.setTexture(this->texture);

	start.setFillColor(sf::Color::Red);
	end.setFillColor(sf::Color::Blue);

	auto size = maze.getSize();
	auto max_size = sf::Texture::getMaximumSize() / 4;
	cell_size = max_size / static_cast<float>(size);
}

PathFinder::~PathFinder()
{
}

void PathFinder::display(sf::RenderWindow & window)
{
	sf::Vector2f screen{ 1280.0, 720.0 };
	sf::View view{ { screen.x / 2, screen.y / 2 }, screen };

	float factor = 1.0;
	sf::Vector2i last_mouse_position = sf::Mouse::getPosition();

	while (true)
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				if (is_waiting_for_path)
				{
					path_texture_f._Abandon();	 // non standard
												// use with visual c++
				}

				return;
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

				if (event.key.code == sf::Keyboard::Key::R)
				{
					reset();
				}
			}


			if (event.type == sf::Event::MouseMoved)
			{
				sf::Vector2i mouse_position = sf::Mouse::getPosition();

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					view.move(static_cast<float>(last_mouse_position.x - mouse_position.x), static_cast<float>(last_mouse_position.y - mouse_position.y));
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

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					auto x = getPoint(window);

					if (x.has_value())
					{
						if (!is_set_start)
						{
							setStartPoint(x.value());
						}
						else if (!is_set_end)
						{
							setEndPoint(x.value());
						}
					}
				}
			}

		}

		if (is_waiting_for_path)
		{
			if (path_texture_f.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				path_texture = path_texture_f.get();

				path.setTexture(path_texture, true);
				path.setPosition(0, 0);
				is_set_path = true;
				is_waiting_for_path = false;
			}
		}

		window.clear(sf::Color::White);
		window.setView(view);
		if (factor != this->factor)
		{
			sprite.setScale(factor, factor);
			path.setScale(factor, factor);
			scalePoints(factor);

			this->factor = factor;
		}
		window.draw(sprite);
		drawPath(window);
		drawPoints(window);
		window.display();

	}
}

std::optional<Point<int>> PathFinder::getPoint(const sf::RenderWindow & window) const
{
	auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	sf::Rect<float> maze_rec{ sprite.getGlobalBounds() };

	if (!maze_rec.contains(mouse_pos))
	{
		return {};
	}

	auto size = maze.getSize();
	auto cell_size = sprite.getGlobalBounds().height / static_cast<float>(size);

	int x = static_cast<int>(mouse_pos.x / cell_size);
	int y = static_cast<int>(mouse_pos.y / cell_size);

	return { Point<int>{x, y} };
}

void PathFinder::setStartPoint(Point<int> point)
{
	if (maze.getData()[Point<int>::XY_to_index(point, static_cast<int>(maze.getSize()))] == '#')
	{
		return;
	}

	start.setPosition(static_cast<float>(point.getPosition().first) * cell_size, static_cast<float>(point.getPosition().second * cell_size));
	start_p = point;
	is_set_start = true;

	scalePoints(factor);
}

void PathFinder::setEndPoint(Point<int> point)
{
	if (maze.getData()[Point<int>::XY_to_index(point, static_cast<int>(maze.getSize()))] == '#' || point == start_p)
	{
		return;
	}

	end.setPosition(static_cast<float>(point.getPosition().first), static_cast<float>(point.getPosition().second));
	end_p = point;

	is_set_end = true;

	scalePoints(factor);

	is_waiting_for_path = true;
	auto foo = std::bind(&PathFinder::drawPathTexture, this, std::ref(maze), start_p, end_p);
	path_texture_f = std::async(foo);
}

void PathFinder::find_path()
{
	if (is_set_start && is_set_end)
	{
		return;
	}

	if (Point<int>::distance(start_p, end_p) == 1)
	{
		return;
	}
}

void PathFinder::reset()
{
	is_set_start = false;
	is_set_end = false;
	is_set_path = false;
}

void PathFinder::drawPath(sf::RenderTarget & target)
{
	if (!is_set_path)
	{
		return;
	}

	target.draw(path);
}

void PathFinder::drawPoints(sf::RenderTarget & target)
{
	if (is_set_start)
	{
		target.draw(start);
	}

	if (is_set_end)
	{
		target.draw(end);
	}
}

void PathFinder::scalePoints(float factor)
{
	auto current_cell_size = cell_size * factor;

	if (is_set_start)
	{
		start.setSize({ current_cell_size / 1.5f, current_cell_size / 1.5f });
		start.setPosition(start_p.getPosition().first * current_cell_size + current_cell_size / 6, start_p.getPosition().second * current_cell_size + current_cell_size / 6);
	}

	if (is_set_end)
	{
		end.setSize({ current_cell_size / 1.5f, current_cell_size / 1.5f });
		end.setPosition(end_p.getPosition().first * current_cell_size + current_cell_size / 6, end_p.getPosition().second * current_cell_size + current_cell_size / 6);
	}
}

sf::Texture PathFinder::drawPathTexture(const Maze & maze, Point<int> start, Point<int> end)
{
	Maze_solver solver{};
	
	auto is_sloved = solver.solve(maze.getData(), maze.getSize(), start, end, maze.getFloorAndWall().second);

	if (!is_sloved)
	{
		return sf::Texture();
	}

	sf::RenderTexture r_texture{};

	auto size = maze.getSize();
	auto max_size = sf::Texture::getMaximumSize() / 4;
	auto factor = max_size / static_cast<float>(size);

	auto cell_size = 1 * factor;
	sf::RectangleShape rec{ { cell_size / 1.5f, cell_size / 1.5f } };
	rec.setFillColor(sf::Color::Green);

	r_texture.create(max_size, max_size);
	r_texture.clear(sf::Color::Transparent);

	start.setStep(1);

	auto o = solver.getAnswer();

	for (auto & x : o)
	{
		start.move(x);

		rec.setPosition(start.getPosition().first * cell_size + cell_size / 6, start.getPosition().second * cell_size + cell_size / 6);

		r_texture.draw(rec);
	}

	r_texture.display();
	return r_texture.getTexture();
}
