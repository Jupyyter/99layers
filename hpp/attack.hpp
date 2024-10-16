#pragma once
#include "sprite.hpp"

class Attack : public Sprite,public CollisionDetector {
public:
    Attack(sf::Vector2f sp);
float timeOffScreen;
sf::Clock offScreenTimer;
    void update(float deltaTime, const sf::Vector2u& screenres) override;
};

class HammerThrow : public Attack {
public:
    HammerThrow(sf::Vector2f sp);

    void update(float deltaTime, const sf::Vector2u& screenres) override;

private:
    sf::Vector2f velocity;
    bool fc;
};

class Plank : public Attack {
public:
    Plank(sf::Vector2f sp);

    void update(float deltaTime, const sf::Vector2u& screenres) override;

    void draw(sf::RenderWindow& window) const override;

private:
void onCollision(Entity *other) override;
    sf::Sprite secondSprite;
    sf::Vector2f velocity;
    sf::Clock timer;
    bool fc;
};

class LaserBeam : public Attack {
public:
    LaserBeam(sf::Vector2f sp, float rotangle);

    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void onCollision(Entity *other) override;

private:
    sf::Vector2f velocity;
    sf::Clock timer;
    bool fc;
};
class akBullet : public Attack {
public:
    akBullet(sf::Vector2f sp, float rotangle);

    void update(float deltaTime, const sf::Vector2u& screenres) override;
void onCollision(Entity *other) override;
    void draw(sf::RenderWindow& window) const override;
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);

private:
    sf::Vector2f velocity;
    sf::Clock timer;
    bool fc;
};
class TableFall : public Attack {
public:
    TableFall(sf::Vector2f sp);

    void update(float deltaTime, const sf::Vector2u& screenres) override;

private:
void onCollision(Entity *other) override;
    sf::Vector2f velocity;
    sf::Clock timer;
    bool fc;
};