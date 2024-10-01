#include "../hpp/libs.hpp"
class GameMap;

std::unordered_map<std::string, sf::Texture> EditorMap::entityTextures;

EditorMap::EditorMap(sf::RenderWindow &wndref)
    : mx(0), my(0), np(1), wndref(wndref),
      menu({"../imgs/adidas.png", "../imgs/pacman.png", "../imgs/capybaraa.png",
            "../imgs/ikeaman.png","../imgs/ak47.png","../imgs/ak47item.png",
            "../imgs/arrow.png", "../imgs/pengu.png", "../imgs/HorusBrogans.png",
            "../imgs/chronostimepiece.png", "../imgs/groundbreaker.png", "../imgs/runnerspact.png",
            "../imgs/poketIkeaman.png"},
           {"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"},
           wndref)
{
    view.setSize(wndref.getSize().x, wndref.getSize().y);
}

EditorMap::EditorMap(std::string fname, sf::RenderWindow &wndref)
    : EditorMap(wndref)
{
    std::ifstream file(fname, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open file for reading: " << fname << std::endl;
        return;
    }

    // Clear existing entities
    placedEntities.clear();

    // Read the number of entities
    int entityCount;
    file.read(reinterpret_cast<char*>(&entityCount), sizeof(int));

    for (int i = 0; i < entityCount; ++i)
    {
        // Read entity type
        int typeLength;
        file.read(reinterpret_cast<char*>(&typeLength), sizeof(int));
        std::string type(typeLength, '\0');
        file.read(&type[0], typeLength);

        // Read position
        float x, y;
        file.read(reinterpret_cast<char*>(&x), sizeof(float));
        file.read(reinterpret_cast<char*>(&y), sizeof(float));

        // Read size
        float width, height;
        file.read(reinterpret_cast<char*>(&width), sizeof(float));
        file.read(reinterpret_cast<char*>(&height), sizeof(float));

        std::string texturePath;
        if (type == "Object")
        {
            // Read texture info for Objects
            int texIdLength;
            file.read(reinterpret_cast<char*>(&texIdLength), sizeof(int));
            texturePath.resize(texIdLength);
            file.read(&texturePath[0], texIdLength);
        }

        // Create the entity
        std::unique_ptr<Entity> newEntity;
        if (type == "Object")
        {
            newEntity = std::make_unique<Object>(x, y, width, height, texturePath);
        }
        else
        {
            sf::Transformable transform;
            transform.setPosition(x, y);
            newEntity.reset(EntityFactory::createEntity(type, transform, *this));
        }

        // Read editable properties
        int propertyCount;
        file.read(reinterpret_cast<char*>(&propertyCount), sizeof(int));
        
        for (int j = 0; j < propertyCount; ++j)
        {
            // Read property name
            int nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);
            
            // Read property value
            int valueLength;
            file.read(reinterpret_cast<char*>(&valueLength), sizeof(int));
            std::string value(valueLength, '\0');
            file.read(&value[0], valueLength);
            
            // Set the property on the entity
            newEntity->setProperty(name, value);
        }

        // Set up the sprite
        sf::Sprite sprite;
        if (type == "Object")
        {
            auto objectPtr = static_cast<Object*>(newEntity.get());
            sprite = objectPtr->sprite;
        }
        else if (entityTextures.find(type) != entityTextures.end())
        {
            sprite.setTexture(entityTextures[type]);
            sprite.setPosition(x, y);
            sprite.setScale(width / sprite.getTexture()->getSize().x, 
                           height / sprite.getTexture()->getSize().y);
        }

        // Add the entity to placedEntities
        placedEntities.push_back(std::make_unique<Entity::PlacedEntity>(Entity::PlacedEntity{
            std::move(sprite),
            type,
            std::move(newEntity)
        }));
    }
}

EditorMap::~EditorMap()
{
}


