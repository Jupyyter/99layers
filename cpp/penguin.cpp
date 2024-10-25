#include "../hpp/libs.hpp"
#include "../hpp/penguin.hpp"

Penguin::Penguin(sf::Vector2f position)
    : Animation(position,0.1), CollisionDetector(), gravity(980.0f), isColliding(false), 
    speed(200.0f) {
    loadSprite();
    setInitialDirection();
    
}
void Penguin::setInitialDirection()
{
    // Randomly choose left or right direction
    speed = (rand() % 2 == 0) ? -speed : speed;
    //velocity.x = speed;
}
void Penguin::loadSprite() {
    loadSpritesheet("../imgs/penguinSpin/Spin Attack.png", 18, 16);
    addAnimation("spin", 0, 7);
    setAnimation("spin");
}
void Penguin::onCollision(Sprite *other)
{
       if (typeid(*other) == typeid(Terrain))
       {
              switch (CollisionDetector::CollisionSide(getBounds(), other->getBounds()))
              {
              case CollisionInfo::Left:
                     if (!(other->getBounds().top > getBounds().top && other->getBounds().top - getBounds().top > 27 && velocity.y >= 0)) // in case of stairs
                     {
                            position.x = other->getBounds().left + other->getBounds().width;
                            speed=-speed;
                     }
                     break;
              case CollisionInfo::Right:
                     if (!(other->getBounds().top > getBounds().top && other->getBounds().top - getBounds().top > 27 && velocity.y >= 0))
                     {
                            position.x = other->getBounds().left - getBounds().width;
                            speed=-speed;
                     }
                     break;
              case CollisionInfo::Bottom:
                     if (velocity.y >= 0)
                     {
                            position.y = other->getBounds().top - getBounds().height;
                            velocity.y = 0;
                            
                            // Check if its about to walk off the platform
                            float platformEnd;
                            if (speed > 0) // Moving right
                            {
                                   platformEnd = other->getBounds().left + other->getBounds().width;
                                   if (position.x + getBounds().width + 5 >= platformEnd) // 5 pixels threshold
                                   {
                                          speed = -speed; // Reverse direction
                                   }
                            }
                            else // Moving left
                            {
                                   platformEnd = other->getBounds().left;
                                   if (position.x - 5 <= platformEnd) // 5 pixels threshold
                                   {
                                          speed = -speed; // Reverse direction
                                   }
                            }
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
void Penguin::update(float deltaTime, const sf::Vector2u& screenres) {
    if (isOnScreen()) {
        velocity.y += gravity * deltaTime;
        position.y += velocity.y * deltaTime;
        
        velocity.x = speed * deltaTime;
        position.x += velocity.x;

        Animation::update(deltaTime,screenres);
    }
}

void Penguin::draw(sf::RenderWindow& window)const  {
    Animation::draw(window);
}