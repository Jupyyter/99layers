#include "../hpp/libs.hpp"

Boss::Boss(sf::Vector2u windowSize)
    : targetWidth(200.0f), windowSize(windowSize)
{
    sprite.setOrigin(514, 366);
    loadAndScaleImage("../imgs/ikeaman.jpg");
    setInitialPosition();
    atimer.restart();
    ptimer.restart();
    ltimer.restart();
}

void Boss::loadAndScaleImage(const std::string &imagePath)
{
    assert(texture.loadFromFile(imagePath));
    assert(eyetexture.loadFromFile("../imgs/redeye.png"));

    eyesprite.setTexture(eyetexture);
    sprite.setTexture(texture);

    // Calculate the scaling factor to achieve the target width
    float scaleFactor = targetWidth / sprite.getLocalBounds().width;
    sprite.setScale(scaleFactor, scaleFactor);
}

void Boss::setInitialPosition()
{
    // Position the sprite a little above the middle of the screen
    float x = (windowSize.x - sprite.getGlobalBounds().width) / 2.0f;
    float y = (windowSize.y - sprite.getGlobalBounds().height) / 2.0f - 150.0f;
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
    eyesprite.setPosition(x - eyesprite.getGlobalBounds().width / 2.0f, y - eyesprite.getGlobalBounds().height / 2.0f);
}

void Boss::update(float deltaTime, GameMap& gamemap, const sf::Vector2u& screenres)
{
    // Calculate direction to player
    sf::Vector2f bossPosition = sprite.getPosition();
    sf::Vector2f playerCenter(gamemap.playerBounds->left + gamemap.playerBounds->width / 2.0f, gamemap.playerBounds->top + gamemap.playerBounds->height / 2.0f);
    sf::Vector2f direction = playerCenter - bossPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize direction
    if (distance > 0)
    {
        direction /= distance;
    }

    // Set movement speed (you can adjust this value)
    float movementSpeed = 100.0f; // pixels per second

    // Move towards player
    sprite.move(direction * movementSpeed * deltaTime);
    position = sprite.getPosition();

    // Update eye sprite position
    eyesprite.setPosition(sprite.getPosition().x - eyesprite.getGlobalBounds().width / 2.0f,
                          sprite.getPosition().y - eyesprite.getGlobalBounds().height / 2.0f);

    // Face the player
    if (playerCenter.x < bossPosition.x && sprite.getScale().y > 0)
    {
        sprite.scale(1, -1);
    }
    else if (playerCenter.x > bossPosition.x && sprite.getScale().y < 0)
    {
        sprite.scale(1, -1);
    }

    // Rotate to face player
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159;
    sprite.setRotation(angle);

    // Create Attacks
    if (isOnScreen((gamemap.getPartBounds())))
    {
        if (ptimer.getElapsedTime().asSeconds() >= 3.5)
        {
            gamemap.spawn("plank",this->sprite.getPosition().x,this->sprite.getPosition().y,0);
            ptimer.restart();
        }

        if (ltimer.getElapsedTime().asSeconds() >= 0.8)
        {
            gamemap.spawn("laser",this->sprite.getPosition().x,this->sprite.getPosition().y,this->sprite.getRotation());
            ltimer.restart();
        }

        if (ttimer.getElapsedTime().asSeconds() >= 2.3)
        {
            gamemap.spawn("table",sf::Vector2f(gamemap.playerBounds->left + gamemap.playerBounds->width / 2.0f, gamemap.getPartBounds().top).x,sf::Vector2f(gamemap.playerBounds->left + gamemap.playerBounds->width / 2.0f, gamemap.getPartBounds().top).y,this->sprite.getRotation());
            ttimer.restart();
        }
    }
}
void Boss::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    // Draw the red eye
    window.draw(this->eyesprite);
}

void Boss::resetTimers()
{
    this->ttimer.restart();
    this->ltimer.restart();
    this->atimer.restart();
    this->ptimer.restart();
}

Boss::~Boss()
{
}