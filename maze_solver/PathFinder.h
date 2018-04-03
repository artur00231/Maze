#pragma once

#include <optional>
#include <future>

#include "Maze.h"
#include "Maze_solver.h"
#include "SFML\Graphics.hpp"

class PathFinder
{
public:
	PathFinder(Maze && maze, sf::Texture && texture);
	~PathFinder();

	void display(sf::RenderWindow & window);

private:
	std::optional<Point<int>> getPoint(const sf::RenderWindow & window) const;
	void setStartPoint(Point<int> point);
	void setEndPoint(Point<int> point);
	void find_path();
	void reset();

	void drawPath(sf::RenderTarget & target);
	void drawPoints(sf::RenderTarget & target);
	void scalePoints(float factor);
	sf::Texture drawPathTexture(const Maze & maze, Point<int> start, Point<int> end);

	sf::Sprite path;
	sf::Texture path_texture;

	sf::RectangleShape start, end;
	Point<int> start_p, end_p;
	bool is_set_start{ 0 }, is_set_end{ 0 }, is_set_path{ 0 }, is_waiting_for_path{ 0 };
	float cell_size;
	std::future<sf::Texture> path_texture_f;

	sf::Sprite sprite;
	sf::Texture texture;
	Maze maze;
	Maze_solver solver;
	std::future<bool> solver_answer;

	float factor{ 1.0f };
};

