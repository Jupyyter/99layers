#include "../hpp/libs.hpp"
Idk::Idk(sf::Vector2f position)
    : Animation(position,0.1), CollisionDetector(), moveSpeed(100.0f), moveDistance(200), initialX(position.x), gravity(980.0f), movingRight(true)
{
    loadSprite();
}

void Idk::loadSprite()
{
    loadSpritesheet("../imgs/arrow.png", 32, 32);
    addAnimation("default", 0, 1);
    setAnimation("default");
}
void Idk::onCollision(Sprite *other)
{
}
void Idk::update(float deltaTime, const sf::Vector2u &screenres)
{
    if (isOnScreen())
    {
        velocity.y += gravity * deltaTime;
        if (movingRight)
        {
            velocity.x = moveSpeed;
            if (position.x >= initialX + moveDistance)
            {
                movingRight = false;
            }
        }
        else
        {
            velocity.x = -moveSpeed;
            if (position.x <= initialX)
            {
                movingRight = true;
            }
        }
        // Update position
        position += velocity * deltaTime;

        // Update animation
        Animation::update(deltaTime, screenres);
    }
}

void Idk::draw(sf::RenderWindow &window) const
{
    Animation::draw(window);
}
bool Idk::isOutOfBounds(const sf::Vector2u &windowSize) const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds.top > windowSize.y || bounds.left + bounds.width < 0 || bounds.left > windowSize.x;
}
