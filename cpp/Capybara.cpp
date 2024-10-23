#include "../hpp/libs.hpp"
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