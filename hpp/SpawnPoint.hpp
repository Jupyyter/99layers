#pragma once
class SpawnPoint : public Npc {
public:
    SpawnPoint(sf::Vector2f position);
private:
    void loadSprite();
    void handleTextBoxInput() override;
};