#include "../hpp/libs.hpp"
class GameMap;

std::unordered_map<std::string, sf::Texture> EditorMap::objectTextures;
std::vector<std::string> EditorMap::Menu::textureNames;

EditorMap::EditorMap(sf::RenderWindow& window)
    : window(window),
      menu({"../imgs/adidas.png", "../imgs/pacman.png", "../imgs/capybaraa.png",
            "../imgs/ikeaman.png", "../imgs/ak47item.png",
            "../imgs/arrow.png", "../imgs/pengu.png", "../imgs/HorusBrogans.png",
            "../imgs/chronostimepiece.png", "../imgs/groundbreaker.png", "../imgs/runnerspact.png",
            "../imgs/poketIkeaman.png", "../imgs/spawn.png","../imgs/bloodParticles.png","../imgs/hedgehog1.png",
            "../imgs/woodSign.png","../imgs/sign0.png"},
           {"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png",
            "../imgs/brick.png", "../imgs/brick1.png", "../imgs/brick2.png",
            "../imgs/texture0.png","../imgs/texture1.png","../imgs/texture2.png"
            , "../imgs/dirt0.png", "../imgs/dirt1.png"},
           {"../imgs/background.png","../imgs/background4.png"},
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

    placedObjects.clear();
    int objectCount;
    file.read(reinterpret_cast<char*>(&objectCount), sizeof(int));

    for (int i = 0; i < objectCount; ++i) {
        PlacedObject object;
        int typeLength;
        file.read(reinterpret_cast<char*>(&typeLength), sizeof(int));
        object.type.resize(typeLength);
        file.read(&object.type[0], typeLength);

        float x, y, width, height;
        file.read(reinterpret_cast<char*>(&x), sizeof(float));
        file.read(reinterpret_cast<char*>(&y), sizeof(float));
        file.read(reinterpret_cast<char*>(&width), sizeof(float));
        file.read(reinterpret_cast<char*>(&height), sizeof(float));

        int texturePathLength;
        file.read(reinterpret_cast<char*>(&texturePathLength), sizeof(int));
        object.texturePath.resize(texturePathLength);
        file.read(&object.texturePath[0], texturePathLength);

        if (!object.texture.loadFromFile(object.texturePath)) {
            std::cerr << "Failed to load texture: " << object.texturePath << std::endl;
        }
        object.sprite.setTexture(object.texture);
        object.sprite.setPosition(x, y);

        if (object.type == "Terrain") {
            const_cast<sf::Texture&>(object.texture).setRepeated(true);
            object.sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        } else {
            sf::Vector2u textureSize = object.texture.getSize();
            if (textureSize.x > 0 && textureSize.y > 0) {
                float scaleX = width / textureSize.x;
                float scaleY = height / textureSize.y;
                object.sprite.setScale(scaleX, scaleY);
            } else {
                std::cerr << "Texture has invalid size: " << object.texturePath << std::endl;
                object.sprite.setScale(1, 1);
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

            object.properties[name] = value;
        }

        placedObjects.push_back(std::move(object));
    }

    std::cout << "\nLoaded " << placedObjects.size() << " objects from " << fname << std::endl;
}

void EditorMap::saveToFile(const std::string& fname) {
    std::ofstream file(fname, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << fname << std::endl;
        return;
    }

    int objectCount = placedObjects.size();
    file.write(reinterpret_cast<const char*>(&objectCount), sizeof(int));

    for (const auto& object : placedObjects) {
        int typeLength = object.type.length();
        file.write(reinterpret_cast<const char*>(&typeLength), sizeof(int));
        file.write(object.type.c_str(), typeLength);

        sf::Vector2f pos = object.sprite.getPosition();
        sf::FloatRect bounds = object.sprite.getGlobalBounds();
        file.write(reinterpret_cast<const char*>(&pos.x), sizeof(float));
        file.write(reinterpret_cast<const char*>(&pos.y), sizeof(float));
        file.write(reinterpret_cast<const char*>(&bounds.width), sizeof(float));
        file.write(reinterpret_cast<const char*>(&bounds.height), sizeof(float));

        int texturePathLength = object.texturePath.length();
        file.write(reinterpret_cast<const char*>(&texturePathLength), sizeof(int));
        file.write(object.texturePath.c_str(), texturePathLength);

        int propertyCount = object.properties.size();
        file.write(reinterpret_cast<const char*>(&propertyCount), sizeof(int));
        for (const auto& prop : object.properties) {
            int nameLength = prop.first.length();
            int valueLength = prop.second.length();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
            file.write(prop.first.c_str(), nameLength);
            file.write(reinterpret_cast<const char*>(&valueLength), sizeof(int));
            file.write(prop.second.c_str(), valueLength);
        }
    }

    std::cout << "\nSaved " << fname << std::endl;
}

void EditorMap::updateObjectProperty(int index, const std::string& property, const std::string& value) {
    if (index >= 0 && index < placedObjects.size()) {
        placedObjects[index].properties[property] = value;
    }
}

void EditorMap::removeObject(int index) {
    if (index >= 0 && index < placedObjects.size()) {
        std::cout << "\nDeleted \"" << placedObjects[index].type << "\"\n";
        placedObjects.erase(placedObjects.begin() + index);
    }
}

void EditorMap::addObject(int x, int y, int w, int h, const std::string& type) {
    w = std::max(w, 10);
    h = std::max(h, 10);

    sf::Vector2u windowSize = window.getSize();
    int adjustedX = x + (mx * windowSize.x);
    int adjustedY = y + (my * windowSize.y);

    PlacedObject object;
    object.type = type;

    if (type == "Background") {
        if (menu.isBackgroundSelected() && menu.selectedIndex - menu.objectTextures.size() - menu.textures.size() < menu.backgroundTextures.size()) {
            const sf::Texture& selectedTexture = menu.backgroundTextures[menu.selectedIndex - menu.objectTextures.size() - menu.textures.size()];
            object.texturePath = "../imgs/" + menu.backgroundNames[menu.selectedIndex - menu.objectTextures.size() - menu.textures.size()] + ".png";
            object.texture = selectedTexture;
            object.sprite.setTexture(object.texture);
            
            sf::Vector2u textureSize = object.texture.getSize();
            
            object.properties["originalWidth"] = std::to_string(textureSize.x);
            object.properties["originalHeight"] = std::to_string(textureSize.y);
            
            object.sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
            
            float scaleX = static_cast<float>(w) / textureSize.x;
            float scaleY = static_cast<float>(h) / textureSize.y;
            object.sprite.setScale(scaleX, scaleY);
        }
    } else if (type == "Terrain") {
        if (!menu.isObjectSelected() && menu.selectedIndex - menu.objectTextures.size() < menu.textures.size()) {
            const sf::Texture& selectedTexture = menu.textures[menu.selectedIndex - menu.objectTextures.size()];
            object.texturePath = "../imgs/" + menu.textureNames[menu.selectedIndex - menu.objectTextures.size()] + ".png";
            object.texture = selectedTexture;
            object.sprite.setTexture(object.texture);
            const_cast<sf::Texture&>(object.texture).setRepeated(true);
            object.sprite.setTextureRect(sf::IntRect(0, 0, w, h));
        }
    } else if (objectTextures.find(type) != objectTextures.end()) {
        object.texture = objectTextures[type];
        object.sprite.setTexture(object.texture);
        object.texturePath = "../imgs/" + type + ".png";
        adjustedX = x;
        adjustedY = y;
    }

    object.sprite.setPosition(sf::Vector2f(adjustedX, adjustedY));

    auto descriptors = ObjectFactory::getPropertyDescriptors(type);
    for (const auto& desc : descriptors) {
        object.properties[desc.name] = desc.defaultValue;
    }

    std::cout << adjustedX << "   " << adjustedY << "   added \"" << type << "\"\n";
    placedObjects.push_back(std::move(object));
}

void EditorMap::drawEditorObjects(sf::RenderWindow& window) {
    for (const auto& object : placedObjects) {
        if (object.type == "Background") {
            sf::Sprite backgroundSprite = object.sprite;
            backgroundSprite.setTexture(object.texture);
            window.draw(backgroundSprite);
        }
    }

    for (const auto& object : placedObjects) {
        if (object.type != "Background") {
            if (!object.texture.getSize().x || !object.texture.getSize().y)
                continue;

            sf::Sprite objectSprite = object.sprite;
            objectSprite.setTexture(object.texture);

            if (&object == propertyEditor.selectedObject && propertyEditor.isOpen) {
                objectSprite.setColor(sf::Color(255, 92, 3, 255));
            } else {
                objectSprite.setColor(sf::Color::White);
            }

            window.draw(objectSprite);
        }
    }
}
EditorMap::Menu::Menu(const std::vector<std::string>& objectPaths, const std::vector<std::string>& texturePaths, 
                      const std::vector<std::string>& backgroundPaths, sf::RenderWindow& window)
    : window(window)
{
    auto loadTextures = [this](const std::vector<std::string>& paths, std::vector<sf::Texture>& textures, std::vector<std::string>& names) {
        for (const auto& path : paths) {
            sf::Texture texture;
            if (texture.loadFromFile(path)) {
                textures.push_back(texture);
                std::string name = getFileNameWithoutExtension(path);
                names.push_back(name);
                EditorMap::objectTextures[name] = texture;  // Add this line
            }
        }
    };

    loadTextures(objectPaths, objectTextures, objectNames);
    loadTextures(texturePaths, textures, textureNames);
    loadTextures(backgroundPaths, backgroundTextures, backgroundNames);
}

bool EditorMap::Menu::isObjectSelected() const { 
    return selectedIndex < objectTextures.size(); 
}

bool EditorMap::Menu::isBackgroundSelected() const {
    return selectedIndex >= objectTextures.size() + textures.size() &&
           selectedIndex < objectTextures.size() + textures.size() + backgroundTextures.size();
}

const std::string& EditorMap::Menu::getSelectedName() const {
    if (isObjectSelected()) {
        return objectNames[selectedIndex];
    } else if (isBackgroundSelected()) {
        return backgroundNames[selectedIndex - (objectTextures.size() + textures.size())];
    } else {
        return textureNames[selectedIndex - objectTextures.size()];
    }
}

std::string EditorMap::Menu::getFileNameWithoutExtension(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    std::string fileName = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);
    size_t lastDot = fileName.find_last_of(".");
    return (lastDot != std::string::npos) ? fileName.substr(0, lastDot) : fileName;
}

