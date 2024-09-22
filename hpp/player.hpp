#pragma once
class Animation;
class Item;
class Inventory;

class Player : public Animation, public CollisionDetector {
    friend class Item;
public:
    Player(sf::Vector2f position,GameMap& gamemap);
    Player(sf::Vector2f position);
    void update(float deltaTime, GameMap& gamemap, const sf::Vector2u& screenres) override;
    sf::FloatRect place;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override {
        // Implement if different from Sprite, otherwise let it use Sprite's implementation
        return Animation::getBounds();
    }
    void onCollision(Entity* other) override;

private:
Inventory *inventory;
bool *gameover;
    void handleInput();
    void updateAnimation();
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);
    void checkBounds(const sf::Vector2u& screenres, GameMap& gamemap);

    float gravity;
    float jumpForce;
    float moveSpeed;
    bool isGrounded;
    void loadAnimations();
    void loadShaders();

    //some item and attack related bullshit
    bool onehitinvin, gothitinv, isStasis;
    sf::Shader stasishad;
};