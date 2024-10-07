#pragma once
class Idk :public Animation, public CollisionDetector {
public:
    Idk(sf::Vector2f position, float moveDistance);
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    bool isOutOfBounds(const sf::Vector2u& windowSize) const;

private:
    void loadSprite();
    void onCollision(Entity* other) override;

    sf::RectangleShape shape;
    float moveSpeed;
    float moveDistance;
    float initialX;
    float gravity;
    bool movingRight;
};