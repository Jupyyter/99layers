#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class Background : public Sprite {
public:
    Background(float x, float y, float width, float height, const std::string& texturePath);
    virtual ~Background() = default;

    // Override Entity virtual methods
    void update(float deltaTime, const sf::Vector2u& windowSize) override;
    void draw(sf::RenderWindow& window) const override;
    bool isOnScreen() const override;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float originalWidth;
    float originalHeight;
    
    void initializeSprite(float width, float height);
};