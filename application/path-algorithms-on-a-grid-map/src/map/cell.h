#ifndef __Cell_h__
#define __Cell_h__

#include "cellbase.h"

class Cell : public CellBase
{
public:
	Cell(int x, int y, char symbol, Color color, bool shadow, float scaleFactor);
	virtual std::string SVGPrint(const std::string& color) const;
	virtual void Draw(sf::RenderWindow& window, float scaleFactor) const;

	sf::Color GetColor() const;
	virtual void SetColor(sf::Color& color);

	int GetId() const;
	void SetId(int id);

	int GetWeight() const;
	void SetWeight(int weight);

	char GetSymbol() const;
	int GetX() const;
	int GetY() const;

private:
	int _id;
	int _weight;
	int _x, _y;
	char _symbol;
	bool _shadowed;
	Color _color;
	float _cellSize;
};

#endif __Cell_h__
