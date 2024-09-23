#include "../hpp/libs.hpp"

Boss::Boss(const sf::Vector2f &initialPosition)
    : Sprite()
{
    loadAndScaleImage();
    setPosition(initialPosition);
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

void Boss::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres)
{
    sf::Vector2f playerCenter(gamemap.playerBounds->left + gamemap.playerBounds->width / 2.0f,
                              gamemap.playerBounds->top + gamemap.playerBounds->height / 2.0f);
    sf::Vector2f direction = playerCenter - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0)
    {
        direction /= distance;
    }

    float movementSpeed = 70.0f;
    move(direction * movementSpeed * deltaTime);
    position += direction * movementSpeed * deltaTime;
    setPosition(position);

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
    if (isOnScreen(gamemap.getPartBounds()))
    {
        if (ptimer.getElapsedTime().asSeconds() >= 3.5)
        {
            gamemap.spawn("plank", position.x, position.y, 0);
            ptimer.restart();
        }

        if (ltimer.getElapsedTime().asSeconds() >= 0.05)
        {
            gamemap.spawn("laser", position.x, position.y, sprite.getRotation());
            ltimer.restart();
        }

        if (ttimer.getElapsedTime().asSeconds() >= 2.3)
        {
            gamemap.spawn("table", playerCenter.x, gamemap.getPartBounds().top, sprite.getRotation());
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