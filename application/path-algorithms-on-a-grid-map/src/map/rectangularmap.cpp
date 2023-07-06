#include "rectangularmap.h"
#include "cell.h"
#include "order.h"
#include <stack>
#include <queue>
#include <unordered_set>

RectangularMap::RectangularMap(int width, int height, shared_ptr<Focus> focus, RenderWindow& window)
	: MapBase(window, width, height),
	_mapLoaded(false),
	_focus(focus)
{
	_focus->TopLeftX = 0;
	_focus->TopLeftY = 0;
	_focus->BottomRightX = width - 1;
	_focus->BottomRightY = height - 1;

	// Subscribe for changes
	this->_focus->Listen(this);
}

int RectangularMap::_VertexIndex(int row, int column)
{
	return row * _width + column;
}

/// <summary>
///  Creates a fully Graph representation of the map, that:
///  1. Avoids non - moveable cells to build the paths more efficiently(than in Grid).
///  2. Allows applying appropriate graph algorithms based on map data.
/// </summary>
void RectangularMap::InitialiseGraph()
{
  	MapBase::InitialiseGraph();

	if (_mapLoaded)
	{
		if (_isWeighten)
		{
			if (_isNegativeWeighten)
			{
				std::cout << "Load multi-color map with weighten graph containing negative edges. Will use Bellman-Ford for finding path." << std::endl;
			}
			else
			{
				std::cout << "Load multi-color map with weighten graph. Will use Dijkstra or A* for finding path." << std::endl;
			}
		}
		else
		{
			std::cout << "Load simple map with blocks and grass. Will use BFS for finding path." << std::endl;
		}

		// Check connections of the current GRID CELL to other cells above, below, left and right:
		// Left cell (rr - 1, cc)
		// Right cell (rr + 1, cc)
		// Up cell (rr, cc - 1)
		// Down cell (rr, cc + 1)
		vector<int> dr{ -1, +1, 0, 0 };
		vector<int> dc{ 0, 0, -1, +1 };

		unordered_set<int> visited;

		for (int rr = 0; rr < _grid.size(); rr++)
		{
			for (int cc = 0; cc < _grid[rr].size(); cc++)
			{
				if (_grid[rr][cc]->GetId() > -1)
				{
					for (int i = 0; i < 4; i++)
					{
						int r = rr + dr[i];
						int c = cc + dc[i];

						if (r >= 0 && c >= 0 && r < _width && c < _height &&
							_grid[r][c]->GetId() > -1)
						{
							if (_isNegativeWeighten)
							{
								// Implement a little trick. Since every edge of the GRID is dually-connected,
								// we never get DAG - instead, it will be always cyclic. This will make it impossible to
								// run any Path finding algorithm on it, since it will contain negative cycles.
								// So, what we do here, is making specifically negative cells Single-Directed by removing second edge.

								if (_grid[r][c]->GetWeight() < 0 && visited.count(_grid[r][c]->GetId()) == 0)
								{
									_adjacencyList[_grid[rr][cc]->GetId()].push_back(_grid[r][c]);
									visited.insert(_grid[r][c]->GetId());
								}

								//_edgesList.push_back(
								//	std::make_shared<Edge>(_grid[rr][cc]->GetId(), _grid[r][c]->GetId(), _grid[r][c]->GetWeight()));
							}
							else
							{
								_adjacencyList[_grid[rr][cc]->GetId()].push_back(_grid[r][c]);
							}
						}
					}
				}
			}
		}
	}
}

void RectangularMap::Draw() const
{
	MapBase::Draw();

	_focus->Draw(_window, _scaleFactor);

	_window.display();
}

