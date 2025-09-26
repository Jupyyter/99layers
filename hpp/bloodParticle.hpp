#pragma once
class bloodParticle:public Sprite ,public CollisionDetector {
public:
bloodParticle(sf::Vector2f position);
void update(float deltaTime, const sf::Vector2u& windowSize) override;
private:
    void spawnParticles(sf::Vector2f position);
    void collide(Sprite *other) override;
    void loadSprite();
};