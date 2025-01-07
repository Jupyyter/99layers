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
    bool fc;
};

class Plank : public Attack {
public:
    Plank(sf::Vector2f sp);

    void update(float deltaTime, const sf::Vector2u& screenres) override;

    void draw(sf::RenderWindow& window) const override;

private:
    sf::Sprite secondSprite;
    sf::Clock timer;
    bool fc;
};

class LaserBeam : public Attack {
public:
    LaserBeam(sf::Vector2f sp, float rotangle);

    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void onCollision(Sprite *other) override;

private:
    sf::Clock timer;
    bool fc;
};
class akBullet : public Attack {
public:
    akBullet(sf::Vector2f sp, float rotangle);

    void update(float deltaTime, const sf::Vector2u& screenres) override;
void onCollision(Sprite *other) override;
    void draw(sf::RenderWindow& window) const override;

private:
    sf::Clock timer;
    bool fc;
};
class TableFall : public Attack {
public:
    TableFall(sf::Vector2f sp);

    void update(float deltaTime, const sf::Vector2u& screenres) override;

private:
    sf::Clock timer;
    bool fc;
};
class Boomerang : public Attack {
private:
    float rotationSpeed;
    float timeElapsed;
    sf::Vector2f initialVelocity;
    bool returningPhase;

public:
    Boomerang(sf::Vector2f sp) ;

    void update(float deltaTime, const sf::Vector2u &screenres) override;
};
class Boomerang2 : public Attack {
private:
    float timeElapsed;
    float timeSinceLastRedirect;
    float rotationSpeed;
    void updateDirectionToPlayer();
public:
    Boomerang2(sf::Vector2f sp);
    void update(float deltaTime, const sf::Vector2u &screenres) override;
};