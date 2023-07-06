#include "focus.h"

Focus::Focus(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	TopLeftX = topLeftX;
	TopLeftY = topLeftY;
	BottomRightX = bottomRightX;
	BottomRightY = bottomRightY;

	Color = sf::Color::Red;
}

void Focus::Draw(sf::RenderWindow& window, float scaleFactor) const
{
	float size = scaleFactor * (float)DEFAULT_SQUARE_TILE_SIZE;

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(BottomRightX * size - TopLeftX * size, BottomRightY * size - TopLeftY * size));
	rectangle.setPosition(sf::Vector2f(TopLeftX * size, TopLeftY * size));
	rectangle.setOutlineColor(Color);
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineThickness(2);

	window.draw(rectangle);
}

void Focus::FocusIn()
{
	TopLeftX += 1;
	TopLeftY += 1;
	BottomRightX -= 1;
	BottomRightY -= 1;
	Notify();
}

void Focus::FocusOut()
{
	TopLeftX -= 1;
	TopLeftY -= 1;
	BottomRightX += 1;
	BottomRightY += 1;
	Notify();
}

void Focus::MoveUp()
{
	TopLeftY -= 1;
	BottomRightY -= 1;
	Notify();
}

void Focus::MoveDown()
{
	TopLeftY += 1;
	BottomRightY += 1;
	Notify();
}

void Focus::MoveLeft()
{
	TopLeftX -= 1;
	BottomRightX -= 1;
	Notify();
}

void Focus::MoveRight()
{
	TopLeftX += 1;
	BottomRightX += 1;
	Notify();
}

//------------------------------------------------------- Implementation of NotifyVisiblePartChanged -----------------------------------------------------

void Focus::Listen(VisiblePartObserver* observer)
{
	_listObserver.push_back(observer);
}

void Focus::Detach(VisiblePartObserver* observer)
{
	_listObserver.remove(observer);
}

void Focus::Notify()
{
	std::list<VisiblePartObserver*>::iterator iterator = _listObserver.begin();

	while (iterator != _listObserver.end())
	{
		(*iterator)->UpdateVisiblePart(TopLeftX, TopLeftY, BottomRightX, BottomRightY);
		++iterator;
	}
}