void RectangularMap::_Scale(int width, int height)
{
	_width = width;
	_height = height;

	_focus->BottomRightX = _width;
	_focus->BottomRightY = _height;

	_visibleTopLeftX = 0;
	_visibleTopLeftY = 0;
	_visibleBottomRightX = _width;
	_visibleBottomRightY = _height;

	float height_factor = (float)SCREEN_HEIGHT / (float)(_height * DEFAULT_SQUARE_TILE_SIZE); 
	float width_factor = (float)SCREEN_WIDTH / (float)(_width * DEFAULT_SQUARE_TILE_SIZE);

	if (height_factor < 1 || width_factor < 1) // If map doesn't fit to the screen, then we need to scale...
	{
		if (height_factor < width_factor)
		{
			_scaleFactor = height_factor;
		}
		else
		{
			_scaleFactor = width_factor;
		}
	}
}

/// <summary>
/// Loads data file into a grid representation of the map.
/// </summary>
bool RectangularMap::LoadMap(const std::string& filepath, bool shadow)
{
	if (_mapLoaded)
	{
		return _mapLoaded;
	}

	ifstream readstream;
	readstream.open(filepath);

	Color freeColor = Color::Green;
	freeColor.r = 34;
	freeColor.g = 177;
	freeColor.b = 76;

	Color blockColor = Color::Black; 
	Color feeColor = Color::Magenta;
	feeColor.r = 168;
	feeColor.g = 168;
	feeColor.b = 168;

	Color negativeFeeColor = Color::Cyan;

	if (readstream)
	{
		int mapSize = 0;
		int maxTips = 0;
		int roverCost = 0;
		readstream >> mapSize >> maxTips >> roverCost;

		_Scale(mapSize, mapSize);

		// Fill the grid.
		_verticesNumber = 0;
		for (int row = 0; row < _height; row++)
		{
			string line;
			readstream >> line; // std::getline(std::cin, line); 
			_grid.emplace_back(vector<std::shared_ptr<Cell>>(mapSize));

			if (line.size() != _height)
			{
				return _mapLoaded;
			}

			for (int col = 0; col < _width; col++)
			{
				int x = col;
				int y = row;
				if (line[col] == BLOCK_CELL)
				{
					_grid[row][col] = std::make_shared<Cell>(x, y, line[col], blockColor, shadow, _scaleFactor);
					_grid[row][col]->SetId(-1);
				}
				else
				{
					Color color;
					if (line[col] == FREE_CELL)
					{
						color = freeColor;
					}
					else if (line[col] == GRASS_CELL)
					{
						color = feeColor;
						_isWeighten = true; // Is map data more complicated than simple grid?
					}
					else if (line[col] == WATER_CELL)
					{
						color = negativeFeeColor;
						_isWeighten = true; // Is map data more complicated than simple grid?
						_isNegativeWeighten = true;
					}

					_grid[row][col] = std::make_shared<Cell>(x, y, line[col], color, shadow, _scaleFactor);
					_grid[row][col]->SetId(_verticesNumber);
					_moveableCells.push_back(_grid[row][col]);
					++_verticesNumber;
				}
			}
		}

		_mapLoaded = true;

		// Make a graph
 		InitialiseGraph();
	}
	readstream.close();

	return _mapLoaded;
}

bool RectangularMap::IsReady()
{
	return _mapLoaded;
}

std::tuple<float, float, float, float> RectangularMap::GetCoordinateBounds() const
{
	return std::make_tuple(0 * _scaleFactor, 0 * _scaleFactor, _width * _scaleFactor, _height * _scaleFactor);
}

std::vector<std::shared_ptr<Cell>> RectangularMap::GetPath(int x1, int y1, int x2, int y2) const
{
	if (_isWeighten)
	{
		if (_isNegativeWeighten)
		{
			bool hasNegativeCycle = false;
			std::vector<std::shared_ptr<Cell>> path;

			std::tie(hasNegativeCycle, path) = _GetPathByBellmanFord(x1, y1, x2, y2);

			if (hasNegativeCycle)
				return {};
			else
				return path;
		}
		else
		{
			//return _GetPathByDijkstra(x1, y1, x2, y2);
			return _GetPathByAStar(x1, y1, x2, y2);
		}
	}
	else
	{
		//return _GetPathByBFSOnGrid(x1, y1, x2, y2);
		return _GetPathByBFSOnGraph(x1, y1, x2, y2);
	}
}

