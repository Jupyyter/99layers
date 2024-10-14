#pragma once
#include "libs.hpp"

class AK47;

class Inventory : public Sprite {
public:
    Inventory();
    ~Inventory() = default;

    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow &window) const override;
    void reset(Player* player);
    sf::RectangleShape borderHighlight;
    std::vector<int> activeSlots;
    void moveItemToInventory(int slotIndex);
    void moveItemToActiveSlot(int itemIndex, int slotIndex);
void addItem(Item* item);
private:
    void updateItemPositions();
    void loadPanel();
    void loadItems();
    void loadTexts();
    void selectItem(int i, bool isActiveSlot = false);
    int getHoverCell(sf::RenderWindow &window);
    int getActiveHoverCell(sf::RenderWindow &window);
    bool isOnScreen() const override;

    sf::Font font;
    sf::Text text;
    sf::Texture bpTex, cellTex;
    sf::Sprite bpSprite, cellSprite[40], selectedSquare, selectedItemS, activeCellS[3];
    std::vector<Item*> allItems;
    Item::Active* active[3];
    std::vector<int> ownedItems, unownedItems;
    int selectedItem, pgcount, movedItem;
    bool shouldDraw, fc, movingItem;

    // New text elements
    sf::Text infoText, itemsText, activeItemsText;
    sf::Text zText, xText, cText;
};