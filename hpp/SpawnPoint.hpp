#pragma once
class SpawnPoint : public Npc {
public:
    SpawnPoint(sf::Vector2f position);
private:
    void loadSprite();
    // Override the handleTextBoxInput method
    void handleTextBoxInput() override;
};