#include "mapbase.h"

MapBase::MapBase(RenderWindow& window, int width, int height) :
	_window(window),
	_width(width),
	_height(height),
	_scaleFactor(1),
	_visibleTopLeftX(0),
	_visibleTopLeftY(0),
	_visibleBottomRightX(_grid.size()),
	_visibleBottomRightY(_grid.size()),
	_isNegativeWeighten(false),
	_isWeighten(false)
{
	_verticesNumber = width * height;
}

MapBase::~MapBase()
{
}

/// <summary>
///  Creates a fully Graph representation of the map, that:
///  1. Avoids non - moveable cells to build the paths more efficiently(than in Grid).
///  2. Allows applying appropriate graph algorithms based on map data.
/// </summary>
void MapBase::InitialiseGraph()
{
	_adjacencyList.clear();
	_adjacencyList.resize(_verticesNumber);

	_edgesList.clear();
}

void MapBase::Draw() const
{
	// Uncomment to see the graph (will be the same, actually, as the usual picture).
	//Color graphColor = Color::Cyan;
	//for (int i = 0; i < _verticesNumber; ++i)
	//{
	//	for (const auto& cell : _adjacencyList[i])
	//	{
	//		cell->SetColor(graphColor);
	//	}
	//}

	float cellSize = _scaleFactor * (float)DEFAULT_SQUARE_TILE_SIZE;

	if (cellSize <= 5) // Use highly efficient pixel by pixel renderring.
	{
		sf::Uint8* pixels = new sf::Uint8[_width * _height * 4];

		for (int j = _visibleTopLeftY; j < _visibleBottomRightY; j++)
		{
			for (int i = _visibleTopLeftX; i < _visibleBottomRightX; i++)
			{
				int index = (_grid[j][i]->GetY() * _width + _grid[j][i]->GetX());

				sf::Color color = _grid[j][i]->GetColor();
				pixels[index * 4] = color.r;
				pixels[index * 4 + 1] = color.g;
				pixels[index * 4 + 2] = color.b;
				pixels[index * 4 + 3] = color.a;
			}
		}

		sf::Texture texture;
		texture.create(_width, _height);
		texture.update(pixels);
		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setScale(cellSize, cellSize);
		_window.draw(sprite);
	}
	else
	{
		for (int j = _visibleTopLeftY; j < _visibleBottomRightY; j++)
		{
			for (int i = _visibleTopLeftX; i < _visibleBottomRightX; i++)
			{
				_grid[j][i]->Draw(_window, _scaleFactor);
			}
		}
	}
}

void MapBase::PrintMapSVG(const std::string& filename) const
{
	std::ofstream svgfile(filename + ".svg");
	if (!svgfile)
	{
		std::cerr << "Error opening " << filename << ".svg for writing.\n";
		std::cerr << "Terminating.";
		exit(1);
	}
	double xmin, ymin, xmax, ymax;
	std::tie(xmin, ymin, xmax, ymax) = GetCoordinateBounds();
	int xresolution = (xmax - xmin + 2) * 30,
		yresolution = (ymax - ymin + 2) * 30;

	svgfile << "<svg width=\"" << xresolution << "\" height=\"" << yresolution
		<< "\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
	svgfile << "<g transform=\"translate(" << (1 - xmin) * 30 << ","
		<< yresolution - (1 - ymin) * 30 << ") scale(1,-1)\">" << std::endl;
	svgfile << "<rect x=\"" << (xmin - 1) * 30 << "\" y=\"" << (ymin - 1) * 30
		<< "\" width=\"" << xresolution << "\" height=\"" << yresolution
		<< "\" fill=\"white\"/>" << std::endl;

	for (int i = 0; i < _grid.size(); i++)
	{
		for (const auto& cell : _grid[i])
		{
			if (cell->GetSymbol() == '.') // Alternatively and better: to convert cell->Color -> into STRING
				svgfile << cell->SVGPrint("green") << "\n";
			else
				svgfile << cell->SVGPrint("black") << "\n";
		}
	}

	//for (int i = 0; i < _verticesNumber; ++i)
	//{
	//	for (const auto& cell : _adjacencyList[i])
	//	{
	//		svgfile << cell->SVGPrint("cyan") << "\n";
	//	}
	//}

	svgfile << "</g>" << std::endl;
	svgfile << "</svg>" << std::endl;
}

std::shared_ptr<Cell> MapBase::GetCellFromGrid(int row, int column) const
{
	return _grid[row][column];
}

std::shared_ptr<Cell> MapBase::GetFirstMoveableCell() const
{
	if (_moveableCells.size() > 0)
		return _moveableCells[0];

	return NULL;
}

void MapBase::UpdateVisiblePart(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	if (topLeftX >= 0 && topLeftX <= _grid.size() &&
		topLeftY >= 0 && topLeftY <= _grid.size() &&
		bottomRightX >= 0 && bottomRightX <= _grid.size() &&
		bottomRightY >= 0 && bottomRightY <= _grid.size())
	{
		_visibleTopLeftX = topLeftX;
		_visibleTopLeftY = topLeftY;
		_visibleBottomRightX = bottomRightX;
		_visibleBottomRightY = bottomRightY;
	}
}