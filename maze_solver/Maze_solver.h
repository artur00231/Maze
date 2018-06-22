#pragma once

#include <vector>
#include "Point.h"

class Maze_solver
{
public:
	Maze_solver();
	Maze_solver(const Maze_solver & maze_solver) = delete;
	Maze_solver operator=(const Maze_solver & maze_solver) = delete;

	bool solve(const char * map, std::size_t map_size, Point<int> start, Point<int> end, char wall = '#');
	const std::vector<Directions>& getAnswer() const;

	~Maze_solver();
protected:

private:
	std::vector<Directions> path;
	std::size_t map_size;
};

