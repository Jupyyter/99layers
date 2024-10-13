#include "../hpp/libs.hpp"
class HappyEnd;
Entity *EntityFactory::createEntity(const std::string &type,sf::Transformable transform)
{
    if (type == "pacman")
    {
        return new PacMan(transform.getPosition());
    }
    else if (type == "arrow")
    {
        return new Idk(transform.getPosition(), 200);
    }
    else if (type == "capybaraa")
    {
        return new Npc(transform.getPosition());
    }
    else if (type == "pengu")
    {
        return new Penguin(transform.getPosition());
    }
    else if (type == "HorusBrogans")
    {
        return new HB(transform.getPosition());
    }
    else if (type == "runnerspact")
    {
        return new RP(transform.getPosition());;
    }
    else if (type == "groundbreaker")
    {
        return new GB(transform.getPosition());
    }
    else if (type == "chronostimepiece")
    {
        return new CTP(transform.getPosition());
    }
    else if(type=="poketIkeaman"){
        return new II(transform.getPosition());
    }
    else if(type=="ikeaman"){
        return new Boss(transform.getPosition());
    }
    else if(type=="laser"){
        return new LaserBeam(transform.getPosition(),transform.getRotation());
    }
    else if(type=="plank"){
        return new Plank(transform.getPosition());
    }
    else if(type=="table"){
        return new TableFall(transform.getPosition());
    }
    else if (type=="adidas"){
        Player *player = new Player(transform.getPosition());
        world->playerRef=player;
        return player;
    }
    else if(type=="ak47"){
        return new AK47();
    }
    else if(type=="ak47item"){
        AK *item = new AK(transform.getPosition());
        return item;
    }
    else if(type=="inventory"){
        return new Inventory();
    }
    else if(type=="akBullet"){
         return new akBullet(transform.getPosition(),transform.getRotation());
    }
    else if(type=="HappyEnd"){
        return new HappyEnd(transform.getPosition());
    }
    else if(type=="LaserEnd"){
        return new LaserEnd(transform.getPosition());
    }
    return nullptr;
}
std::vector<PropertyDescriptor> EntityFactory::getPropertyDescriptors(const std::string& type)
{
    if (type == "pacman")
    {
        return PacMan::getPropertyDescriptors();
    }
    else if (type == "arrow")
    {
        return Idk::getPropertyDescriptors();
    }
    else if (type == "capybaraa")
    {
        return Npc::getPropertyDescriptors();
    }
    else if (type == "pengu")
    {
        return Penguin::getPropertyDescriptors();
    }
    else if (type == "HorusBrogans")
    {
        return HB::getPropertyDescriptors();
    }
    else if (type == "runnerspact")
    {
        return RP::getPropertyDescriptors();
    }
    else if (type == "groundbreaker")
    {
        return GB::getPropertyDescriptors();
    }
    else if (type == "chronostimepiece")
    {
        return CTP::getPropertyDescriptors();
    }
    else if(type=="poketIkeaman"){
        return II::getPropertyDescriptors();
    }
    else if(type=="ikeaman"){
        return Boss::getPropertyDescriptors();
    }
    else if (type=="adidas"){
        return Player::getPropertyDescriptors();
        
    }
    else if(type=="ak47"){
        return AK47::getPropertyDescriptors();
    }
    else if(type=="ak47item"){
        return AK::getPropertyDescriptors();
    }
    else /*if(type=="HappyEnd")*/{
        return HappyEnd::getPropertyDescriptors();
    }
}