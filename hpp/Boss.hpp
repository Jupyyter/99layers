#pragma once

class Boss : public Sprite {
public:
    Boss(const sf::Vector2f& initialPosition);
    void update(float deltaTime, GameMap& gamemap, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) override;
    void resetTimers();

private:
    sf::Clock atimer;
    sf::Clock ptimer;
    sf::Clock ltimer;
    sf::Clock ttimer;

    sf::Sprite eyeSprite;
    sf::Texture eyeTexture;

    void loadAndScaleImage();
    void updateEyePosition();
};