void EditorMap::saveToFile(const std::string& fname)
{
    std::ofstream file(fname, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open file for writing: " << fname << std::endl;
        return;
    }

    // Write the number of entities
    int entityCount = placedEntities.size();
    file.write(reinterpret_cast<const char*>(&entityCount), sizeof(int));

    for (const auto& entity : placedEntities)
    {
        // Write entity type
        int typeLength = entity->type.length();
        file.write(reinterpret_cast<const char*>(&typeLength), sizeof(int));
        file.write(entity->type.c_str(), typeLength);

        // Write position
        float x = entity->sprite.getPosition().x;
        float y = entity->sprite.getPosition().y;
        file.write(reinterpret_cast<const char*>(&x), sizeof(float));
        file.write(reinterpret_cast<const char*>(&y), sizeof(float));

        // Write size
        sf::FloatRect bounds = entity->sprite.getGlobalBounds();
        float width = bounds.width;
        float height = bounds.height;
        file.write(reinterpret_cast<const char*>(&width), sizeof(float));
        file.write(reinterpret_cast<const char*>(&height), sizeof(float));

        // Write texture info for Objects
        if (entity->type == "Object")
        {
            auto objectPtr = static_cast<Object*>(entity->entity.get());
            int texIdLength = objectPtr->texid.length();
            file.write(reinterpret_cast<const char*>(&texIdLength), sizeof(int));
            file.write(objectPtr->texid.c_str(), texIdLength);
        }

        // Write editable properties
        auto properties = entity->entity->getEditableProperties();
        int propertyCount = properties.size();
        file.write(reinterpret_cast<const char*>(&propertyCount), sizeof(int));
        
        for (const auto& prop : properties)
        {
            // Write property name
            int nameLength = prop.first.length();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
            file.write(prop.first.c_str(), nameLength);
            
            // Write property value
            int valueLength = prop.second.length();
            file.write(reinterpret_cast<const char*>(&valueLength), sizeof(int));
            file.write(prop.second.c_str(), valueLength);
        }
    }
}


void EditorMap::changePart(int x, int y)
{
    mx += x;
    my += y;
    //if (obj[mx][my].empty())
        np++;
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    view.setCenter(wx / 2 + wx * mx, wy / 2 + wy * my);
    wndref.setView(view);
}

sf::FloatRect EditorMap::getPartBounds()
{
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    return sf::FloatRect(mx * wx, my * wy, wx, wy);
}

void EditorMap::removeEntity(int index)
{
    if (index >= 0 && index < placedEntities.size())
    {
        std::cout << "\n"
                  << placedEntities[index]->sprite.getPosition().x
                  << " " << placedEntities[index]->sprite.getPosition().y
                  << " " << placedEntities[index]->type << " deleted";
        placedEntities.erase(placedEntities.begin() + index);
    }
}

void EditorMap::addEntity(int x, int y, int w, int h, const std::string &type)
{
    // Ensure minimum size
    w = std::max(w, 10);
    h = std::max(h, 10);

    // Calculate the actual position based on the current map part
    int wx = wndref.getSize().x;
    int wy = wndref.getSize().y;
    int adjustedX = x + (mx * wx);
    int adjustedY = y + (my * wy);

    std::unique_ptr<Entity> newEntity;
    if (type == "Object")
    {
        // Get the selected texture name from the menu
        const std::string &selectedTextureName = menu.getSelectedName();
        
        // Construct the full path to the texture
        std::string texturePath = "../imgs/" + selectedTextureName + ".png";
        
        newEntity = std::make_unique<Object>(adjustedX, adjustedY, w, h, texturePath);
    }
    else
    {
        sf::Transformable transform;
        transform.setPosition(sf::Vector2f(adjustedX, adjustedY));
        transform.setRotation(0);
        transform.setScale(1, 1);
        newEntity.reset(EntityFactory::createEntity(type, transform, *this));
    }

    if (newEntity)
    {
        sf::Sprite sprite;
        if (type == "Object")
        {
            auto objectPtr = static_cast<Object*>(newEntity.get());
            sprite = objectPtr->sprite;
            sprite.setPosition(sf::Vector2f(adjustedX, adjustedY));
        }
        else if (entityTextures.find(type) != entityTextures.end())
        {
            sprite.setTexture(entityTextures[type]);
            sprite.setPosition(sf::Vector2f(adjustedX, adjustedY));
        }

        auto placedEntity = std::make_unique<Entity::PlacedEntity>(Entity::PlacedEntity{
            std::move(sprite),
            type,
            std::move(newEntity)
        });

        // Insert the placedEntity at the correct position based on priorityLayer
        auto insertPos = std::lower_bound(placedEntities.begin(), placedEntities.end(), placedEntity,
            [](const std::unique_ptr<Entity::PlacedEntity>& a, const std::unique_ptr<Entity::PlacedEntity>& b) {
                return a->entity->priorityLayer < b->entity->priorityLayer;
            });

        placedEntities.insert(insertPos, std::move(placedEntity));
    }
}

void EditorMap::drawEditorEntities(sf::RenderWindow &window, const Entity::PlacedEntity *selectedEntity, bool &isOpen)
{
    for (const auto &entityPtr : placedEntities)
    {
        sf::Sprite entitySprite = entityPtr->sprite;

        if (entityPtr.get() == selectedEntity && isOpen)
        {
            entitySprite.setColor(sf::Color(255, 92, 3, 255)); // Pink color
        }
        else
        {
            entitySprite.setColor(sf::Color::White);
        }

        window.draw(entitySprite);

        // If it's an Object, draw its outline
        if (entityPtr->type == "Object")
        {
            sf::RectangleShape outline(entitySprite.getGlobalBounds().getSize());
            outline.setPosition(entitySprite.getPosition());
            outline.setFillColor(sf::Color::Transparent);
            window.draw(outline);
        }
    }
}

