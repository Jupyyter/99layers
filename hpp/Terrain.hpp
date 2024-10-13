#pragma once
#include "sprite.hpp"

class Terrain : public Sprite
{
public:
    Terrain(int x, int y, int w, int h, const std::string& tname);
    ~Terrain() = default;

    void draw(sf::RenderWindow &window) const override;

    sf::RectangleShape rect;
    std::string texid;

private:
    void initializeObject(int x, int y, int w, int h, const std::string& tname);
};