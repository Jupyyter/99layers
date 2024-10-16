#pragma once
class Inventory;

class Item : public Sprite, public CollisionDetector {
    friend class Inventory;
public:
    class Active {
    public:
        virtual void activate() = 0;
    };

    Item(const sf::Vector2f &position, float sizet, float speedb, float jumpb, std::string description, std::string name, std::string fname);
    void onCollision(Entity *other) override;
    virtual ~Item() = default;
    bool shouldApplyItemChangesToPlayer;
    virtual void updateOwned(Player *player) = 0;
    virtual void applyItemChanges()=0;
    void update(float deltaTime, const sf::Vector2u &screenres) override;
    void draw(sf::RenderWindow &window)const  override;
    
    virtual std::string customText();


protected:
    float sizet, speedb, jumpb;
    std::string description, name;

protected:
    void applyStats(Player* player);
    void addStats(Player *player, float speed, float jump);
    void setOneHitInv(Player* player, bool val);
    bool getOneHitInv(Player* player);
    bool isGrounded(Player* player);
    float getJumpForce(Player* player);
    void setStasis(Player* player, bool stasis);
};
class II:public Item{
    public:
    II(const sf::Vector2f &position);
    ~II() = default;

    void updateOwned(Player *player) override;
    // update is inherited from Item

    void applyItemChanges() override;

};
class AK:public Item{
    public:
    AK(const sf::Vector2f &position);
    ~AK() = default;

    void updateOwned(Player *player) override;
    // update is inherited from Item

    void applyItemChanges() override;

};
//stands for Horus's Brogans
class HB : public Item {
public:
    HB(const sf::Vector2f &position);
    ~HB() = default;

    void updateOwned(Player *player) override;
    // update is inherited from Item

    void applyItemChanges() override;

private:
    sf::Clock ctimer;
    sf::Clock btimer;
    bool pinv, fc;
};

//stands for Runner's Pact
class RP : public Item{
    public:
    RP(const sf::Vector2f &position);
    ~RP() = default;

    void updateOwned(Player *player) override;//the update if you own the item
    void applyItemChanges() override;

    //for custom text writing
    std::string customText() override;

    private:
    float px, xdis, accsp;
};

//stands for Ground Breaker
class GB: public Item{
    public:
    GB(const sf::Vector2f &position);
    ~GB() = default;

    void updateOwned(Player *player) override;//the update if you own the item
    void applyItemChanges() override;

    private:
    int jc;
    bool isGroundedP;
    float jpowerg;
};

//stands for Chronos Time Piece
class CTP : public Item, public Item::Active{
    public:
    CTP(const sf::Vector2f &position);
    ~CTP() = default;
    void applyItemChanges() override;
    void updateOwned(Player *player) override;//the update if you own the item
    void activate() override;//the functions that gets called when the button related to your active ability gets pressed

    private:
    sf::Clock timer;
    bool activated, fc;
};