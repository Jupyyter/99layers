#include "../hpp/libs.hpp"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
Terrain::Terrain(int x, int y, int w, int h, const std::string &tname) : Sprite(sf::Vector2f(x, y)), rotation(0.0f), what(0.0f), isFalling(false), CollisionDetector(), maxFallSpeed(800), verticalVelocity(0), originalPosition(position)
{
    priorityLayer = -1;
    if (!texture.loadFromFile(tname))
    {
        std::cerr << "Failed to load texture: " << tname << std::endl;
    }
    else
    {
        sprite.setTexture(texture);
        // Set texture to repeat
        texture.setRepeated(true);
        // Set the texture rect to tile the texture if necessary
        sprite.setTextureRect(sf::IntRect(0, 0, w, h));

        // Set the origin to the center of the sprite
        // sprite.setOrigin(w / 2.0f, h / 2.0f);
        // Adjust position to account for the new origin
    }
}

void Terrain::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}
void Terrain::onCollision(Sprite *other)
{
    if (what == 1 && typeid(*other) == typeid(Player))
    {
        isFalling = true; // Start falling when colliding with player
    }
}
void Terrain::update(float deltaTime, const sf::Vector2u &screenres)
{
    // sprite.setRotation(sprite.getRotation() + rotation * deltaTime);
    if (what == 1 && isFalling)
    {
        // Update vertical velocity with gravity
        verticalVelocity += gravity * deltaTime;

        // Clamp falling speed to maximum
        if (verticalVelocity > maxFallSpeed)
        {
            verticalVelocity = maxFallSpeed;
        }

        // Update position
        position.y += verticalVelocity * deltaTime;

        // Optional: Remove or reset the terrain if it falls below the screen
        if (!isOnScreen())
        {
            shouldBeDead = true;
        }
    }
    else if (what == 2) {
        // Define parameters for cyclic movement
        const float amplitude = 150.0f;  // Maximum vertical displacement
        const float frequency = 0.2f;    // Cycles per second
        
        // Track time as a member variable
        static float elapsedTime = 0.0f;
        elapsedTime += deltaTime;
        
        // Calculate vertical position using sine wave
        float verticalOffset = amplitude * std::sin(2 * M_PI * frequency * elapsedTime);
        
        // Update sprite position with cyclic movement
        position.y = originalPosition.y + verticalOffset;
        
        // Update sprite position
        sprite.setPosition(position);
    }
}

std::vector<PropertyDescriptor> Terrain::getPropertyDescriptors()
{
    return {
        {"what", "0",
         [](Object *e, const std::string &v)
         { static_cast<Terrain *>(e)->what = std::stof(v); },
         [](const Object *e)
         { return std::to_string(static_cast<const Terrain *>(e)->what); }},
    };
}