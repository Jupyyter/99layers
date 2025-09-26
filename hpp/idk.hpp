#pragma once
class Idk :public Animation, public CollisionDetector {
public:
    Idk(sf::Vector2f position);
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    bool isOutOfBounds(const sf::Vector2u& windowSize) const;

private:
    void loadSprite();
    void onCollision(Sprite* other) override;

    sf::RectangleShape shape;
    float moveSpeed;
    float moveDistance;
    float initialX;
    float gravity;
    bool movingRight;
};