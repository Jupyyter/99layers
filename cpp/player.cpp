#include "../hpp/libs.hpp"
#include <iostream>

Player::Player(sf::Vector2f position) : Animation(position, 0.1), CollisionDetector(), inventory(new Inventory()), isJump(false)
{
       priorityLayer = 4;
       world->spawn(inventory);
       loadAnimations();
       loadShaders();

       gravity = 980.0f;
       jumpForce = -500.0f;
       moveSpeed = 200.0f;
       isGrounded = false;
       onehitinvin = false;
       gothitinv = false;
       isStasis = false;
       world->isPlayerValid=true;
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
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
       {
              velocity.x = -moveSpeed;
              flipped = false;
              isMoving = true;
       }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       {
              velocity.x = moveSpeed;
              flipped = true;
              isMoving = true;
       }
       else
       {
              velocity.x = 0;
       }
       if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && isGrounded && isJump)
       {
              velocity.y = jumpForce;
              isMoving = true;
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
       isMoving = false;
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
       {
              *(world->gameOver) = true;
       }
       if (!isStasis)
       {
              if (velocity.y > 2777)
              {
                     *(world->gameOver) = true;
              }
              updateAnimation();
              handleInput();
              if (isOnScreen())
              {
                     velocity.y += gravity * deltaTime;
              }

              Animation::update(deltaTime, screenres);
              isGrounded = false;
       }
}
void Player::onCollision(Sprite *other)
{
       if (typeid(*other) == typeid(PacMan)||typeid(*other) == typeid(TableFall)||typeid(*other) == typeid(LaserBeam)||typeid(*other) == typeid(hedgehog))
       {
              *(world->gameOver) = true;
              this->shouldBeDead = true;
              inventory->shouldBeDead=true;
              world->isPlayerValid=false;
              world->spawn("bloodParticles", position.x, position.y);
       }
       else if (typeid(*other) == typeid(Terrain))
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
              world->changePart(1, 0, true);
              checkBounds(screenres);
       }
       else if (this->position.x < bounds.left)
       {
              world->changePart(-1, 0, true);
              checkBounds(screenres);
       }
       else if (this->position.y < bounds.top)
       {
              world->changePart(0, -1, true);
              checkBounds(screenres);
       }
       else if (this->position.y > bounds.top + bounds.height)
       {
              world->changePart(0, 1, true);
              checkBounds(screenres);
       }
}