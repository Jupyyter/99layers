#include "../hpp/libs.hpp"
class HappyEnd;
class bloodParticle;

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
    Object* obj = nullptr;
    switch (hash(type.c_str()))
    {
    case "pacman"_hash:
        obj = new PacMan(transform.getPosition());
        break;
    case "arrow"_hash:
        obj = new Idk(transform.getPosition());
        break;
    case "capybaraa"_hash:
        obj = new Capybara(transform.getPosition());
        break;
    case "pengu"_hash:
        obj = new Penguin(transform.getPosition());
        break;
    case "HorusBrogans"_hash:
        obj = new HB(transform.getPosition());
        break;
    case "runnerspact"_hash:
        obj = new RP(transform.getPosition());
        break;
    case "groundbreaker"_hash:
        obj = new GB(transform.getPosition());
        break;
    case "chronostimepiece"_hash:
        obj = new CTP(transform.getPosition());
        break;
    case "poketIkeaman"_hash:
        obj = new II(transform.getPosition());
        break;
    case "ikeaman"_hash:
        obj = new Boss(transform.getPosition());
        break;
    case "laser"_hash:
        obj = new LaserBeam(transform.getPosition(), transform.getRotation());
        break;
    case "plank"_hash:
        obj = new Plank(transform.getPosition());
        break;
    case "table"_hash:
        obj = new TableFall(transform.getPosition());
        break;
    case "adidas"_hash:
    {
        sf::Vector2f pos=world->playerSpawnPosition;
        Player *player;
        if(pos.x==0){
            player = new Player(transform.getPosition());
        }
        else{
            player = new Player(pos);
        }
        world->playerRef = player;
        obj = player;
        break;
    }
    case "ak47"_hash:
        obj = new AK47();
        break;
    case "ak47item"_hash:
        obj = new AK(transform.getPosition());
        break;
    case "inventory"_hash:
        obj = new Inventory();
        break;
    case "akBullet"_hash:
        obj = new akBullet(transform.getPosition(), transform.getRotation());
        break;
    case "HappyEnd"_hash:{}
        obj = new HappyEnd(transform.getPosition());
        break;
    case "LaserEnd"_hash:{
        obj = new LaserEnd(transform.getPosition());
        break;

    }
    case "spawn"_hash:
        obj = new SpawnPoint(transform.getPosition());
        break;
    case "bloodParticles"_hash:
        obj = new bloodParticles(transform.getPosition());
        break;
    case "bloodParticle"_hash:
        obj = new bloodParticle(transform.getPosition());
        break;
    case "hedgehog1"_hash:
        obj = new hedgehog(transform.getPosition());
        break;
    case "woodSign"_hash:
        obj = new woodSign(transform.getPosition());
        break;
    case "sign0"_hash:
        obj = new sign0(transform.getPosition());
        break;
    case "TableFall"_hash:{
        obj = new TableFall(transform.getPosition());
        break;
    }
    }
    if(obj){
        obj->setName(type);
    }
    return obj;
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
case "bloodParticle"_hash:
        return bloodParticle::getPropertyDescriptors();
    case "woodSign"_hash:
        return woodSign::getPropertyDescriptors();
    default:
        return HappyEnd::getPropertyDescriptors();
    }
}