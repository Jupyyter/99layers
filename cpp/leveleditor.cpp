#include "../hpp/libs.hpp"
#include <SFML/System/Clock.hpp>
#include <thread>
#include <chrono>
sf::Vector2f getEntityOrigin(const sf::Sprite& sprite) {
    const sf::FloatRect& bounds = sprite.getLocalBounds();
    return sf::Vector2f(bounds.width / 2, bounds.height / 2);
}

LevelEditor::LevelEditor() 
    : window(sf::VideoMode(1024, 768), "Level Editor", sf::Style::Titlebar | sf::Style::Close),
      map(window), view(sf::FloatRect(0, 0, 1024, 768)),
      zoomLevel(1.0f), gridSize(1024, 768) {
    window.setVerticalSyncEnabled(true);
    window.setView(view);
    map.loadFromFile("../map.mib");

    transrect.setColor(sf::Color(255, 255, 255, 128));
    if (const sf::Texture* initialTexture = map.menu.textures.empty() ? nullptr : &map.menu.textures[0])
        transrect.setTexture(*initialTexture, true);

    updateEntityPreview(map.menu.entityTextures.empty() ? nullptr : &map.menu.entityTextures[0]);

    if (!font.loadFromFile("../fonts/Arial.ttf"))
        throw std::runtime_error("Failed to load font!");

    map.propertyEditor.setup(font);
    isDraggingCamera = false;

    updateGrid();
}
void LevelEditor::updateGrid() {
    grid = sf::VertexArray(sf::Lines);
    sf::Color gridColor(128, 128, 128, 64);
    
    // Get the current view bounds
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    float gridHorizontalSpacing = 1024.0f; // Spacing between grid lines
    float gridVerticalSpacing = 768.0f;
    // Calculate grid boundaries that extend well beyond the view
    float startX = viewCenter.x - viewSize.x * 10;
    float endX = viewCenter.x + viewSize.x * 10;
    float startY = viewCenter.y - viewSize.y * 10;
    float endY = viewCenter.y + viewSize.y * 10;
    
    // Draw vertical lines
    for (float x = startX - fmod(startX, gridHorizontalSpacing); x <= endX; x += gridHorizontalSpacing) {
        grid.append(sf::Vertex(sf::Vector2f(x, startY), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(x, endY), gridColor));
    }
    
    // Draw horizontal lines
    for (float y = startY - fmod(startY, gridVerticalSpacing); y <= endY; y += gridVerticalSpacing) {
        grid.append(sf::Vertex(sf::Vector2f(startX, y), gridColor));
        grid.append(sf::Vertex(sf::Vector2f(endX, y), gridColor));
    }
}

void LevelEditor::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                handleKeyPress(event);
                break;
            case sf::Event::MouseButtonPressed:
                handleMousePress(event);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseRelease(event);
                break;
            case sf::Event::MouseMoved:
                handleMouseMove(event);
                break;
            case sf::Event::MouseWheelScrolled:
                handleMouseScroll(event);
                break;
            default:
                break;
        }
        map.propertyEditor.handleInput(event, window);
    }
}

void LevelEditor::handleMouseScroll(const sf::Event& event) {
    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        float zoomFactor = 1.1f;
        if (event.mouseWheelScroll.delta > 0)
            zoomLevel *= zoomFactor;
        else if (event.mouseWheelScroll.delta < 0)
            zoomLevel /= zoomFactor;

        zoomLevel = std::max(0.1f, std::min(zoomLevel, 10.0f));
        view.setSize(window.getDefaultView().getSize());
        view.zoom(1.f / zoomLevel);
        window.setView(view);
        updateGrid(); // Update grid when zooming
    }
}
void LevelEditor::handleKeyPress(const sf::Event& event) {
    if (event.key.code == sf::Keyboard::Escape) {
        // Close any open menus
        map.menu.isOpen = false;
        map.propertyEditor.isOpen = false;
        return;
    }

    if (map.propertyEditor.isOpen) {
        if (event.key.code == sf::Keyboard::Enter)
            map.propertyEditor.applyChanges();
        return;
    }

    switch (event.key.code) {
        case sf::Keyboard::E:
            map.menu.isOpen = !map.menu.isOpen;
            break;
        case sf::Keyboard::S:
            map.saveToFile("../map.mib");
            break;
        case sf::Keyboard::D:
            removeEntityAtMousePosition();
            break;
        default:
            break;
    }
}

void LevelEditor::updateEntityPreview(const sf::Texture* entityTexture) {
    if (!entityTexture) return;
    
    entityPreview.setTexture(*entityTexture, true);
    entityPreview.setOrigin(getEntityOrigin(entityPreview));

    const float maxPreviewSize = 64.0f;
    sf::Vector2f scale(std::min(maxPreviewSize / entityPreview.getLocalBounds().width, 1.0f),
                       std::min(maxPreviewSize / entityPreview.getLocalBounds().height, 1.0f));
    entityPreview.setScale(scale);
    entityPreview.setColor(sf::Color(255, 255, 255, 192));
}

