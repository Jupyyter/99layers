#include "../hpp/libs.hpp"
class GameMap;

std::unordered_map<std::string, sf::Texture> EditorMap::entityTextures;
std::vector<std::string> EditorMap::Menu::textureNames;

EditorMap::EditorMap(sf::RenderWindow& window)
    : window(window),
      menu({"../imgs/adidas.png", "../imgs/pacman.png", "../imgs/capybaraa.png",
            "../imgs/ikeaman.png", "../imgs/ak47.png", "../imgs/ak47item.png",
            "../imgs/arrow.png", "../imgs/pengu.png", "../imgs/HorusBrogans.png",
            "../imgs/chronostimepiece.png", "../imgs/groundbreaker.png", "../imgs/runnerspact.png",
            "../imgs/poketIkeaman.png"},
           {"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"},
           window)
{
    view.setSize(window.getSize().x, window.getSize().y);
}

EditorMap::EditorMap(const std::string& fname, sf::RenderWindow& window)
    : EditorMap(window)
{
    loadFromFile(fname);
}

void EditorMap::loadFromFile(const std::string& fname) {
    std::ifstream file(fname, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << fname << std::endl;
        return;
    }

    placedEntities.clear();
    int entityCount;
    file.read(reinterpret_cast<char*>(&entityCount), sizeof(int));

    for (int i = 0; i < entityCount; ++i) {
        PlacedEntity entity;
        int typeLength;
        file.read(reinterpret_cast<char*>(&typeLength), sizeof(int));
        entity.type.resize(typeLength);
        file.read(&entity.type[0], typeLength);

        float x, y, width, height;
        file.read(reinterpret_cast<char*>(&x), sizeof(float));
        file.read(reinterpret_cast<char*>(&y), sizeof(float));
        file.read(reinterpret_cast<char*>(&width), sizeof(float));
        file.read(reinterpret_cast<char*>(&height), sizeof(float));

        int texturePathLength;
        file.read(reinterpret_cast<char*>(&texturePathLength), sizeof(int));
        entity.texturePath.resize(texturePathLength);
        file.read(&entity.texturePath[0], texturePathLength);

        if (!entity.texture.loadFromFile(entity.texturePath)) {
            std::cerr << "Failed to load texture: " << entity.texturePath << std::endl;
        }
        entity.sprite.setTexture(entity.texture);
        entity.sprite.setPosition(x, y);
        
        if (entity.type == "Terrain") {
            const_cast<sf::Texture&>(entity.texture).setRepeated(true);
            entity.sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        } else {
            sf::Vector2u textureSize = entity.texture.getSize();
            if (textureSize.x > 0 && textureSize.y > 0) {
                float scaleX = width / textureSize.x;
                float scaleY = height / textureSize.y;
                entity.sprite.setScale(scaleX, scaleY);
            } else {
                std::cerr << "Texture has invalid size: " << entity.texturePath << std::endl;
                entity.sprite.setScale(1, 1);
            }
        }

        int propertyCount;
        file.read(reinterpret_cast<char*>(&propertyCount), sizeof(int));
        for (int j = 0; j < propertyCount; ++j) {
            int nameLength, valueLength;
            std::string name, value;

            file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
            name.resize(nameLength);
            file.read(&name[0], nameLength);

            file.read(reinterpret_cast<char*>(&valueLength), sizeof(int));
            value.resize(valueLength);
            file.read(&value[0], valueLength);

            entity.properties[name] = value;
        }

        placedEntities.push_back(std::move(entity));
    }

    std::cout << "\nLoaded " << placedEntities.size() << " entities from " << fname << std::endl;
}

