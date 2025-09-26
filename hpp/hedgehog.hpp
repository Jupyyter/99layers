#pragma once

class hedgehog : public Animation, public CollisionDetector {
public:
    hedgehog(sf::Vector2f spawnPosition);
    ~hedgehog();
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;

private:
void onCollision(Sprite* other) override;

    void loadSprite();
};