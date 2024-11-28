#include "../hpp/libs.hpp"

Terrain::Terrain(int x, int y, int w, int h, const std::string& tname) : Sprite(sf::Vector2f(x ,y)), rotation(0.0f),what(0.0f),isFalling(false), CollisionDetector(),maxFallSpeed(800),verticalVelocity(0)
{
    priorityLayer = -1;
    if (!texture.loadFromFile(tname)) {
        std::cerr << "Failed to load texture: " << tname << std::endl;
    } else {
        sprite.setTexture(texture);
        // Set texture to repeat
        texture.setRepeated(true);
        // Set the texture rect to tile the texture if necessary
        sprite.setTextureRect(sf::IntRect(0, 0, w, h));
        
        // Set the origin to the center of the sprite
        //sprite.setOrigin(w / 2.0f, h / 2.0f);
    // Adjust position to account for the new origin
    }
    
}

void Terrain::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}
void Terrain::onCollision(Sprite *other)
{
    if(what == 1 && typeid(*other) == typeid(Player)) {
            isFalling = true; // Start falling when colliding with player
        }
}
void Terrain::update(float deltaTime, const sf::Vector2u& screenres)
{
    //sprite.setRotation(sprite.getRotation() + rotation * deltaTime);
    if (isFalling) {
            // Update vertical velocity with gravity
            verticalVelocity += gravity * deltaTime;
            
            // Clamp falling speed to maximum
            if (verticalVelocity > maxFallSpeed) {
                verticalVelocity = maxFallSpeed;
            }
            
            // Update position
            position.y += verticalVelocity * deltaTime;
            
            // Optional: Remove or reset the terrain if it falls below the screen
            if (!isOnScreen()) {
                shouldBeDead=true;
            }
        }
}

std::vector<PropertyDescriptor> Terrain::getPropertyDescriptors() {
    return {
        {"what", "0",
            [](Object* e, const std::string& v) { static_cast<Terrain*>(e)->what = std::stof(v); },
            [](const Object* e) { return std::to_string(static_cast<const Terrain*>(e)->what); }
        },
    };
}