void EditorMap::Menu::draw() {
    if (!isOpen) return;

    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    const float itemSize = 64.0f;
    const float itemSpacing = 10.0f;
    const float startX = 50.0f;
    const float startY = 50.0f;
    const float windowWidth = static_cast<float>(window.getSize().x);

    float currentX = startX;
    float currentY = startY;

    auto drawItem = [&](const sf::Texture& texture, size_t index) {
        sf::Sprite sprite(texture);
        if (currentX + itemSize > windowWidth - startX) {
            currentX = startX;
            currentY += itemSize + itemSpacing;
        }
        sprite.setPosition(currentX, currentY);
        sprite.setScale(itemSize / texture.getSize().x, itemSize / texture.getSize().y);
        window.draw(sprite);

        if (index == selectedIndex) {
            sf::RectangleShape highlight({itemSize, itemSize});
            highlight.setPosition(sprite.getPosition());
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(sf::Color::Yellow);
            highlight.setOutlineThickness(2.0f);
            window.draw(highlight);
        }

        currentX += itemSize + itemSpacing;
    };

    size_t index = 0;
    for (const auto& texture : objectTextures) drawItem(texture, index++);
    for (const auto& texture : textures) drawItem(texture, index++);
    for (const auto& texture : backgroundTextures) drawItem(texture, index++);

    window.setView(originalView);
}

