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
      map(window), view(sf::FloatRect(0, 0, 1024, 768)) {
    window.setVerticalSyncEnabled(true);
    window.setView(view);
    map.loadFromFile("../map.mib");

    transrect.setColor(sf::Color(255, 255, 255, 128));
    if (const sf::Texture* initialTexture = map.getSelectedTexture())
        transrect.setTexture(*initialTexture, true);

    updateEntityPreview(map.getEntityTexture(map.getSelectedName()));

    if (!font.loadFromFile("../fonts/Arial.ttf"))
        throw std::runtime_error("Failed to load font!");

    map.propertyEditor.setup(font);
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
                handleMouseClick(event);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseRelease(event);
                break;
            case sf::Event::MouseMoved:
                if (!map.propertyEditor.isOpen)
                    entityPreview.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                break;
            default:
                break;
        }
        map.propertyEditor.handleInput(event, window);
    }
}

void LevelEditor::handleKeyPress(const sf::Event& event) {
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
        case sf::Keyboard::Left:
            map.changePart(-1, 0);
            break;
        case sf::Keyboard::Right:
            map.changePart(1, 0);
            break;
        case sf::Keyboard::Up:
            map.changePart(0, -1);
            break;
        case sf::Keyboard::Down:
            map.changePart(0, 1);
            break;
        case sf::Keyboard::D:
            removeEntityAtMousePosition();
            break;
        default:
            break;
    }
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

void LevelEditor::handleMouseClick(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (map.handleMenuClick(mousePos, window)) {
            placingTexture = !map.menu.isEntitySelected();
            if (placingTexture) {
                if (const sf::Texture* selectedTexture = map.getSelectedTexture())
                    transrect.setTexture(*selectedTexture, true);
            } else {
                updateEntityPreview(map.getSelectedTexture());
            }
        } else if (!map.menu.isOpen && !map.propertyEditor.isOpen) {
            firstClick = mousePos;
            leftClickPressed = true;
        }
    } else if (event.mouseButton.button == sf::Mouse::Right) {
        openPropertyEditor();
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
    map.propertyEditor.updateForEntity(clickedEntity, font);
}

void LevelEditor::handleMouseRelease(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left && !map.menu.isOpen && !map.propertyEditor.isOpen) {
        leftClickPressed = false;
        if (placingTexture) {
            map.addEntity(std::min(secondClick.x, firstClick.x), std::min(secondClick.y, firstClick.y),
                          std::abs(secondClick.x - firstClick.x), std::abs(secondClick.y - firstClick.y), "Terrain");
        } else {
            sf::Vector2f entityPos = entityPreview.getPosition();
            sf::Vector2f entitySize = sf::Vector2f(entityPreview.getGlobalBounds().width, entityPreview.getGlobalBounds().height);
            map.addEntity(entityPos.x - entitySize.x / 2, entityPos.y - entitySize.y / 2, 0, 0, map.getSelectedName());
        }
        secondClick = sf::Vector2i(0, 0);
        transrect.setScale(1, 1);
    }
}

void LevelEditor::updateTransrect() {
    if (leftClickPressed && placingTexture) {
        secondClick = sf::Mouse::getPosition(window);
        sf::Vector2f size(std::abs(secondClick.x - firstClick.x), std::abs(secondClick.y - firstClick.y));
        sf::Vector2f position(std::min(secondClick.x, firstClick.x) + map.getPartBounds().left,
                              std::min(secondClick.y, firstClick.y) + map.getPartBounds().top);

        transrect.setPosition(position);
        if (transrect.getTexture()) {
            transrect.setScale(size.x / transrect.getTexture()->getSize().x,
                               size.y / transrect.getTexture()->getSize().y);
        }
    }
}

void LevelEditor::update(float deltaTime) {
    updateTransrect();
}

void LevelEditor::render() {
    window.clear(sf::Color::Black);
    map.drawEditorEntities(window, map.propertyEditor.selectedEntity, map.propertyEditor.isOpen);
    if (placingTexture && leftClickPressed)
        window.draw(transrect);
    if (!placingTexture && !map.menu.isOpen && !map.propertyEditor.isOpen)
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
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}