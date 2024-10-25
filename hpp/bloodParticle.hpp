#pragma once
class bloodParticle:public Animation ,public CollisionDetector {
public:
bloodParticle(sf::Vector2f position);
void update(float deltaTime, const sf::Vector2u& windowSize) override;
private:
    void spawnParticles(sf::Vector2f position);
    void onCollision(Sprite *other) override;
    void loadSprite();
};