void EditorMap::saveToFile(const std::string& fname) {
    std::ofstream file(fname, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << fname << std::endl;
        return;
    }

    int entityCount = placedEntities.size();
    file.write(reinterpret_cast<const char*>(&entityCount), sizeof(int));

    for (const auto& entity : placedEntities) {
        int typeLength = entity.type.length();
        file.write(reinterpret_cast<const char*>(&typeLength), sizeof(int));
        file.write(entity.type.c_str(), typeLength);

        sf::Vector2f pos = entity.sprite.getPosition();
        sf::FloatRect bounds = entity.sprite.getGlobalBounds();
        file.write(reinterpret_cast<const char*>(&pos.x), sizeof(float));
        file.write(reinterpret_cast<const char*>(&pos.y), sizeof(float));
        file.write(reinterpret_cast<const char*>(&bounds.width), sizeof(float));
        file.write(reinterpret_cast<const char*>(&bounds.height), sizeof(float));

        int texturePathLength = entity.texturePath.length();
        file.write(reinterpret_cast<const char*>(&texturePathLength), sizeof(int));
        file.write(entity.texturePath.c_str(), texturePathLength);

        int propertyCount = entity.properties.size();
        file.write(reinterpret_cast<const char*>(&propertyCount), sizeof(int));
        for (const auto& prop : entity.properties) {
            int nameLength = prop.first.length();
            int valueLength = prop.second.length();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
            file.write(prop.first.c_str(), nameLength);
            file.write(reinterpret_cast<const char*>(&valueLength), sizeof(int));
            file.write(prop.second.c_str(), valueLength);
        }
    }

    std::cout << "Saved " << fname << std::endl;
}

void EditorMap::updateEntityProperty(int index, const std::string& property, const std::string& value) {
    if (index >= 0 && index < placedEntities.size()) {
        placedEntities[index].properties[property] = value;
    }
}

void EditorMap::changePart(int x, int y) {
    mx += x;
    my += y;
    sf::Vector2u windowSize = window.getSize();
    view.setCenter(windowSize.x / 2 + windowSize.x * mx, windowSize.y / 2 + windowSize.y * my);
    window.setView(view);
}

sf::FloatRect EditorMap::getPartBounds() {
    sf::Vector2u windowSize = window.getSize();
    return sf::FloatRect(mx * windowSize.x, my * windowSize.y, windowSize.x, windowSize.y);
}

void EditorMap::removeEntity(int index) {
    if (index >= 0 && index < placedEntities.size()) {
        std::cout << "\nDeleted \"" << placedEntities[index].type << "\"\n";
        placedEntities.erase(placedEntities.begin() + index);
    }
}

void EditorMap::addEntity(int x, int y, int w, int h, const std::string& type) {
    w = std::max(w, 10);
    h = std::max(h, 10);

    sf::Vector2u windowSize = window.getSize();
    int adjustedX = x + (mx * windowSize.x);
    int adjustedY = y + (my * windowSize.y);

    PlacedEntity entity;
    entity.type = type;

    if (type == "Terrain") {
        if (!menu.isEntitySelected() && menu.selectedIndex - menu.entityTextures.size() < menu.textures.size()) {
            const sf::Texture& selectedTexture = menu.textures[menu.selectedIndex - menu.entityTextures.size()];
            entity.texturePath = "../imgs/" + menu.textureNames[menu.selectedIndex - menu.entityTextures.size()] + ".png";
            entity.texture = selectedTexture;
            entity.sprite.setTexture(entity.texture);
            const_cast<sf::Texture&>(entity.texture).setRepeated(true);
            entity.sprite.setTextureRect(sf::IntRect(0, 0, w, h));
        }
    } else if (entityTextures.find(type) != entityTextures.end()) {
        entity.texture = entityTextures[type];
        entity.sprite.setTexture(entity.texture);
        entity.texturePath = "../imgs/" + type + ".png";
        adjustedX = x;
        adjustedY = y;
    }

    entity.sprite.setPosition(sf::Vector2f(adjustedX, adjustedY));

    auto descriptors = EntityFactory::getPropertyDescriptors(type);
    for (const auto& desc : descriptors) {
        entity.properties[desc.name] = desc.defaultValue;
    }

    std::cout << adjustedX << "   " << adjustedY << "   added \"" << type << "\"\n";
    placedEntities.push_back(std::move(entity));
}

