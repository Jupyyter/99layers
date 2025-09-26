#pragma once
class bloodParticles:public Sprite  {
public:
bloodParticles(sf::Vector2f position);
void update(float deltaTime, const sf::Vector2u& windowSize) override;
void draw(sf::RenderWindow& window) const override;
private:
    void spawnParticles(sf::Vector2f position);
};