/// <summary>
/// Gets weight of edge.
/// </summary>
int RectangularMap::_GetWeight(const char symbol) const
{
	switch (symbol)
	{
	case FREE_CELL: // Normal moveables (road) 1-weight grid/map cell.
		return 1;
	case BLOCK_CELL: // Non-moveable, 0-weight cell.
		return 100;
	case GRASS_CELL: // Imitate positive-weight cells.
		return 2;
	case WATER_CELL: // Imitate negative-weight cells.
		return -5;   // -1 is already allocated.
	}
}

/// <summary>
/// Single source shortest path algorithm for weighten graphs that cannot deal with negative weights.
/// O((E+V)log(V))
/// </summary>
std::vector<std::shared_ptr<Cell>> RectangularMap::_GetPathByDijkstra(int x1, int y1, int x2, int y2) const
{
 	int n = _adjacencyList.size();
	vector<int> shortestPath(n, INF); // Shortest distance from Start to i
	vector<int> previousVertex(n, -1); // Previous node in shortest path to i.

	shared_ptr<Cell> startCell = GetCellFromGrid(y1, x1);
	int startId = startCell->GetId();
	shortestPath[startId] = 0;

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
	q.push({ 0, startId });

	while (!q.empty())
	{
		pair<int, int> c = q.top();
		q.pop();

		int distance = c.first;
		int currentId = c.second;

		if (shortestPath[currentId] < distance) // Защитное условие, т.к. из очереди ничего не удаляется
		{
			continue;
		}

		for (size_t i = 0; i < _adjacencyList[currentId].size(); ++i)
		{
			shared_ptr<Cell> toCell = _adjacencyList[currentId][i];
			int toIndex = toCell->GetId();
			int weight_vu = _GetWeight(toCell->GetSymbol());

			int new_distance = distance + weight_vu;
			if (shortestPath[toIndex] > new_distance)
			{
				shortestPath[toIndex] = new_distance;
				previousVertex[toIndex] = currentId;
				q.push({ new_distance, toIndex });
			}
		}
	}

	std::vector<int> resultInd = _RetrievePathCellIds(_grid[y2][x2]->GetId(), previousVertex);

	// Convert to list of Cells.
	std::vector<std::shared_ptr<Cell>> result;
	for (auto& v : resultInd)
	{
		if (v >= 0 && v < _moveableCells.size())
			result.push_back(_moveableCells[v]);
		else
			return {};
	}

	return result;
}

std::vector<std::shared_ptr<Cell>> RectangularMap::_GetPathByAStar(int x1, int y1, int x2, int y2) const
{
	// Euclidian distance from node start to specified node id.
	auto calcEuristic = [](int x, int y, int x2, int y2) {
		return sqrt(
			pow(abs(
				x2 > x ?
				x2 - x :
				x - x2), 2) +
			pow(abs(
				y2 > y ?
				y2 - y :
				y - y2), 2));
	};

	int n = _adjacencyList.size();
	vector<int> shortestPath(n, INF); // Shortest distance from Start to i
	vector<int> previousVertex(n, -1); // Previous node in shortest path to i.

	int startId = _grid[y1][x1]->GetId();
	int finishId = _grid[y2][x2]->GetId();

	shortestPath[startId] = 0;

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
	q.push({0, startId });

	while (!q.empty())
	{
		pair<int, int> cur = q.top();
		q.pop();

		int currentId = cur.second;

		if (currentId == finishId)
		{
			// FOUND
			break;
		}

		for (size_t i = 0; i < _adjacencyList[currentId].size(); ++i)
		{
			shared_ptr<Cell> toCell = _adjacencyList[currentId][i];
			int toIndex = toCell->GetId();
			int weight = _GetWeight(toCell->GetSymbol());

			int newDistance = shortestPath[currentId] + weight;
			if (shortestPath[toIndex] > newDistance)
			{
				shortestPath[toIndex] = newDistance;
				previousVertex[toIndex] = currentId;
				int priority = newDistance + calcEuristic(_moveableCells[toIndex]->GetX(), _moveableCells[toIndex]->GetY(), x2, y2);
				q.push({ priority, toIndex });
			}
		}
	}

	std::vector<int> resultInd = _RetrievePathCellIds(finishId, previousVertex);

	// Convert to list of Cells.
	std::vector<std::shared_ptr<Cell>> result;
	for (auto& v : resultInd)
	{
		if (v >= 0 && v < _moveableCells.size())
			result.push_back(_moveableCells[v]);
		else
			return {};
	}

	return result;
}

