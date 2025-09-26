#include "../hpp/libs.hpp"
Npc::Npc(sf::Vector2f position, float frameInterval, const std::string &imgPath)
    : Animation(position, frameInterval), 
      InteractiveObject(imgPath),
      gravity(980.0f),
      wasKeyPressed(false)
{
    priorityLayer = 1;
    world->spawn(textBox);
}

void Npc::update(float deltaTime, const sf::Vector2u& screenres)
{
    if (isOnScreen())
    {
        velocity.y += gravity * deltaTime;
        Animation::update(deltaTime, screenres);
        InteractiveObject::update(deltaTime, screenres);
        handleTextBoxInput();
    }
}

void Npc::updateInteraction(float deltaTime)
{
}

bool Npc::shouldEndInteraction() const
{
    if (!world->isPlayerValid) return true;
    
    sf::FloatRect npcBounds = getBounds();
    sf::FloatRect playerBounds = world->playerRef->getBounds();
    return !npcBounds.intersects(playerBounds);
}

void Npc::handleTextBoxInput()
{
    bool isQPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
    
    if (isQPressed && !wasKeyPressed && isInteracting && textBox->isVisible())
    {
        if (!textBox->isFullyDisplayed()) {
            textBox->showFullText();
        }
    }
    
    wasKeyPressed = isQPressed;
}

void Npc::draw(sf::RenderWindow& window) const
{
    Animation::draw(window);
    textBox->draw(window);
}

std::vector<PropertyDescriptor> Npc::getPropertyDescriptors() {
    return {
        {"text", "",
            [](Object* e, const std::string& v) { static_cast<Npc*>(e)->text = v; },
            [](const Object* e) { return static_cast<const Npc*>(e)->text; }
        },
        {"extraWidth", "0",
            [](Object* e, const std::string& v) { static_cast<Npc*>(e)->extraWidth = std::stof(v); },
            [](const Object* e) { return std::to_string(static_cast<const Npc*>(e)->extraWidth); }
        },
        {"extraHeight", "0",
            [](Object* e, const std::string& v) { static_cast<Npc*>(e)->extraHeight = std::stof(v); },
            [](const Object* e) { return std::to_string(static_cast<const Npc*>(e)->extraHeight); }
        }
    };
}