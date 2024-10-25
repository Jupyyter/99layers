#include "../hpp/libs.hpp"
bloodParticles::bloodParticles(sf::Vector2f position):Sprite(position)

{
    spawnParticles(position);
}

void bloodParticles::spawnParticles(sf::Vector2f position)
{
       for(int i=0;i<50;i++){

       world->spawn("bloodParticle",position.x,position.y);
       }
}
void bloodParticles::draw(sf::RenderWindow& window)const{

}
void bloodParticles::update(float deltaTime, const sf::Vector2u& windowSize){

}