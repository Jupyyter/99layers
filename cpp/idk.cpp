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
void Idk::onCollision(Entity *other)
{
    if (typeid(*other) == typeid(Object))
    {
        setPosition(position);
        switch (CollisionDetector::CollisionSide(getBounds(), other->getBounds()))
        {
        case CollisionInfo::Left:
            if (!(other->getBounds().top > getBounds().top && other->getBounds().top - getBounds().top > 27 && velocity.y >= 0)) // in case of stairs
            {
                position.x = other->getBounds().left + other->getBounds().width;
                velocity.x = 0;
            }
            break;
        case CollisionInfo::Right:
            if (!(other->getBounds().top > getBounds().top && other->getBounds().top - getBounds().top > 27 && velocity.y >= 0))
            {
                position.x = other->getBounds().left - getBounds().width;
                velocity.x = 0;
            }
            break;
        case CollisionInfo::Bottom:
            if (velocity.y >= 0)
            {
                position.y = other->getBounds().top - getBounds().height;
                velocity.y = 0;
            }
            break;
        case CollisionInfo::Top:
        {
            position.y = other->getBounds().top + other->getBounds().height;
            velocity.y = 0;

            break;
        }
        default:
            break;
        }
        setPosition(position);
    }
}
void Idk::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres)
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

        // Update animation
        Animation::update(deltaTime, gamemap, screenres);
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
