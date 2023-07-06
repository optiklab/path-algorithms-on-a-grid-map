#ifndef COMMON_H
#define COMMON_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cmath>
#include <fstream>

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

const int DEFAULT_VERTICAL_CELLS = 20;
const int DEFAULT_HORIZONTAL_CELLS = 20;
const int DEFAULT_SQUARE_TILE_SIZE = 30;

const char FREE_CELL = '.';  // Normal moveables (road) 1-weight grid/map cell.
const char BLOCK_CELL = '#'; // Non-moveable, 0-weight cell.
const char GRASS_CELL = 'O'; // Imitate positive-weight cells.
const char WATER_CELL = 'W'; // Imitate negative-weight cells.


const int INF = 1e6;
const int NEG_INF = -1e6;
const string DEFAULT_ROUTE = "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";

// Configuration structure
struct AppConfig
{
    string font;
    string map;
    bool shadow = false;
};

#endif
