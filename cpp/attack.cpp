#include "../hpp/libs.hpp"

Attack::Attack(sf::Vector2f sp) : Sprite(sp), CollisionDetector(), offScreenTimer(), timeOffScreen(0.f)
{
    priorityLayer = 5;
    solid = false;
}
void Attack::update(float deltaTime, const sf::Vector2u &screenres)
{
    if (!isOnScreen())
    {
        timeOffScreen += offScreenTimer.restart().asSeconds();
        if (timeOffScreen >= 1.0f)
        {
            shouldBeDead = true;
        }
    }
    else
    {
        timeOffScreen = 0.f;
        offScreenTimer.restart();
    }
}

HammerThrow::HammerThrow(sf::Vector2f sp) : Attack(sp), fc(true)
{
    loadTexture("../imgs/hammer.jpg");
    this->sprite.scale(32.0f / texture.getSize().x, 32.0f / texture.getSize().y);
    position.x -= sprite.getGlobalBounds().height;
}

void HammerThrow::update(float deltaTime, const sf::Vector2u &screenres)
{
}

Plank::Plank(sf::Vector2f sp) : Attack(sp), fc(true)
{
    loadTexture("../imgs/plank.png");
    this->sprite.scale(20.0f / texture.getSize().x, 100.0f / texture.getSize().y);
    secondSprite.setTexture(texture);
    secondSprite.setScale(20.0f / texture.getSize().x, 100.0f / texture.getSize().y);
}
void Plank::update(float deltaTime, const sf::Vector2u &screenres)
{
    float elapsedTime = timer.getElapsedTime().asSeconds();
    if (elapsedTime < 0.9f)
    {
        if (fc)
        {
            position = sf::Vector2f(world->playerRef->getBounds().left, world->playerRef->getBounds().top);
            fc = false;
        }
        sprite.setPosition(sf::Vector2f(position.x - 5 * world->playerRef->getBounds().width, position.y - 20));
        secondSprite.setPosition(sf::Vector2f(position.x + 5 * world->playerRef->getBounds().width, position.y - 20));
    }
    else
    {
        sprite.setPosition(sf::Vector2f(position.x - 5 * world->playerRef->getBounds().width + 500 * (elapsedTime - 0.9f), position.y - 20));
        secondSprite.setPosition(sf::Vector2f(position.x + 5 * world->playerRef->getBounds().width - 500 * (elapsedTime - 0.9f), position.y - 20));

        if (sprite.getPosition().x >= position.x)
            shouldBeDead = true;
    }
    if (sprite.getGlobalBounds().intersects(world->playerRef->getBounds()) || secondSprite.getGlobalBounds().intersects(world->playerRef->getBounds()))
        (*world->gameOver) = true;
}

void Plank::draw(sf::RenderWindow &window) const
{
    Sprite::draw(window);
    window.draw(secondSprite);
}

LaserBeam::LaserBeam(sf::Vector2f sp, float rotangle) : Attack(sp), fc(true)
{
    loadTexture("../imgs/laser.png");
    sprite.rotate(90 + rotangle);
}
void LaserBeam::onCollision(Sprite *other)
{
    // claudeai showed this trick to me
    if (typeid(*other) == typeid(akBullet) || typeid(*other) == typeid(Terrain))
    {
        shouldBeDead = true;
        world->spawn("LaserEnd", position.x, position.y, 0);
    }
}
void LaserBeam::update(float deltaTime, const sf::Vector2u &screenres)
{
    Attack::update(deltaTime, screenres); // Call base class update for off-screen check

    if (fc)
    {
        velocity = sf::Vector2f(world->playerRef->getBounds().left + world->playerRef->getBounds().width / 2.0f, world->playerRef->getBounds().top + world->playerRef->getBounds().height / 2.0f) - position;
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity /= length;
        velocity *= 1100.0f;
        fc = false;
    }
}
akBullet::akBullet(sf::Vector2f sp, float rotangle) : Attack(sp)
{
    loadTexture("../imgs/akBullet.png");

    // Set the bullet's rotation
    sprite.setRotation(rotangle);

    // Calculate the velocity based on the rotation angle
    float radians = rotangle * 3.14159f / 180.f;
    velocity = sf::Vector2f(std::cos(radians), std::sin(radians));
    velocity *= 797.0f;
}
void akBullet::onCollision(Sprite *other)
{
    if (typeid(*other) == typeid(LaserBeam) || typeid(*other) == typeid(Boss) || typeid(*other) == typeid(Terrain))
    {
        shouldBeDead = true;
        world->spawn("HappyEnd", position.x, position.y, 0);
    }
}
void akBullet::update(float deltaTime, const sf::Vector2u &screenres)
{
    Attack::update(deltaTime, screenres); // Call base class update for off-screen check
}
void akBullet::draw(sf::RenderWindow &window) const
{
    Sprite::draw(window);
}
TableFall::TableFall(sf::Vector2f sp) : Attack(sp)
{
    loadTexture("../imgs/table.png");
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y);
    this->sprite.scale(60.0f / texture.getSize().x, 30.0f / texture.getSize().y);
}

