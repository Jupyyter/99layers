#pragma once

class Inventory {
public:
    Inventory(Map& map, Player* player, sf::RenderWindow& window);
    ~Inventory() = default;

    void update();
    void draw();
    void reset(Player* player);
    sf::RectangleShape borderHighlight;
    std::vector<int> activeSlots;
    void moveItemToInventory(int slotIndex);
    void moveItemToActiveSlot(int itemIndex, int slotIndex);

private:
    void updateItemPositions();
    void loadPanel();
    void loadItems();
    void selectItem(int i, bool isActiveSlot = false);
    int getHoverCell();
    int getActiveHoverCell();

    sf::Font font;
    sf::Text text;
    sf::Texture bpTex, cellTex;
    sf::Sprite bpSprite, cellSprite[40], selectedSquare, selectedItemS, activeCellS[3];
    std::vector<Item*> *allItems;
    Item::Active* active[3];
    std::vector<int> ownedItems, unownedItems;
    int selectedItem, pgcount, movedItem;
    bool shouldDraw, fc, movingItem;
    Map &mapr;
    Player *playerr;
    sf::RenderWindow &windowr;
};