#include "../hpp/libs.hpp"

LaserEnd::LaserEnd(const sf::Vector2f &initialPosition) : Animation(), animationStarted(false), animationCompleted(false)
{
    priorityLayer = 6;
    shouldBeDead = false;
    setPosition(initialPosition);
    loadSpritesheet("../imgs/LaserEnd.png", 32, 96);  // Assuming the spritesheet is 32x96

    // Randomly choose one of the three animations
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);
    int randomRow = dis(gen);

    // Add a single animation, using the randomly chosen row
    addAnimation("go", randomRow, 6);
    setAnimation("go");

    sprite.setOrigin(16, 16);
}

void LaserEnd::update(float deltaTime, GameMap& gamemap, const sf::Vector2u& screenres)
{
    Animation::update(deltaTime, gamemap, screenres);
    // Check if the animation has completed one full cycle
    if (currentFrame == 5)  // Last frame is always 5 for all rows
    {
        if (animationCompleted)
        {
            animationCompleted = true;
            shouldBeDead = true;
        }
        animationCompleted = true;
    }
}