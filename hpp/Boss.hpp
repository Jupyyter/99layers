#pragma once

class Sprite;
class CollisionDetector;

class Boss final : public Sprite, public CollisionDetector {
public:
    explicit Boss(const sf::Vector2f& initialPosition);
    ~Boss() override = default;

    // Prevent copying
    Boss(const Boss&) = delete;
    Boss& operator=(const Boss&) = delete;

    // Allow moving
    Boss(Boss&&) noexcept = default;
    Boss& operator=(Boss&&) noexcept = default;

    // Override virtual functions
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const override;
    
    // Public interface
    void resetTimers() noexcept;
    [[nodiscard]] inline int getHP() const noexcept { return hp; }
    
private:
    // Constants
    static constexpr float BASE_SPEED = 40.0f;
    static constexpr float CHASE_SPEED = 50.0f;
    static constexpr float Y_MOVEMENT_FACTOR = 0.5f;
    static constexpr int INITIAL_HP = 77;
    static constexpr float PI = 3.14159f;
    
    // Member variables
    sf::Clock atimer;
    sf::Clock ptimer;
    sf::Clock ltimer;
    sf::Clock ttimer;
    sf::Sprite eyeSprite;
    sf::Texture eyeTexture;
    bool hasAppearedOnScreen{false};
    int currentPhase{1};
    float maxHealth{255.0f};
    int hp{INITIAL_HP};

    // Private member functions
    void loadAndScaleImage();
    void updateEyePosition() noexcept;
    void onCollision(Sprite* other) override;
    void updatePhase(float healthPercentage) noexcept;
    void handleAttacks() noexcept;
};