#include "../hpp/libs.hpp"
class GameMap;

Boss::Boss(const sf::Vector2f& initialPosition)
    : Sprite(initialPosition)
    , CollisionDetector()
{
    loadAndScaleImage();
    resetTimers();
}

void Boss::loadAndScaleImage() {
    static const char* BOSS_TEXTURE_PATH = "../imgs/ikeaman.png";
    static const char* EYE_TEXTURE_PATH = "../imgs/redeye.png";
    
    loadTexture(BOSS_TEXTURE_PATH);
    sprite.setOrigin(103, 73);

    if (!eyeTexture.loadFromFile(EYE_TEXTURE_PATH)) {
        throw std::runtime_error("Failed to load eye texture");
    }
    eyeSprite.setTexture(eyeTexture);
    updateEyePosition();
}

void Boss::update(float deltaTime, const sf::Vector2u& screenres) {
    if (!hasAppearedOnScreen) {
        hasAppearedOnScreen = isOnScreen();
        if (!hasAppearedOnScreen) return;
    }
    
    if (!world || !world->isPlayerValid) return;

    // Get player center position
    const auto& playerBounds = world->playerRef->getBounds();
    sf::Vector2f playerCenter{
        playerBounds.left + playerBounds.width * 0.5f,
        playerBounds.top + playerBounds.height * 0.5f
    };

    // Calculate rotation point (the origin we set at 103, 73)
    sf::Vector2f rotationPoint = position;

    // Calculate angle from rotation point to player
    float dx = playerCenter.x - rotationPoint.x;
    float dy = playerCenter.y - rotationPoint.y;
    float angle = std::atan2(dy, dx) * 180 / PI;

    // Set the rotation
    sprite.setRotation(angle);

    // Calculate movement direction
    sf::Vector2f direction{dx, dy};
    float distance = std::hypot(direction.x, direction.y);
    
    if (distance > 0.0f) {
        direction /= distance;
    }

    // Apply slower vertical movement
    direction.y *= Y_MOVEMENT_FACTOR;

    // Calculate and apply movement
    float movementSpeed = BASE_SPEED + (world->playerRef->isMoving ? CHASE_SPEED : 0.0f);
    sf::Vector2f movement = direction * movementSpeed * deltaTime;
    
    move(movement);
    position += movement;
    updateEyePosition();

    // Handle phases and attacks
    sf::Color currentColor = sprite.getColor();
    float healthPercentage = (currentColor.r + currentColor.g + currentColor.b) / (3.0f * 255.0f);
    
    updatePhase(healthPercentage);
    handleAttacks();
}

void Boss::updatePhase(float healthPercentage) noexcept {
    if (healthPercentage <= 0.3f) {
        currentPhase = 3;
    } else if (healthPercentage <= 0.6f) {
        currentPhase = 2;
    }
}

void Boss::handleAttacks() noexcept {
    if (!world) return;

    const auto& playerBounds = world->playerRef->getBounds();
    float playerCenterX = playerBounds.left + playerBounds.width * 0.5f;

    switch (currentPhase) {
        case 1:
            if (ltimer.getElapsedTime().asSeconds() >= 4.0f) {
                world->spawn("laser", position.x, position.y, sprite.getRotation());
                ltimer.restart();
            }
            break;

        case 2:
            if (ltimer.getElapsedTime().asSeconds() >= 0.7f) {
                world->spawn("laser", position.x, position.y, sprite.getRotation());
                ltimer.restart();
            }
            if (ttimer.getElapsedTime().asSeconds() >= 0.7f) {
                world->spawn("table", playerCenterX, world->getPartBounds().top, sprite.getRotation());
                ttimer.restart();
            }
            break;

        case 3:
            if (ltimer.getElapsedTime().asSeconds() >= 0.12f) {
                world->spawn("laser", position.x, position.y, sprite.getRotation());
                ltimer.restart();
            }
            if (ttimer.getElapsedTime().asSeconds() >= 0.6f) {
                world->spawn("table", playerCenterX, world->getPartBounds().top, sprite.getRotation(),sf::Vector2f(0, 1000));
                ttimer.restart();
            }
            break;
    }
}

void Boss::draw(sf::RenderWindow& window) const {
    Sprite::draw(window);
    window.draw(eyeSprite);
}

void Boss::resetTimers() noexcept {
    ttimer.restart();
    ltimer.restart();
    atimer.restart();
    ptimer.restart();
}

void Boss::updateEyePosition() noexcept {
    eyeSprite.setPosition(
        position.x - eyeSprite.getGlobalBounds().width * 0.5f,
        position.y - eyeSprite.getGlobalBounds().height * 0.5f
    );
}

void Boss::onCollision(Sprite* other) {
    if (auto* attack = dynamic_cast<akBullet*>(other)) {
        sf::Color color = sprite.getColor();
        
        // Subtract 2 from each component using bit manipulation for efficiency
        color.r = std::max(0, static_cast<int>(color.r) - 2);
        color.g = std::max(0, static_cast<int>(color.g) - 2);
        color.b = std::max(0, static_cast<int>(color.b) - 2);

        sprite.setColor(color);

        // Check if all components are 0 using bitwise operations
        if (!(color.r | color.g | color.b)) {
            shouldBeDead = true;
        }
    }
}