void EditorMap::drawEditorEntities(sf::RenderWindow& window) {
    for (const auto& entity : placedEntities) {
        if (!entity.texture.getSize().x || !entity.texture.getSize().y)
            continue;

        sf::Sprite entitySprite = entity.sprite;
        entitySprite.setTexture(entity.texture);

        if (&entity == propertyEditor.selectedEntity && propertyEditor.isOpen) {
            entitySprite.setColor(sf::Color(255, 92, 3, 255)); // Orange highlight color
        } else {
            entitySprite.setColor(sf::Color::White);
        }

        window.draw(entitySprite);

        if (entity.type == "Terrain") {
            sf::RectangleShape outline(entitySprite.getGlobalBounds().getSize());
            outline.setPosition(entitySprite.getPosition());
            outline.setFillColor(sf::Color::Transparent);
            window.draw(outline);
        }
    }
}

// Menu methods
EditorMap::Menu::Menu(const std::vector<std::string>& entityPaths, const std::vector<std::string>& texturePaths, sf::RenderWindow& window)
    : window(window) {
    for (const auto& path : entityPaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            entityTextures.push_back(texture);
            entityNames.push_back(getFileNameWithoutExtension(path));
            if (EditorMap::entityTextures.find(getFileNameWithoutExtension(path)) == EditorMap::entityTextures.end()) {
                EditorMap::entityTextures[getFileNameWithoutExtension(path)] = texture;
            }
        }
    }

    for (const auto& path : texturePaths) {
        sf::Texture tex;
        if (tex.loadFromFile(path)) {
            textures.push_back(std::move(tex));
            textureNames.push_back(getFileNameWithoutExtension(path));
        }
    }
}

void EditorMap::Menu::selectNext() {
    selectedIndex = (selectedIndex + 1) % (entityTextures.size() + textures.size());
}

void EditorMap::Menu::selectPrevious() {
    selectedIndex = (selectedIndex - 1 + entityTextures.size() + textures.size()) % (entityTextures.size() + textures.size());
}

const std::string& EditorMap::Menu::getSelectedName() const {
    return isEntitySelected() ? entityNames[selectedIndex] : textureNames[selectedIndex - entityTextures.size()];
}

std::string EditorMap::Menu::getFileNameWithoutExtension(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    std::string fileName = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);
    size_t lastDot = fileName.find_last_of(".");
    return (lastDot != std::string::npos) ? fileName.substr(0, lastDot) : fileName;
}

bool EditorMap::handleMenuClick(const sf::Vector2i& mousePosition) {
    if (!menu.isOpen)
        return false;

    const float itemSize = 64.0f;
    const float itemSpacing = 10.0f;
    const float startX = 50.0f;
    const float startY = 50.0f;
    const float windowWidth = static_cast<float>(window.getSize().x);

    float currentX = startX;
    float currentY = startY;

    size_t totalItems = menu.entityTextures.size() + menu.textures.size();
    for (size_t i = 0; i < totalItems; ++i) {
        if (currentX + itemSize > windowWidth - startX) {
            currentX = startX;
            currentY += itemSize + itemSpacing;
        }

        sf::FloatRect itemBounds(currentX, currentY, itemSize, itemSize);
        if (itemBounds.contains(mousePosition.x, mousePosition.y)) {
            menu.selectedIndex = i;
            return true;
        }

        currentX += itemSize + itemSpacing;
    }
    return false;
}

