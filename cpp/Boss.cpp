#include "../hpp/libs.hpp"
class GameMap;

Boss::Boss(const sf::Vector2f &initialPosition)
    : Sprite(initialPosition), CollisionDetector(), hasAppearedOnScreen(false)
{
    loadAndScaleImage();
    resetTimers();
    maxHealth = 255; // Since we're using color values as HP
    currentPhase = 1;
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
    if (!world->isPlayerValid)
    {
        return;
    }

    // Get player position for targeting
    sf::Vector2f playerCenter(world->playerRef->getBounds().left + world->playerRef->getBounds().width / 2.0f,
                              world->playerRef->getBounds().top + world->playerRef->getBounds().height / 2.0f);
    sf::Vector2f direction = playerCenter - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize direction
    if (distance > 0)
    {
        direction /= distance;
    }

    // Reduce Y movement by half
    direction.y *= 0.5f;

    // Base movement speed when player is not moving
    float baseSpeed = 40.0f;
    // Additional speed when player is moving
    float chaseSpeed = 50.0f;
    
    // Calculate final movement speed
    float movementSpeed = baseSpeed;
    if (world->playerRef->isMoving)
    {
        movementSpeed += chaseSpeed;
    }

    // Apply movement
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

    // Get current health percentage
    sf::Color currentColor = sprite.getColor();
    float healthPercentage = (currentColor.r + currentColor.g + currentColor.b) / (3.0f * 255.0f);

    // Update current phase based on health
    if (healthPercentage <= 0.3f)
    {
        currentPhase = 3;
    }
    else if (healthPercentage <= 0.6f)
    {
        currentPhase = 2;
    }

    // Create Attacks based on current phase
    switch (currentPhase)
    {
    case 1: // First phase
        if (ltimer.getElapsedTime().asSeconds() >= 5.0f)
        {
            world->spawn("laser", position.x, position.y, sprite.getRotation());
            ltimer.restart();
        }
        break;

    case 2: // Second phase
        if (ltimer.getElapsedTime().asSeconds() >= 0.7f)
        {
            world->spawn("laser", position.x, position.y, sprite.getRotation());
            ltimer.restart();
        }
        if (ttimer.getElapsedTime().asSeconds() >= 1.0f)
        {
            world->spawn("table", playerCenter.x, world->getPartBounds().top, sprite.getRotation());
            ttimer.restart();
        }
        break;

    case 3: // Final phase
        if (ltimer.getElapsedTime().asSeconds() >= 0.2f)
        {
            world->spawn("laser", position.x, position.y, sprite.getRotation());
            ltimer.restart();
        }
        if (ttimer.getElapsedTime().asSeconds() >= 0.5f)
        {
            world->spawn("table", playerCenter.x, world->getPartBounds().top, sprite.getRotation());
            ttimer.restart();
        }
        break;
    }
}
void Boss::draw(sf::RenderWindow &window) const
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

void Boss::onCollision(Sprite *other)
{
    akBullet *attack = dynamic_cast<akBullet *>(other);

    if (attack)
    {
        sf::Color color = sprite.getColor();

        // Subtract 2 from each component
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