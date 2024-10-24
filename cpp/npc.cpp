#include "../hpp/libs.hpp"
Npc::Npc(sf::Vector2f position,float frameInterval,const std::string &imgPath)
    : Animation(position,frameInterval), CollisionDetector(), gravity(980.0f),textBox(new TextBox(sf::Vector2f(world->wndref.getPosition().x/2, 657),"", 0.007f,imgPath)), isInteracting(false)
{
    std::cout<<imgPath<<" ";
    priorityLayer = 1;
    world->spawn(textBox);
}

void Npc::update(float deltaTime, const sf::Vector2u& screenres)
{
    if (isOnScreen())
    {
        velocity.y += gravity * deltaTime;
        position.y += velocity.y * deltaTime;
        Animation::update(deltaTime, screenres);
        
        // Check if we're still colliding with the player
        if (isInteracting)
        {
            sf::FloatRect npcBounds = getBounds();
            sf::FloatRect playerBounds = world->playerRef->getBounds();
            if (!npcBounds.intersects(playerBounds))
            {
                isInteracting = false;
                textBox->setString("");
            }
        }
    }
}


void Npc::draw(sf::RenderWindow& window) const
{
    Animation::draw(window);
    // Draw textbox at its screen position
    textBox->draw(window);
}

void Npc::onCollision(Object* other)
{
    // Handle terrain collisions
    if (typeid(*other) == typeid(Terrain))
    {
        switch (CollisionDetector::CollisionSide(getBounds(), other->getBounds()))
        {
            case CollisionInfo::Left:
                if (!(other->getBounds().top > getBounds().top && 
                    other->getBounds().top - getBounds().top > 27 && velocity.y >= 0))
                {
                    position.x = other->getBounds().left + other->getBounds().width;
                    velocity.x = 0;
                }
                break;
            case CollisionInfo::Right:
                if (!(other->getBounds().top > getBounds().top && 
                    other->getBounds().top - getBounds().top > 27 && velocity.y >= 0))
                {
                    position.x = other->getBounds().left - getBounds().width;
                    velocity.x = 0;
                }
                break;
            case CollisionInfo::Bottom:
                if (velocity.y >= 0)
                {
                    position.y = other->getBounds().top - getBounds().height;
                    velocity.y = 0;
                }
                break;
            case CollisionInfo::Top:
                position.y = other->getBounds().top + other->getBounds().height;
                velocity.y = 0;
                break;
        }
    }
    // Handle player collision
    else if (typeid(*other) == typeid(Player))
    {
        if (!isInteracting)
        {
            textBox->setString(text);
            isInteracting = true;
        }
    }
}
std::vector<PropertyDescriptor> Npc::getPropertyDescriptors() {
    return {
        {"text", "",
            [](Object* e, const std::string& v) { static_cast<Npc*>(e)->text = v; },
            [](const Object* e) { return static_cast<const Npc*>(e)->text; }
        }
    };
}