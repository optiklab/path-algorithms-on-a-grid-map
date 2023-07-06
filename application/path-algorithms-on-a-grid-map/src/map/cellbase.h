#ifndef CELLBASE_H
#define CELLBASE_H

#include "common.h"

class CellBase
{
public:
	virtual std::string SVGPrint(const std::string& color) const = 0;

	virtual void Draw(sf::RenderWindow& window, float scaleFactor) const = 0;

	virtual void SetColor(sf::Color& color) = 0;
};

#endif
