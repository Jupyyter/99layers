#include "../hpp/libs.hpp"
class EditorMap;

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

GameMap::~GameMap()
{
    for (auto *entity : activeEntities)
    {
        delete entity;
    }
}

void GameMap::loadFromFile(const std::string &fname)
{
    std::ifstream file(fname, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open file for reading: " << fname << std::endl;
        return;
    }

    // Clear existing entities
    activeEntities.clear();
    placedEntities.clear();

    int entityCount;
    file.read(reinterpret_cast<char *>(&entityCount), sizeof(int));

    for (int i = 0; i < entityCount; ++i)
    {
        EditorMap::PlacedEntity entity;

        // Read entity type
        int typeLength;
        file.read(reinterpret_cast<char *>(&typeLength), sizeof(int));
        entity.type.resize(typeLength);
        file.read(&entity.type[0], typeLength);

        // Read position and size
        float x, y, width, height;
        file.read(reinterpret_cast<char *>(&x), sizeof(float));
        file.read(reinterpret_cast<char *>(&y), sizeof(float));
        file.read(reinterpret_cast<char *>(&width), sizeof(float));
        file.read(reinterpret_cast<char *>(&height), sizeof(float));

        // Read texture path
        int texturePathLength;
        file.read(reinterpret_cast<char *>(&texturePathLength), sizeof(int));
        entity.texturePath.resize(texturePathLength);
        file.read(&entity.texturePath[0], texturePathLength);

        // Set up the sprite
        entity.sprite.setPosition(x, y);
        sf::Texture texture;
        std::string fullTexturePath = entity.texturePath;
        if (texture.loadFromFile(fullTexturePath))
        {
            entity.sprite.setTexture(texture);
            if (entity.type == "Terrain")
            {
                entity.sprite.setTextureRect(sf::IntRect(0, 0, width, height));
            }
            else
            {
                float scaleX = width / texture.getSize().x;
                float scaleY = height / texture.getSize().y;
                entity.sprite.setScale(scaleX, scaleY);
            }
        }

        // Read properties
        int propertyCount;
        file.read(reinterpret_cast<char *>(&propertyCount), sizeof(int));
        for (int j = 0; j < propertyCount; ++j)
        {
            int nameLength, valueLength;
            file.read(reinterpret_cast<char *>(&nameLength), sizeof(int));
            std::string name(nameLength, '\0');
            file.read(&name[0], nameLength);

            file.read(reinterpret_cast<char *>(&valueLength), sizeof(int));
            std::string value(valueLength, '\0');
            file.read(&value[0], valueLength);
            entity.properties[name] = value;
        }

        placedEntities.push_back(std::move(entity));
    }
}
void GameMap::draw() const
{
    for (const auto &entity : activeEntities)
    {
        entity->draw(wndref);
    }
}

void GameMap::changePart(int x, int y)
{
    mx += x;
    my += y;
    np++;
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    view.setCenter(wx / 2 + wx * mx, wy / 2 + wy * my);
    wndref.setView(view);
}

sf::FloatRect GameMap::getPartBounds() const
{
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    return sf::FloatRect(mx * wx, my * wy, wx, wy);
}

void GameMap::resetEntities()
{
    changePart(0, 0);
    activeEntities.clear();
    spawnEntities();
}

void GameMap::spawnEntities()
{
    for (const auto &placedEntity : placedEntities)
    {
        sf::Vector2f entityPos = placedEntity.sprite.getPosition();
        sf::Vector2f entitySize = placedEntity.sprite.getGlobalBounds().getSize();
        Entity* newEntity = nullptr;

        if (placedEntity.type == "Terrain")
        {
            auto terrainPtr = std::make_unique<Terrain>(
                static_cast<int>(entityPos.x),
                static_cast<int>(entityPos.y),
                static_cast<int>(entitySize.x),
                static_cast<int>(entitySize.y),
                placedEntity.texturePath);
            newEntity = terrainPtr.release();
        }
        else
        {
            sf::Transformable transform;
            transform.setPosition(placedEntity.sprite.getPosition());
            transform.setScale(placedEntity.sprite.getScale());
            newEntity = EntityFactory::createEntity(placedEntity.type, transform);
        }

        if (newEntity)
        {
            newEntity->setPosition(entityPos);
            // Set entity properties
            auto descriptors = EntityFactory::getPropertyDescriptors(placedEntity.type);
            for (const auto &desc : descriptors)
            {
                auto it = placedEntity.properties.find(desc.name);
                if (it != placedEntity.properties.end() && desc.setter)
                {
                    desc.setter(newEntity, it->second);
                }
            }
            spawn(newEntity);
        }
    }
}

void GameMap::updateEntities(float deltaTime, const sf::Vector2u &windowSize)
{
    const size_t entityCount = activeEntities.size();
    // first update all (physics and stuff)
    for (size_t i = 0; i < entityCount; ++i)
    {
        activeEntities[i]->update(deltaTime, windowSize);
    }
    // override things based on collisions
    for (size_t i = 0; i < entityCount; ++i)
    {
        Entity *entity = activeEntities[i];

        for (size_t j = i + 1; j < entityCount; ++j)
        {
            Entity *otherEntity = activeEntities[j];
            if (checkCollision(entity->getSprite(), otherEntity->getSprite()))
            {
                entity->onCollision(otherEntity);
                otherEntity->onCollision(entity);
            }
        }
    }
}

void GameMap::removeDeadEntities()
{
    activeEntities.erase(
        std::remove_if(activeEntities.begin(), activeEntities.end(),
                       [](Entity *entity)
                       {
                           if (entity->shouldBeDead)
                           {
                               delete entity;
                               return true;
                           }
                           return false;
                       }),
        activeEntities.end());
}

void GameMap::drawEntities(sf::RenderWindow &window) const
{
    for (const auto *entity : activeEntities)
    {
        if (entity->isOnScreen())
        {
            entity->draw(window);
        }
    }
}

void GameMap::spawn(const std::string &entityName, float x, float y, float rotation)
{
    sf::Transformable transform;
    transform.setPosition(x, y);
    transform.setRotation(rotation);
    Entity *entity = EntityFactory::createEntity(entityName, transform);
    if (entity)
    {
        auto insertPos = std::lower_bound(activeEntities.begin(), activeEntities.end(), entity,
                                          [](const Entity *a, const Entity *b)
                                          {
                                              return a->priorityLayer < b->priorityLayer;
                                          });
        activeEntities.insert(insertPos, entity);
    }
}

void GameMap::spawn(Entity *entity)
{
    if (entity)
    {
        auto insertPos = std::lower_bound(activeEntities.begin(), activeEntities.end(), entity,
                                          [](const Entity *a, const Entity *b)
                                          {
                                              return a->priorityLayer < b->priorityLayer;
                                          });
        activeEntities.insert(insertPos, entity);
    }
}
