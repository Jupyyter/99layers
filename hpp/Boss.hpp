#pragma once

class Boss : public Sprite, public CollisionDetector
{
public:
    Boss(const sf::Vector2f &initialPosition);
    void update(float deltaTime, const sf::Vector2u &screenres) override;
    void draw(sf::RenderWindow &window) const override;
    void resetTimers();
    int hp = 77;

private:
    sf::Clock atimer;
    sf::Clock ptimer;
    sf::Clock ltimer;
    sf::Clock ttimer;

    sf::Sprite eyeSprite;
    sf::Texture eyeTexture;
    bool hasAppearedOnScreen=false;

    void loadAndScaleImage();
    void updateEyePosition();
    void onCollision(Object *other) override;

    int currentPhase;
float maxHealth;
};