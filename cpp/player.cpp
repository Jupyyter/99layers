#include "../hpp/libs.hpp"
#include <iostream>

Player::Player(sf::Vector2f position) : Animation(), CollisionDetector(), inventory(new Inventory())
{
       priorityLayer = 4;
       world->spawn(inventory);
       loadAnimations();
       loadShaders();
       setPosition(position);

       gravity = 980.0f;
       jumpForce = -500.0f;
       moveSpeed = 200.0f;
       isGrounded = false;
       onehitinvin = false;
       gothitinv = false;
       isStasis = false;
}
void Player::loadAnimations()
{
       loadSpritesheet("../imgs/player.png", 29, 38);
       addAnimation("idle", 0, 4);
       addAnimation("running", 1, 6);
       addAnimation("jumping", 2, 3);
       setAnimation("idle");
}

void Player::loadShaders()
{
       assert(this->stasishad.loadFromFile("../shaders/stasis.frag", sf::Shader::Fragment));
       this->stasishad.setUniform("texture", sf::Shader::CurrentTexture);
}

void Player::handleInput()
{
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)||sf::Keyboard::isKeyPressed(sf::Keyboard::A))
       {
              velocity.x = -moveSpeed;
              flipped = false;
       }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)||sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       {
              velocity.x = moveSpeed;
              flipped = true;
       }
       else
       {
              velocity.x = 0;
       }

       if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && isGrounded)
       {
              velocity.y = jumpForce;
       }
}

void Player::updateAnimation()
{
       if (!isGrounded)
       {
              pause();
              setAnimation("jumping");
              if (velocity.y < 0) // Rising
              {
                     setCurrentFrame(0);
              }
              else if (std::abs(velocity.y) < 200) // Near apex
              {
                     setCurrentFrame(1);
              }
              else // Falling
              {
                     setCurrentFrame(2);
              }
       }
       else if (std::abs(velocity.x) > 0)
       {
              resume();
              setAnimation("running");
       }
       else
       {
              resume();
              setAnimation("idle");
       }
}

void Player::update(float deltaTime, const sf::Vector2u &screenres)
{
       if (!isStasis)
       {
              if (velocity.y > 5777)
              {
                     *(world->gameOver) = true;
              }
              updateAnimation();
              handleInput();
              if (isOnScreen())
              {
                     velocity.y += gravity * deltaTime;
                     position += velocity * deltaTime;
              }

              checkBounds(screenres);
              Animation::update(deltaTime, screenres);
              isGrounded = false;
              setPosition(position);
       }
}
void Player::onCollision(Entity *other)
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
                            isGrounded = true;
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
void Player::draw(sf::RenderWindow &window) const
{
       if (isStasis)
       {
              window.draw(sprite, &stasishad);
       }
       else
       {
              window.draw(sprite);
       }
}

void Player::checkBounds(const sf::Vector2u &screenres)
{
       auto bounds = world->getPartBounds();
       if (this->position.x > bounds.left + bounds.width)
       {
              world->changePart(1, 0);
       }
       else if (this->position.x < bounds.left)
       {
              world->changePart(-1, 0);
       }
       else if (this->position.y < bounds.top)
       {
              world->changePart(0, -1);
       }
       else if (this->position.y > bounds.top + bounds.height)
       {
              world->changePart(0, 1);
       }
}
// what am i doing idk i seem to be too incapable to code which is paradoxically is the only thing i know how to do (or not this is subjective) well that is unfortunate