std::tuple<bool, std::vector<std::shared_ptr<Cell>>> RectangularMap::_GetPathByBellmanFord(int x1, int y1, int x2, int y2) const
{
	int verticesNumber = _adjacencyList.size();
	vector<int> shortestPath(verticesNumber, INF); // Shortest distance from Start to i
	vector<int> previousVertex(verticesNumber, -1); // Previous node in shortest path to i.

	shared_ptr<Cell> startCell = GetCellFromGrid(y1, x1);
	int startId = startCell->GetId();
	shortestPath[startId] = 0;

	bool updated = false;
	// For each vertex, apply relaxation for all the edges V - 1 times.
	// It's important that we should be able to find optimal solution for not more than V - 1 relaxation steps.
	for (int v = 0; v < verticesNumber - 1; v++)
	{
		updated = false;
		for (size_t fromId = 0; fromId < _adjacencyList.size(); ++fromId)
		{
			for (size_t to = 0; to < _adjacencyList[fromId].size(); ++to)
			{
				shared_ptr<Cell> toCell = _adjacencyList[fromId][to];
				int toId = toCell->GetId();
				int weight_vu = _GetWeight(toCell->GetSymbol());

				int new_distance = shortestPath[fromId] + weight_vu;
				if (shortestPath[toId] > new_distance)
				{
					shortestPath[toId] = new_distance;
					previousVertex[toId] = fromId;
					updated = true;
				}
			}
		}
		// Only in the worst case does it take V-1 iterations for the Bellman-Ford
		// algorithm to complete. Another stopping condition is when we're unable to
		// relax an edge, this means we have reached the optimal solution early.
		if (!updated)
			break;
	}

	// Run one more relaxation step to detect which nodes are part of a negative cycle. 
	bool hasNegativeCycle = false;
	for (int k = 0; k < verticesNumber - 1; k++)
	{
		for (size_t fromId = 0; fromId < _adjacencyList.size(); ++fromId)
		{
			for (size_t to = 0; to < _adjacencyList[fromId].size(); ++to)
			{
				shared_ptr<Cell> toCell = _adjacencyList[fromId][to];
				int toId = toCell->GetId();
				int weight_vu = _GetWeight(toCell->GetSymbol());

				if (shortestPath[toId] > shortestPath[fromId] + weight_vu)
				{
					shortestPath[toId] = NEG_INF;
					previousVertex[toId] = -2;
					hasNegativeCycle = true;
				}
			}
		}
	}

	// Reconstruct the path.
	std::vector<std::shared_ptr<Cell>> result;

	int destinationCellId = _grid[y2][x2]->GetId();

	if (shortestPath[destinationCellId] == NEG_INF)
	{
		cout << "Graph representation of the map contains negative cycles. There is infinite number of shortest paths between the start and finish." << endl;
	}
	else
	{
		std::vector<int> path;
		for (int v = destinationCellId; v != startId && v != -1 && v != -2; v = previousVertex[v])
		{
			path.push_back(v);
		}
		std::reverse(path.begin(), path.end());

		// Convert to list of Cells.
		for (auto& v : path)
		{
			if (v >= 0 && v < _moveableCells.size())
				result.push_back(_moveableCells[v]);
			else
				break;
		}
	}

	return std::make_tuple(hasNegativeCycle, result);
}