// Menu methods
EditorMap::Menu::Menu(const std::vector<std::string> &entityPaths, const std::vector<std::string> &texturePaths, sf::RenderWindow &window)
    : selectedIndex(0), window(window)
{
    for (const auto &path : entityPaths)
    {
        sf::Texture texture;
        if (texture.loadFromFile(path))
        {
            entityTextures.push_back(texture);
            entityNames.push_back(getFileNameWithoutExtension(path));
            if (EditorMap::entityTextures.find(getFileNameWithoutExtension(path)) == EditorMap::entityTextures.end())
            {
                EditorMap::entityTextures[getFileNameWithoutExtension(path)] = texture;
            }
        }
    }

    for (const auto &path : texturePaths)
    {
        sf::Texture tex;
        if (tex.loadFromFile(path))
        {
            textures.push_back(std::move(tex));
            textureNames.push_back(getFileNameWithoutExtension(path));
        }
    }
}

void EditorMap::Menu::selectNext()
{
    selectedIndex = (selectedIndex + 1) % (entityTextures.size() + textures.size());
}

void EditorMap::Menu::selectPrevious()
{
    selectedIndex = (selectedIndex - 1 + entityTextures.size() + textures.size()) % (entityTextures.size() + textures.size());
}

const std::string &EditorMap::Menu::getSelectedName() const
{
    if (isEntitySelected())
    {
        return entityNames[selectedIndex];
    }
    else
    {
        return textureNames[selectedIndex - entityTextures.size()];
    }
}

std::string EditorMap::Menu::getFileNameWithoutExtension(const std::string &path)
{
    size_t lastSlash = path.find_last_of("/\\");
    std::string fileName = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);

    size_t lastDot = fileName.find_last_of(".");
    if (lastDot != std::string::npos)
    {
        fileName = fileName.substr(0, lastDot);
    }

    return fileName;
}

// EditorMap methods
bool EditorMap::handleMenuClick(const sf::Vector2i &mousePosition, sf::RenderWindow &window)
{
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
    for (size_t i = 0; i < totalItems; ++i)
    {
        if (currentX + itemSize > windowWidth - startX)
        {
            currentX = startX;
            currentY += itemSize + itemSpacing;
        }

        sf::FloatRect itemBounds(currentX, currentY, itemSize, itemSize);
        if (itemBounds.contains(mousePosition.x, mousePosition.y))
        {
            menu.selectedIndex = i;
            return true;
        }

        currentX += itemSize + itemSpacing;
    }
    return false;
}

const std::string &EditorMap::getSelectedName() const
{
    return menu.getSelectedName();
}

const sf::Texture *EditorMap::getSelectedTexture() const
{
    if (menu.isEntitySelected())
    {
        return &menu.entityTextures[menu.selectedIndex];
    }
    else
    {
        return &menu.textures[menu.selectedIndex - menu.entityTextures.size()];
    }
}

const sf::Texture *EditorMap::getEntityTexture(const std::string &entityName) const
{
    auto it = entityTextures.find(entityName);
    if (it != entityTextures.end())
    {
        return &(it->second);
    }
    return nullptr;
}

// PropertyEditor methods
void EditorMap::PropertyEditor::setup(sf::Font &loadedFont)
{
    font = &loadedFont;
    background.setFillColor(sf::Color(200, 200, 200));
    background.setSize(sf::Vector2f(200, 400));
    background.setPosition(824, 0);
    selectedInputBox = -1;
}
void EditorMap::Menu::draw()
{
    if (isOpen)
    {
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
        for (size_t i = 0; i < totalItems; i++)
        {
            sf::Sprite sprite;
            if (i < entityTextures.size())
            {
                sprite.setTexture(entityTextures[i]);
            }
            else
            {
                sprite.setTexture(textures[i - entityTextures.size()]);
            }

            // Check if the next item would exceed the window width
            if (currentX + itemSize > windowWidth - startX)
            {
                // Move to the next line
                currentX = startX;
                currentY += itemSize + itemSpacing;
            }

            sprite.setPosition(currentX, currentY);
            sprite.setScale(itemSize / sprite.getTexture()->getSize().x, itemSize / sprite.getTexture()->getSize().y);
            window.draw(sprite);

            if (i == selectedIndex)
            {
                sf::RectangleShape highlight;
                highlight.setSize(sf::Vector2f(itemSize, itemSize));
                highlight.setPosition(sprite.getPosition());
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Yellow);
                highlight.setOutlineThickness(2.0f);
                window.draw(highlight);
            }

            // Move to the next item position
            currentX += itemSize + itemSpacing;
        }

        window.setView(originalView);
    }
}

