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

       if (typeid(*other) == typeid(Terrain))
       {
              switch (CollisionDetector::CollisionSide(getBounds(), other->getBounds()))
              {
              case CollisionInfo::Left:
                     if (!(other->getBounds().top > getBounds().top &&
                           other->getBounds().top - getBounds().top > 27 && velocity.y >= 0))
                     {
                            position.x = other->getBounds().left + other->getBounds().width;
                            velocity.x = 0;
                     }
                     break;
              case CollisionInfo::Right:
                     if (!(other->getBounds().top > getBounds().top &&
                           other->getBounds().top - getBounds().top > 27 && velocity.y >= 0))
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
                     position.y = other->getBounds().top + other->getBounds().height;
                     velocity.y = 0;
                     break;
              }
       }
}
  