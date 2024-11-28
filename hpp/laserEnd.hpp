#pragma once

class LaserEnd : public Animation {
public:
    LaserEnd(const sf::Vector2f &initialPosition);
    virtual ~LaserEnd() = default;

    void update(float deltaTime, const sf::Vector2u& screenres) override;

private:
    bool animationStarted;
    bool animationCompleted;
};