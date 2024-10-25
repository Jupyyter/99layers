#pragma once

class PacMan : public Animation, public CollisionDetector {
public:
    PacMan(sf::Vector2f spawnPosition);
    ~PacMan();  // Added destructor
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    bool shouldRemove();
    static std::vector<PropertyDescriptor> getPropertyDescriptors();
    float speed = 100.0f;
    float degrees = 0.0f;

private:
void stopSound();
void onCollision(Sprite* other) override;
    bool hasAppearedOnScreen;
    float lifeTimer;
    const float lifeDuration;
    std::string customString;
    float customFloat;
    void loadSprite();
    void updateDirection();

    // Sound related members
    sf::SoundBuffer soundBuffer;
    sf::Sound wakaSound;
    bool isSoundPlaying;
    void initializeSound();
    void updateSound();
};