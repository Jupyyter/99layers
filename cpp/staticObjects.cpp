#include "../hpp/libs.hpp"

sign0::sign0(sf::Vector2f position)
    : Sprite(position)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign0.png");
}
sign1::sign1(sf::Vector2f position)
    : Sprite(position),what(0)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign1.png");
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