//std::tuple<bool, std::vector<std::shared_ptr<Cell>>> RectangularMap::_GetPathByBellmanFordEdgesList(int x1, int y1, int x2, int y2) const
//{
//	int n = _edgesList.size();
//	vector<int> shortestPath(n, INF); // Shortest distance from Start to i
//	vector<int> previousVertex(n, -1); // Previous node in shortest path to i.
//
//	shared_ptr<Cell> startCell = GetCellFromGrid(y1, x1);
//	int startId = startCell->GetId();
//	shortestPath[startId] = 0;
//
//	// Only in the worst case does it take V-1 iterations for the Bellman-Ford
//	// algorithm to complete. Another stopping condition is when we're unable to
//	// relax an edge, this means we have reached the optimal solution early.
//	bool relaxedAnEdge = false;
//
//	// For each vertex, apply relaxation for all the edges
//	for (int v = 0; v < n - 1 && relaxedAnEdge; v++)
//	{
//		for (shared_ptr<Edge> edge : _edgesList)
//		{
//			int toId = edge->GetToId();
//			int fromId = edge->GetFromId();
//			if (shortestPath[toId] > shortestPath[fromId] + edge->GetWeight())
//			{
//				shortestPath[toId] = shortestPath[fromId] + edge->GetWeight();
//				previousVertex[toId] = fromId;
//			}
//		}
//	}
//
//	// Run algorithm a second time to detect which nodes are part
//	// of a negative cycle. A negative cycle has occurred if we
//	// can find a better path beyond the optimal solution.
//	relaxedAnEdge = false;
//	for (int v = 0; v < n - 1 && relaxedAnEdge; v++)
//	{
//		relaxedAnEdge = false;
//		for (shared_ptr< Edge> edge : _edgesList)
//		{
//			int toId = edge->GetToId();
//			if (shortestPath[toId] > shortestPath[edge->GetFromId()] + edge->GetWeight())
//			{
//				shortestPath[toId] = NEG_INF;
//				previousVertex[toId] = -1;
//				relaxedAnEdge = true;
//				break;
//			}
//		}
// 
//		if (relaxedAnEdge)
//			break;
//	}
//
//	std::vector<int> path;
//	int destinationCellId = _grid[y2][x2]->GetId();
//	for (int v = destinationCellId; v != startId && v != -1; v = previousVertex[v])
//	{
//		path.push_back(v);
//	}
//	std::reverse(path.begin(), path.end());
//
//	// Convert to list of Cells.
//	std::vector<std::shared_ptr<Cell>> result;
//	for (auto& v : path)
//	{
//		if (v >= 0 && v < _moveableCells.size())
//			result.push_back(_moveableCells[v]);
//		else
//			return {};
//	}
//  return std::make_tuple(relaxedAnEdge, result);
//}

