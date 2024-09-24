#pragma once
class AK47;
class Inventory {
public:
    Inventory(GameMap& gamemap);
    ~Inventory() = default;

    void update(Player* player,GameMap &gamemap);
    void draw(sf::RenderWindow &window);
    void reset(Player* player);
    sf::RectangleShape borderHighlight;
    std::vector<int> activeSlots;
    void moveItemToInventory(int slotIndex);
    void moveItemToActiveSlot(int itemIndex, int slotIndex);

private:
    void updateItemPositions();
    void loadPanel(GameMap& gamemap);
    void loadItems();
    void selectItem(int i, bool isActiveSlot = false);
    int getHoverCell(sf::RenderWindow &window);
    int getActiveHoverCell(sf::RenderWindow &window);

    sf::Font font;
    sf::Text text;
    sf::Texture bpTex, cellTex;
    sf::Sprite bpSprite, cellSprite[40], selectedSquare, selectedItemS, activeCellS[3];
    std::vector<Item*> *allItems;
    Item::Active* active[3];
    std::vector<int> ownedItems, unownedItems;
    int selectedItem, pgcount, movedItem;
    bool shouldDraw, fc, movingItem;
};