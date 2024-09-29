#pragma once

class Boss : public Sprite
{
public:
    Boss(const sf::Vector2f &initialPosition);
    void update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) override;
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

    void loadAndScaleImage();
    void updateEyePosition();
    void onCollision(Entity *other) override;
};