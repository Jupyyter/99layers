#pragma once
#include "animation.hpp"

class HappyEnd : public Animation {
public:
    HappyEnd(const sf::Vector2f &initialPosition);
    virtual ~HappyEnd() = default;

    void update(float deltaTime, GameMap& gamemap, const sf::Vector2u& screenres) override;

private:
    bool animationStarted;
    bool animationCompleted;
};