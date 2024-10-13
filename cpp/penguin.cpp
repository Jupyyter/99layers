#include "../hpp/libs.hpp"
#include "../hpp/penguin.hpp"

Penguin::Penguin(sf::Vector2f position)
    : Animation(), CollisionDetector(), gravity(980.0f), isColliding(false), 
    speed(200.0f) {
    setPosition(position);
    loadSprite();
}

void Penguin::loadSprite() {
    loadSpritesheet("../imgs/penguinSpin/Spin Attack.png", 18, 16);
    addAnimation("spin", 0, 7);
    setAnimation("spin");
}
void Penguin::onCollision(Entity *other)
{
       if (typeid(*other) == typeid(Terrain))
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
void Penguin::update(float deltaTime, const sf::Vector2u& screenres) {
    if (isOnScreen()) {
        velocity.y += gravity * deltaTime;
        position.y += velocity.y * deltaTime;
        
        velocity.x = speed * deltaTime;
        position.x += velocity.x;

        setPosition(position);
        Animation::update(deltaTime,screenres);
    }
}

void Penguin::draw(sf::RenderWindow& window)const  {
    Animation::draw(window);
}