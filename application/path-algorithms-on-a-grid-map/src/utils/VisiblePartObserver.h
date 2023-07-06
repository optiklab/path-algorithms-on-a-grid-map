#pragma once
#ifndef __VisiblePartObserver_h__
#define __VisiblePartObserver_h__

#include "common.h"

class VisiblePartObserver
{
public:
	virtual ~VisiblePartObserver()
	{
	}

	virtual void UpdateVisiblePart(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY) = 0;
};

#endif __VisiblePartObserver_h__