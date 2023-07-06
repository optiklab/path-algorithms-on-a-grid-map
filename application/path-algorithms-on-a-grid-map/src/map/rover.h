#pragma once
#ifndef __Rover_h__
#define __Rover_h__

#include "cellbase.h"

class Rover
{
public:
    int PositionX = 0;
    int PositionY = 0;

    string route = DEFAULT_ROUTE;

    vector<int> dist;
    vector<int> path;
    vector<bool> visited;
    vector<int> pathToTake;
    vector<int> pathToPut;
};

#endif __Rover_h__