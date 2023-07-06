#ifndef MAPBASE_H
#define MAPBASE_H

#include "graph.h"
#include "focus.h"
#include "VisiblePartObserver.h"

class MapBase : public VisiblePartObserver
{
public:
	MapBase(RenderWindow& window, int width, int height);
	virtual ~MapBase();

	/// <summary>
	///  Creates a fully Graph representation of the map, that:
	///  1. Avoids non - moveable cells to build the paths more efficiently(than in Grid).
	///  2. Allows applying appropriate graph algorithms based on map data.
	/// </summary>
	virtual void InitialiseGraph() = 0;

	/// <summary>
	/// Loads data file into a grid representation of the map.
	/// </summary>
	virtual bool LoadMap(const std::string& filepath, bool shadow) = 0;

	virtual bool IsReady() = 0;

	virtual void Draw() const;

	void PrintMapSVG(const std::string& filename) const;

	std::shared_ptr<Cell> GetCellFromGrid(int row, int column) const;

public:
	virtual std::tuple<float, float, float, float> GetCoordinateBounds() const = 0;
	virtual std::shared_ptr<Cell> GetFirstMoveableCell() const;

	virtual std::vector<std::shared_ptr<Cell>> GetPath(int x1, int y1, int x2, int y2) const = 0;

public:
	void UpdateVisiblePart(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);

protected:
	int _width;
	int _height;
	int _verticesNumber;

	// List of cells to easily get Cell Description (coordinates, symbol, color, etc.) by Index
	vector<std::shared_ptr<Cell>> _moveableCells;

	// Grid representation of our map.
	vector<vector<std::shared_ptr<Cell>>> _grid;

	// Graph representations of our map that:
	// 1. Avoids non-moveable cells to build the paths more efficiently (than in Grid).
	// 2. Allows applying appropriate graph algorithms based on map data.
	Graph _adjacencyList;
	GraphEdgesList _edgesList;

	/// <summary>
	/// Is set if map cells loaded from the file contain more than 2 potential states (like: block, grass and water).
	/// </summary>
	bool _isWeighten;

	/// <summary>
	/// Is set if map cells loaded from the file contain at least one state considered as negative (e.g. water).
	/// </summary>
	bool _isNegativeWeighten;

	// Visualization staff.
	RenderWindow& _window;
	float _scaleFactor;

	float _visibleTopLeftX;
	float _visibleTopLeftY;
	float _visibleBottomRightX;
	float _visibleBottomRightY;
};

#endif
