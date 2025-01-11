#include "../hpp/libs.hpp"

sign0::sign0(sf::Vector2f position)
    : Sprite(position)
{
    priorityLayer = 1;
    loadTexture("../imgs/sign0.png");
}
sign1::sign1(sf::Vector2f position)
    : Sprite(position), what(0), activated(false), InteractiveObject("../imgs/flowey.png")
// textBox(new TextBox(sf::Vector2f(world->wndref.getPosition().x/2, 657), "", 0.007f, "../imgs/sign1.png"))
{
    priorityLayer = 1;
    loadTexture("../imgs/sign1.png");
    world->spawn(textBox);
}
void sign1::update(float deltaTime, const sf::Vector2u &screenres)
{
    textBox->update(deltaTime, screenres);
    InteractiveObject::update(deltaTime, screenres);

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
            switch (what)
            {
            case 0:
                world->spawn("boomerangg", position.x, position.y);
                break;

            case 1:
            {
                sf::Vector2f playerCenter(
                    world->playerRef->getBounds().left + world->playerRef->getBounds().width / 2.0f,
                    world->playerRef->getBounds().top + world->playerRef->getBounds().height / 2.0f);
                world->spawn("table", position.x, world->getPartBounds().top, sprite.getRotation(), sf::Vector2f(0, 2200));
                break;
            }
            case 2:
                world->spawn("boomerangg2", position.x, position.y);
                break;
            case 3:
            {
                sf::Vector2f playerCenter(
                    world->playerRef->getBounds().left + world->playerRef->getBounds().width / 2.0f,
                    world->playerRef->getBounds().top + world->playerRef->getBounds().height / 2.0f);
                world->spawn("table", position.x, world->getPartBounds().top, sprite.getRotation(), sf::Vector2f(0, 1100));
                break;
            }
            default:
                std::cerr << "Unhandled 'what' value: " << what << std::endl;
                break;
            }

            activated = true;
        }
    }
}

void sign1::updateInteraction(float deltaTime)
{
    // Any continuous interaction logic can go here
}

bool sign1::shouldEndInteraction() const
{
    if (!world->isPlayerValid)
        return true;

    sf::FloatRect npcBounds = getBounds();
    sf::FloatRect playerBounds = world->playerRef->getBounds();
    return !npcBounds.intersects(playerBounds);
}

std::vector<PropertyDescriptor> sign1::getPropertyDescriptors()
{
    return {
        {"text", "",
         [](Object *e, const std::string &v)
         { static_cast<sign1 *>(e)->text = v; },
         [](const Object *e)
         { return static_cast<const sign1 *>(e)->text; }},
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