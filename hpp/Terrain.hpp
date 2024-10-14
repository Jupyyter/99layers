#pragma once
#include "sprite.hpp"

class Terrain : public Sprite
{
public:
    Terrain(int x, int y, int w, int h, const std::string& tname);
    ~Terrain() = default;

    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow &window) const override;
    static std::vector<PropertyDescriptor> getPropertyDescriptors();

    float rotation=0.0f;
};