void EditorMap::PropertyEditor::draw(sf::RenderWindow &window)
{
    if (!selectedEntity)
    {
        isOpen = false;
        return;
    }
    isOpen = true;
    window.draw(background);
    for (const auto &label : labels)
        window.draw(label);
    for (const auto &box : inputBoxes)
        window.draw(box);
    for (auto &text : inputTexts)
    {
        wrapText(text, 170); // Reduced width to account for padding
        window.draw(text);
    }

    if (selectedInputBox >= 0 && selectedInputBox < inputBoxes.size())
    {
        sf::RectangleShape highlight = inputBoxes[selectedInputBox];
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Red);
        highlight.setOutlineThickness(2);
        window.draw(highlight);
    }
}
void EditorMap::PropertyEditor::updateForEntity(Entity::PlacedEntity *entity, sf::Font &font)
{
    selectedEntity = entity;
    labels.clear();
    inputBoxes.clear();
    inputTexts.clear();
    selectedInputBox = -1;
    if (!entity)
        return;

    auto properties = entity->entity->getEditableProperties();
    float yOffset = 10;
    for (const auto &prop : properties)
    {
        sf::Text label(prop.first + ":", font, 14);
        label.setPosition(834, yOffset);
        labels.push_back(label);

        sf::Text inputText(prop.second, font, 14);
        inputText.setPosition(838, yOffset + 22);
        inputText.setFillColor(sf::Color::Black);

        float requiredHeight = calculateRequiredHeight(inputText, 180);

        sf::RectangleShape inputBox(sf::Vector2f(180, requiredHeight));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(1);
        inputBox.setPosition(834, yOffset + 20);

        inputBoxes.push_back(inputBox);
        inputTexts.push_back(inputText);

        yOffset += 30 + requiredHeight;
    }

    isOpen = true;
    adjustBackgroundSize();
}

void EditorMap::PropertyEditor::handleInput(sf::Event &event, sf::RenderWindow &window)
{
    if (!isOpen || !selectedEntity)
        return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (size_t i = 0; i < inputBoxes.size(); ++i)
        {
            if (inputBoxes[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                selectedInputBox = i;
                return;
            }
        }
        selectedInputBox = -1;
    }
    else if (event.type == sf::Event::TextEntered && selectedInputBox != -1)
    {
        if (event.text.unicode < 128)
        {
            std::string currentText = inputTexts[selectedInputBox].getString();
            if (event.text.unicode == '\b')
            {
                if (!currentText.empty())
                    currentText.pop_back();
            }
            else
            {
                currentText += static_cast<char>(event.text.unicode);
            }
            inputTexts[selectedInputBox].setString(currentText);

            float requiredHeight = calculateRequiredHeight(inputTexts[selectedInputBox], 180);
            inputBoxes[selectedInputBox].setSize(sf::Vector2f(180, requiredHeight));
            adjustLayout();
        }
    }
}

void EditorMap::PropertyEditor::applyChanges()
{
    if (!selectedEntity)
    {
        return;
    }
    std::cout << "\nSaved";
    auto properties = selectedEntity->entity->getEditableProperties();
    for (size_t i = 0; i < properties.size() && i < inputTexts.size(); ++i)
    {
        selectedEntity->entity->setProperty(properties[i].first, inputTexts[i].getString());
    }
}

void EditorMap::PropertyEditor::wrapText(sf::Text &text, float maxWidth)
{
    const std::string &words = text.getString();
    std::string wrappedText;
    std::string line;
    sf::Text testText = text;

    for (char c : words)
    {
        if (c == '\n')
        {
            wrappedText += line + '\n';
            line.clear();
        }
        else
        {
            testText.setString(line + c);
            if (testText.getLocalBounds().width > maxWidth && !line.empty())
            {
                wrappedText += line + '\n';
                line = c;
            }
            else
            {
                line += c;
            }
        }
    }
    wrappedText += line;
    text.setString(wrappedText);
}

float EditorMap::PropertyEditor::calculateRequiredHeight(const sf::Text &text, float maxWidth)
{
    sf::Text tempText = text;
    wrapText(tempText, maxWidth);
    return tempText.getLocalBounds().height + 20;
}

void EditorMap::PropertyEditor::adjustLayout()
{
    float yOffset = 10;
    for (size_t i = 0; i < inputBoxes.size(); ++i)
    {
        labels[i].setPosition(834, yOffset);
        inputBoxes[i].setPosition(834, yOffset + 20);
        inputTexts[i].setPosition(838, yOffset + 22);

        yOffset += 30 + inputBoxes[i].getSize().y;
    }
    adjustBackgroundSize();
}

void EditorMap::PropertyEditor::adjustBackgroundSize()
{
    if (inputBoxes.empty())
        return;

    float lastBoxBottom = inputBoxes.back().getPosition().y + inputBoxes.back().getSize().y;
    background.setSize(sf::Vector2f(200, lastBoxBottom + 20));
}