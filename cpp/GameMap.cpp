#include "libs.hpp"

GameMap::Object::Object(int x, int y, int w, int h, std::string tname) : rect(sf::Vector2f(w, h))
{
    if (!tex.loadFromFile(tname)) {
        std::cerr << "Failed to load texture: " << tname << std::endl;
    }
    tex.setRepeated(true);
    rect.setTexture(&tex);
    rect.setTextureRect(sf::IntRect(0, 0, w, h));
    rect.setPosition(x, y);

    size_t lastSlash = tname.find_last_of("/\\");
    size_t lastDot = tname.find_last_of(".");
    if (lastSlash == std::string::npos) lastSlash = 0;
    else lastSlash++;
    if (lastDot == std::string::npos || lastDot < lastSlash) lastDot = tname.length();
    texid = tname.substr(lastSlash, lastDot - lastSlash);
}

void GameMap::Object::draw(sf::RenderWindow &window)
{
    window.draw(rect);
}

GameMap::GameMap(sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover)
{
    view.setSize(wndref.getSize().x, wndref.getSize().y);
}

GameMap::GameMap(std::string fname, sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover)
{
    view.setSize(wndref.getSize().x, wndref.getSize().y);
    loadFromFile(fname);
}

void GameMap::loadFromFile(const std::string& fname)
{
    if (std::filesystem::exists(fname))
    {
        std::ifstream file(fname, std::ios::binary);
        int size;
        file.read(reinterpret_cast<char*>(&size), sizeof(int));
        np = size;

        // Load objects
        for (int j = 0; j < size; j++)
        {
            int cmx, cmy, ssize;
            file.read(reinterpret_cast<char*>(&cmx), sizeof(int));
            file.read(reinterpret_cast<char*>(&cmy), sizeof(int));
            file.read(reinterpret_cast<char*>(&ssize), sizeof(int));
            for (int i = 0; i < ssize; i++)
            {
                int cx, cy, cw, ch, nameLength;
                file.read(reinterpret_cast<char*>(&cx), sizeof(int));
                file.read(reinterpret_cast<char*>(&cy), sizeof(int));
                file.read(reinterpret_cast<char*>(&cw), sizeof(int));
                file.read(reinterpret_cast<char*>(&ch), sizeof(int));
                file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
                std::string texName(nameLength, '\0');
                file.read(&texName[0], nameLength);
                obj[cmx][cmy].push_back(new Object(cx, cy, cw, ch, "../imgs/" + texName + ".png"));
            }
        }

        // Load entities
        int entityCount;
        file.read(reinterpret_cast<char*>(&entityCount), sizeof(int));
        for (int i = 0; i < entityCount; ++i)
        {
            int nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));

            std::string entityType(nameLength, '\0');
            file.read(&entityType[0], nameLength);

            float x, y;
            file.read(reinterpret_cast<char*>(&x), sizeof(float));
            file.read(reinterpret_cast<char*>(&y), sizeof(float));

            // Create the entity
            std::unique_ptr<Entity> newEntity(EntityFactory::createEntity(entityType, sf::Vector2f(x, y), *this));
            if (newEntity)
            {
                // Load entity properties
                int propertyCount;
                file.read(reinterpret_cast<char*>(&propertyCount), sizeof(int));
                for (int j = 0; j < propertyCount; ++j)
                {
                    file.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
                    std::string propName(nameLength, '\0');
                    file.read(&propName[0], nameLength);

                    int valueLength;
                    file.read(reinterpret_cast<char*>(&valueLength), sizeof(int));
                    std::string propValue(valueLength, '\0');
                    file.read(&propValue[0], valueLength);

                    newEntity->setProperty(propName, propValue);
                }

                activeEntities.push_back(newEntity.release());
            }
        }
    }
}

GameMap::~GameMap()
{
    for (const auto &x : obj)
    {
        for (const auto &y : x.second)
        {
            for (auto* object : y.second)
            {
                delete object;
            }
        }
    }

    for (auto* entity : activeEntities)
    {
        delete entity;
    }

    for (auto* item : allItems)
    {
        delete item;
    }
}

void GameMap::draw()
{
    for (const auto& object : obj[mx][my])
    {
        object->draw(wndref);
    }
}

void GameMap::changePart(int x, int y)
{
    mx += x;
    my += y;
    if (obj[mx][my].empty())
        np++;
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    view.setCenter(wx / 2 + wx * mx, wy / 2 + wy * my);
    wndref.setView(view);
}

sf::FloatRect GameMap::getPartBounds()
{
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    return sf::FloatRect(mx * wx, my * wy, wx, wy);
}

std::vector<sf::FloatRect> GameMap::getObjectBounds()
{
    std::vector<sf::FloatRect> bounds;
    for (const auto& object : obj[mx][my])
    {
        bounds.push_back(object->rect.getGlobalBounds());
    }
    return bounds;
}
/*std::vector<sf::FloatRect> GameMap::getEntityBounds()
{
    std::vector<sf::FloatRect> bounds;
    for (const auto& entity : activeEntities)
    {
        bounds.push_back(entity->getGlobalBounds());
    }
    return bounds;
}*/

void GameMap::resetEntities(sf::FloatRect &playerBounds)
{
    for (auto* entity : activeEntities)
    {
        delete entity;
    }
    activeEntities.clear();

    for (auto* item : allItems)
    {
        delete item;
    }
    allItems.clear();

    spawnEntities(playerBounds);
}

void GameMap::spawnEntities(sf::FloatRect &playerBounds)
{
    // This function would typically create entities based on the map data
    // For now, it's left as a placeholder
    // You would implement this based on your game's specific requirements
}

void GameMap::updateEntities(float deltaTime, const sf::Vector2u &windowSize)
{
    for (auto* entity : activeEntities)
    {
        entity->update(deltaTime, *this, windowSize);
    }
    removeDeadEntities();
}

void GameMap::drawEntities(sf::RenderWindow &window)
{
    for (auto* entity : activeEntities)
    {
        entity->draw(window);
    }
}

void GameMap::removeDeadEntities()
{
    activeEntities.erase(
        std::remove_if(activeEntities.begin(), activeEntities.end(),
            [](Entity* entity) {
                if (entity->shouldBeDead) {
                    delete entity;
                    return true;
                }
                return false;
            }),
        activeEntities.end()
    );
}