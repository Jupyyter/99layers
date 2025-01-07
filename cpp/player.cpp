#include "../hpp/libs.hpp"
#include <iostream>

Player::Player(sf::Vector2f position) : Animation(position, 0.1), CollisionDetector(), inventory(new Inventory()), isJump(false),wasUpPressed(false),isPushed(false), pushTimer(0.0f)
{
       priorityLayer = 4;
       world->spawn(inventory);
       loadAnimations();
       loadShaders();

       gravity = 980.0f;
       jumpForce = -484.0f;
       moveSpeed = 170.0f;
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
        // Only handle horizontal input if not being pushed or if grounded
        if (!isPushed || isGrounded)
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
            
            bool isUpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            
            if (isUpPressed && !wasUpPressed && isGrounded && isJump)
            {
                velocity.y = jumpForce;
                isMoving = true;
            }

            wasUpPressed = isUpPressed;
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

            // If we hit the ground while being pushed, regain control
            if (isGrounded && isPushed)
            {
                isPushed = false;
            }
            
            isGrounded = false;
        }
        else{
              velocity=sf::Vector2f(0,0);
        }
}
void Player::onCollision(Sprite *other)
{
        if (!isStasis&&(typeid(*other) == typeid(PacMan) || typeid(*other) == typeid(TableFall) || 
            typeid(*other) == typeid(LaserBeam) || typeid(*other) == typeid(hedgehog)|| typeid(*other) == typeid(Boomerang)|| typeid(*other) == typeid(Boomerang2)))
        {
            *(world->gameOver) = true;
            this->shouldBeDead = true;
            world->isPlayerValid = false;
            world->spawn("bloodParticles", position.x, position.y);
        }
        else if(typeid(*other) == typeid(Penguin))
        {
            // Calculate direction vector from penguin to player
            sf::Vector2f penguinPos = other->getPosition();
            sf::Vector2f direction = position - penguinPos;
            
            // Normalize the direction vector
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction.x /= length;
                direction.y /= length;
            }
            
            // Apply impulse force
            isPushed = true;
            velocity = direction * PUSH_FORCE;
            
            // Small position adjustment to prevent sticking
            position += direction * 5.0f;
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