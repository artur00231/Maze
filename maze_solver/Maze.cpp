#include "Maze.h"

#include <vector>
#include <chrono>
#include <random>

Maze::Maze()
{
}

Maze::Maze(char floor, char wall) : floor{ floor }, wall{ wall }
{
}

bool Maze::makeNewMaze(std::size_t size, std::size_t seed, std::size_t imperfection_factor)
{
	if (size < 3) { return false; }

	row_size = size;
	this->seed = seed;

	this->imperfection_factor = imperfection_factor;

	bottom_right_border = size - 3;

	try {
		map.reset(new char[size * size]);
	}
	catch (std::bad_alloc exception)
	{
		row_size = 0;
		map.reset(nullptr);
		this->seed = 0;
		bottom_right_border = 0;

		return false;
	}

	for (char * i = map.get(); i < map.get() + (size * size); i++)
	{
		(*i) = wall;
	}

	generate_maze();

	return true;
}

std::size_t Maze::getSize() const
{
	return row_size;
}

const char * Maze::getData() const
{
	return map.get();
}

std::size_t Maze::getSeed() const
{
	return seed;
}

std::pair<char, char> Maze::getFloorAndWall() const
{
	return { floor, wall };
}

Maze::~Maze()
{
}

void Maze::generate_maze()
{
	std::vector<std::pair<std::size_t, std::size_t>> queue;
	
	// (1, 1) is a starting location
	queue.push_back(std::make_pair(1, 1));
	map[XY_to_index(1, 1)] = floor;
	std::pair<std::size_t, std::size_t> last = std::make_pair(1, 1);

	std::default_random_engine generator{ seed };
	std::uniform_int_distribution<int> distribution{ 0, 3 };

	while (!queue.empty())
	{
		auto rand_number = distribution(generator);

		auto direction = make_move(last, rand_number, map.get());

		// second == Y
		// first == X
		switch (direction)
		{
		case 0:
			last.second -= 2;
			map[XY_to_index(last.first, last.second)] = floor;
			map[XY_to_index(last.first, last.second + 1)] = floor;
			queue.push_back(last);
			break;

		case 1:
			last.first += 2;
			map[XY_to_index(last.first, last.second)] = floor;
			map[XY_to_index(last.first - 1, last.second)] = floor;
			queue.push_back(last);
			break;

		case 2:
			last.second += 2;
			map[XY_to_index(last.first, last.second)] = floor;
			map[XY_to_index(last.first, last.second - 1)] = floor;
			queue.push_back(last);
			break;

		case 3:
			last.first -= 2;
			map[XY_to_index(last.first, last.second)] = floor;
			map[XY_to_index(last.first + 1, last.second)] = floor;
			queue.push_back(last);
			break;

		case -1:
			last = queue.back();
			queue.pop_back();
			break;
		default:
			break;
		}
	}
	
	if (imperfection_factor != 0)
	{
		// Delete random cells
		std::poisson_distribution<int> c_p_distribution(row_size * row_size / imperfection_factor);
		std::uniform_int_distribution<int> c_distribution{ 0, static_cast<int>(row_size * row_size) };

		auto cells_to_delete = c_p_distribution(generator);

		while (cells_to_delete-- > 0)
		{
			auto index = c_distribution(generator);
			auto[X, Y] = index_to_XY(static_cast<std::size_t>(index));

			if (map[XY_to_index(X, Y)] == floor)
			{
				if (distribution(generator) == 1)
				{
					cells_to_delete++;
				}

				continue;
			}

			if (!(X != 0 && X != (row_size - 1) && Y != 0 && Y != (row_size - 1)))
			{
				continue;
			}

			int num_of_walls_v = 0;
			int num_of_walls_h = 0;

			if (map[XY_to_index(X, Y + 1)] == wall)
			{
				if (countWallsAroundCell(X, Y + 1) == 1)
				{
					continue;
				}

				num_of_walls_v++;
			}

			if (map[XY_to_index(X, Y - 1)] == wall)
			{
				if (countWallsAroundCell(X, Y - 1) == 1)
				{
					continue;
				}

				num_of_walls_v++;
			}

			if (map[XY_to_index(X + 1, Y)] == wall)
			{
				if (countWallsAroundCell(X + 1, Y) == 1)
				{
					continue;
				}

				num_of_walls_h++;
			}

			if (map[XY_to_index(X - 1, Y)] == wall)
			{
				if (countWallsAroundCell(X - 1, Y) == 1)
				{
					continue;
				}

				num_of_walls_h++;
			}

			if (num_of_walls_v == 2 && num_of_walls_h == 2) { cells_to_delete++; continue; }
			if (num_of_walls_v == 1) { cells_to_delete++; continue; }
			if (num_of_walls_h == 1) { cells_to_delete++; continue; }

			map[XY_to_index(X, Y)] = floor;
		}
	}

}

int Maze::make_move(std::pair<std::size_t, std::size_t> point, int direction, char * map)
{
	int counter = 0;

	while (counter < 4)
	{
		// second == Y
		// first == X
		switch (direction)
		{
		case 0:
			if (point.second > top_left_border)
			{
				if (map[XY_to_index(point.first, point.second - 2)] != floor)
				{
					return 0;
				}
			}
			break;

		case 1:
			if (point.first < bottom_right_border)
			{
				if (map[XY_to_index(point.first + 2, point.second)] != floor)
				{
					return 1;
				}
			}
			break;

		case 2:
			if (point.second < bottom_right_border)
			{
				if (map[XY_to_index(point.first, point.second + 2)] != floor)
				{
					return 2;
				}
			}
			break;

		case 3:
			if (point.first > top_left_border)
			{
				if (map[XY_to_index(point.first - 2, point.second)] != floor)
				{
					return 3;
				}
			}
			break;
		default:
			break;
		}

		direction++;
		if (direction > 3) { direction = 0; }
		counter++;
	}

	return -1;
}

int Maze::countWallsAroundCell(std::size_t X, std::size_t Y) const
{
	int num_of_walls = 0;

	if (map[XY_to_index(X, Y + 1)] == wall) num_of_walls++;
	if (map[XY_to_index(X, Y - 1)] == wall) num_of_walls++;
	if (map[XY_to_index(X + 1, Y)] == wall) num_of_walls++;
	if (map[XY_to_index(X - 1, Y)] == wall) num_of_walls++;

	return num_of_walls;
}
