#ifndef __RectangularMap_h__
#define __RectangularMap_h__

#include "mapbase.h"

class RectangularMap : public MapBase
{
public:
	RectangularMap(int width, int height, shared_ptr<Focus> focus, RenderWindow& window);

	/**
	 * \brief Creates a fully connected Graph to represent the map - grid of cells available to move through. 
	 */
	virtual void InitialiseGraph();

	virtual bool MapBase::LoadMap(const std::string& filepath, bool shadow);

	virtual bool IsReady();

	virtual void Draw() const;

public:
	virtual std::tuple<float, float, float, float> GetCoordinateBounds() const;
	virtual std::vector<std::shared_ptr<Cell>> GetPath(int x1, int y1, int x2, int y2) const;

private:
	void _Scale(int width, int height);

	/// <summary>
	/// BFS works only for non-weightened graphs, which is exactly what I have here in the Grid 
	/// defined in some files where I have only 2 states: block and grass.
	/// Additionally, I converted grid into Adjacency List and trying to use it to find the path.
	/// </summary>
	std::vector<std::shared_ptr<Cell>> _GetPathByBFSOnGraph(int x1, int y1, int x2, int y2) const;
	std::vector<int> _RetrievePathCellIds(int destinationCellId, const vector<int> paths) const;

	/// <summary>
	/// BFS works only for non-weightened graphs, which is exactly what I have here in the Grid 
	/// defined in some files where I have only 2 states: block and grass.
	/// This method implements BFS right on the Grid, without using any other graph representation.
	/// </summary>
	std::vector<std::shared_ptr<Cell>> _GetPathByBFSOnGrid(int x1, int y1, int x2, int y2) const;

	string _TryGetDirections(const vector<int> path) const;
	int _VertexIndex(int row, int column);

	/// <summary>
	/// Single source shortest path algorithm for weighten graphs that cannot deal with negative weights.
	/// O((E+V)log(V))
	/// </summary>
	std::vector<std::shared_ptr<Cell>> _GetPathByDijkstra(int x1, int y1, int x2, int y2) const;

	/// <summary>
	/// Single source shortest path algorithm for weighten graphs with additional heuristic to speed up search.
	/// However, it still cannot deal with negative weights.
	/// </summary>
	std::vector<std::shared_ptr<Cell>> _GetPathByAStar(int x1, int y1, int x2, int y2) const;

	/// <summary>
	/// Gets weight of edge.
	/// </summary>
	int _GetWeight(const char symbol) const;

	/// <summary>
	/// Single source shortest path algorithm for weighten graphs that easily handles Negative-weights in a graph.
	/// Works using adjacency list. O((E*V)) - worse than Dijkstra.
	/// </summary>
	std::tuple<bool, std::vector<std::shared_ptr<Cell>>> _GetPathByBellmanFord(int x1, int y1, int x2, int y2) const;

	/// <summary>
	/// Single source shortest path algorithm for weighten graphs that easily handles Negative-weights in a graph.
	/// Using Edges list. O((E*V)) - worse than Dijkstra.
	/// </summary>
	//std::tuple<bool, std::vector<std::shared_ptr<Cell>>> _GetPathByBellmanFordEdgesList(int x1, int y1, int x2, int y2) const;

private:
///////////////////////////////////////////////////////////////////////// For DAGs only /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// Not aplicable for this app yet ///////////////////////////////////////////
///////////////////////////////////////////////////////// I can start use it later for other maps ///////////////////////////////////////////

	/// <summary>
	/// Single source shortest path algorithm to find shortest distance lengths to every node from given starting node. 
	/// Works on weighten graphs even with negative weights.
	/// </summary>
	std::vector<int> _GetSingleSourceShortedPaths(int x1, int y1) const;

	/// <summary>
	/// Single source shortest path algorithm to find longest distance lengths to every node from given starting node. 
	/// Works on weighten graphs even with negative weights.
	/// </summary>
	std::vector<int> _GetSingleSourceLongestPaths(int x1, int y1) const;

	/// <summary>
	/// Sort by time of exit from each node during DFS.
	/// </summary>
	std::vector<std::shared_ptr<Cell>> _TopologicalSortByDFS(Graph graph) const;

	/// <summary>
	/// DFS for topological sorting.
	/// </summary>
	void _TopologicalSortDFSExplore(int startCellId, Graph graph, std::vector<bool> visited, std::vector<int> sorted) const;

	/// <summary>
	/// Uses DFS with graph coloring algorithm to detect cycles.
	/// </summary>
	std::vector<std::shared_ptr<Cell>> _TopologicalSortCyclesProtected(Graph graph) const;

private:
	bool _mapLoaded;

	shared_ptr<Focus> _focus;
};

#endif __RectangularMap_h__
