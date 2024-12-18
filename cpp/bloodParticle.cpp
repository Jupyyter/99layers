#include "../hpp/libs.hpp"
#define M_PI 3.14159265358979323846
// Constructor - initialize random movement
bloodParticle::bloodParticle(sf::Vector2f position) : Sprite(position), CollisionDetector() {
       priorityLayer=2;
    loadSprite();
    spawnParticles(position);
}
void bloodParticle::spawnParticles(sf::Vector2f position) {
    // Random angle between 0 and 360 degrees
    float angle = static_cast<float>(rand() % 360) * M_PI / 180.0f;
    
    // Random speed between 100 and 300
    float speed = 177.0f + static_cast<float>(rand() % 301);  // 100 to 300
    
    // Set initial velocity based on angle and speed
    velocity.x = std::cos(angle) * speed;
    velocity.y = std::sin(angle) * speed;
}
void bloodParticle::update(float deltaTime, const sf::Vector2u& windowSize) {
    const float GRAVITY = 687.0f;  // Gravity constant (pixels/second^2)
    
    // Apply gravity
    velocity.y += GRAVITY * deltaTime;
    
    // Update position based on velocity
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    // Calculate rotation angle based on velocity direction
    /*if (std::abs(velocity.x) > 0.1f || std::abs(velocity.y) > 0.1f) {
        float angle = std::atan2(velocity.y, velocity.x);
        // Convert radians to degrees and set rotation
        sprite.setRotation(angle * 180.0f / M_PI);
    }*/
    
    // Optional: Add some drag/air resistance
    const float DRAG = 0.98f;
    velocity.x *= DRAG;
    velocity.y *= DRAG;

}
void bloodParticle::onCollision(Sprite *other)
{
        if (typeid(*other) == typeid(Terrain))
       {
              switch (CollisionDetector::CollisionSide(getBounds(), other->getBounds()))
              {
              case CollisionInfo::Left:
                     if (!(other->getBounds().top > getBounds().top && other->getBounds().top - getBounds().top > 27 && velocity.y >= 0)) // in case of stairs
                     {
                            position.x = other->getBounds().left + other->getBounds().width;
                            velocity.x = 0;
                     }
                     break;
              case CollisionInfo::Right:
                     if (!(other->getBounds().top > getBounds().top && other->getBounds().top - getBounds().top > 27 && velocity.y >= 0))
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
                            velocity.x *= 0.5;
                     }
                     break;
              case CollisionInfo::Top:
              {
                     position.y = other->getBounds().top + other->getBounds().height;
                     velocity.y = 0;
                     velocity.x *= 0.5;

                     break;
              }
              default:
                     break;
              }
       }
}
void bloodParticle::loadSprite() {
    loadTexture("../imgs/bloodParticles.png");
}