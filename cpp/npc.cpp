#include "../hpp/libs.hpp"
class TextBox;
Npc::Npc(sf::Vector2f position)
    : Animation(position), CollisionDetector(), gravity(980.0f), isColliding(false)
{
       priorityLayer=1;
       textBox = std::make_unique<TextBox>("", 0.007f);
       loadSprite();
}

void Npc::loadSprite()
{
       loadSpritesheet("../imgs/capybara.png", 64, 64);
       addAnimation("idle", 1, 8);
       setAnimation("idle");
}

void Npc::update(float deltaTime,  const sf::Vector2u &screenres)
{
       if (isOnScreen())
       {
              velocity.y += gravity * deltaTime;
              position.y += velocity.y * deltaTime;
              checkCollisionWithPlayer(world->playerRef->getBounds());
              Animation::update(deltaTime, screenres);

              if (textBox)
              {
                     textBox->update(deltaTime);
              }
       }
}

void Npc::draw(sf::RenderWindow &window)const 
{
       Animation::draw(window);
       textBox->draw(window);
}

void Npc::checkCollisionWithPlayer(const sf::FloatRect &playerBounds)
{
       sf::FloatRect npcBounds = sprite.getGlobalBounds();
       if (npcBounds.intersects(playerBounds))
       {
              if (!isColliding)
              {
                     if (textBox)
                     {
                            textBox->setString(text);
                     }
                     isColliding = true;
              }
       }
       else
       {
              if (isColliding)
              {
                     textBox->setString("");
              }
              isColliding = false;
       }
}
void Npc::onCollision(Object *other)
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
std::vector<PropertyDescriptor> Npc::getPropertyDescriptors() {
    return {
        {"text", "",
            [](Object* e, const std::string& v) { static_cast<Npc*>(e)->text = v; },  // Direct assignment to string
            [](const Object* e) { return static_cast<const Npc*>(e)->text; }  // Return string directly
        }
    };
}