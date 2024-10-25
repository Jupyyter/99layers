#include "../hpp/libs.hpp"
class HappyEnd;

// Helper function to create compile-time string hashes
constexpr unsigned int hash(const char *str, int h = 0)
{
    return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
}

// String literal operator to create compile-time string hashes
constexpr unsigned int operator"" _hash(const char *p, size_t)
{
    return hash(p);
}

Object *ObjectFactory::createObject(const std::string &type, sf::Transformable transform)
{
    switch (hash(type.c_str()))
    {
    case "pacman"_hash:
        return new PacMan(transform.getPosition());
    case "arrow"_hash:
        return new Idk(transform.getPosition());
    case "capybaraa"_hash:
        return new Capybara(transform.getPosition());
    case "pengu"_hash:
        return new Penguin(transform.getPosition());
    case "HorusBrogans"_hash:
        return new HB(transform.getPosition());
    case "runnerspact"_hash:
        return new RP(transform.getPosition());
    case "groundbreaker"_hash:
        return new GB(transform.getPosition());
    case "chronostimepiece"_hash:
        return new CTP(transform.getPosition());
    case "poketIkeaman"_hash:
        return new II(transform.getPosition());
    case "ikeaman"_hash:
        return new Boss(transform.getPosition());
    case "laser"_hash:
        return new LaserBeam(transform.getPosition(), transform.getRotation());
    case "plank"_hash:
        return new Plank(transform.getPosition());
    case "table"_hash:
        return new TableFall(transform.getPosition());
    case "adidas"_hash:
    {
        Player *player = new Player(transform.getPosition());
        world->playerRef = player;
        return player;
    }
    case "ak47"_hash:
        return new AK47();
    case "ak47item"_hash:
        return new AK(transform.getPosition());
    case "inventory"_hash:
        return new Inventory();
    case "akBullet"_hash:
        return new akBullet(transform.getPosition(), transform.getRotation());
    case "HappyEnd"_hash:{}
        return new HappyEnd(transform.getPosition());
    case "LaserEnd"_hash:
        return new LaserEnd(transform.getPosition());
    case "spawn"_hash:
        return new SpawnPoint(transform.getPosition());
    case "bloodParticles"_hash:
        return new bloodParticles(transform.getPosition());
    default:
        return nullptr;
    }
}

std::vector<PropertyDescriptor> ObjectFactory::getPropertyDescriptors(const std::string &type)
{
    switch (hash(type.c_str()))
    {
    case "pacman"_hash:
        return PacMan::getPropertyDescriptors();
    case "arrow"_hash:
        return Idk::getPropertyDescriptors();
    case "capybaraa"_hash:
        return Capybara::getPropertyDescriptors();
    case "pengu"_hash:
        return Penguin::getPropertyDescriptors();
    case "HorusBrogans"_hash:
        return HB::getPropertyDescriptors();
    case "runnerspact"_hash:
        return RP::getPropertyDescriptors();
    case "groundbreaker"_hash:
        return GB::getPropertyDescriptors();
    case "chronostimepiece"_hash:
        return CTP::getPropertyDescriptors();
    case "poketIkeaman"_hash:
        return II::getPropertyDescriptors();
    case "ikeaman"_hash:
        return Boss::getPropertyDescriptors();
    case "adidas"_hash:
        return Player::getPropertyDescriptors();
    case "ak47"_hash:
        return AK47::getPropertyDescriptors();
    case "ak47item"_hash:
        return AK::getPropertyDescriptors();
    case "Terrain"_hash:
        return Terrain::getPropertyDescriptors();
    case "spawn"_hash:
        return SpawnPoint::getPropertyDescriptors();
        case "bloodParticles"_hash:
    return bloodParticles::getPropertyDescriptors();
    default:
        return HappyEnd::getPropertyDescriptors();
    }
}