/// <summary>
/// BFS works only for non-weightened graphs, which is exactly what I have here in the Grid.
/// Additionally, I converted grid into Adjacency List and trying to use it to find the path.
/// </summary>
std::vector<std::shared_ptr<Cell>> RectangularMap::_GetPathByBFSOnGraph(int x1, int y1, int x2, int y2) const
{
	std::vector<shared_ptr<Cell>> result;

	if (x1 == x2 && y1 == y2)
	{
		return result;
	}

	// Convert current Rover position X, Y into Cells?
	shared_ptr<Cell> startCell = GetCellFromGrid(y1, x1);
	shared_ptr<Cell> endCell = GetCellFromGrid(y2, x2);

	int n = _adjacencyList.size();

	vector<bool> visited(n, false);
	vector<int> dist(n, -1);
	vector<int> path(n, -1);

	int startId = startCell->GetId();

	queue<shared_ptr<Cell>> queue;
	queue.push(startCell);

	visited[startId] = true;
	path[startId] = -2;

	while (!queue.empty())
	{
		shared_ptr<Cell> v = queue.front();
		queue.pop();
		int adjIndex = v->GetId(); // We set it during Graph Init.

		for (size_t i = 0; i < _adjacencyList[adjIndex].size(); ++i)
		{
			shared_ptr<Cell> to = _adjacencyList[adjIndex][i];
			int toIndex = to->GetId();

			if (!visited[toIndex])
			{
				visited[toIndex] = true;
				queue.push(to);

				if (dist[adjIndex] == -1)
				{
					dist[adjIndex] = 0;
				}

				dist[toIndex] = dist[adjIndex] + 1;
				path[toIndex] = adjIndex;

				//if (dist[toIndex] >= max) // If we move out of limits (MAX)
				//{
				//	remember path, dist and visited, and exit.
				//	return;
				//}
			}
		}
	}

 	std::vector<int> resultInd = _RetrievePathCellIds(endCell->GetId(), path);

	// Convert to list of Cells.
  	for (auto& v : resultInd)
	{
		if (v >= 0 && v < _moveableCells.size())
			result.push_back(_moveableCells[v]);
		else
			return {};
	}

	return result;
}

