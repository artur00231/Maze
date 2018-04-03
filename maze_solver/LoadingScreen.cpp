#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
	loadResource();
}

void LoadingScreen::drawLoadingScreen(sf::RenderTarget & target)
{
	target.clear(sf::Color::White);
	target.draw(text);
	drawLoadingCircle(target);
}

void LoadingScreen::setString(const sf::String & string)
{
	text.setString(string);

	sf::Rect<float> text_size = text.getLocalBounds();
	text.setOrigin(text_size.width / 2.0f, text_size.height / 2.0f);

	sf::Vector2f screen{ 1280.0f, 720.0f };

	text.setPosition(screen.x / 2.0f, screen.y / 3.0f);
}

void LoadingScreen::loadResource()
{
	font.loadFromFile("fonts/calibri.ttf");

	text.setFont(font);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(50);
	text.setString("Loading...");
	
	sf::Rect<float> text_size = text.getLocalBounds();
	text.setOrigin(text_size.width / 2.0f, text_size.height / 2.0f);

	sf::Vector2f screen{ 1280.0f, 720.0f };

	text.setPosition(screen.x / 2.0f, screen.y / 3.0f);
}

void LoadingScreen::drawLoadingCircle(sf::RenderTarget & target)
{
	constexpr std::size_t steps = 60 * 4;
	sf::Vector2f screen{ 1280.0f, 720.0f };
	constexpr std::size_t circles = 8;
	sf::Vector2f center{ screen.x / 2.0f,  screen.y / 3.0f * 2.0f };
	constexpr float circle_radius = 10, loading_circle_radius = 35;
	constexpr float angle_between_circles = 360.0f / circles;
	constexpr float angle_step = 360.0f / steps;

	sf::CircleShape circle;
	circle.setRadius(circle_radius);

	circle.setOrigin(circle_radius, circle_radius + loading_circle_radius);
	circle.setPosition(center);

	step++;

	for (size_t i = 0; i < circles; i++)
	{
		circle.setRotation(angle_between_circles * i + angle_step * step);

		float alpha = 255 * ((i + 1) / static_cast<float>(circles));

		circle.setFillColor(sf::Color{ 0, 0, 0, static_cast<sf::Uint8>(alpha) });

		target.draw(circle);

		if (step > steps - 1)
		{
			step = 0;
		}
	}
}