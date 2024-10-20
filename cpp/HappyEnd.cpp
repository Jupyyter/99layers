#include "../hpp/libs.hpp"

HappyEnd::HappyEnd(const sf::Vector2f &initialPosition) : Animation(initialPosition), animationStarted(false), animationCompleted(false)
{
       priorityLayer=6;
       shouldBeDead = false;
       loadSpritesheet("../imgs/HappyEnd.png", 32, 32);
       addAnimation("go", 0, 5);
       setAnimation("go");
       sprite.setOrigin(16, 16);
}

void HappyEnd::update(float deltaTime, const sf::Vector2u &screenres)
{

       Animation::update(deltaTime, screenres);

       // Check if the animation has completed one full cycle
       if (currentFrame == 4)
       {
              if (animationCompleted)
              {
                     animationCompleted = true;
                     shouldBeDead = true;
              }
              animationCompleted = true;
       }
}