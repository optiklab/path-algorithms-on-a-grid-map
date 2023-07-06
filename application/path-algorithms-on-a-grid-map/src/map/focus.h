#pragma once
#ifndef __Focus_h__
#define __Focus_h__

#include "common.h"
#include "NotifyVisiblePartChanged.h"

class Focus : public NotifyVisiblePartChanged
{
public:
    Focus(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);

    void Draw(sf::RenderWindow& window, float scaleFactor) const;

    void FocusIn();
    void FocusOut();

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    // Implementation of NotifyVisiblePartChanged
public:
    void Listen(VisiblePartObserver* observer);
    void Detach(VisiblePartObserver* observer);
    void Notify();

public:
    float TopLeftX;
    float TopLeftY;
    float BottomRightX;
    float BottomRightY;

    Color Color;

private:
    std::list<VisiblePartObserver*> _listObserver;
};

#endif __Focus_h__