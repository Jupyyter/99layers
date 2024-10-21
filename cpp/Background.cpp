#include "../hpp/libs.hpp"
Background::Background(float x, float y, float width, float height, const std::string& texturePath)
    : Sprite(sf::Vector2f(x,y)), originalWidth(width), originalHeight(height)
{
    // Set object properties
    priorityLayer = -1000; // Ensure background is drawn behind other object
    
    // Load texture
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "\nFailed to load background texture: " << texturePath;
        return;
    }
    
    sprite.setTexture(texture);
    
    // Initialize sprite properties
    initializeSprite(width, height);
}

void Background::initializeSprite(float width, float height) {
    sf::Vector2u textureSize = texture.getSize();
    
    if (textureSize.x > 0 && textureSize.y > 0) {
        // Calculate scaling factors to maintain aspect ratio while filling the specified dimensions
        float scaleX = width / static_cast<float>(textureSize.x);
        float scaleY = height / static_cast<float>(textureSize.y);
        
        sprite.setScale(scaleX, scaleY);
    }
}

void Background::update(float deltaTime, const sf::Vector2u& windowSize) {
    //parallax
}

void Background::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Background::isOnScreen() const {
    // Backgrounds are always considered on screen
    return true;
}
