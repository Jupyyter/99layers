#include "../hpp/libs.hpp"
#include "../hpp/PacMan.hpp"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
PacMan::PacMan(sf::Vector2f spawnPosition)
    : Animation(spawnPosition, 0.1), CollisionDetector(),
      hasAppearedOnScreen(false), lifeTimer(0.0f),
      lifeDuration(5.0f), speed(100.0f), degrees(0.0f),
      isSoundPlaying(false), what(0)
{
    solid=false;
    priorityLayer=3;
    position = spawnPosition;
    loadSprite();
    initializeSound();
}

PacMan::~PacMan()
{
    stopSound();
}

void PacMan::initializeSound()
{
    if (!soundBuffer.loadFromFile("../audio/wakawaka.wav"))
    {
        std::cerr << "Error loading waka-waka sound!" << std::endl;
        return;
    }
    wakaSound.setBuffer(soundBuffer);
    wakaSound.setLoop(true);
}

void PacMan::loadSprite()
{
    loadSpritesheet("../imgs/pacmann.png", 32, 32);
    addAnimation("default", 0, 3);
    setAnimation("default");
    sprite.setOrigin(16, 16);
}
void PacMan::updateDirectionToPlayer(sf::Vector2f playerPos, float deltaTime)
{
    // Calculate direction to player
    sf::Vector2f directionToPlayer = playerPos - position;
    float distanceToPlayer = std::sqrt(directionToPlayer.x * directionToPlayer.x + 
                                        directionToPlayer.y * directionToPlayer.y);
    
    // Normalize direction
    if (distanceToPlayer > 0) {
        directionToPlayer /= distanceToPlayer;
    }

    // Calculate angle to player
    float angleToPlayer = std::atan2(directionToPlayer.y, directionToPlayer.x) * 180 / M_PI;
    
    // Calculate current angle
    float currentAngle = sprite.getRotation();

    // Calculate the smallest rotation
    float angleDifference = angleToPlayer - currentAngle;
    
    // Normalize angle difference
    while (angleDifference > 180) angleDifference -= 360;
    while (angleDifference < -180) angleDifference += 360;

    // Adjust rotation speed based on current speed
    float currentSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float effectiveRotationSpeed = rotationSpeed * (2.0f - std::min(1.0f, currentSpeed / maxSpeed));
    
    // Rotate towards the player
    float rotationAmount = effectiveRotationSpeed * deltaTime;
    if (std::abs(angleDifference) < rotationAmount) {
        currentAngle = angleToPlayer;
    } else {
        currentAngle += (angleDifference > 0 ? 1 : -1) * rotationAmount;
    }
    
    // Set sprite rotation
    sprite.setRotation(currentAngle);

    // Calculate velocity
    float radians = currentAngle * M_PI / 180.0f;
    velocity.x = std::cos(radians) * speed;
    velocity.y = std::sin(radians) * speed;

    // Adjust speed based on alignment with player direction
    float dotProduct = directionToPlayer.x * velocity.x + directionToPlayer.y * velocity.y;
    float speedMultiplier = (dotProduct > 0) ? 1.2f : 0.8f;
    
    // Clamp speed
    speed = std::max(minSpeed, std::min(maxSpeed, speed * speedMultiplier));
}
void PacMan::updateDirection()
{
    if (what == 0) {
        // Original behavior
        float adjustedDegrees = 360.0f - degrees;
        float radians = adjustedDegrees * M_PI / 180.0f;
        velocity.x = std::cos(radians) * speed;
        velocity.y = std::sin(radians) * speed;

        float rotation = std::atan2(velocity.y, velocity.x) * 180 / M_PI;
        sprite.setRotation(rotation);
    }
}

void PacMan::stopSound()
{
    if (wakaSound.getStatus() == sf::Sound::Playing)
    {
        wakaSound.stop();
        isSoundPlaying = false;
    }
}

void PacMan::updateSound()
{
    bool shouldPlaySound = hasAppearedOnScreen && isOnScreen();

    if (shouldPlaySound && !isSoundPlaying)
    {
        wakaSound.play();
        isSoundPlaying = true;
    }
    else if (!shouldPlaySound && isSoundPlaying)
    {
        stopSound();
    }
}

void PacMan::update(float deltaTime, const sf::Vector2u &screenres)
{
    if (!hasAppearedOnScreen)
    {
        if (isOnScreen())
        {
            if(what==1){
                lifeDuration=10;
            }
            hasAppearedOnScreen = true;
            lifeTimer = 0.0f;
        }
        else
        {
            return;
        }
    }

    // Player tracking behavior
    if (what == 1 && world && world->playerRef) {
        playerPosition = world->playerRef->getPosition();
        updateDirectionToPlayer(playerPosition, deltaTime);
    }
    else {
        // Original behavior
        updateDirection();
    }

    updateSound();

    lifeTimer += deltaTime;

    if (lifeTimer >= lifeDuration)
    {
        stopSound();
        shouldBeDead = true;
    }

    Animation::update(deltaTime, screenres);
}


bool PacMan::shouldRemove()
{
    if (lifeTimer >= lifeDuration)
    {
        stopSound();
    }
    return lifeTimer >= lifeDuration;
}

void PacMan::draw(sf::RenderWindow &window) const
{
    Animation::draw(window);
}

void PacMan::onCollision(Sprite *other)
{
}

std::vector<PropertyDescriptor> PacMan::getPropertyDescriptors()
{
    return {
        {"speed", "100.0",
         [](Object *e, const std::string &v)
         { static_cast<PacMan *>(e)->speed = std::stof(v); },
         [](const Object *e)
         { return std::to_string(static_cast<const PacMan *>(e)->speed); }},
        {"degrees", "0.0",
         [](Object *e, const std::string &v)
         { static_cast<PacMan *>(e)->degrees = std::stof(v); },
         [](const Object *e)
         { return std::to_string(static_cast<const PacMan *>(e)->degrees); }},
        {"what", "0",
         [](Object *e, const std::string &v)
         { static_cast<PacMan *>(e)->what = std::stof(v); },
         [](const Object *e)
         { return std::to_string(static_cast<const PacMan *>(e)->what); }}};
}