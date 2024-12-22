#include "../hpp/libs.hpp"
hedgehog::hedgehog(sf::Vector2f spawnPosition)
    : Animation(spawnPosition, 0.1), CollisionDetector()
{
       position = spawnPosition;
       loadSprite();
}

hedgehog::~hedgehog()
{
}

void hedgehog::loadSprite()
{
       loadSpritesheet("../imgs/hedgehog.png", 20, 17);
       addAnimation("default", 0, 5);
       setAnimation("default");
}

void hedgehog::update(float deltaTime, const sf::Vector2u &screenres)
{
       if (isOnScreen())
       {
              velocity.y += 290 * deltaTime;
              Animation::update(deltaTime, screenres);
       }
}

void hedgehog::draw(sf::RenderWindow &window) const
{
       Animation::draw(window);
}

void hedgehog::onCollision(Sprite *other)
{
}
  