void TableFall::update(float deltaTime, const sf::Vector2u &screenres)
{
    Attack::update(deltaTime, screenres); // Call base class update for off-screen check
    velocity.y += 10;
}
Boomerang::Boomerang(sf::Vector2f sp) : Attack(sp), rotationSpeed(360.0f), timeElapsed(0.0f),
                                        returningPhase(false)
{
    loadTexture("../imgs/boomerangg.png");
    this->sprite.scale(32.0f / texture.getSize().x, 32.0f / texture.getSize().y);
    sprite.setOrigin(16, 16);

    // Calculate initial direction towards player
    sf::Vector2f toPlayer = sf::Vector2f(
                                world->playerRef->getBounds().left + world->playerRef->getBounds().width / 2.0f,
                                world->playerRef->getBounds().top + world->playerRef->getBounds().height / 2.0f) -
                            position;
    // Normalize the direction vector
    float length = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
    toPlayer /= length;

    // Set initial velocity
    velocity = toPlayer * 300.0f;
    initialVelocity = velocity;
}

void Boomerang::update(float deltaTime, const sf::Vector2u &screenres)
{
    Attack::update(deltaTime, screenres);

    timeElapsed += deltaTime;

    // Rotate the sprite
    sprite.rotate(rotationSpeed * deltaTime);

    if (timeElapsed < 1.0f)
    {
        // Gradually curve the velocity to the right/left
        float perpX = -velocity.y;
        float perpY = velocity.x;
        float perpMagnitude = std::sqrt(perpX * perpX + perpY * perpY);
        perpX /= perpMagnitude;
        perpY /= perpMagnitude;

        // Add perpendicular component to create curve
        velocity.x += perpX * 500.0f * deltaTime;
        velocity.y += perpY * 500.0f * deltaTime;
    }
    else if (timeElapsed < 2.0f && !returningPhase)
    {
        // Start return phase
        returningPhase = true;

        // Calculate return velocity
        sf::Vector2f returnDir = -velocity;
        float returnSpeed = std::sqrt(returnDir.x * returnDir.x + returnDir.y * returnDir.y);
        returnDir /= returnSpeed;

        velocity = returnDir * 300.0f;
    }
    else if (timeElapsed >= 2.0f)
    {
        // Maintain a somewhat straight path back
        sf::Vector2f currentDir = velocity;
        float currentSpeed = std::sqrt(currentDir.x * currentDir.x + currentDir.y * currentDir.y);
        currentDir /= currentSpeed;

        // Slightly adjust velocity to maintain speed
        velocity = currentDir * 300.0f;
    }

    // Normalize velocity to maintain consistent speed
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    velocity = (velocity / speed) * 300.0f;

    if (timeElapsed >= 4.0f)
    {
        shouldBeDead = true;
    }
}
Boomerang2::Boomerang2(sf::Vector2f sp) : Attack(sp), rotationSpeed(360.0f),
                                          timeElapsed(0.0f), timeSinceLastRedirect(0.0f)
{
    loadTexture("../imgs/boomerangg.png");
    this->sprite.scale(32.0f / texture.getSize().x, 32.0f / texture.getSize().y);
    sprite.setOrigin(16, 16);

    // Calculate initial direction towards player
    updateDirectionToPlayer();
}

void Boomerang2::update(float deltaTime, const sf::Vector2u &screenres)
{
    Attack::update(deltaTime, screenres);

    timeElapsed += deltaTime;
    timeSinceLastRedirect += deltaTime;

    // Rotate the sprite
    sprite.rotate(rotationSpeed * deltaTime);

    // Update direction every second
    if (timeSinceLastRedirect >= 1.0f)
    {
        updateDirectionToPlayer();
        timeSinceLastRedirect = 0.0f;
    }

    // Die after 5 seconds
    if (timeElapsed >= 5.0f)
    {
        shouldBeDead = true;
    }
}

void Boomerang2::updateDirectionToPlayer()
{
    if (world->isPlayerValid)
    {
        // Calculate direction towards current player position
        sf::Vector2f toPlayer = sf::Vector2f(
                                    world->playerRef->getBounds().left + world->playerRef->getBounds().width / 2.0f,
                                    world->playerRef->getBounds().top + world->playerRef->getBounds().height / 2.0f) -
                                position;

        // Normalize the direction vector
        float length = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
        toPlayer /= length;

        // Set velocity in the new direction
        velocity = toPlayer * 500.0f;
    }
}