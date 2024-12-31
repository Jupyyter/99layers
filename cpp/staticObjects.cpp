#include "../hpp/libs.hpp"

sign0::sign0(sf::Vector2f position)
    : Sprite(position)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign0.png");
}
sign1::sign1(sf::Vector2f position)
    : Sprite(position), what(0), activated(false)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign1.png");
}
void sign1::update(float deltaTime, const sf::Vector2u &screenres)
{
    if (!activated)
    {
        // Get positions
        sf::Vector2f playerPos = world->playerRef->getPosition();
        sf::Vector2f signPos = this->getPosition();

        // Define detection range for x-axis (adjust these values as needed)
        float xRange = 50.0f; // How close the player needs to be horizontally

        // Check if player is within horizontal range and above the sign
        if (std::abs(playerPos.x - signPos.x) < xRange && playerPos.y < signPos.y)
        {
            std::cout << "Player is near and above the sign!" << std::endl;
            activated = true;
            world->spawn("boomerangg", position.x, position.y);
        }
    }
}
std::vector<PropertyDescriptor> sign1::getPropertyDescriptors()
{
    return {
        {"what", "0",
         [](Object *e, const std::string &v)
         { static_cast<sign1 *>(e)->what = std::stof(v); },
         [](const Object *e)
         { return std::to_string(static_cast<const sign1 *>(e)->what); }}};
}
sign2::sign2(sf::Vector2f position)
    : Sprite(position)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign2.png");
}