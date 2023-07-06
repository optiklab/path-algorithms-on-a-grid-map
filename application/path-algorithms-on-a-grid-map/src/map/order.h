#ifndef __Order_h__
#define __Order_h__

#include "cellbase.h"
#include "coordinate.h"

class Order : public CellBase
{
public:
	Order(int x1, int y1, int x2, int y2, Color& color);
	virtual std::string SVGPrint(const std::string& color) const;
	virtual void Draw(sf::RenderWindow& window, float scaleFactor) const;
	virtual void SetColor(sf::Color& color);

	Coordinate GetPickupLocation() const;
	Coordinate GetDropoffLocation() const;

private:
	int _x1, _y1;
	int _x2, _y2;
	Color& _color;
};

#endif __Order_h__
