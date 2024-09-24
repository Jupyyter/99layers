#include "../hpp/libs.hpp"

AK47::AK47(GameMap &gamemap) : Sprite(), playerBounds(gamemap.playerBounds)
{
       std::cout<<"asasasa";
       loadTexture("../imgs/ak47.png");
       priorityLayer=2;
       // Set the origin to the center of the sprite
       sf::FloatRect bounds = sprite.getLocalBounds();
       sprite.setOrigin(bounds.width / 2, bounds.height / 2);

}
AK47::AK47() : Sprite()
{
       loadTexture("../imgs/ak47.png");

       // Set the origin to the center of the sprite
       sf::FloatRect bounds = sprite.getLocalBounds();
       sprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void AK47::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres)
{
       sf::Vector2i mousePosition = sf::Mouse::getPosition(gamemap.wndref);
       updatePosition();
       if (playerBounds)
       {
              sf::Vector2f gunCenter = getPosition();
              sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
              sf::Vector2f direction = mousePositionF - gunCenter;
              float angle = std::atan2(direction.y, direction.x) * 180 / PI;
              sprite.setRotation(angle);
              // Flip the sprite if the mouse is on the left side of the player
              if (mousePosition.x < gunCenter.x)
              {
                     sprite.setScale(1, -1); // Flip vertically
              }
              else
              {
                     sprite.setScale(1, 1); // Normal orientation
              }
       }
}

void AK47::updatePosition()
{
       if (playerBounds)
       {
              float centerX = playerBounds->left + playerBounds->width / 2;
              float centerY = playerBounds->top + playerBounds->height / 2;
              setPosition(centerX, centerY);
       }
}