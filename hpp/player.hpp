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

private:
Inventory *inventory;
bool *gameover;
    void handleInput();
    void updateAnimation();
    void checkBounds(const sf::Vector2u& screenres);

    float gravity;
    float jumpForce;
    float moveSpeed;
    bool isGrounded;
    void loadAnimations();
    void loadShaders();

    //some item and attack related that has something to do with the items
    bool onehitinvin, gothitinv, isStasis;
    sf::Shader stasishad;
};