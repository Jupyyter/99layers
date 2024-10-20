#pragma once
class Animation : public Sprite {
public:
    Animation(const sf::Vector2f& position);
    virtual ~Animation() = default;

    void loadSpritesheet(const std::string& filename, int cellWidth, int cellHeight);
    void addAnimation(const std::string& name, int row, int frameCount);
    void update(float deltaTime, const sf::Vector2u& screenres) override;

    // Set the current animation
    void setAnimation(const std::string& name);

    // Set the time interval between frames
    void setFrameInterval(float interval);
    void pause();
    void resume();

    // Set the current frame of the animation
    void setCurrentFrame(int frame);

    // Check if the animation is paused
    bool isPaused() const;
    sf::FloatRect getBounds() const override {
        // Implement if different from Sprite, otherwise let it use Sprite's implementation
        return Sprite::getBounds();
    }

protected:
    std::map<std::string, std::vector<sf::IntRect>> animations; // Map of animation sequences
    std::string currentAnimation;
    int currentFrame;
    float frameTime;
    float frameInterval;
    int cellWidth;
    int cellHeight;
    bool paused;

    // Update the texture rectangle for the current frame
    void updateTextureRect();
};