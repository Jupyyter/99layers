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

void GameMap::loadFromFile(const std::string &fname)
{
    if (std::filesystem::exists(fname))
    {
        std::ifstream file(fname, std::ios::binary);
        int size;
        file.read(reinterpret_cast<char *>(&size), sizeof(int));
        np = size;

        // Load objects
        for (int j = 0; j < size; j++)
        {
            int cmx, cmy, ssize;
            file.read(reinterpret_cast<char *>(&cmx), sizeof(int));
            file.read(reinterpret_cast<char *>(&cmy), sizeof(int));
            file.read(reinterpret_cast<char *>(&ssize), sizeof(int));
            for (int i = 0; i < ssize; i++)
            {
                int cx, cy, cw, ch, nameLength;
                file.read(reinterpret_cast<char *>(&cx), sizeof(int));
                file.read(reinterpret_cast<char *>(&cy), sizeof(int));
                file.read(reinterpret_cast<char *>(&cw), sizeof(int));
                file.read(reinterpret_cast<char *>(&ch), sizeof(int));
                file.read(reinterpret_cast<char *>(&nameLength), sizeof(int));
                std::string texName(nameLength, '\0');
                file.read(&texName[0], nameLength);
                obj[cmx][cmy].push_back(std::make_unique<Object>(cx, cy, cw, ch, "../imgs/" + texName + ".png"));
            }
        }

        // Load placedEntities and allItems
        int entityCount;
        file.read(reinterpret_cast<char *>(&entityCount), sizeof(int));
        for (int i = 0; i < entityCount; ++i)
        {
            int nameLength;
            file.read(reinterpret_cast<char *>(&nameLength), sizeof(int));

            std::string entityType(nameLength, '\0');
            file.read(&entityType[0], nameLength);

            float x, y;
            file.read(reinterpret_cast<char *>(&x), sizeof(float));
            file.read(reinterpret_cast<char *>(&y), sizeof(float));

            sf::Transformable transform;
            transform.setPosition(sf::Vector2f(x, y));
            transform.setRotation(0);
            transform.setScale(1, 1);
            // Create the entity
            Entity *newEntity = EntityFactory::createEntity(entityType, transform, *this, true);
            if (newEntity)
            {
                // Load entity properties
                int propertyCount;
                file.read(reinterpret_cast<char *>(&propertyCount), sizeof(int));
                for (int j = 0; j < propertyCount; ++j)
                {
                    file.read(reinterpret_cast<char *>(&nameLength), sizeof(int));
                    std::string propName(nameLength, '\0');
                    file.read(&propName[0], nameLength);

                    int valueLength;
                    file.read(reinterpret_cast<char *>(&valueLength), sizeof(int));
                    std::string propValue(valueLength, '\0');
                    file.read(&propValue[0], valueLength);

                    newEntity->setProperty(propName, propValue);
                }

                auto placedEntity = std::make_unique<Entity::PlacedEntity>();
                placedEntity->type = entityType;
                placedEntity->sprite.setPosition(x, y);
                placedEntity->entity.reset(newEntity);
                placedEntities.push_back(std::move(placedEntity));

                // If the entity is an item, it's already added to allItems by EntityFactory
            }
        }
    }
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

void GameMap::draw() const
{
    for (const auto &object : obj.at(mx).at(my))
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

sf::FloatRect GameMap::getPartBounds() const
{
    int wx = wndref.getSize().x, wy = wndref.getSize().y;
    return sf::FloatRect(mx * wx, my * wy, wx, wy);
}

std::vector<sf::FloatRect> GameMap::getObjectBounds() const
{
    std::vector<sf::FloatRect> bounds;
    bounds.reserve(obj.at(mx).at(my).size());
    for (const auto &object : obj.at(mx).at(my))
    {
        bounds.push_back(object->rect.getGlobalBounds());
    }
    return bounds;
}

void GameMap::resetEntities()
{
    changePart(0, 0);
    activeEntities.clear();
    allItems.clear();
    spawnEntities();
}

void GameMap::updateEntities(float deltaTime, const sf::Vector2u &windowSize)
{
    const size_t entityCount = activeEntities.size();
    for (size_t i = 0; i < entityCount; ++i)
    {
        Entity *entity = activeEntities[i];
        entity->update(deltaTime, *this, windowSize);

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

void GameMap::spawnEntities()
{
    for (const auto &placedEntity : placedEntities)
    {
        const sf::Vector2f position = placedEntity->sprite.getPosition();
        const std::string &type = placedEntity->type;
        sf::Transformable transform;
        transform.setPosition(position);
        std::unique_ptr<Entity> entity(EntityFactory::createEntity(type, transform, *this, true));
        if (entity)
        {
            auto properties = placedEntity->entity->getEditableProperties();
            for (const auto &prop : properties)
            {
                entity->setProperty(prop.first, prop.second);
            }
            spawn(entity.release());
        }
    }
}
bool GameMap::checkPixelPerfectCollision(sf::Sprite s1, sf::Sprite s2, int step) {
        sf::FloatRect intersection;
        if (s1.getGlobalBounds().intersects(s2.getGlobalBounds(), intersection)) {
            sf::Transform t1 = s1.getTransform().getInverse();
            sf::Transform t2 = s2.getTransform().getInverse();

            for (int x = static_cast<int>(intersection.left); x < intersection.left + intersection.width; x += step) {
                for (int y = static_cast<int>(intersection.top); y < intersection.top + intersection.height; y += step) {
                    sf::Vector2f p1 = t1.transformPoint(static_cast<float>(x), static_cast<float>(y));
                    sf::Vector2f p2 = t2.transformPoint(static_cast<float>(x), static_cast<float>(y));

                    if (s1.getTextureRect().contains(static_cast<int>(p1.x), static_cast<int>(p1.y)) &&
                        s2.getTextureRect().contains(static_cast<int>(p2.x), static_cast<int>(p2.y))) {
                        sf::Color c1 = s1.getTexture()->copyToImage().getPixel(static_cast<unsigned int>(p1.x), static_cast<unsigned int>(p1.y));
                        sf::Color c2 = s2.getTexture()->copyToImage().getPixel(static_cast<unsigned int>(p2.x), static_cast<unsigned int>(p2.y));
                        if (c1.a > 0 && c2.a > 0) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }