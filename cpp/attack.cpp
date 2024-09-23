#include "../hpp/libs.hpp"

Attack::Attack(sf::Vector2f sp) : Sprite(),offScreenTimer(), timeOffScreen(0.f) {
    setPosition(sp);
}
void Attack::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) {
    if (!isOnScreen(gamemap.getPartBounds())) {
        timeOffScreen += offScreenTimer.restart().asSeconds();
        if (timeOffScreen >= 1.0f) {
            shouldBeDead = true;
        }
    } else {
        timeOffScreen = 0.f;
        offScreenTimer.restart();
    }
}

HammerThrow::HammerThrow(sf::Vector2f sp) : Attack(sp), fc(true) {
    loadTexture("../imgs/hammer.jpg");
    this->sprite.scale(32.0f / texture.getSize().x, 32.0f / texture.getSize().y);
    position.x-=sprite.getGlobalBounds().height;
}

void HammerThrow::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) {
    
}

Plank::Plank(sf::Vector2f sp) : Attack(sp), fc(true) {
    loadTexture("../imgs/plank.png");
    this->sprite.scale(20.0f / texture.getSize().x, 100.0f / texture.getSize().y);
    secondSprite.setTexture(texture);
    secondSprite.setScale(20.0f / texture.getSize().x, 100.0f / texture.getSize().y);
}

void Plank::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) {
    float elapsedTime = timer.getElapsedTime().asSeconds();
    if (elapsedTime < 0.9f) {
        if (fc) {
            setPosition(gamemap.playerBounds->left, gamemap.playerBounds->top);
            fc = false;
        }
        sprite.setPosition(sf::Vector2f(position.x - 5 * gamemap.playerBounds->width, position.y - 20));
        secondSprite.setPosition(sf::Vector2f(position.x + 5 * gamemap.playerBounds->width, position.y - 20));
    } else {
        sprite.setPosition(sf::Vector2f(position.x - 5 * gamemap.playerBounds->width + 500 * (elapsedTime - 0.9f), position.y - 20));
        secondSprite.setPosition(sf::Vector2f(position.x + 5 * gamemap.playerBounds->width - 500 * (elapsedTime - 0.9f), position.y - 20));

        if (sprite.getPosition().x >= position.x)
            shouldBeDead=true;
    }
    if (sprite.getGlobalBounds().intersects(*gamemap.playerBounds) || secondSprite.getGlobalBounds().intersects(*gamemap.playerBounds))
        (*gamemap.gameOver)=true;
}

void Plank::draw(sf::RenderWindow &window)const  {
    Sprite::draw(window);
    window.draw(secondSprite);
}

LaserBeam::LaserBeam(sf::Vector2f sp, float rotangle) : Attack(sp), fc(true) {
    loadTexture("../imgs/laser.png");
    sprite.rotate(90 + rotangle);
    priorityLayer = 2;
}


void LaserBeam::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) {
    Attack::update(deltaTime, gamemap, screenres);  // Call base class update for off-screen check
    
    if (fc) {
        velocity = sf::Vector2f(gamemap.playerBounds->left + gamemap.playerBounds->width / 2.0f, gamemap.playerBounds->top + gamemap.playerBounds->height/ 2.0f) - position;
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity /= length;
        velocity *= 20.0f;
        position += velocity;
        setPosition(position);
        fc = false;
    } else {
        position += velocity;
        setPosition(position);
    }
    
    if (sprite.getGlobalBounds().intersects(*gamemap.playerBounds))
        (*gamemap.gameOver) = true;
}

TableFall::TableFall(sf::Vector2f sp) : Attack(sp) {
    loadTexture("../imgs/table.png");
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y);
    this->sprite.scale(60.0f / texture.getSize().x, 30.0f / texture.getSize().y);
}

void TableFall::update(float deltaTime, GameMap &gamemap, const sf::Vector2u &screenres) {
    Attack::update(deltaTime, gamemap, screenres);  // Call base class update for off-screen check
    
    velocity.y += 0.5f;
    position += velocity;
    setPosition(position);
    
    if (sprite.getGlobalBounds().intersects(*gamemap.playerBounds))
        (*gamemap.gameOver) = true;
}