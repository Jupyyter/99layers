#pragma once

class Penguin : public Animation, public CollisionDetector {
public:
    Penguin(sf::Vector2f position);
    
    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) const  override;


    // New methods specific to Penguin
    void setSpeed(float newSpeed) { speed = newSpeed; }
    float getSpeed() const { return speed; }

    bool isMovingRight() const { return speed > 0; }

private:
    void loadSprite();
     void onCollision(Object* other) override;

    float gravity;
    bool isColliding;
    float speed;
};