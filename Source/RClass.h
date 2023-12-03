#include <SFML/Graphics.hpp>
#include <functional>

class RClass : public sf::Sprite
{
public:
	void Create(const sf::Vector2u &size, const int selectedNormalIndex);
	void DrawRFunc(const std::function<float(const sf::Vector2f &)> &rfunc, const sf::FloatRect &subSpace);
	void UpdatePalette(const sf::Color &firstColor, const sf::Color &secondColor);
	void SaveImageToFile(const std::string &filename);

private:
	sf::Color fColor;
	sf::Color sColor;
	sf::Texture texture;
	sf::Image image;
	int index;
};