// PropertyEditor implementation
void EditorMap::PropertyEditor::setup(sf::Font& loadedFont) {
    font = &loadedFont;
    background.setFillColor(sf::Color(200, 200, 200));
    background.setPosition(0, 0);
}

void EditorMap::PropertyEditor::updateForObject(PlacedObject* object) {
    selectedObject = object;
    labels.clear();
    inputBoxes.clear();
    inputTexts.clear();
    selectedInputBox = -1;
    if (!object) return;

    auto propertyDescriptors = ObjectFactory::getPropertyDescriptors(object->type);
    float yOffset = 10;
    for (const auto& descriptor : propertyDescriptors) {
        labels.emplace_back(descriptor.name + ":", *font, 14);
        labels.back().setPosition(834, yOffset);

        std::string value = object->properties[descriptor.name];
        inputTexts.emplace_back(value, *font, 14);
        inputTexts.back().setPosition(838, yOffset + 22);
        inputTexts.back().setFillColor(sf::Color::Black);

        float requiredHeight = calculateRequiredHeight(inputTexts.back(), 180);

        inputBoxes.emplace_back(sf::Vector2f(180, requiredHeight));
        inputBoxes.back().setFillColor(sf::Color::White);
        inputBoxes.back().setOutlineColor(sf::Color::Black);
        inputBoxes.back().setOutlineThickness(1);
        inputBoxes.back().setPosition(844, yOffset + 20);

        yOffset += 30 + requiredHeight;
    }

    isOpen = true;
    adjustLayout();
}

