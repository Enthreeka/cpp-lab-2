#include "RClass.h"

float createMatrix(const std::vector<std::vector<float>> &matrix)
{
	return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
		matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
		matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

sf::Color interpolateColors(const sf::Color &color1, const sf::Color &color2, float t)
{
	float r = color1.r + (color2.r - color1.r) * t;
	float g = color1.g + (color2.g - color1.g) * t;
	float b = color1.b + (color2.b - color1.b) * t;
	float a = color1.a + (color2.a - color1.a) * t;

	return sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b),
					 static_cast<sf::Uint8>(a));
}

void RClass::Create(const sf::Vector2u &size, const int selectedNormalIndex)
{
	image.create(size.x, size.y, sf::Color::Cyan);
	texture.loadFromImage(image);
	setTexture(texture);

	fColor = sf::Color::Black;
	sColor = sf::Color::White;

	index = selectedNormalIndex;
}

void RClass::DrawRFunc(const std::function<float(const sf::Vector2f &)> &rfunc, const sf::FloatRect &subSpace)
{
	sf::Vector2f spaceStep = {subSpace.width / static_cast<float>(image.getSize().x),
							  subSpace.height / static_cast<float>(image.getSize().y)};

	for (int x = 0; x < image.getSize().x - 1; ++x)
	{
		for (int y = 0; y < image.getSize().y - 1; ++y)
		{
			sf::Vector2f spacePoint1 = {subSpace.left + static_cast<float>(x) * spaceStep.x,
										subSpace.top + static_cast<float>(y) * spaceStep.y};

			const float z1 = rfunc(spacePoint1);

			sf::Vector2f spacePoint2 = {subSpace.left + static_cast<float>(x + 1) * spaceStep.x,
										subSpace.top + static_cast<float>(y) * spaceStep.y};

			const float z2 = rfunc(spacePoint2);

			sf::Vector2f spacePoint3 = {subSpace.left + static_cast<float>(x) * spaceStep.x,
										subSpace.top + static_cast<float>(y + 1) * spaceStep.y};

			const float z3 = rfunc(spacePoint3);

			const float A = createMatrix({
				{spacePoint1.y, z1, 1},
				{spacePoint2.y, z2, 1},
				{spacePoint3.y, z3, 1},
			});

			const float B = createMatrix({
				{spacePoint1.x, z1, 1},
				{spacePoint2.x, z2, 1},
				{spacePoint3.x, z3, 1},
			});

			const float C = createMatrix({
				{spacePoint1.x, spacePoint1.y, 1},
				{spacePoint2.x, spacePoint2.y, 1},
				{spacePoint3.x, spacePoint3.y, 1},
			});

			const float D = createMatrix({
				{spacePoint1.x, spacePoint1.y, z1},
				{spacePoint2.x, spacePoint2.y, z2},
				{spacePoint3.x, spacePoint3.y, z3},
			});

			const float lenPv = std::sqrt(A * A + B * B + C * C + D * D);

			float nx = A / lenPv;
			float ny = B / lenPv;
			float nz = C / lenPv;
			float nw = D / lenPv;

			float selectedNormal = nx;

			switch (index)
			{
			case 0:
				break;
			case 1:
				selectedNormal = ny;
				break;
			case 2:
				selectedNormal = nz;
				break;
			case 3:
				selectedNormal = nw;
				break;
			}

			auto pixelColor = interpolateColors(fColor, sColor, (1.f + selectedNormal) / 2);
			image.setPixel(x, y, pixelColor);
		}
	}

	texture.update(image);
}

void RClass::UpdatePalette(const sf::Color &firstColor, const sf::Color &secondColor)
{
	for (int x = 0; x < image.getSize().x - 1; ++x)
	{
		for (int y = 0; y < image.getSize().y - 1; ++y)
		{
			float t = (static_cast<float>(image.getPixel(x, y).r) - fColor.r) / (sColor.r - fColor.r);
			auto pixelColor = interpolateColors(firstColor, secondColor, t);
			image.setPixel(x, y, pixelColor);
		}
	}

	fColor = firstColor;
	sColor = secondColor;
	texture.update(image);
}

void RClass::SaveImageToFile(const std::string &filename) { image.saveToFile(filename); }
