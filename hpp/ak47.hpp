#pragma once

class AK47 : public Sprite {
public:
    AK47();
    void update(float deltaTime,  const sf::Vector2u &screenres) override;

private:
    Player *playerRef;
    void updatePosition();
    static constexpr float PI = 3.14159265358979323846f;
    
    sf::Clock shootCooldown;
    static constexpr float shootCooldownTime = 0.07f; // Adjust this value to change the fire rate
};