#include "order.h"
#include <tuple>

Order::Order(int x1, int y1, int x2, int y2, Color& color) : _x1(x1), _y1(y1), _x2(x2), _y2(y2), _color(color)
{

}

std::string Order::SVGPrint(const std::string& color) const
{
	return "<rect x=\"" + std::to_string(_x1 * DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"y=\"" + std::to_string(_y1 * DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"width=\"" + std::to_string(DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"height=\"" + std::to_string(DEFAULT_SQUARE_TILE_SIZE) + "\" " +
		"stroke=\"" + color + "\" " +
		"fill=\"" + color + "\" />";
}


void Order::Draw(sf::RenderWindow& window, float scaleFactor) const
{
	sf::RectangleShape tile;

	float cellSize = scaleFactor * (float)DEFAULT_SQUARE_TILE_SIZE;

	tile.setSize(sf::Vector2f(cellSize, cellSize));

	tile.setPosition(sf::Vector2f(_x1 * scaleFactor, _y1 * scaleFactor));

	tile.setFillColor(_color);

	window.draw(tile);
}

void Order::SetColor(sf::Color& color)
{
	_color = color;
}

Coordinate Order::GetPickupLocation() const
{
	return std::make_tuple(_x1, _y1);
}

Coordinate Order::GetDropoffLocation() const
{
	return std::make_tuple(_x2, _y2);
}