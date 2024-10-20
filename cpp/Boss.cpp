#include "../hpp/libs.hpp"
class GameMap;
Boss::Boss(const sf::Vector2f &initialPosition)
    : Sprite(initialPosition), CollisionDetector(),hasAppearedOnScreen(false)
{
    loadAndScaleImage();
    resetTimers();
}

void Boss::loadAndScaleImage()
{
    loadTexture("../imgs/ikeaman.png");
    sprite.setOrigin(103, 73);

    if (!eyeTexture.loadFromFile("../imgs/redeye.png"))
    {
        std::cerr << "Failed to load eye texture" << std::endl;
    }
    eyeSprite.setTexture(eyeTexture);
    updateEyePosition();
}

void Boss::update(float deltaTime, const sf::Vector2u &screenres)
{
    std::cout<<hasAppearedOnScreen<<" ";
    if (!hasAppearedOnScreen)
    {
        if (isOnScreen())
        {
            hasAppearedOnScreen = true;
        }
        else
        {
            return;
        }
    }
    sf::Vector2f playerCenter(world->playerRef->getBounds().left + world->playerRef->getBounds().width / 2.0f,
                              world->playerRef->getBounds().top + world->playerRef->getBounds().height / 2.0f);
    sf::Vector2f direction = playerCenter - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0)
    {
        direction /= distance;
    }

    float movementSpeed = 70.0f;
    move(direction * movementSpeed * deltaTime);
    position += direction * movementSpeed * deltaTime;

    updateEyePosition();

    // Face the player
    if (playerCenter.x < position.x && sprite.getScale().y > 0)
    {
        sprite.scale(1, -1);
    }
    else if (playerCenter.x > position.x && sprite.getScale().y < 0)
    {
        sprite.scale(1, -1);
    }

    // Rotate to face player
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159;
    sprite.setRotation(angle);

    // Create Attacks
     if (isOnScreen())
    {
        if (ptimer.getElapsedTime().asSeconds() >= 3.5)
        {
            world->spawn("plank", position.x, position.y, 0);
            ptimer.restart();
        }

        if (ltimer.getElapsedTime().asSeconds() >= 0.05)
        {
            world->spawn("laser", position.x, position.y, sprite.getRotation());
            ltimer.restart();
        }

        if (ttimer.getElapsedTime().asSeconds() >= 2.3)
        {
            world->spawn("table", playerCenter.x, world->getPartBounds().top, sprite.getRotation());
            ttimer.restart();
        }
    }
}

void Boss::draw(sf::RenderWindow &window)const 
{
    Sprite::draw(window);
    window.draw(eyeSprite);
}

void Boss::resetTimers()
{
    ttimer.restart();
    ltimer.restart();
    atimer.restart();
    ptimer.restart();
}

void Boss::updateEyePosition()
{
    eyeSprite.setPosition(position.x - eyeSprite.getGlobalBounds().width / 2.0f,
                          position.y - eyeSprite.getGlobalBounds().height / 2.0f);
}
void Boss::onCollision(Object *other)
{
    // claudeai showed this trick to me
    akBullet* attack = dynamic_cast<akBullet*>(other);
    
    if (attack)
    {
        sf::Color color = sprite.getColor();
        
        // Subtract 10 from each component, starting with red

            color.r = std::max(0, color.r - 2);

            color.g = std::max(0, color.g - 2);


            color.b = std::max(0, color.b - 2);
        
        // Set the new color
        sprite.setColor(color);
        
        // Check if all components are 0
        if (color.r == 0 && color.g == 0 && color.b == 0)
        {
            shouldBeDead = true;
        }
    }
}