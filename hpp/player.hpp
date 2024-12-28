#pragma once
class Animation;
class Item;
class Inventory;

class Player : public Animation, public CollisionDetector {
    friend class Item;
public:
    Player(sf::Vector2f position);
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const  override;
    sf::FloatRect getBounds() const override {
        // Implement if different from Sprite, otherwise let it use Sprite's implementation
        return Animation::getBounds();
    }
    void onCollision(Sprite* other) override;
    bool isMoving;
    bool isJump;
    bool wasUpPressed;
void checkBounds(const sf::Vector2u& screenres);
Inventory *inventory;
private:
bool *gameover;
    void handleInput();
    void updateAnimation();

    float gravity;
    float jumpForce;
    float moveSpeed;
    void loadAnimations();
    void loadShaders();
    void checkContinuousCollision(const sf::Vector2f& start, const sf::Vector2f& end);
    float rayIntersectsRect(const sf::Vector2f& rayOrigin, const sf::Vector2f& rayDirection, const sf::FloatRect& rect);

    //some item and attack related that has something to do with the items
    bool onehitinvin, gothitinv, isStasis;
    sf::Shader stasishad;
};