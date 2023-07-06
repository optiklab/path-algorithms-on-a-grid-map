#include "cell.h"
#include <tuple>

Cell::Cell(int x, int y, char symbol, Color color, bool shadow, float scaleFactor) : 
	_x(x), _y(y), _symbol(symbol), _color(color), _id(0), _shadowed(shadow)
{
	if (_symbol == FREE_CELL)
	{
		_weight = 1;
	}
	else if (_symbol == GRASS_CELL)
	{
		_weight = 20; // Cell is still allows to move, but applies FEE.
	}
	else if (_symbol == WATER_CELL)
	{
		_weight = -1;
	}
	else
	{
		_weight = 100;
	}

	_cellSize = scaleFactor * (float)DEFAULT_SQUARE_TILE_SIZE;
}

std::string Cell::SVGPrint(const std::string& color) const
{
	return "<rect x=\"" + std::to_string(_x * DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"y=\"" + std::to_string(_y * DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"width=\"" + std::to_string(DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"height=\"" + std::to_string(DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"stroke=\"" + color + "\" " +
		"fill=\"" + color + "\" />";
}

void Cell::Draw(sf::RenderWindow& window, float scaleFactor) const
{
	if (_shadowed)
	{
		sf::RectangleShape shadow;
		shadow.setSize(sf::Vector2f(_cellSize, _cellSize));
		shadow.setPosition(sf::Vector2f(_x * _cellSize, _y * _cellSize));
		Color c = _color;
		c.b += 100;
		shadow.setFillColor(c);
		window.draw(shadow);
	}

	sf::RectangleShape tile;

	if (_cellSize > 5)
	{
		tile.setSize(sf::Vector2f(_cellSize - 5, _cellSize - 5));
	}
	else
	{
		tile.setSize(sf::Vector2f(_cellSize, _cellSize));
	}

	tile.setPosition(sf::Vector2f(_x * _cellSize, _y * _cellSize));
	tile.setFillColor(_color);
	window.draw(tile);
}

void Cell::SetColor(sf::Color& color)
{
	_color = color;
}

sf::Color Cell::GetColor() const
{
	return _color;
}

char Cell::GetSymbol() const
{
	return _symbol;
}

int Cell::GetX() const
{
	return _x;
}

int Cell::GetY() const
{
	return _y;
}

int Cell::GetId() const
{
	return _id;
}

void Cell::SetId(int id)
{
	_id = id;
}

void Cell::SetWeight(int weight)
{
	_weight = weight;
}

int Cell::GetWeight() const
{
	return _weight;
}