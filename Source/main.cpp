#include <iostream>
#include <SFML/Graphics.hpp>
#include "imgui-SFML.h"
#include "imgui.h"
#include "RClass.h"


float RAnd(float w1, float w2)
{
	return w1 + w2 + std::sqrt((w1 * w1 + w2 * w2) - 2 * w1 * w2); 
}

float ROr(float w1, float w2)
{
	return w1 + w2 - std::sqrt((w1 * w1 + w2 * w2) - 2 * w1 * w2); 
}

std::vector<RClass *> sprites;
int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "cpp-lab-2");
	window.setFramerateLimit(60);
	if (!ImGui::SFML::Init(window))
	{
		std::cout << "ImGui initialization failed";
		return -1;
	}

	auto spriteSize = sf::Vector2u{window.getSize().x / 2, window.getSize().y / 2};

	RClass rClassNX;
	rClassNX.Create(spriteSize, 0);
	sprites.push_back(&rClassNX);

	RClass rClassNY;
	rClassNY.Create(spriteSize, 1);
	rClassNY.setPosition(spriteSize.x, 0);
	sprites.push_back(&rClassNY);

	RClass rClassNZ;
	rClassNZ.Create(spriteSize, 2);
	rClassNZ.setPosition(0, spriteSize.y);
	sprites.push_back(&rClassNZ);

	RClass rClassNW;
	rClassNW.Create(spriteSize, 3);
	rClassNW.setPosition(spriteSize.x, spriteSize.y);
	sprites.push_back(&rClassNW);

	std::function<float(const sf::Vector2f &)> rFunction[5];

	rFunction[0] = [](const sf::Vector2f &point) -> float { return std::sin(point.x) + std::cos(point.y); };
	rFunction[1] = [](const sf::Vector2f &point) -> float { return std::cos(point.x) * std::sin(point.y); };
	rFunction[2] = [](const sf::Vector2f &point) -> float { return std::cos(point.x + point.y); };
	rFunction[3] = [](const sf::Vector2f &point) -> float { return point.x * point.x + point.y * point.y - 200; };
	rFunction[4] = [](const sf::Vector2f &point) -> float { return std::sin(point.x) * std::cos(point.y); };

	std::function<float(const sf::Vector2f &)> complexFunction = [&rFunction](const sf::Vector2f &point) -> float
	{
		return RAnd(RAnd(ROr(RAnd(rFunction[0](point), rFunction[1](point)), rFunction[2](point)), rFunction[3](point)),
					ROr(rFunction[4](point), rFunction[0](point)));
	};

	sf::FloatRect subSpace(-10.f, -10.f, 20.f, 20.f);

	static ImVec4 firstColor(0, 0, 0, 1);
	static ImVec4 secondColor(1, 1, 1, 1);

	for (RClass *sprite : sprites)
	{
		sprite->DrawRFunc(complexFunction, subSpace);
	}

	sf::Clock deltaClock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Controls");

		if (ImGui::ColorEdit3("First color", &firstColor.x))
		{
		}
		if (ImGui::ColorEdit3("Second color", &secondColor.x))
		{
		}

		if (ImGui::Button("Update"))
		{
			auto sfFirstColor =
				sf::Color(static_cast<sf::Uint8>(firstColor.x * 255), static_cast<sf::Uint8>(firstColor.y * 255),
						  static_cast<sf::Uint8>(firstColor.z * 255), static_cast<sf::Uint8>(firstColor.w * 255));

			auto sfSecondColor =
				sf::Color(static_cast<sf::Uint8>(secondColor.x * 255), static_cast<sf::Uint8>(secondColor.y * 255),
						  static_cast<sf::Uint8>(secondColor.z * 255), static_cast<sf::Uint8>(secondColor.w * 255));

			for (RClass *sprite : sprites)
			{
				sprite->UpdatePalette(sfFirstColor, sfSecondColor);
			}
		}


		ImGui::Text("Press 'S' to save nx,ny,nz,nw images");

		if (event.key.code == sf::Keyboard::S)
		{
			rClassNX.SaveImageToFile("images/nx.png");
			rClassNY.SaveImageToFile("images/ny.png");
			rClassNZ.SaveImageToFile("images/nz.png");
			rClassNW.SaveImageToFile("images/nw.png");
		}
	
		ImGui::End();

		window.clear();

		window.draw(rClassNX);
		window.draw(rClassNY);
		window.draw(rClassNZ);
		window.draw(rClassNW);

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}