void LevelEditor::removeEntityAtMousePosition() {
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (int i = map.placedEntities.size() - 1; i >= 0; --i) {
        if (map.placedEntities[i].sprite.getGlobalBounds().contains(worldPos)) {
            map.removeEntity(i);
            break;
        }
    }
}

void LevelEditor::handleMousePress(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (map.handleMenuClick(mousePos)) {
            if (map.menu.isEntitySelected()) {
                updateEntityPreview(&map.menu.entityTextures[map.menu.selectedIndex]);
            } else {
                if (const sf::Texture* selectedTexture = &map.menu.textures[map.menu.selectedIndex - map.menu.entityTextures.size()])
                    transrect.setTexture(*selectedTexture, true);
            }
        } else if (!map.menu.isOpen && !map.propertyEditor.isOpen) {
            firstClickWorld = window.mapPixelToCoords(mousePos);
            leftClickPressed = true;
        }
    } else if (event.mouseButton.button == sf::Mouse::Right) {
        openPropertyEditor();
    } else if (event.mouseButton.button == sf::Mouse::Middle) {
        isDraggingCamera = true;
        lastMousePos = sf::Mouse::getPosition(window);
    }
}

void LevelEditor::openPropertyEditor() {
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    EditorMap::PlacedEntity* clickedEntity = nullptr;
    for (auto& entity : map.placedEntities) {
        if (entity.sprite.getGlobalBounds().contains(worldPos)) {
            clickedEntity = &entity;
            break;
        }
    }
    
    if (clickedEntity) {
        map.propertyEditor.updateForEntity(clickedEntity);
    } else {
        // Close menus when right-clicking empty space
        map.menu.isOpen = false;
        map.propertyEditor.isOpen = false;
    }
}
void LevelEditor::handleMouseRelease(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left && !map.menu.isOpen && !map.propertyEditor.isOpen) {
        leftClickPressed = false;
        if (map.menu.isEntitySelected()) {
            map.addEntity(entityPreview.getGlobalBounds().getPosition().x, entityPreview.getGlobalBounds().getPosition().y, 0, 0, map.menu.getSelectedName());
        } else {
            float x = std::min(firstClickWorld.x, currentMousePosWorld.x);
            float y = std::min(firstClickWorld.y, currentMousePosWorld.y);
            float w = std::abs(currentMousePosWorld.x - firstClickWorld.x);
            float h = std::abs(currentMousePosWorld.y - firstClickWorld.y);
            map.addEntity(x, y, w, h, "Terrain");
        }

        transrect.setScale(1, 1);
    } else if (event.mouseButton.button == sf::Mouse::Middle) {
        isDraggingCamera = false;
    }
}

void LevelEditor::handleMouseMove(const sf::Event& event) {
    if (isDraggingCamera) {
        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(lastMousePos) - window.mapPixelToCoords(currentMousePos);
        view.move(delta);
        window.setView(view);
        lastMousePos = currentMousePos;
        updateGrid(); // Update grid when camera moves
    }
    
    currentMousePosWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (!map.propertyEditor.isOpen || !map.menu.isOpen) {
        entityPreview.setPosition(currentMousePosWorld);
    }
}


void LevelEditor::updateTransrect() {
    if (leftClickPressed && !map.menu.isEntitySelected()) {
        sf::Vector2f size = currentMousePosWorld - firstClickWorld;
        sf::Vector2f position(
            std::min(firstClickWorld.x, currentMousePosWorld.x),
            std::min(firstClickWorld.y, currentMousePosWorld.y)
        );

        transrect.setPosition(position);
        if (transrect.getTexture()) {
            transrect.setScale(
                std::abs(size.x) / transrect.getTexture()->getSize().x,
                std::abs(size.y) / transrect.getTexture()->getSize().y
            );
        }
    }
}

void LevelEditor::update(float deltaTime) {
    updateTransrect();
}

void LevelEditor::render() {
    window.clear(sf::Color::Black);
    window.draw(grid);
    map.drawEditorEntities(window);
    if (!map.menu.isEntitySelected() && leftClickPressed)
        window.draw(transrect);
    if (map.menu.isEntitySelected() && !map.menu.isOpen && !map.propertyEditor.isOpen)
        window.draw(entityPreview);
    map.menu.draw();
    map.propertyEditor.draw(window);
    window.display();
}

void LevelEditor::run() {
    sf::Clock frameClock;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    const float maxDeltaTime = 1.0f / 30.0f;

    while (window.isOpen()) {
        sf::Time frameTime = frameClock.restart();
        float deltaTime = std::min(frameTime.asSeconds(), maxDeltaTime);

        handleEvents();
        update(deltaTime);
        render();

        sf::Time sleepTime = timePerFrame - frameClock.getElapsedTime();
        if (sleepTime > sf::Time::Zero)
            std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
    }
}

int main() {
    try {
        LevelEditor editor;
        editor.run();
    } catch (const std::exception& e) {
        //std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}