void EditorMap::Menu::draw() {
    if (!isOpen)
        return;

    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    const float itemSize = 64.0f;
    const float itemSpacing = 10.0f;
    const float startX = 50.0f;
    const float startY = 50.0f;
    const float windowWidth = static_cast<float>(window.getSize().x);

    float currentX = startX;
    float currentY = startY;

    size_t totalItems = entityTextures.size() + textures.size();
    for (size_t i = 0; i < totalItems; i++) {
        sf::Sprite sprite;
        sprite.setTexture(i < entityTextures.size() ? entityTextures[i] : textures[i - entityTextures.size()]);

        if (currentX + itemSize > windowWidth - startX) {
            currentX = startX;
            currentY += itemSize + itemSpacing;
        }

        sprite.setPosition(currentX, currentY);
        sprite.setScale(itemSize / sprite.getTexture()->getSize().x, itemSize / sprite.getTexture()->getSize().y);
        window.draw(sprite);

        if (i == selectedIndex) {
            sf::RectangleShape highlight({itemSize, itemSize});
            highlight.setPosition(sprite.getPosition());
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(2.0f);
            window.draw(highlight);
        }

        currentX += itemSize + itemSpacing;
    }

    window.setView(originalView);
}

// PropertyEditor methods
void EditorMap::PropertyEditor::setup(sf::Font& loadedFont) {
    font = &loadedFont;
    background.setFillColor(sf::Color(200, 200, 200));
    background.setPosition(0, 0); // Will be adjusted in draw()
}


void EditorMap::PropertyEditor::updateForEntity(PlacedEntity* entity) {
    selectedEntity = entity;
    labels.clear();
    inputBoxes.clear();
    inputTexts.clear();
    selectedInputBox = -1;
    if (!entity)
        return;

    auto propertyDescriptors = EntityFactory::getPropertyDescriptors(entity->type);
    float yOffset = 10;
    for (const auto& descriptor : propertyDescriptors) {
        labels.emplace_back(descriptor.name + ":", *font, 14);
        labels.back().setPosition(834, yOffset);

        std::string value = entity->properties[descriptor.name];
        inputTexts.emplace_back(value, *font, 14);
        inputTexts.back().setPosition(838, yOffset + 22);
        inputTexts.back().setFillColor(sf::Color::Black);

        float requiredHeight = calculateRequiredHeight(inputTexts.back(), 180);

        inputBoxes.emplace_back(sf::Vector2f(180, requiredHeight));
        inputBoxes.back().setFillColor(sf::Color::White);
        inputBoxes.back().setOutlineColor(sf::Color::Black);
        inputBoxes.back().setOutlineThickness(1);
        inputBoxes.back().setPosition(834, yOffset + 20);

        yOffset += 30 + requiredHeight;
    }

    isOpen = true;
    adjustBackgroundSize();
}

void EditorMap::PropertyEditor::draw(sf::RenderWindow& window) {
    if (!isOpen) return;

    // Save the current view
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    // Position in top-right corner with padding
    float padding = 10.0f;
    float maxWidth = 220.0f;
    float windowWidth = static_cast<float>(window.getSize().x);
    
    // Adjust background position and size
    background.setPosition(windowWidth - maxWidth - padding, padding);
    if (!labels.empty()) {
        float lastBoxBottom = inputBoxes.back().getGlobalBounds().top + inputBoxes.back().getGlobalBounds().height;
        background.setSize(sf::Vector2f(maxWidth, lastBoxBottom + 20));
    }
    
    window.draw(background);

    // Update positions of all elements relative to the background
    float yOffset = padding + 10;
    for (size_t i = 0; i < labels.size(); i++) {
        // Position elements relative to the background
        float xPos = windowWidth - maxWidth;
        
        labels[i].setPosition(xPos + padding, yOffset);
        inputBoxes[i].setPosition(xPos + padding, yOffset + 20);
        inputTexts[i].setPosition(xPos + padding + 4, yOffset + 22);
        
        window.draw(labels[i]);
        window.draw(inputBoxes[i]);
        
        // Wrap and draw text
        sf::Text wrappedText = inputTexts[i];
        wrapText(wrappedText, maxWidth - 42);
        window.draw(wrappedText);
        
        yOffset += 30 + inputBoxes[i].getSize().y;
    }

    // Draw highlight for selected input box
    if (selectedInputBox >= 0 && selectedInputBox < inputBoxes.size()) {
        sf::RectangleShape highlight = inputBoxes[selectedInputBox];
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Red);
        highlight.setOutlineThickness(2);
        window.draw(highlight);
    }

    // Restore the original view
    window.setView(originalView);
}

