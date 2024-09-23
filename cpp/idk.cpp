#include "../hpp/libs.hpp"
Idk::Idk(sf::Vector2f position, float moveDistance)
    : Animation(), CollisionDetector(), moveSpeed(100.0f), moveDistance(moveDistance), initialX(position.x), gravity(980.0f), movingRight(true)
{
    setPosition(position);
    loadSprite();
}

void Idk::loadSprite()
{
    loadSpritesheet("../imgs/arrow.png", 32, 32);
    addAnimation("default", 0, 1);
    setAnimation("default");
}
void Idk::update(float deltaTime, GameMap& gamemap, const sf::Vector2u& screenres)
{
    if (isOnScreen(gamemap.getPartBounds()))
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
        setPosition(position);
        manageCollisions(gamemap.getObjectBounds());

        // Update animation
        Animation::update(deltaTime,gamemap,screenres);
    }
}

void Idk::draw(sf::RenderWindow &window)const 
{
    Animation::draw(window);
}
bool Idk::isOutOfBounds(const sf::Vector2u &windowSize) const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds.top > windowSize.y || bounds.left + bounds.width < 0 || bounds.left > windowSize.x;
}

void Idk::manageCollisions(const std::vector<sf::FloatRect> &objectBounds)
{
    sf::FloatRect platformBounds = sprite.getGlobalBounds();
    for (const auto &obstacle : objectBounds)
    {
        CollisionInfo collision = checkCollision(platformBounds, {obstacle});
        if (collision.collided)
        {
            switch (collision.side)
            {
            case CollisionSide::Left:
            case CollisionSide::Right:
                velocity.x = -velocity.x; // Reverse horizontal direction
                movingRight = !movingRight;
                break;
            case CollisionSide::Bottom:
                position.y = obstacle.top - platformBounds.height;
                velocity.y = 0;
                break;
            case CollisionSide::Top:
                position.y = obstacle.top + obstacle.height;
                velocity.y = 0;
                break;
            default:
                break;
            }
            setPosition(position);
        }
    }
}