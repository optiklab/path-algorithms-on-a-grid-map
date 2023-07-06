#ifndef __Navigator_h__
#define __Navigator_h__

#include "order.h"
#include "rover.h"
#include "mapbase.h"

using namespace std;

class Navigator
{
public:
	Navigator();

	bool LoadTasks(const std::string& filepath);
	void SetMap(std::shared_ptr<MapBase> map);
	void InitRoverPosition(int x, int y);
	void Navigate();
	bool IsReady();

private:
	void _RunDelivery(shared_ptr<Order> order);

private:
	std::shared_ptr<MapBase> _map;
	std::shared_ptr<Rover> _rover;

	bool _tasksLoaded;
	int _totalIterations;
	int _totalOrders;
	vector<shared_ptr<Order>> _orders;
	int _currentOrder;

	// I don't need this complexity for now.
	//vector<vector<Order>> _iterationsOrders;
	//int _nextIteration;
	//int _lastFinishedIteration;
	//int _currentOrderWithinIteration;
};

#endif __Navigator_h__