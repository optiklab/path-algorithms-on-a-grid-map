#include "common.h"
#include "screen.h"
#include "map/navigator.h"
#include "map/rectangularmap.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

//g++ -std=c++17 "main.cpp" -o main
//g++ -std=c++20 "main.cpp" -o main

// Load configuration from a JSON file
AppConfig loadConfiguration(const std::string& filename) 
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open the configuration file.");
	}

	json jsonData;
	file >> jsonData;
	file.close();

	AppConfig config;
	config.font = jsonData["font"].get<string>();
	config.map = jsonData["map"].get<string>();
	config.shadow = jsonData["shadow"].get<bool>();

	return config;
}

int main(int argc, char** argv)
{
    // Initialize default window
	RenderWindow window(
		VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
		"Map");

	// Load the configuration from the JSON file
	std::string configFilename = "../../data/appConfig.json";
	AppConfig config = loadConfiguration(configFilename);

	sf::Font font;
	font.loadFromFile(config.font);
	string filepath = config.map;
	bool shadowed = config.shadow;

	// Tests:
	//string filepath = "../../data/maps/test_08_low_res_simple_map"; // Low resolution simple map: one block
	//string filepath = "../../data/maps/test_10"; // Low resolution simple map: three blocks
	//string filepath = "../../data/maps/test_07_partially_blocked_map"; // Corner case: no way out here for rover...
	//string filepath = "../../data/maps/test_13"; // Bigger resolution (no blocks)
	//string filepath = "../../data/maps/test_14"; // Even bigger resolution (no blocks)
	//string filepath = "../../data/maps/test_16"; // Even more bigger resolution (no blocks)
	//string filepath = "../../data/maps/test_12_low_res_complex_map"; // Low resolution complex map	
	//string filepath = "../../data/maps/test_25_xmax";
	//string filepath = "../../data/maps/test_15";
	//string filepath = "../../data/maps/test_17_yandex_high_res"; // High resolution
	//string filepath = "../../data/maps/test_18_yandex_super_high_res"; // Super high resolution
	//string filepath = "../../data/maps/test_19_yandex_super_max_res"; // Super Max high resolution
	//string filepath = "../../data/maps/test_20_yandex_real_map"; // This is real map
	//string filepath = "../../data/maps/test_21_yandex_real_map";
	//string filepath = "../../data/maps/test_22_yandex_real_map";
	//string filepath = "../../data/maps/test_23_yandex_real_map";
	//string filepath = "../../data/maps/test_24_yandex_real_map";
	
    // Low resolution complex map with non-binary transparency: we have not only Block and Road, but also Grass, Gravel, etc. 
	// So, our Graph will have WEIGHTS, which means BFS will not work, and we will use either Dijkstra, A-Star.
	//string filepath = "../../data/maps/test_26_weighten_graph_map";

	//string filepath = "../../data/maps/test_27_yandex_weighten"; // Medium-resolution
	//string filepath = "../../data/maps/test_28_yandex_weighten_high_res"; // High-resolution
	//string filepath = "../../data/maps/test_29_yandex_weighten_real_map"; // Real map
	
	// Low resolution complex map with non-binary transparency: we have Blocks, Roads, Grass/Gravel AAAND negative-weighten Edges/Cells - let's call it Water.
	// So, BFS will not work, neither Dijkstra or A-Star does... so we use Bellman Ford.
	// This requires us to implement a little trick. Since every edge of the GRID is dually-connected, we never get DAG - instead, it will be always cyclic.
	// This will make it impossible to run any Path finding algorithm on it, since it will contain negative cycles (non Floyd-Warshall, non Bellman-Ford handle that as is).
	// So, what we do here, is making specifically negative cells Single-Directed by removing second edge.
	//string filepath = "../../data/maps/test_30_multi_weight_small";
	//string filepath = "../../data/maps/test_31_multi_weight_graph_map";

	std::shared_ptr<Focus> focus = std::make_shared<Focus>(0, 0, DEFAULT_HORIZONTAL_CELLS, DEFAULT_VERTICAL_CELLS);
	std::shared_ptr<MapBase> map = std::make_shared<RectangularMap>(DEFAULT_HORIZONTAL_CELLS, DEFAULT_VERTICAL_CELLS, focus, window);
	std::shared_ptr<Navigator> navigator = std::make_shared<Navigator>();

	// Redraw.
	window.clear(Color::Black);

	std::cout << "Initialising graph..." << std::endl;
	map->InitialiseGraph();

	window.clear(Color::Black);

	drawStart(window, font);

	bool skipReRendering = false; // Works for quite large maps.

	while (window.isOpen())
	{
		// Handle key events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Space)
				{
					if (navigator->IsReady())
					{
						std::cout << "Building route started..." << std::endl;
						navigator->Navigate();
						std::cout << "Building route end..." << std::endl;
					}
					else
					{
						if (map->IsReady()) // Do it only once (for now).
						{
							skipReRendering = true;
							std::cout << "Load tasks from file..." << std::endl;
							if (navigator->LoadTasks(filepath))
							{
								navigator->SetMap(map);
							}
							std::cout << "Load tasks end..." << std::endl;
						}
						else
						{
							drawLoading(window, font);

							// When nothing is ready yet, we simply load map from file.
							std::cout << "Load map start..." << std::endl;
							if (!map->LoadMap(filepath, shadowed))
							{
								return 0;
							}
							std::cout << "Load map end..." << std::endl;

							drawProcessing(window, font);
						}
					}
				}
				else if (event.key.code == Keyboard::PageUp)
				{
					std::cout << "FocusOut..." << std::endl;
					focus->FocusOut();
				}
				else if (event.key.code == Keyboard::PageDown)
				{
					std::cout << "FocusIn..." << std::endl;
					focus->FocusIn();
				}
				else if (event.key.code == Keyboard::Up)
				{
					std::cout << "MoveUp..." << std::endl;
					focus->MoveUp();
				}
				else if (event.key.code == Keyboard::Down)
				{
					std::cout << "MoveDown..." << std::endl;
					focus->MoveDown();
				}
				else if (event.key.code == Keyboard::Left)
				{
					std::cout << "MoveLeft..." << std::endl;
					focus->MoveLeft();
				}
				else if (event.key.code == Keyboard::Right)
				{
					std::cout << "MoveRight..." << std::endl;
					focus->MoveRight();
				}

				std::cout << "Drawing map data started..." << std::endl;

				if (!skipReRendering)
					draw(window, map, font);

				skipReRendering = false;

				std::cout << "Drawing map data end..." << std::endl;

				// Uncomment to generate SVG. Do not run for large (real) maps (SVG will be huge and not opened).
				//string outputprefix = "result";
				//map->PrintMapSVG(outputprefix);
			}
		}
	}

    return 0;
}