void EditorMap::PropertyEditor::draw(sf::RenderWindow& window) {
    if (!isOpen) return;

    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    float padding = 10.0f;
    float maxWidth = 220.0f;
    float windowWidth = static_cast<float>(window.getSize().x);

    background.setPosition(windowWidth - maxWidth - padding, padding);
    window.draw(background);

    float yOffset = padding + 10;
    for (size_t i = 0; i < labels.size(); i++) {
        float xPos = windowWidth - maxWidth;

        labels[i].setPosition(xPos + padding, yOffset);
        inputBoxes[i].setPosition(xPos + padding, yOffset + 20);
        inputTexts[i].setPosition(xPos + padding + 4, yOffset + 22);

        window.draw(labels[i]);
        window.draw(inputBoxes[i]);

        sf::Text wrappedText = inputTexts[i];
        wrapText(wrappedText, maxWidth - 42);
        window.draw(wrappedText);

        yOffset += 30 + inputBoxes[i].getSize().y;
    }

    if (selectedInputBox >= 0 && selectedInputBox < inputBoxes.size()) {
        sf::RectangleShape highlight = inputBoxes[selectedInputBox];
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Red);
        highlight.setOutlineThickness(2);
        window.draw(highlight);
    }

    window.setView(originalView);
}

void EditorMap::PropertyEditor::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (!isOpen || !selectedObject) return;

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
bool EditorMap::handleMenuClick(const sf::Vector2i& mousePosition) {
    if (!menu.isOpen) return false;

    const float itemSize = 64.0f;
    const float spacing = 10.0f;
    const float startX = 50.0f;
    const float startY = 50.0f;
    
    float x = startX, y = startY;
    size_t totalItems = menu.objectTextures.size() + menu.textures.size() + menu.backgroundTextures.size();

    for (size_t i = 0; i < totalItems; ++i) {
        if (x + itemSize > window.getSize().x - startX) {
            x = startX;
            y += itemSize + spacing;
        }

        if (sf::FloatRect(x, y, itemSize, itemSize).contains(mousePosition.x, mousePosition.y)) {
            menu.selectedIndex = i;
            return true;
        }

        x += itemSize + spacing;
    }
    return false;
}
void EditorMap::PropertyEditor::handleTextInput(char inputChar) {
    if (selectedInputBox == -1) return;

    std::string currentText = inputTexts[selectedInputBox].getString();
    if (inputChar == '\b') {
        if (!currentText.empty()) currentText.pop_back();
    } else {
        currentText += inputChar;
    }
    inputTexts[selectedInputBox].setString(currentText);

    float requiredHeight = calculateRequiredHeight(inputTexts[selectedInputBox], 180);
    float currentHeight = inputBoxes[selectedInputBox].getSize().y;

    if (std::abs(requiredHeight - currentHeight) > 5.0f) {
        inputBoxes[selectedInputBox].setSize(sf::Vector2f(180, requiredHeight));
        adjustLayout();
    }
}

void EditorMap::PropertyEditor::applyChanges() {
    if (!selectedObject) return;

    std::cout << "\nSaved";

    auto propertyDescriptors = ObjectFactory::getPropertyDescriptors(selectedObject->type);
    for (size_t i = 0; i < propertyDescriptors.size() && i < inputTexts.size(); ++i) {
        const std::string& propertyName = propertyDescriptors[i].name;
        const std::string& newValue = inputTexts[i].getString();
        selectedObject->properties[propertyName] = newValue;
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
        inputTexts[i].setPosition(840, yOffset + 22);
        yOffset += 30 + inputBoxes[i].getSize().y;
    }
    
    if (!inputBoxes.empty()) {
        float lastBoxBottom = inputBoxes.back().getPosition().y + inputBoxes.back().getSize().y;
        float newHeight = lastBoxBottom + 20;
        background.setSize(sf::Vector2f(220, std::max(newHeight, background.getSize().y)));
    }
}