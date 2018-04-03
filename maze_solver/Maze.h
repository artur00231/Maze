#pragma once

#include <memory>

class Maze
{
public:
	Maze();
	Maze(const Maze & maze) = delete;
	Maze& operator=(const Maze & maze) = delete;

	Maze(Maze && maze) = default;
	Maze& operator=(Maze && maze) = default;

	// Size should be a odd number
	// Minimal size is 3
	void makeNewMaze(std::size_t size, std::size_t seed, std::size_t imperfection_factor);
	std::size_t getSize() const;
	const char* getData() const;
	std::size_t getSeed() const;

	~Maze();

protected:
	void generate_maze();
	int make_move(std::pair<std::size_t, std::size_t> point, int direction, char * map);
	int countWallsAroundCell(std::size_t X, std::size_t Y) const;

	inline std::size_t XY_to_index(std::size_t X, std::size_t Y) const { return (row_size * Y + X); }
	inline std::pair<std::size_t, std::size_t> index_to_XY(std::size_t index) const { return {index % row_size, index / row_size }; }

private:
	std::size_t row_size = 0, seed = 0;
	std::size_t imperfection_factor{ 0 };
	std::unique_ptr<char[]> map;
	std::size_t bottom_right_border = 0, top_left_border = 2;
};

