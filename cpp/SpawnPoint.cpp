#include "../hpp/libs.hpp"

void SpawnPoint::loadSprite()
{
    loadSpritesheet("../imgs/savePoint.png", 32, 32);
    addAnimation("idle", 0, 2);
    setAnimation("idle");
}

SpawnPoint::SpawnPoint(sf::Vector2f position)
    : Npc(position, 0.3, "../imgs/spawn.png")
{
    priorityLayer = 1;
    loadSprite();
}

void SpawnPoint::handleTextBoxInput()
{
    bool isQPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
    
    if (isQPressed && !wasKeyPressed && isInteracting && textBox->isVisible())
    {
        if (!textBox->isFullyDisplayed()) {
            textBox->showFullText();
        }
        else if (!textBox->isHighlighted()) {
            textBox->setHighlighted(true);
            textBox->playSound();
            world->playerSpawnPosition=position;
            world->playerRef->inventory->saveItems();
        }
    }
    
    wasKeyPressed = isQPressed;
}