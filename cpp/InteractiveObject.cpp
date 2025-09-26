#include "../hpp/libs.hpp"

InteractiveObject::InteractiveObject(const std::string &imgPath)
    : isInteracting(false),textBox(new TextBox(sf::Vector2f(world->wndref.getPosition().x/2, 657), "", 0.007f, imgPath))
{
}

void InteractiveObject::update(float deltaTime, const sf::Vector2u& screenres)
{
    if (isInteracting)
    {
        if (shouldEndInteraction())
        {
            onInteractionEnd();
            isInteracting = false;
        }
        else
        {
            updateInteraction(deltaTime);
        }
    }
}

void InteractiveObject::onCollision(Sprite* other)
{
    if (typeid(*other) == typeid(Player) && !isInteracting)
    {
        isInteracting = true;
        onInteractionStart();
    }
}