void EditorMap::PropertyEditor::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (!isOpen || !selectedEntity)
        return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        selectedInputBox = -1;
        for (size_t i = 0; i < inputBoxes.size(); ++i) {
            if (inputBoxes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                selectedInputBox = i;
                break;
            }
        }
    } else if (event.type == sf::Event::TextEntered && selectedInputBox != -1) {
        if (event.text.unicode < 128) {
            handleTextInput(static_cast<char>(event.text.unicode));
        }
    }
}
void EditorMap::PropertyEditor::handleTextInput(char inputChar) {
    if (selectedInputBox == -1) return;

    std::string currentText = inputTexts[selectedInputBox].getString();
    if (inputChar == '\b') {
        if (!currentText.empty())
            currentText.pop_back();
    } else {
        currentText += inputChar;
    }
    inputTexts[selectedInputBox].setString(currentText);

    // Calculate new height without changing the layout yet
    float requiredHeight = calculateRequiredHeight(inputTexts[selectedInputBox], 180);
    float currentHeight = inputBoxes[selectedInputBox].getSize().y;

    // Only adjust layout if the height has changed significantly
    if (std::abs(requiredHeight - currentHeight) > 5.0f) {
        inputBoxes[selectedInputBox].setSize(sf::Vector2f(180, requiredHeight));
        adjustLayout();
    }
}
void EditorMap::PropertyEditor::applyChanges() {
    if (!selectedEntity)
        return;
    
    std::cout << "\nSaved";
    
    auto propertyDescriptors = EntityFactory::getPropertyDescriptors(selectedEntity->type);
    
    for (size_t i = 0; i < propertyDescriptors.size() && i < inputTexts.size(); ++i) {
        const std::string& propertyName = propertyDescriptors[i].name;
        const std::string& newValue = inputTexts[i].getString();
        selectedEntity->properties[propertyName] = newValue;
    }
}

void EditorMap::PropertyEditor::wrapText(sf::Text& text, float maxWidth) {
    std::string words = text.getString();
    std::string wrappedText;
    std::string line;
    sf::Text testText = text;

    for (char c : words) {
        if (c == '\n') {
            wrappedText += line + '\n';
            line.clear();
        } else {
            testText.setString(line + c);
            if (testText.getLocalBounds().width > maxWidth && !line.empty()) {
                wrappedText += line + '\n';
                line = c;
            } else {
                line += c;
            }
        }
    }
    wrappedText += line;
    text.setString(wrappedText);
}

float EditorMap::PropertyEditor::calculateRequiredHeight(const sf::Text& text, float maxWidth) {
    sf::Text tempText = text;
    wrapText(tempText, maxWidth);
    return tempText.getLocalBounds().height + 20;
}

void EditorMap::PropertyEditor::adjustLayout() {
    float yOffset = 10;
    for (size_t i = 0; i < inputBoxes.size(); ++i) {
        labels[i].setPosition(834, yOffset);
        inputBoxes[i].setPosition(834, yOffset + 20);
        inputTexts[i].setPosition(838, yOffset + 22);
        yOffset += 30 + inputBoxes[i].getSize().y;
    }
    adjustBackgroundSize();
}

void EditorMap::PropertyEditor::adjustBackgroundSize() {
    if (inputBoxes.empty())
        return;
    float lastBoxBottom = inputBoxes.back().getPosition().y + inputBoxes.back().getSize().y;
    float newHeight = lastBoxBottom + 20;
    
    // Smoothly adjust the background size
    sf::Vector2f currentSize = background.getSize();
    float targetHeight = std::max(newHeight, currentSize.y);
    background.setSize(sf::Vector2f(200, targetHeight));
}
