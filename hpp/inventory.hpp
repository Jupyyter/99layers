#pragma once
#include "libs.hpp"

class AK47;

class Inventory : public Sprite {
public:
    Inventory();
    ~Inventory() = default;

    void update(float deltaTime, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow &window) const override;
    void addItem(Item* item);
    void moveItemToInventory(int slotIndex);
    void moveItemToActiveSlot(int itemIndex, int slotIndex);
    void saveItems();

void updateItemScaling();
    void updateTextScaling();
    float m_currentScale;
    sf::RectangleShape borderHighlight;
    std::vector<int> activeSlots;
void updateScale();
void handleScaling();
private:
    sf::Vector2u m_lastWindowSize;
sf::Vector2f m_originalScale;
    sf::Vector2f m_baseResolution;
    void centerPanel();
    float getScaleFactor() const;
    void loadResources();
    void setupUI();
    void setupTexts();
    void handleInventoryToggle();
    void handleItemSelection();
    void handleItemMovement();
    void handleActiveItems();
    void updateItemVisibility();
    void drawUI(sf::RenderWindow &window) const;
    void drawItems(sf::RenderWindow &window) const;
    void drawSelectedItem(sf::RenderWindow &window) const;
    void updateItemPositions();
    void selectItem(int i, bool isActiveSlot);
    void startItemMovement(const sf::Vector2i &mousePos);
    void finishItemMovement(const sf::Vector2i &mousePos);
    int getHoverCell(sf::RenderWindow &window);
    int getActiveHoverCell(sf::RenderWindow &window);
    bool isOnScreen() const override;

    sf::Font font;
    sf::Text text;
    sf::Texture bpTex, cellTex;
    sf::Sprite bpSprite, cellSprite[40], selectedSquare, selectedItemS, activeCellS[3];
    std::vector<Item*> allItems;
    Item::Active* active[3];
    std::vector<int> ownedItems;
    int selectedItem, pgcount, movedItem;
    //first check -- fc
    bool shouldDraw, fc, movingItem;

    // Text elements
    sf::Text infoText, itemsText, activeItemsText;
    sf::Text zText, xText, cText;
};