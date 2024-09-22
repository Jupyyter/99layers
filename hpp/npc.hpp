#pragma once
class Npc : public Animation, public CollisionDetector {
public:
    Npc(sf::Vector2f position);
    void update(float deltaTime,GameMap& gamemap, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) override;
    std::vector<std::pair<std::string, std::string>> getEditableProperties() const override;
     void setProperty(const std::string& name, const std::string& value) override;
    std::unique_ptr<TextBox> textBox;
void onCollision(Entity* other) override;
private:
    void loadSprite();
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);
    void checkCollisionWithPlayer(const sf::FloatRect& playerBounds);
    float gravity;
    sf::RenderWindow* window;
    bool isColliding;
    std::string customString;
    std::string text;
};