std::vector<int> RectangularMap::_RetrievePathCellIds(int destinationCellId, const vector<int> paths) const
{
	std::vector<int> path;
	for (int v = destinationCellId; v != -2; v = paths[v])
	{
		if (v == -1)
			break;
		path.push_back(v);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

string RectangularMap::_TryGetDirections(const vector<int> path) const // , const vector <Cell>& cells, string route, int& startPoint)
{
	string route;

	if (path[0] >= _moveableCells.size() || path[0] < 0)
		return route;

	int position = 0; // startPoint;
	auto last = _moveableCells[path[0]];

	for (size_t i = 1; i < path.size(); ++i)
	{
		if (path[i] >= 0)
			return route;

		auto cell = _moveableCells[path[i]];

		if (cell->GetY() > last->GetY())
		{
			route[position] = 'D';
		}
		else if (cell->GetY() < last->GetY())
		{
			route[position] = 'U';
		}
		else
		{
			if (cell->GetX() > last->GetY())
			{
				route[position] = 'R';
			}
			else
			{
				route[position] = 'L';
			}
		}

		last = cell;
		++position;

		if (position >= path.size())
		{
			// NOT reached the destination :(
			return route;
		}
	}

	return route;
}

/// <summary>
/// BFS works only for non-weightened graphs, which is exactly what I have here in the Grid.
/// This method implements BFS right on  the Grid, without using any other graph representation.
/// </summary>
std::vector<std::shared_ptr<Cell>> RectangularMap::_GetPathByBFSOnGrid(int x1, int y1, int x2, int y2) const
{
	std::vector<shared_ptr<Cell>> result;

	if (x1 == x2 && y1 == y2)
	{
		return result;
	}

	// Two queues for two coordinates (X, Y). Queues updated synchronously.
	queue<int> xQueue;
	queue<int> yQueue;

	vector<vector<bool>> visited(_width, vector<bool>(_height));

	int n = _width * _height;
	vector<int> dist(n, -1);
	vector<int> path(n, -1);

	bool atDestination = false;

	xQueue.push(x1); // Start x
	yQueue.push(y1); // Start y

	visited[y1][x1] = true;
 	path[x1 + _width * y1] = -2;

	vector<int> dr{ -1, +1, 0, 0 }; // Row
	vector<int> dc{ 0, 0, -1, +1 }; // Column

	// Find paths to all cells by BFS until we reach the goal cell.
	while (!xQueue.empty() && !yQueue.empty())
	{
		int curX = xQueue.front();
		int curY = yQueue.front();
		xQueue.pop();
		yQueue.pop();

		if (curX == x2 && curY == y2)
		{
			atDestination = true;
			break;
		}

		int v = curX + curY * _width;

		// Add all neighbour cells into queues:
		// Left cell (rr - 1, cc)
		// Right cell (rr + 1, cc)
		// Up cell (rr, cc - 1)
		// Down cell (rr, cc + 1)
		for (int i = 0; i < 4; i++)
		{
			int r = curY + dr[i]; // Remember! X, Y and Row, Column - are opposite terms.
			int c = curX + dc[i];

			if (r >= 0 && c >= 0 && 
				r < _width && c < _height &&
				visited[r][c] == false &&
				_grid[r][c]->GetId() > -1 && _grid[r][c]->GetSymbol() != '#') // Avoid blocks
			{
				xQueue.push(c);
				yQueue.push(r);
				visited[r][c] = true;

				if (dist[v] == -1)
				{
					dist[v] = 0;
				}

				int to = c + r * _width;
				dist[to] = dist[v] + 1;
				path[to] = v;
			}
		}
	}

	//for (int i = 0; i < _width; i++)
	//{
	//	for (int j = 0; j < _height; j++)
	//	{
	//		cout << visited[i][j] << " ,";
	//	}

	//	cout << " ;" << endl;
	//}

	// Since we found destination, now we need to reconstruct the path to it.
	if (atDestination)
	{
		std::vector<int> resultInd = _RetrievePathCellIds(x2 + _width * y2, path);

		// Convert to list of Cells.
		for (auto& v : resultInd)
		{
			int y = v / _width;
			int x = v - y * _width;

			result.push_back(_grid[y][x]);
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////// For DAGs only /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Not aplicable for this app ///////////////////////////////////////////

/// <summary>
/// Find shortest distance lengths to every node from given starting node. Works on weighten graphs even with negative weights.
/// </summary>
std::vector<int> RectangularMap::_GetSingleSourceShortedPaths(int x1, int y1) const
{
	// FIRST: topologically sort Cells
	std::vector<std::shared_ptr<Cell>> topSorted = _TopologicalSortCyclesProtected(_adjacencyList);

	// OR Alternatively we can do this:
	//if (_IsDAGbyTarjansStronglyComponent(_adjacencyList)) // TODO Not yet implemented // Check if it is DAG
	//{
	//	topSorted = _TopologicalSortByDFS(_adjacencyList); // If so, let's sort it topologically.
	//}
	//else
	//{
	//	return {};
	//}

	// OR Alternatively:
	//  topSorted = _AllTopologicalSortsByIndegree(_adjacencyList); // TODO Not implemented
	// Get needed sort.

	// Now, find MINIMAL distance to EVERY CELL.
	std::vector<int> dist = std::vector<int>(_verticesNumber, -1);
	int startId = _grid[y1][x1]->GetId();
	dist[startId] = 0;

	for (auto& cell : topSorted)
	{
		int fromId = cell->GetId();

		if (dist[fromId] != -1)
		{
			for (auto toCell : _adjacencyList[fromId])
			{
				int newDist = dist[fromId] + _GetWeight(toCell->GetSymbol());

				int toId = toCell->GetId();

				if (dist[toId] == -1)
				{
					dist[toId] = newDist;
				}
				else
				{
					dist[toId] = std::min(dist[toId], newDist);
				}
			}
		}
	}

	// ALGORITHM TODO:
	//std::vector<int> topSort = _topologicalSort(_edgesList);
	//std::vector<int> dist = std::vector<int>(_verticesNumber, -1);
	//dist[0] = 0;

	//for (int i = 0; i < _verticesNumber; i++)
	//{
	//	int nodeIndex = topSort[i];

	//	if (dist[nodeIndex] != -1)
	//	{
	//		std::vector<Edge> adjacentEdges = _edgesList[nodeIndex];
	//		
	//		for (Edge edge : adjacentEdges)
	//		{
	//			int newDist = dist[nodeIndex] + edge.Weight;
	//			if (dist[edge.To] == -1)
	//			{
	//				dist[edge.To] = newDist;
	//			}
	//			else
	//			{
	//				dist[edge.To] = std:min(dist[edge.To], newDist);
	//			}
	//		}
	//	}
	//}

	return dist;
}

/// <summary>
/// Finds longest distance lengths to every node from given starting node. Works on weighten graphs even with negative weights.
/// </summary>
std::vector<int> RectangularMap::_GetSingleSourceLongestPaths(int x1, int y1) const
{
	// FIRST: multiply all weights to -1
	for (auto& cell : _moveableCells)
	{
		cell->SetWeight(cell->GetWeight() * -1); // Potentiall corruptive, if exception happens.
	}

	// SECOND: find SSSP
	std::vector<int> distances = _GetSingleSourceShortedPaths(x1, y1);

	// THIRD: multiply all weights to -1 again
	for (auto& cell : _moveableCells)
	{
		cell->SetWeight(cell->GetWeight() * -1);
	}

	return distances;
}

/// <summary>
/// Sort by time of exit from each node during DFS.
/// </summary>
std::vector<std::shared_ptr<Cell>> RectangularMap::_TopologicalSortByDFS(Graph graph) const // TODO
{
	std::vector<bool> visited(_verticesNumber, false);
	std::vector<int> sorted(_verticesNumber);

	for (int i = 0; i < _verticesNumber; i++)
	{
		_TopologicalSortDFSExplore(i, graph, visited, sorted);
	}

	// Convert to list of Cells.
	std::vector<std::shared_ptr<Cell>> result;
	for (auto& v : sorted)
	{
		if (v >= 0 && v < _moveableCells.size()) // Guard
			result.push_back(_moveableCells[v]);
		else
			return {};
	}

	return result;
}

/// <summary>
/// DFS for topological sorting.
/// </summary>
void RectangularMap::_TopologicalSortDFSExplore(int cellId, Graph graph, std::vector<bool> visited, std::vector<int> sorted) const
{
	visited[cellId] = true;

	// Pre Visit Time.

	for (auto cellId : graph[cellId])
	{
		int id = cellId->GetId();

		if (!visited[id])
		{
			_TopologicalSortDFSExplore(id, graph, visited, sorted);
		}
	}

	// Post Visit Time.
	sorted.push_back(cellId);
}

/// <summary>
/// Uses DFS with graph coloring algorithm to detect cycles.
/// </summary>
std::vector<std::shared_ptr<Cell>> RectangularMap::_TopologicalSortCyclesProtected(Graph graph) const
{
	std::vector<int> sorted(_verticesNumber);

	constexpr int WHITE = 0; /// indicates the node hasn't been explored
	constexpr int GREY = 1;	 /// indicates node is in stack waiting to be explored
	constexpr int BLACK = 2; /// indicates node has already been explored

	vector<int> visited(_verticesNumber, WHITE);

	std::stack<int> stack;
	int start = graph[0][0]->GetId();
	stack.push(start);
	visited[start] = GREY;

	while (!stack.empty())
	{
		int v = stack.top(); // Get item
		stack.pop(); // Remove item from stack

		// <-- do not mark as visited (BLACK) until visit all subnodes!

		if (visited[v] == GREY) // Current item to handle!
		{
			for (int i = 0; i < graph[v].size(); i++)
			{
				int neighbourTo = graph[v][i]->GetId();
				if (visited[neighbourTo] == WHITE) // NOT yet  visited
				{
					stack.push(neighbourTo);
					visited[neighbourTo] = GREY; // Next current item to handle!
				}
				else if (visited[neighbourTo] == GREY)
				{
					// Cycle detected!
					return {};
				}
			}
			sorted.push_back(v);  //<-- topological
			visited[v] = BLACK; // Mark as Visited!
		}
	}

	// Convert to list of Cells.
	std::vector<std::shared_ptr<Cell>> result;
	for (auto& v : sorted)
	{
		if (v >= 0 && v < _moveableCells.size()) // Guard
			result.push_back(_moveableCells[v]);
		else
			return {};
	}

	return result;
}
