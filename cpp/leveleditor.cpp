#include "../hpp/libs.hpp"
class EditorMap;

sf::Vector2f getEntityOrigin(const sf::Sprite &sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    return sf::Vector2f(bounds.width / 2, bounds.height / 2);
}

void updateEntityPreview(sf::Sprite &entityPreview, const sf::Texture *entityTexture)
{
    if (entityTexture)
    {
        entityPreview.setTexture(*entityTexture, true);
        entityPreview.setOrigin(getEntityOrigin(entityPreview));

        const float maxPreviewSize = 64.0f;
        sf::Vector2f scale(1.0f, 1.0f);
        if (entityPreview.getLocalBounds().width > maxPreviewSize ||
            entityPreview.getLocalBounds().height > maxPreviewSize)
        {
            scale.x = maxPreviewSize / entityPreview.getLocalBounds().width;
            scale.y = maxPreviewSize / entityPreview.getLocalBounds().height;
        }
        entityPreview.setScale(scale);

        entityPreview.setColor(sf::Color(255, 255, 255, 192)); // Semi-transparent
    }
}

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    EditorMap map("../map.mib", window);
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    sf::Sprite transrect;        // This is the transparent placeholder sprite for textures
    sf::Sprite entityPreview;    // This is the preview sprite for entities
    sf::Vector2i fc(0, 0), sc(0, 0); // Stands for first click, second click
    bool lc = false;             // Stands for left click, used to determine if the left click is currently being pressed
    bool placingTexture = false; // True for texture, false for entity

    window.setView(view);

    transrect.setColor(sf::Color(255, 255, 255, 128));
    const sf::Texture* initialTexture = map.getSelectedTexture();
    if (initialTexture) {
        transrect.setTexture(*initialTexture, true);
    }

    // Initialize entity preview
    const sf::Texture *initialEntityTexture = map.getEntityTexture(map.getSelectedName());
    updateEntityPreview(entityPreview, initialEntityTexture);

    EditorMap::PropertyEditor propertyEditor;
    sf::Font font;
    if (!font.loadFromFile("../fonts/Arial.ttf"))
    {
        // Handle font loading error
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }
    propertyEditor.setup(font);

    // Frame rate control
    sf::Clock frameClock;
    sf::Clock updateClock;
    const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    const float maxDeltaTime = 1.0f / 30.0f; // Maximum allowed delta time

    while (window.isOpen())
    {
        sf::Time frameTime = frameClock.restart();
        float deltaTime = std::min(frameTime.asSeconds(), maxDeltaTime); // Cap delta time

        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::E && !propertyEditor.isOpen)
                {
                    map.menu.isOpen = !map.menu.isOpen;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (map.handleMenuClick(mousePos,window))
                    {
                        if (map.menu.isEntitySelected())
                        {
                            placingTexture = false;
                            const sf::Texture *entityTexture = map.getSelectedTexture();
                            updateEntityPreview(entityPreview, entityTexture);
                        }
                        else
                        {
                            placingTexture = true;
                            const sf::Texture* selectedTexture = map.getSelectedTexture();
                            if (selectedTexture) {
                                transrect.setTexture(*selectedTexture, true);
                            }
                        }
                    }
                    else if (!map.menu.isOpen && !propertyEditor.isOpen)
                    {
                        fc = mousePos;
                        lc = true;
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    Entity::PlacedEntity *clickedEntity = nullptr;
                    for (auto &placedEntity : map.placedEntities)
                    {
                        if (placedEntity->sprite.getGlobalBounds().contains(worldPos))
                        {
                            map.menu.isOpen = false;
                            clickedEntity = placedEntity.get();
                            break;
                        }
                    }

                    propertyEditor.updateForEntity(clickedEntity, font);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && !map.menu.isOpen && !propertyEditor.isOpen)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    lc = false;
                    if (placingTexture)
                    {
                        map.addObject(std::min(sc.x, fc.x), std::min(sc.y, fc.y),
                                      std::abs(sc.x - fc.x), std::abs(sc.y - fc.y));
                    }
                    else
                    {
                        sf::Vector2f entityPos = entityPreview.getPosition();
                        sf::Vector2f entitySize = sf::Vector2f(entityPreview.getGlobalBounds().width, entityPreview.getGlobalBounds().height);
                        map.addEntity(entityPos.x - entitySize.x / 2, entityPos.y - entitySize.y / 2, map.getSelectedName());
                    }
                    sc = sf::Vector2i(0, 0);
                    transrect.setScale(1, 1);
                }
            }
            else if (event.type == sf::Event::MouseMoved && !propertyEditor.isOpen)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                entityPreview.setPosition(worldPos);
            }

            propertyEditor.handleInput(event, window);
        }

        if (lc && placingTexture)
        {
            sc = sf::Mouse::getPosition(window);
            sf::Vector2f size(std::abs(sc.x - fc.x), std::abs(sc.y - fc.y));
            sf::Vector2f position(std::min(sc.x, fc.x) + map.getPartBounds().left,
                                  std::min(sc.y, fc.y) + map.getPartBounds().top);
            
            transrect.setPosition(position);
            if (transrect.getTexture()) {
                transrect.setScale(
                    size.x / transrect.getTexture()->getSize().x,
                    size.y / transrect.getTexture()->getSize().y
                );
            }
        }

        if (!propertyEditor.isOpen)
        {
            // Handle keyboard input for map navigation and saving
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                map.saveToFile("../map.mib");
                std::cout << "\nsaved map";
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {} // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                map.changePart(-1, 0);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {} // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                map.changePart(1, 0);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {} // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                map.changePart(0, -1);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {} // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                map.changePart(0, 1);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {} // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                bool deleted = false;

                // Check if an object is clicked
                std::vector<sf::FloatRect> objBounds = map.getObjectBounds();
                for (int i = 0; i < objBounds.size(); i++)
                {
                    if (objBounds[i].contains(worldPos))
                    {
                        map.removeObject(i);
                        deleted = true;
                        break;
                    }
                }

                // If no object was deleted, check for entities
                if (!deleted)
                {
                    // Check for entity deletion
                    for (int i = map.placedEntities.size() - 1; i >= 0; --i)
                    {
                        if (map.placedEntities[i]->sprite.getGlobalBounds().contains(worldPos))
                        {
                            map.removeEntity(i);
                            break;
                        }
                    }
                }
            }
        }

        // Apply property changes when Enter is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            propertyEditor.applyChanges();
        }

        // Drawing
        window.clear(sf::Color::Black);
        map.draw();
        map.drawEditorEntities(window, propertyEditor.selectedEntity, propertyEditor.isOpen);
        if (placingTexture && lc)
        {
            window.draw(transrect);
        }
        if (!placingTexture && !map.menu.isOpen && !propertyEditor.isOpen)
        {
            window.draw(entityPreview);
        }
        map.menu.draw();
        propertyEditor.draw(window);
        window.display();

        // Frame rate control
        sf::Time sleepTime = timePerFrame - frameClock.getElapsedTime();
        if (sleepTime > sf::Time::Zero)
            std::this_thread::sleep_for(std::chrono::microseconds(sleepTime.asMicroseconds()));
    }
    return 0;
}