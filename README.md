# Path algorithms on a grid map

## Disclarimer

The project is a bit dirty and unfinished, since it was created for learning and testing some ideas.

The project is supported by the article [Exploring path finding algorithms](https://github.com/optiklab/path-algorithms-on-a-grid-map/wiki).

An implementation of a classic approach to a problem of finding path on a grid that represents maps. Using algorithms like BFS, Dijkstra's and A-Star I'm finding paths on imaginary maps of various scale: from a squared map 5 x 5 to maps that represente more realistic data on a square of 1000 x 1000 (see screenshots for examples).

## The story behind

After reading a while on the topic, I have found that the problem of finding routes in a grid were in focus for quite a while between 2010 and 2020. Many pro-game developers have found nice optimizations and created new algorithms in this area.

I have found it interesting to play with it and build a little application that finds routes on a grid map using well-known Graph algorithms and SFML graphics library to visualize my findings. Have fun!

## Dependencies

The project is written in C++ with use of STL, SFML for graphics. All dependencies already located in the '/dependencies' folder.
Project file is built using Visual Studio 2022 and Microsoft Windows.
I didn't check the project on any other platforms except Windows (sorry).

## How to run

By default, config file located at '/data/appConfig.json' selects proper test case with some map. So, you basically need to open the project using VS and press F5. Then just press SPACE multiple times. As a result you should get something like this:

![1](https://github.com/optiklab/optiklab.github.io/blob/master/img/yandex-rover/picture_0.jpg)

While it might be hard to see on picture due to large scale, but there is shortest path between two points on the map. You can play with configuration and tests cases (described below), so you will get other results:

![2](https://github.com/optiklab/optiklab.github.io/blob/master/img/yandex-rover/picture_1.jpg)

![3](https://github.com/optiklab/optiklab.github.io/blob/master/img/yandex-rover/picture_2.jpg)

![4](https://github.com/optiklab/optiklab.github.io/blob/master/img/yandex-rover/picture_3.jpg)

To play with different test maps you need to point "map" configuration parameter to the right test data file located at '/data/maps'. Format of files described below and it does contain everything it needs to run the application.

It's hard to put comments in to JSON configuration file, so you can find some details about test cases right in the main.cpp comments or here in the readme.

## Original task description
```bash
N - town size NxN (N <= 2000).
MaxTips - max possible revenue for 1 order (MaxTips <= 50 000).
CostC - cost of building robot i.e. robot price (CostC <= 1 000 000 000).
T - Total iterations (T <= 100 000) each 60 robot steps (seconds/max possible delivery time).
D - Total number of expected orders (D <= 10 000 000).
k - Number of new orders for iteration
Srow, Scol - coordinates of location to take the order
Frow, Fcol - coordinates of location to drop the order
```

### Input example:
```bash
N MaxTips CostC
###
..#
..#
T D
k
Srow, Scol, Frow, Fcol
Srow, Scol, Frow, Fcol
...
k
Srow, Scol, Frow, Fcol
...
```

### Output example:

R - number of robots I put in town (1 <= R <= 100).

## Success:

Tips you get on each iteration = max(0, MaxTips - DeliveryTime)
Total score = TotalTips - R*CostC

## Current task description

I decided to keep the original format of test files located in 'data/maps' that actually contain the description of a map itself and coordinates of orders to pickup and dropoff. I decided NOT to deal with Cost Efficiency analysis and NOT to calculate the amount of rovers needed. Instead, this little application focuses on map renderring and quick find of the path between two coordinates. There are plenty of space for improvements, so potentially I may end up adding some features as ideas come to my mind.

## What this code base does

Well, I'm not interested in playing with orders or total costs, especially because I don't have a Test System with examples of good answers for all provided test cases.

However, I was still interested in building paths using various algorithms on Grid that represents a map (would it be a map for game, or anything else).

So, I have collected all the tests cases available, added some more information to make it more fun, and implemented everything from scratch.

I used SFML for graphics.

## Test cases:

Simple test cases to start from:
```bash
"../../data/maps/test_08_low_res_simple_map"; // Low resolution simple map: one block
"../../data/maps/test_10"; // Low resolution simple map: three blocks
"../../data/maps/test_07_partially_blocked_map"; // Corner case: no way out here for rover...
"../../data/maps/test_13"; // Bigger resolution (no blocks)
"../../data/maps/test_14"; // Even bigger resolution (no blocks)
"../../data/maps/test_16"; // Even more bigger resolution (no blocks)
"../../data/maps/test_12_low_res_complex_map"; // Low resolution complex map	
```

Fun xmas tree as the map
```bash	
"../../data/maps/test_25_xmax";
"../../data/maps/test_15";
"../../data/maps/test_17_yandex_high_res"; // High resolution
"../../data/maps/test_18_yandex_super_high_res"; // Super high resolution
"../../data/maps/test_19_yandex_super_max_res"; // Super Max high resolution
"../../data/maps/test_20_yandex_real_map"; // This is real map
"../../data/maps/test_21_yandex_real_map";
"../../data/maps/test_22_yandex_real_map";
"../../data/maps/test_23_yandex_real_map";
"../../data/maps/test_24_yandex_real_map";
```

More like real maps:
```bash	
"../../data/maps/test_15";
"../../data/maps/test_17_yandex_high_res"; // High resolution
"../../data/maps/test_18_yandex_super_high_res"; // Super high resolution
"../../data/maps/test_19_yandex_super_max_res"; // Super Max high resolution
"../../data/maps/test_20_yandex_real_map"; // This is real map
"../../data/maps/test_21_yandex_real_map";
"../../data/maps/test_22_yandex_real_map";
"../../data/maps/test_23_yandex_real_map";
"../../data/maps/test_24_yandex_real_map";
```

Tests cases that were not part of original task, to make it more complex and fun. Those are low resolution complex map with non-binary transparency: we have not only Block and Road, but also Grass, Gravel, etc. 
So, our Graph will have WEIGHTS, which means BFS will not work, and we will use either Dijkstra, A-Star.
```bash	
"../../data/maps/test_26_weighten_graph_map";
"../../data/maps/test_27_yandex_weighten"; // Medium-resolution
"../../data/maps/test_28_yandex_weighten_high_res"; // High-resolution
"../../data/maps/test_29_yandex_weighten_real_map"; // Real map
```

Finally, low resolution complex map with non-binary transparency: we have Blocks, Roads, Grass/Gravel AAAND negative-weighten Edges/Cells - let's call it Water.
So, BFS will not work, neither Dijkstra or A-Star does... so we use Bellman Ford.
This requires us to implement a little trick. Since every edge of the GRID is dually-connected, we never get DAG - instead, it will be always cyclic.
This will make it impossible to run any Path finding algorithm on it, since it will contain negative cycles (non Floyd-Warshall, non Bellman-Ford handle that as is).
So, what we do here, is making specifically negative cells Single-Directed by removing second edge.
```bash	
"../../data/maps/test_30_multi_weight_small";
"../../data/maps/test_31_multi_weight_graph_map";
```

## Author

Copyright (C) 2023 Anton "optiklab" Yarkov

https://github.com/optiklab/path-algorithms-on-a-grid-map-for-yandex-rover

See LICENSE file in the repo.
