#pragma once
class LevelEditor {
private:
    sf::RenderWindow window;
    EditorMap map;
    sf::View view;
    sf::Sprite transrect, entityPreview;
    sf::Vector2i firstClick, secondClick;
    bool leftClickPressed = false;
    sf::Font font;
    sf::Vector2f firstClickWorld, currentMousePosWorld;
    float zoomLevel;
    sf::VertexArray grid;
    sf::Vector2f gridSize;

    void handleEvents();
    void update(float deltaTime);
    void render();
    void handleMouseRelease(const sf::Event& event);
    void handleKeyPress(const sf::Event& event);
    void updateTransrect();
    void updateEntityPreview(const sf::Texture* entityTexture);
    void removeEntityAtMousePosition();
    void openPropertyEditor();

    void handleMousePress(const sf::Event& event);
    void handleMouseMove(const sf::Event& event);
    void handleMouseScroll(const sf::Event& event);
    void updateGrid();

    bool isDraggingCamera;
    sf::Vector2i lastMousePos;

public:
    LevelEditor();
    void run();
};