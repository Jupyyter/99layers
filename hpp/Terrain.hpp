#pragma once
#include "sprite.hpp"

class Terrain : public Sprite, public CollisionDetector
{
public:
    Terrain(int x, int y, int w, int h, const std::string& tname);
    ~Terrain() = default;

    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow &window) const override;
    static std::vector<PropertyDescriptor> getPropertyDescriptors();
void onCollision(Sprite* other) override;
    float rotation=0.0f;
    float what=0.0f;
    bool isFalling;
    float verticalVelocity;
    const float gravity = 981.0f; // pixels per second squared
    const float maxFallSpeed = 1000.0f; // maximum fall speed
};