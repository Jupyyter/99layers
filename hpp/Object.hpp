#pragma once
#include "sprite.hpp"

class Object : public Sprite
{
public:
    Object(int x, int y, int w, int h, const std::string& tname);
    ~Object() = default;

    void draw(sf::RenderWindow &window) const override;

    sf::RectangleShape rect;
    std::string texid;

private:
    void initializeObject(int x, int y, int w, int h, const std::string& tname);
};