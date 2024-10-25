#include "../hpp/libs.hpp"
bloodParticle::bloodParticle(sf::Vector2f position):Sprite(position), CollisionDetector()

{
    spawnParticles(position);
}

void bloodParticle::spawnParticles(sf::Vector2f position)
{
}
void bloodParticle::update(float deltaTime, const sf::Vector2u& windowSize){

}
void bloodParticle::onCollision(Sprite *other)
{
       if (typeid(*other) == typeid(Terrain))
       {
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
       }
}
void Penguin::loadSprite() {
    loadSpritesheet("../imgs/bloodParticles.png", 32, 8);
    addAnimation("blood", 0, 1);
    setAnimation("blood");
    sprite.setOrigin(16, 16);
}