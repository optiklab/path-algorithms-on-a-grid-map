#pragma once
#ifndef __NotifyVisiblePartChanged_h__
#define __NotifyVisiblePartChanged_h__

#include "common.h"
#include "VisiblePartObserver.h"

class NotifyVisiblePartChanged
{
public:
	virtual ~NotifyVisiblePartChanged()
	{
	}

	virtual void Listen(VisiblePartObserver* observer) = 0;
	virtual void Detach(VisiblePartObserver* observer) = 0;
	//virtual void Listen(shared_ptr<VisiblePartObserver> observer) = 0;
	//virtual void Detach(shared_ptr<VisiblePartObserver> observer) = 0;
	virtual void Notify() = 0;
};

#endif __NotifyVisiblePartChanged_h__