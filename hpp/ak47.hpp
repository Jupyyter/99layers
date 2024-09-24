#pragma once

class AK47 : public Sprite {
public:
    AK47(GameMap& gamemap);
    AK47();
    void update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) override;

private:
       sf::FloatRect *playerBounds;
    void updatePosition();
    static constexpr float PI = 3.14159265358979323846f;
};
