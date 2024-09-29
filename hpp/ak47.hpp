#pragma once

class AK47 : public Sprite {
public:
    AK47(GameMap& gamemap);
    AK47();
    void update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) override;

private:
    Player *playerRef;
    void updatePosition();
    static constexpr float PI = 3.14159265358979323846f;
    
    sf::Clock shootCooldown;
    static constexpr float shootCooldownTime = 0.1f; // Adjust this value to change the fire rate
};