#include "../hpp/libs.hpp"
Object::Object(int x, int y, int w, int h, const std::string& tname) : rect(sf::Vector2f(w, h))
{
    priorityLayer=-1;
    if (!texture.loadFromFile(tname)) {
        std::cerr << "Failed to load texture: " << tname << std::endl;
        // Set a default color if texture loading fails
        rect.setFillColor(sf::Color::Red);
    } else {
        sprite.setTexture(texture);
        // Set texture to repeat
        texture.setRepeated(true);
        // Set the texture rect to tile the texture if necessary
        sprite.setTextureRect(sf::IntRect(0, 0, w, h));
    }
    
    sprite.setPosition(x, y);
    
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color::Transparent);
    rect.setSize(sf::Vector2f(w,h));

    texid = tname;
}

void Object::draw(sf::RenderWindow &window) const
{
    window.draw(rect);
    window.draw(sprite);
}
