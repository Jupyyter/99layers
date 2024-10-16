#pragma once

class LevelEditor {
private:
    sf::RenderWindow window;
    EditorMap map;
    sf::View view;
    sf::Sprite transrect, entityPreview;
    sf::Vector2i firstClick, secondClick;
    bool leftClickPressed = false, placingTexture = false;
    sf::Font font;

    void handleEvents();
    void update(float deltaTime);
    void render();
    void handleMouseClick(const sf::Event& event);
    void handleMouseRelease(const sf::Event& event);
    void handleKeyPress(const sf::Event& event);
    void updateTransrect();
    void updateEntityPreview(const sf::Texture* entityTexture);
    void removeEntityAtMousePosition();
    void openPropertyEditor();

public:
    LevelEditor();
    void run();
};

sf::Vector2f getEntityOrigin(const sf::Sprite& sprite);