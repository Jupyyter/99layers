#include "../hpp/libs.hpp"

AK47::AK47() : Sprite(sf::Vector2f(0, 0)), playerRef(world->playerRef)
{
    loadTexture("../imgs/ak47.png");
    priorityLayer = 5;
    // Set the origin to the center of the sprite
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
}
void AK47::update(float deltaTime, const sf::Vector2u &screenres) {
    if(!world->isPlayerValid){
        shouldBeDead=true;
    }
    else{
        sf::Vector2i mousePosition = sf::Mouse::getPosition(world->wndref);
        updatePosition();
        if (playerRef) {
            // Get the current view
            sf::View currentView = world->wndref.getView();
            
            // Convert mouse position to world coordinates
            sf::Vector2f worldMousePos = world->wndref.mapPixelToCoords(mousePosition, currentView);
            sf::Vector2f gunCenter = getPosition();
            sf::Vector2f direction = worldMousePos - gunCenter;
            float angle = std::atan2(direction.y, direction.x) * 180 / PI;
            sprite.setRotation(angle);
            // Flip the sprite if the mouse is on the left side of the player
            if (worldMousePos.x < gunCenter.x) {
                sprite.setScale(1, -1); // Flip vertically
            } else {
                sprite.setScale(1, 1); // Normal orientation
            }
            // Check if left mouse button is pressed
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                // Check if enough time has passed since the last shot
                if (shootCooldown.getElapsedTime().asSeconds() >= shootCooldownTime) {
                    // Spawn a bullet
                    world->spawn("akBullet", gunCenter.x, gunCenter.y, sprite.getRotation());
                    // Reset the cooldown timer
                    shootCooldown.restart();
                }
            }
        }
    }
}

void AK47::updatePosition()
{
    if (playerRef)
    {
        position = sf::Vector2f(world->playerRef->getBounds().left + playerRef->getBounds().width / 2,
                                world->playerRef->getBounds().top + playerRef->getBounds().height / 2);
    }
}