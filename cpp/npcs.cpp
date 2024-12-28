#include "../hpp/libs.hpp"
void woodSign::loadSprite()
{
    loadTexture("../imgs/woodSign.png");
}
woodSign::woodSign(sf::Vector2f position)
    : Npc(position,0.1,"../imgs/woodSign.png")
{
    priorityLayer = 1;
    loadSprite();
}

void Capybara::loadSprite()
{
    loadSpritesheet("../imgs/capybara.png", 64, 64);
    addAnimation("idle", 1, 8);
    setAnimation("idle");
}
Capybara::Capybara(sf::Vector2f position)
    : Npc(position,0.1,"../imgs/capybaraa.png")
{
    priorityLayer = 1;
    loadSprite();
}

void crow::loadSprite()
{
    loadSpritesheet("../imgs/idleCrow.png", 28, 32);
    addAnimation("idle", 0, 4);
    setAnimation("idle");
    scale(2,2);
}
crow::crow(sf::Vector2f position)
    : Npc(position,0.1,"../imgs/croww.png")
{
    priorityLayer = 1;
    loadSprite();
}