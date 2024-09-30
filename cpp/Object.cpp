#include "../hpp/libs.hpp"
Object::Object(int x, int y, int w, int h, std::string tname) : rect(sf::Vector2f(w, h))
{
    if (!tex.loadFromFile(tname))
    {
        std::cerr << "Failed to load texture: " << tname << std::endl;
    }
    tex.setRepeated(true);
    rect.setTexture(&tex);
    rect.setTextureRect(sf::IntRect(0, 0, w, h));
    rect.setPosition(x, y);

    size_t lastSlash = tname.find_last_of("/\\");
    size_t lastDot = tname.find_last_of(".");
    if (lastSlash == std::string::npos)
        lastSlash = 0;
    else
        lastSlash++;
    if (lastDot == std::string::npos || lastDot < lastSlash)
        lastDot = tname.length();
    texid = tname.substr(lastSlash, lastDot - lastSlash);
}

void Object::draw(sf::RenderWindow &window) const
{
    window.draw(rect);
}
