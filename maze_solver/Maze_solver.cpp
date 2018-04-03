#include "Maze_solver.h"

#include <queue>
#include <unordered_set>

struct Data
{
	int heuristic_pred;
	int cost;
	Point<int> neighbour;
	std::vector<Directions> path;

	friend bool operator<(Data d1, Data d2);
};

inline bool operator<(Data d1, Data d2)
{
	return d1.heuristic_pred > d2.heuristic_pred;
}

inline int heuristic(Point<int> cell, Point<int> end)
{
	return static_cast<int>((Point<int>::distance(cell, end) + 0.5));
}

namespace std
{
	template<>
	struct hash<Point<int>>
	{
		inline std::size_t operator()(const Point<int> & x) const
		{
			std::size_t hash = static_cast<std::size_t>(x.getPosition().first);
			hash = static_cast<std::size_t>(x.getPosition().second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	};

}

inline std::vector<Directions> append_and_return(std::vector<Directions> vec, Directions dir)
{
	vec.push_back(dir);
	return vec;
}

Maze_solver::Maze_solver()
{
}

bool Maze_solver::solve(const char * map, std::size_t map_size, Point<int> start, Point<int> end)
{
	std::priority_queue<Data> queue;
	std::unordered_set<Point<int>> visited;

	start.setStep(1);
	queue.push({ (0 + heuristic(start, end)), 0, start, {} });

	while (!queue.empty())
	{
		Data current_data = queue.top();
		queue.pop();

		if (current_data.neighbour == end)
		{
			this->path = current_data.path;

			return true;
		}

		if (visited.count(current_data.neighbour))
		{
			continue;
		}

		visited.insert(current_data.neighbour);

		auto N_point = Point<int>::move(current_data.neighbour, Directions::N);
		if (map[Point<int>::XY_to_index(N_point, map_size)] != '#' && (!visited.count(N_point)))
		{
			queue.push({ (current_data.cost + heuristic(current_data.neighbour, end)), current_data.cost + 1, Point<int>{ current_data.neighbour.getPosition().first, current_data.neighbour.getPosition().second + 1, 1 }, append_and_return(current_data.path, Directions::N) });
		}

		auto S_point = Point<int>::move(current_data.neighbour, Directions::S);
		if (map[Point<int>::XY_to_index(S_point, map_size)] != '#' && (!visited.count(S_point)))
		{
			queue.push({ (current_data.cost + heuristic(current_data.neighbour, end)), current_data.cost + 1, Point<int>{ current_data.neighbour.getPosition().first, current_data.neighbour.getPosition().second - 1, 1 }, append_and_return(current_data.path, Directions::S) });
		}

		auto E_point = Point<int>::move(current_data.neighbour, Directions::E);
		if (map[Point<int>::XY_to_index(E_point, map_size)] != '#' && (!visited.count(E_point)))
		{
			queue.push({ (current_data.cost + heuristic(current_data.neighbour, end)), current_data.cost + 1, Point<int>{ current_data.neighbour.getPosition().first + 1, current_data.neighbour.getPosition().second, 1 }, append_and_return(current_data.path, Directions::E) });
		}

		auto W_point = Point<int>::move(current_data.neighbour, Directions::W);
		if (map[Point<int>::XY_to_index(W_point, map_size)] != '#' && (!visited.count(W_point)))
		{
			queue.push({ (current_data.cost + heuristic(current_data.neighbour, end)), current_data.cost + 1, Point<int>{ current_data.neighbour.getPosition().first - 1, current_data.neighbour.getPosition().second, 1 }, append_and_return(current_data.path, Directions::W) });
		}
	}

	return false;
}

const std::vector<Directions>& Maze_solver::getAnswer() const
{
	return path;
}

Maze_solver::~Maze_solver()
{
}
