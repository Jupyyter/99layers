#include "../hpp/libs.hpp"
class TextBox;
Npc::Npc(sf::Vector2f position)
    : Animation(), CollisionDetector(), gravity(980.0f), isColliding(false)
{
       textBox = std::make_unique<TextBox>("", 0.007f);
       setPosition(position);
       loadSprite();
}

void Npc::loadSprite()
{
       loadSpritesheet("../imgs/capybara.png", 64, 64);
       addAnimation("idle", 1, 8);
       setAnimation("idle");
}

void Npc::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres)
{
       if (isOnScreen(gamemap.getPartBounds()))
       {
              velocity.y += gravity * deltaTime;
              position.y += velocity.y * deltaTime;
              setPosition(position);
              manageCollisions(gamemap.getObjectBounds());
              checkCollisionWithPlayer(gamemap.playerRef->getBounds());
              Animation::update(deltaTime, gamemap, screenres);

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
void Npc::onCollision(Entity *other)
{
       // std::cout<<"tttttttttttuuuuuuuu";
}
void Npc::manageCollisions(const std::vector<sf::FloatRect> &objectBounds)
{
       sf::FloatRect npcBounds = sprite.getGlobalBounds();
       for (const auto &obstacle : objectBounds)
       {
              CollisionInfo collision = checkCollision(npcBounds, {obstacle});
              if (collision.collided)
              {
                     switch (collision.side)
                     {
                     case CollisionSide::Bottom:
                            position.y = obstacle.top - npcBounds.height;
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
std::vector<std::pair<std::string, std::string>> Npc::getEditableProperties() const
{
       return {
           {"text", text}};
}

void Npc::setProperty(const std::string &text, const std::string &value)
{
       if (text == "text")
       {
              this->text = value;
       }
}