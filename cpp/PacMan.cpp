#include "../hpp/libs.hpp"
#include "../hpp/PacMan.hpp"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
PacMan::PacMan(sf::Vector2f spawnPosition)
    : Animation(), CollisionDetector(), hasAppearedOnScreen(false), lifeTimer(0.0f),
      lifeDuration(5.0f), speed(100.0f), degrees(0.0f)
{
    setPosition(spawnPosition);
     position = spawnPosition;
    loadSprite();
}

void PacMan::loadSprite()
{
    loadSpritesheet("../imgs/pacmann.png", 32, 32);
    addAnimation("default", 0, 3);
    setAnimation("default");
    sprite.setOrigin(16, 16); // Set origin to center of sprite
}

void PacMan::updateDirection()
{
    float adjustedDegrees = 360.0f - degrees;
    float radians = adjustedDegrees * M_PI / 180.0f;

    velocity.x = std::cos(radians) * speed;
    velocity.y = std::sin(radians) * speed;
    
    float rotation = std::atan2(velocity.y, velocity.x) * 180 / M_PI;
    sprite.setRotation(rotation);
}
void PacMan::update(float deltaTime,const sf::Vector2u &screenres)
{
    if (!hasAppearedOnScreen)
    {
        if (isOnScreen())
        {
            hasAppearedOnScreen = true;
            lifeTimer = 0.0f;
        }
        else
        {
            return;
        }
    }

    updateDirection();
    position += velocity * deltaTime;
    setPosition(position);

    lifeTimer += deltaTime;
    if(sprite.getGlobalBounds().intersects(world->playerRef->getBounds())){
        *world->gameOver = true;
    }

    if (lifeTimer >= lifeDuration)
    {
        shouldBeDead = true;
    }
    Animation::update(deltaTime,screenres);
}

bool PacMan::shouldRemove() { return lifeTimer >= lifeDuration; }

void PacMan::draw(sf::RenderWindow &window)const 
{
    Animation::draw(window);
}
std::vector<PropertyDescriptor> PacMan::getPropertyDescriptors() {
    return {
        {"speed", "100.0",
            [](Entity* e, const std::string& v) { static_cast<PacMan*>(e)->speed = std::stof(v); },
            [](const Entity* e) { return std::to_string(static_cast<const PacMan*>(e)->speed); }
        },
        {"degrees", "0.0",
            [](Entity* e, const std::string& v) { 
                static_cast<PacMan*>(e)->degrees = std::stof(v);},
            [](const Entity* e) { return std::to_string(static_cast<const PacMan*>(e)->degrees); }
        }
        /*{"speed", "100.0", &PacMan::speed},
        {"degrees", "0.0", &PacMan::degrees}*///this might work too, but the PropertyDescriptor should look like this:
        /*struct PropertyDescriptor {
    std::string name;
    std::string defaultValue;
    float Entity::*memberPtr;
};
 */ 
    };
}