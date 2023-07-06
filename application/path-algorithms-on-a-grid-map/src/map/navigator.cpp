#include "navigator.h"
#include <chrono>

Navigator::Navigator() : 
	_tasksLoaded(false),
	_currentOrder(-1)
	//_nextIteration(-1),
	//_lastFinishedIteration(-1),
	//_currentOrderWithinIteration(-1)
{
    _rover = make_shared<Rover>();
}

bool Navigator::LoadTasks(const std::string& filepath)
{
	if (_tasksLoaded)
	{
		return _tasksLoaded;
	}

	Color orderColor = Color::Red;
	ifstream readstream;
	readstream.open(filepath);

	if (readstream)
	{
		string line;
		int mapSize = 0;
		int maxTips = 0;
		int roverCost = 0;
		readstream >> mapSize >> maxTips >> roverCost;

		for (int i = 0; i < mapSize; i++)
		{
			readstream >> line;
		}

		readstream >> _totalIterations >> _totalOrders;

		_orders.clear();
		for (int i = 0; i < _totalIterations; i++)
		{
			int newOrders = 0;
			readstream >> newOrders;

			while (newOrders-- > 0)
			{
				int r1, c1, r2, c2; // Rows and Columns
				readstream >> r1 >> c1 >> r2 >> c2;

				Order order(
					c1 - 1, // Columns are X's
					r1 - 1, // Rows are Y's
					c2 - 1,
					r2 - 1,
					orderColor);

				_orders.push_back(make_shared<Order>(order));
			}
		}
	}
	readstream.close();

	_tasksLoaded = true;

	return _tasksLoaded;
}


bool Navigator::IsReady()
{
	return _tasksLoaded && _map->IsReady();
}

void Navigator::SetMap(std::shared_ptr<MapBase> map)
{
	_map = map;

	auto cell = _map->GetFirstMoveableCell();

	InitRoverPosition(cell->GetX(), cell->GetY());
}

void Navigator::InitRoverPosition(int x, int y)
{
	_rover->PositionX = x;
	_rover->PositionY = y;
}

void Navigator::Navigate()
{
	// Solve next task
	if (_totalOrders > 0)
	{
		if (_currentOrder == -1)
			_currentOrder = 0;

		// Solve order
        _RunDelivery(_orders[_currentOrder]);

		// Mark solved

		// Increase _currentOrder
	}

	// Merge results into map

	// Draw map
	if (_tasksLoaded && _map)
		_map->Draw();
}

void Navigator::_RunDelivery(shared_ptr<Order> order)
{
	int x1, y1;
	std::tie(x1, y1) = order->GetPickupLocation();

	///////////////////////////////////////////////////////////////////// Start chrono
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::time_t start_time = std::chrono::system_clock::to_time_t(start);
	char startStr[26];
	ctime_s(startStr, sizeof startStr, &start_time);
	std::cout << "Started computation at " << startStr;

	///////////////////////////////////////////////////////////////////// Start task
	// Find Path from Rover to Start Cell
	auto path1 = _map->GetPath(_rover->PositionX, _rover->PositionY, x1, y1);

	///////////////////////////////////////////////////////////////////// Finish chrono
	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	char endStr[26];
	ctime_s(endStr, sizeof endStr, &end_time);
	std::cout << "Finished computation at " << endStr;

	std::chrono::duration<double> elapsed_time = end - start;
	double count = elapsed_time.count();
	std::cout << "Path search took: " << count << ((count > 1.0) ? " seconds\n" : " milliseconds\n");

	///////////////////////////////////////////////////////////////////// End of measurements

	sf::Color pathColor = sf::Color::Cyan;
	sf::Color goalColor = sf::Color::Red;
	sf::Color startColor = sf::Color::Yellow;

	for (auto& v : path1)
	{
		if (v->GetX() == x1 && v->GetY() == y1)
		{
			v->SetColor(goalColor);
		}
		else if (v->GetX() == _rover->PositionX && v->GetY() == _rover->PositionY)
		{
			v->SetColor(startColor);
		}
		else
		{
			v->SetColor(pathColor);
		}
	}

	// Find Path from Start Cell to End Cell
	int x2, y2;
	std::tie(x2, y2) = order->GetDropoffLocation();
	auto path2 = _map->GetPath(x1, y1, x2, y2);

	for (auto& v : path2)
	{
		if (v->GetX() == x2 && v->GetY() == y2)
		{
			v->SetColor(goalColor);
		}
		else if (v->GetX() == x1 && v->GetY() == y1)
		{
			v->SetColor(startColor);
		}
		else
		{
			v->SetColor(pathColor);
		}
	}

	// Convert into commands for rover
	// _rover->route = DEFAULT_ROUTE;
	// _rover->route = tryGetDirections(path, cells, route, currentPoint);
}