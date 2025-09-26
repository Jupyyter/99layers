#pragma once

class PacMan : public Animation, public CollisionDetector {
public:
    PacMan(sf::Vector2f spawnPosition);
    ~PacMan();
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    bool shouldRemove();
    static std::vector<PropertyDescriptor> getPropertyDescriptors();
    
    float speed = 100.0f;
    float degrees = 0.0f;
    int what = 0;

private:
    void stopSound();
    void onCollision(Sprite* other) override;
    bool hasAppearedOnScreen;
    float lifeTimer;
    float lifeDuration;
    
    void loadSprite();
    void updateDirection();
    void updateDirectionToPlayer(sf::Vector2f playerPos, float deltaTime);
    
    // Sound related members
    sf::SoundBuffer soundBuffer;
    sf::Sound wakaSound;
    bool isSoundPlaying;
    void initializeSound();
    void updateSound();

    // Player tracking members
    sf::Vector2f playerPosition;
    float baseSpeed = 100.0f;
    float minSpeed = 50.0f;
    float maxSpeed = 225.0f;
    float rotationSpeed = 180.0f;  // degrees per second

    sf::Color startColor;  // Starting color (yellow)
    sf::Color endColor;    // End color (purple)
};
