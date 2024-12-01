#include "../hpp/libs.hpp"

Item::Item(const sf::Vector2f &position, float sizet, float speedb, float jumpb, std::string description, std::string name, std::string fname) : sizet(sizet),
                                                                                                                                                 speedb(speedb),
                                                                                                                                                 jumpb(jumpb),
                                                                                                                                                 name(name),
                                                                                                                                                 description(description),
                                                                                                                                                 owned(false),
                                                                                                                                                 shouldApplyItemChangesToPlayer(false),
                                                                                                                                                 CollisionDetector(),
                                                                                                                                                 Sprite(sf::Vector2f(position))
{
    priorityLayer=5;
    this->texture.loadFromFile(fname);
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(position.x, position.y);
}

void Item::applyStats(Player *player)
{
    player->jumpForce *= this->jumpb;
    player->moveSpeed *= this->speedb;
    player->sprite.scale(sizet, sizet);
}

void Item::addStats(Player *player, float speed, float jump)
{
    player->jumpForce += jump;
    player->moveSpeed += speed;
}

std::string Item::customText()
{
    return "";
}

void Item::setOneHitInv(Player *player, bool val)
{
    player->onehitinvin = val;
    if (!val)
        player->gothitinv = false;
}

bool Item::getOneHitInv(Player *player)
{
    return player->gothitinv;
}

bool Item::isGrounded(Player *player)
{
    return player->isGrounded;
}

float Item::getJumpForce(Player *player)
{
    return player->jumpForce;
}

void Item::setStasis(Player *player, bool stasis)
{
    player->isStasis = stasis;
}

void Item::draw(sf::RenderWindow &window) const
{
    Sprite::draw(window);
}
void Item::update(float deltaTime, const sf::Vector2u &screenres)
{
}
void Item::onCollision(Sprite *other)
{
    if (owned == false && typeid(*other) == typeid(Player))
    {
        owned = true;
        shouldApplyItemChangesToPlayer = true;
        invisible = true;
        world->playerRef->inventory->addItem(this);
    }
}
II::II(const sf::Vector2f &position) : Item(position, 1, 1, 1, "IkeaMan is mad at you\nyou better run", "IKEAMAN", "../imgs/poketIkeaman.png")
{
}
void II::update(float deltaTime, const sf::Vector2u &screenres)
{
}
void II::applyItemChanges()
{
}
AK::AK(const sf::Vector2f &position) : Item(position, 1, 1, 1, "AK 47\nthe one and only", "AK 47", "../imgs/ak47item.png")
{
}
void AK::update(float deltaTime, const sf::Vector2u &screenres)
{
}
void AK::applyItemChanges()
{
    world->spawn("ak47", world->playerRef->getPosition().x, world->playerRef->getPosition().y, 0);
}
HB::HB(const sf::Vector2f &position) : Item(position, 1, 1.25, 1.1, "gives you the ability to jump", "Horus's Brogans", "../imgs/HorusBrogans.png")
{
    this->ctimer.restart();
    this->btimer.restart();
    this->fc = true;
}
void HB::update(float deltaTime, const sf::Vector2u &screenres)
{
    if (owned)
    {
        if (ctimer.getElapsedTime().asSeconds() >= 10.0f)
        {
            setOneHitInv(world->playerRef, true);
            pinv = true;
        }
        if (pinv && getOneHitInv(world->playerRef))
        {
            if (fc)
            {
                btimer.restart();
                fc = false;
            }
            else if (btimer.getElapsedTime().asSeconds() >= 0.2f)
            {
                setOneHitInv(world->playerRef, false);
                fc = true;
                ctimer.restart();
            }
        }
    }
}
void HB::applyItemChanges()
{
    world->playerRef->isJump=true;
    applyStats(world->playerRef);
}
RP::RP(const sf::Vector2f &position) : Item(position, 1.0f, 1.0f, 0.85f, "You gain 10 speed for every 300 units of distance traveled,\n up to a max of 200", "Runner's Pact", "../imgs/runnerspact.png")
{
    this->xdis = 0;
    this->accsp = 0;
}
void RP::update(float deltaTime, const sf::Vector2u &screenres)
{
    if (owned&&world->isPlayerValid)
    {
        xdis += std::abs(px - world->playerRef->getBounds().getPosition().x);
        px = world->playerRef->getBounds().getPosition().x;
        if (accsp < 200 && xdis >= 300.0f)
        {
            addStats(world->playerRef, 10, 0);
            accsp += 10;
            xdis -= 300.0f;
        }
    }
}
void RP::applyItemChanges()
{
    applyStats(world->playerRef);
}
std::string RP::customText()
{
    return "Accumulated Speed: " + std::to_string(int(accsp)) + "/200";
}

GB::GB(const sf::Vector2f &position) : Item(position, 1.0f, 1.0f, 1.35f, "Every third jump has 50% bonus jump power", "Groundbreaker", "../imgs/groundbreaker.png")
{
    this->jc = 0;
}
void GB::update(float deltaTime, const sf::Vector2u &screenres)
{
    if (owned)
    {
        if (!isGroundedP && isGrounded(world->playerRef))
        {
            jc++;
            if (jc == 2)
            {
                jpowerg = getJumpForce(world->playerRef) * 0.5f;
                addStats(world->playerRef, 0, jpowerg);
            }
            else if (jc == 3)
            {
                addStats(world->playerRef, 0, -jpowerg);
                jc = 0;
            }
        }
        isGroundedP = isGrounded(world->playerRef);
    }
}
void GB::applyItemChanges()
{
    applyStats(world->playerRef);
}
CTP::CTP(const sf::Vector2f &position) : Item(position, 1, 1, 1, "Active(2 second cooldown):\n When used, you enter a stasis\nthat renders you invulnerable for 2.5 seconds", "Chronos's Time Piece", "../imgs/chronostimepiece.png")
{
    activated = false;
    fc = true;
}
void CTP::applyItemChanges()
{
    applyStats(world->playerRef);
}
void CTP::update(float deltaTime, const sf::Vector2u &screenres)
{
    if(owned){
        if (activated)
    {
        if (fc)
        {
            fc = false;
            setStasis(world->playerRef, true);
            timer.restart();
        }
        else if (timer.getElapsedTime().asSeconds() >= 2.5f)
        {
            fc = true;
            activated = false;
            setStasis(world->playerRef, false);
            timer.restart();
        }
    }
    }
}

void CTP::activate()
{
    if (timer.getElapsedTime().asSeconds() >= 2.0f)
        activated = true;
}
