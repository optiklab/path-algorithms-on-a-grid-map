#ifndef __Screen_h__
#define __Screen_h__

#include "common.h"
#include "map/rectangularmap.h"

void drawStart(RenderWindow& renderWindow, sf::Font font)
{
	renderWindow.clear(Color::Black);

	float xpos = SCREEN_WIDTH / 2;
	float ypos = SCREEN_HEIGHT / 2;

	sf::Text text("Press SPACE", font);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::Red);
	text.setPosition(xpos, ypos);
	renderWindow.draw(text);

	renderWindow.display();
}

void drawLoading(RenderWindow& renderWindow, sf::Font font)
{
	renderWindow.clear(Color::Black);

	float xpos = SCREEN_WIDTH / 2;
	float ypos = SCREEN_HEIGHT / 2;

	sf::Text text("Loading...", font);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::Red);
	text.setPosition(xpos, ypos);
	renderWindow.draw(text);

	renderWindow.display();
}

void drawProcessing(RenderWindow& renderWindow, sf::Font font)
{
	renderWindow.clear(Color::Black);

	float xpos = SCREEN_WIDTH / 2;
	float ypos = SCREEN_HEIGHT / 2;

	sf::Text text("Processing...", font);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::Red);
	text.setPosition(xpos, ypos);
	renderWindow.draw(text);

	renderWindow.display();
}

void drawHelpText(RenderWindow& renderWindow, std::shared_ptr<MapBase> map, sf::Font font)
{
	float xmin, ymin, xmax, ymax;
	std::tie(xmin, ymin, xmax, ymax) = map->GetCoordinateBounds();

	float xpos = DEFAULT_SQUARE_TILE_SIZE / 2 + xmax * DEFAULT_SQUARE_TILE_SIZE;
	float ypos = ymin * DEFAULT_SQUARE_TILE_SIZE;

	sf::Text text("To apply", font);
	text.setCharacterSize(18);
	text.setFillColor(sf::Color::White);
	text.setPosition(xpos, ypos);
	renderWindow.draw(text);

	sf::Text text2("press Space", font);
	text2.setCharacterSize(18);
	text2.setFillColor(sf::Color::White);
	text2.setPosition(xpos, ypos + DEFAULT_SQUARE_TILE_SIZE);
	renderWindow.draw(text2);

	sf::Text text3("To scale", font);
	text3.setCharacterSize(18);
	text3.setFillColor(sf::Color::White);
	text3.setPosition(xpos, ypos + 2 * DEFAULT_SQUARE_TILE_SIZE);
	renderWindow.draw(text3);

	sf::Text text4("press PgDwn, PgUp", font);
	text4.setCharacterSize(18);
	text4.setFillColor(sf::Color::White);
	text4.setPosition(xpos, ypos + 3 * DEFAULT_SQUARE_TILE_SIZE);
	renderWindow.draw(text4);

	sf::Text text5("To select", font);
	text5.setCharacterSize(18);
	text5.setFillColor(sf::Color::White);
	text5.setPosition(xpos, ypos + 4 * DEFAULT_SQUARE_TILE_SIZE);
	renderWindow.draw(text5);

	sf::Text text6("press Up, Down, Left, Right", font);
	text6.setCharacterSize(18);
	text6.setFillColor(sf::Color::White);
	text6.setPosition(xpos, ypos + 5 * DEFAULT_SQUARE_TILE_SIZE);
	renderWindow.draw(text6);

	sf::Texture robotT;
	if (robotT.loadFromFile("../../data/walle.png"))
	{
		sf::Sprite robotS;
		robotS.setTexture(robotT);
		robotS.setPosition(xpos, ypos + 7 * DEFAULT_SQUARE_TILE_SIZE);
		renderWindow.draw(robotS);
	}
}

void draw(RenderWindow& renderWindow, std::shared_ptr<MapBase> map, sf::Font font)
{
	renderWindow.clear(Color::Black);
	drawHelpText(renderWindow, map, font);
	map->Draw();
}


#endif __Screen_h__