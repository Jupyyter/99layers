#include "../hpp/libs.hpp"

Terrain::Terrain(int x, int y, int w, int h, const std::string& tname) : Sprite(sf::Vector2f(x ,y)), rotation(0.0f), CollisionDetector()
{
    priorityLayer = -1;
    if (!texture.loadFromFile(tname)) {
        std::cerr << "Failed to load texture: " << tname << std::endl;
    } else {
        sprite.setTexture(texture);
        // Set texture to repeat
        texture.setRepeated(true);
        // Set the texture rect to tile the texture if necessary
        sprite.setTextureRect(sf::IntRect(0, 0, w, h));
        
        // Set the origin to the center of the sprite
        //sprite.setOrigin(w / 2.0f, h / 2.0f);
    // Adjust position to account for the new origin
    }
    
}

void Terrain::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}
void Terrain::onCollision(Entity *other)
{
}
void Terrain::update(float deltaTime, const sf::Vector2u& screenres)
{
    sprite.setRotation(sprite.getRotation() + rotation * deltaTime);
}

std::vector<PropertyDescriptor> Terrain::getPropertyDescriptors() {
    return {
        {"rotation", "0",
            [](Entity* e, const std::string& v) { static_cast<Terrain*>(e)->rotation = std::stof(v); },
            [](const Entity* e) { return std::to_string(static_cast<const Terrain*>(e)->rotation); }
        },
    };
}