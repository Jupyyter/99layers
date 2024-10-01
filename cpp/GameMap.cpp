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
    for (auto *item : allItems)
    {
        delete item;
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

    // Read the number of entities
    int entityCount;
    file.read(reinterpret_cast<char *>(&entityCount), sizeof(int));

    for (int i = 0; i < entityCount; ++i)
    {
        // Read entity type
        int typeLength;
        file.read(reinterpret_cast<char *>(&typeLength), sizeof(int));
        std::string type(typeLength, '\0');
        file.read(&type[0], typeLength);

        // Read position
        float x, y;
        file.read(reinterpret_cast<char *>(&x), sizeof(float));
        file.read(reinterpret_cast<char *>(&y), sizeof(float));

        // Read size
        float width, height;
        file.read(reinterpret_cast<char *>(&width), sizeof(float));
        file.read(reinterpret_cast<char *>(&height), sizeof(float));

        std::string texturePath;
        if (type == "Object")
        {
            // Read texture info for Objects
            int texIdLength;
            file.read(reinterpret_cast<char *>(&texIdLength), sizeof(int));
            texturePath.resize(texIdLength);
            file.read(&texturePath[0], texIdLength);
        }

        // Create the entity and sprite
        std::unique_ptr<Entity> newEntity;
        sf::Sprite sprite;

        if (type == "Object")
        {
            // Handle Object type
            auto objectPtr = std::make_unique<Object>(x, y, width, height, texturePath);
            sprite = objectPtr->sprite;
            newEntity = std::move(objectPtr);
        }
        else
        {
            // Handle other entity types
            sf::Transformable transform;
            transform.setPosition(x, y);
            transform.setRotation(0);

            const sf::Texture &texture = EditorMap::entityTextures[type];
            float scaleX = width / texture.getSize().x;
            float scaleY = height / texture.getSize().y;
            transform.setScale(scaleX, scaleY);

            sprite.setTexture(texture);
            sprite.setPosition(x, y);
            sprite.setScale(scaleX, scaleY);

            newEntity.reset(EntityFactory::createEntity(type, transform, *this, true));
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
            if (newEntity)
            {
                newEntity->setProperty(name, value);
            }
        }

        if (newEntity)
        {
            // Create and add the placed entity
            auto placedEntity = std::make_unique<Entity::PlacedEntity>(Entity::PlacedEntity{
                std::move(sprite),
                type,
                std::move(newEntity)});

            // Add the entity to placedEntities
            placedEntities.push_back(std::move(placedEntity));
        }
        else
        {
            std::cerr << "Failed to create entity of type: " << type << std::endl;
        }
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
    allItems.clear();
    spawnEntities();
}

void GameMap::spawnEntities()
{
    for (const auto &placedEntity : placedEntities)
    {
        const sf::Vector2f position = placedEntity->sprite.getPosition();
        const sf::Vector2f scale = placedEntity->sprite.getScale();
        const std::string &type = placedEntity->type;

        sf::Transformable transform;
        transform.setPosition(position);
        transform.setScale(scale);

        std::unique_ptr<Entity> entity;
        if (type == "Object")
        {
            auto objectPtr = dynamic_cast<Object *>(placedEntity->entity.get());
            if (objectPtr)
            {
                entity = std::make_unique<Object>(*objectPtr);
            }
        }
        else
        {
            entity.reset(EntityFactory::createEntity(type, transform, *this, true));
        }

        if (entity)
        {
            // Copy properties from placedEntity to the new entity
            auto properties = placedEntity->entity->getEditableProperties();
            for (const auto &prop : properties)
            {
                entity->setProperty(prop.first, prop.second);
            }

            // Ensure the entity's position and scale match the placedEntity
            if (auto *entitySprite = dynamic_cast<sf::Sprite *>(entity.get()))
            {
                entitySprite->setPosition(position);
                entitySprite->setScale(scale);
            }

            spawn(entity.release());
        }
    }
}

void GameMap::updateEntities(float deltaTime, const sf::Vector2u &windowSize)
{
    const size_t entityCount = activeEntities.size();
    //first update all (physics and stuff)
    for (size_t i = 0; i < entityCount; ++i)
    {
        activeEntities[i]->update(deltaTime, *this, windowSize);
    }
    //override things based on collisions
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
        entity->draw(window);
    }
}

void GameMap::spawn(const std::string &entityName, float x, float y, float rotation)
{
    sf::Transformable transform;
    transform.setPosition(x, y);
    transform.setRotation(rotation);
    Entity *entity = EntityFactory::createEntity(entityName, transform, *this, true);
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
