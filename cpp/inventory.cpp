#include "../hpp/libs.hpp"

Inventory::Inventory() : Sprite(sf::Vector2f(world->getPartBounds().width / 2.0f - bpSprite.getGlobalBounds().width / 2.0f,
                         world->getPartBounds().height / 2.0f - bpSprite.getGlobalBounds().height / 2.0f)), 
                         pgcount(1), selectedItem(-1), shouldDraw(false), fc(true), 
                         movingItem(false), activeSlots(3, -1) {
    priorityLayer = 777;
    // Initialize active array
    for (int i = 0; i < 3; i++) {
        active[i] = nullptr;
    }
    loadResources();
}

void Inventory::loadResources() {
    font.loadFromFile("../fonts/font.ttf");
    bpTex.loadFromFile("../imgs/BigPanel.jpg");
    cellTex.loadFromFile("../imgs/Cell.png");

    bpSprite.setTexture(bpTex);
    bpSprite.setPosition(world->getPartBounds().width / 2.0f - bpSprite.getGlobalBounds().width / 2.0f,
                         world->getPartBounds().height / 2.0f - bpSprite.getGlobalBounds().height / 2.0f);

    for (int i = 0; i < 40; i++) {
        cellSprite[i].setTexture(cellTex);
        cellSprite[i].setPosition(bpSprite.getPosition().x + 40 + (i % 5) * cellTex.getSize().x,
                                  bpSprite.getPosition().y + 40 + (i / 5) * cellTex.getSize().y);
    }

    setupUI();
    setupTexts();
}

void Inventory::setupUI() {
    float infoSectionX = cellSprite[4].getPosition().x + cellSprite[4].getGlobalBounds().width + 40;
    selectedSquare.setTexture(cellTex);
    selectedSquare.setPosition(infoSectionX, bpSprite.getPosition().y + 80);
    selectedItemS.setPosition(selectedSquare.getPosition());

    for (int i = 0; i < 3; i++) {
        activeCellS[i].setTexture(cellTex);
        activeCellS[i].setPosition(cellSprite[35].getPosition().x + i * activeCellS->getGlobalBounds().width,
                                   cellSprite[39].getPosition().y + cellSprite[39].getGlobalBounds().height + 40);
    }

    borderHighlight.setSize(sf::Vector2f(cellTex.getSize()));
    borderHighlight.setFillColor(sf::Color::Transparent);
    borderHighlight.setOutlineColor(sf::Color::Red);
    borderHighlight.setOutlineThickness(-2.0f);
}

void Inventory::setupTexts() {
    std::vector<sf::Text*> texts = {&infoText, &itemsText, &activeItemsText, &zText, &xText, &cText};
    std::vector<std::string> strings = {"Info", "Items", "Active Items", "Z/1", "X/2", "C/3"};
    std::vector<sf::Vector2f> positions = {
        sf::Vector2f(cellSprite[4].getPosition().x + cellSprite[4].getGlobalBounds().width + 40, selectedSquare.getPosition().y - 30),
        sf::Vector2f(cellSprite[0].getPosition().x, cellSprite[0].getPosition().y - 30),
        sf::Vector2f(activeCellS[0].getPosition().x, activeCellS[0].getPosition().y - 30)
    };

    for (size_t i = 0; i < texts.size(); i++) {
        texts[i]->setFont(font);
        texts[i]->setCharacterSize(i < 3 ? 20 : 16);
        texts[i]->setString(strings[i]);
        texts[i]->setPosition(i < 3 ? positions[i] : 
            sf::Vector2f(activeCellS[i-3].getPosition().x + activeCellS[i-3].getGlobalBounds().width / 2 - texts[i]->getGlobalBounds().width / 2,
                         activeCellS[i-3].getPosition().y + activeCellS[i-3].getGlobalBounds().height + 5));
    }
}
void Inventory::update(float deltaTime, const sf::Vector2u &screenres) {
    handleInventoryToggle();
    if (shouldDraw) {
        handleItemSelection();
        handleItemMovement();
    }
    handleActiveItems();
}
void Inventory::saveItems(){
    for (size_t i = 0; i < ownedItems.size(); i++){
        world->setItemRespawnOff(allItems[ownedItems[i]]->id);
    }
}
void Inventory::handleInventoryToggle() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !movingItem) {
        if (fc) {
            shouldDraw = !shouldDraw;
            updateItemVisibility();
            fc = false;
        }
    } else fc = true;
}
void Inventory::updateItemVisibility() {
    for (auto &item : allItems) {
        item->invisible = !shouldDraw;
    }
}
void Inventory::handleItemSelection() {
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(world->wndref);
    selectedItem = -1;

    for (size_t i = 0; i < ownedItems.size(); ++i) {
        if (cellSprite[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            selectItem(i, false);
            return;
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (activeCellS[i].getGlobalBounds().contains(mousePos.x, mousePos.y) && activeSlots[i] != -1) {
            selectItem(i, true);
            return;
        }
    }
}

void Inventory::handleItemMovement() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(world->wndref);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (movingItem) {
            allItems[movedItem]->position=(sf::Vector2f(mousePos) - cellSprite[0].getGlobalBounds().getSize() / 2.0f);
        } else {
            startItemMovement(mousePos);
        }
    } else if (movingItem) {
        finishItemMovement(mousePos);
    }
}

void Inventory::startItemMovement(const sf::Vector2i &mousePos) {
    for (int i = 0; i < 3; ++i)
        if (activeCellS[i].getGlobalBounds().contains(mousePos.x, mousePos.y) && activeSlots[i] != -1) {
            movedItem = activeSlots[i];
            movingItem = true;
            active[i] = nullptr;
            activeSlots[i] = -1;
            return;
        }
    for (size_t i = 0; i < ownedItems.size(); ++i)
        if (cellSprite[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            movedItem = ownedItems[i];
            movingItem = true;
            ownedItems.erase(ownedItems.begin() + i);
            return;
        }
}

void Inventory::finishItemMovement(const sf::Vector2i &mousePos) {
    int hoverCell = getHoverCell(world->wndref), activeHoverCell = getActiveHoverCell(world->wndref);
    if (hoverCell != -1 && hoverCell <= ownedItems.size()) {
        ownedItems.insert(ownedItems.begin() + hoverCell, movedItem);
    } else if (activeHoverCell != -1 && dynamic_cast<Item::Active *>(allItems[movedItem])) {
        active[activeHoverCell] = dynamic_cast<Item::Active *>(allItems[movedItem]);
        activeSlots[activeHoverCell] = movedItem;
    } else {
        ownedItems.push_back(movedItem);
    }
    updateItemPositions();
    movingItem = false;
}

void Inventory::handleActiveItems() {
    if (active[0] && (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))) active[0]->activate();
    if (active[1] && (sf::Keyboard::isKeyPressed(sf::Keyboard::X)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))) active[1]->activate();
    if (active[2] && (sf::Keyboard::isKeyPressed(sf::Keyboard::C)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))) active[2]->activate();
}


void Inventory::draw(sf::RenderWindow &window) const {
    if (!shouldDraw) return;

    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    drawUI(window);
    drawItems(window);
    drawSelectedItem(window);

    window.setView(originalView);
}

void Inventory::drawUI(sf::RenderWindow &window) const {
    window.draw(bpSprite);
    for (int i = 0; i < 40; i++) window.draw(cellSprite[i]);
    window.draw(selectedSquare);
    for (int i = 0; i < 3; i++) window.draw(activeCellS[i]);

    window.draw(infoText);
    window.draw(itemsText);
    window.draw(activeItemsText);
    window.draw(zText);
    window.draw(xText);
    window.draw(cText);
}
void Inventory::drawItems(sf::RenderWindow &window) const {
    for (size_t i = 0; i < ownedItems.size(); i++) {
        allItems[ownedItems[i]]->draw(window);
        if (ownedItems[i] == selectedItem) {
            sf::RectangleShape highlight = borderHighlight;
            highlight.setPosition(cellSprite[i].getPosition());
            window.draw(highlight);
        }
    }

    for (int i = 0; i < 3; i++) {
        if (activeSlots[i] != -1) {
            allItems[activeSlots[i]]->position=(activeCellS[i].getPosition());
            allItems[activeSlots[i]]->draw(window);
            if (activeSlots[i] == selectedItem) {
                sf::RectangleShape highlight = borderHighlight;
                highlight.setPosition(activeCellS[i].getPosition());
                window.draw(highlight);
            }
        }
    }

    if (movingItem) allItems[movedItem]->draw(window);
}

void Inventory::drawSelectedItem(sf::RenderWindow &window) const {
    if (selectedItem != -1) {
        window.draw(selectedSquare);
        window.draw(selectedItemS);
        window.draw(text);
    }
}
void Inventory::addItem(Item* item) {
    allItems.push_back(item);
    ownedItems.push_back(allItems.size() - 1);
    item->position=(cellSprite[ownedItems.size() - 1].getPosition());
    item->sprite.setScale(cellSprite[0].getGlobalBounds().width / item->sprite.getGlobalBounds().width,
                          cellSprite[0].getGlobalBounds().height / item->sprite.getGlobalBounds().height);
    item->applyItemChanges();
    item->shouldApplyItemChangesToPlayer = false;
    updateItemPositions();
}

void Inventory::moveItemToInventory(int slotIndex) {
    if (slotIndex >= 0 && slotIndex < 3 && activeSlots[slotIndex] != -1) {
        ownedItems.push_back(activeSlots[slotIndex]);
        active[slotIndex] = nullptr;
        activeSlots[slotIndex] = -1;
        updateItemPositions();
    }
}

void Inventory::moveItemToActiveSlot(int itemIndex, int slotIndex) {
    if (itemIndex >= 0 && itemIndex < ownedItems.size() && slotIndex >= 0 && slotIndex < 3) {
        int itemId = ownedItems[itemIndex];
        // Clear existing item in that slot if any
        for (int i = 0; i < 3; i++) {
            if (activeSlots[i] == itemId) {
                active[i] = nullptr;
                activeSlots[i] = -1;
                break;
            }
        }
        
        // Try to cast the item to Active type
        Item::Active* activeItem = dynamic_cast<Item::Active*>(allItems[itemId]);
        if (activeItem) {  // Only proceed if cast was successful
            ownedItems.erase(ownedItems.begin() + itemIndex);
            active[slotIndex] = activeItem;
            activeSlots[slotIndex] = itemId;
            updateItemPositions();
        }
    }
}

void Inventory::updateItemPositions() {
    for (size_t i = 0; i < ownedItems.size(); i++)
        allItems[ownedItems[i]]->position=(cellSprite[i].getPosition());
    for (int i = 0; i < 3; i++)
        if (activeSlots[i] != -1)
            allItems[activeSlots[i]]->position=(activeCellS[i].getPosition());
}

void Inventory::selectItem(int i, bool isActiveSlot) {
    selectedItem = isActiveSlot ? activeSlots[i] : ownedItems[i];
    if (selectedItem != -1) {
        Item *item = allItems[selectedItem];
        selectedItemS.setTexture(item->texture, true);
        selectedItemS.setScale(item->sprite.getScale());
        borderHighlight.setPosition(isActiveSlot ? activeCellS[i].getPosition() : cellSprite[i].getPosition());

        std::stringstream ss;
        ss << item->name << "\n\n";
        
        int fds = std::round((item->speedb - 1.0f) * 100);
        int fdj = std::round((item->jumpb - 1.0f) * 100);
        int fdsi = std::round((item->sizet - 1.0f) * 100);

        if (fds != 0) ss << fds << "% speed\n";
        if (fdj != 0) ss << fdj << "% jump\n";
        if (fdsi != 0) ss << fdsi << "% size\n";

        ss << "\n" << item->description << "\n\n" << item->customText();
        
        text.setString(ss.str());
        text.setPosition(selectedSquare.getPosition().x, 
                         selectedSquare.getPosition().y + selectedSquare.getGlobalBounds().height + 10);
        text.setCharacterSize(14);
        text.setFont(font);
    }
}

int Inventory::getHoverCell(sf::RenderWindow &window) {
    sf::IntRect zone(cellSprite[0].getPosition().x, cellSprite[0].getPosition().y,
                     cellSprite[0].getGlobalBounds().width * 5, cellSprite[0].getGlobalBounds().height * 8);
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    return zone.contains(mp) ? (mp.y - zone.top) / cellSprite[0].getGlobalBounds().height * 5 +
                               (mp.x - zone.left) / cellSprite[0].getGlobalBounds().width
                             : -1;
}

int Inventory::getActiveHoverCell(sf::RenderWindow &window) {
    sf::IntRect zone(activeCellS[0].getPosition().x, activeCellS[0].getPosition().y,
                     activeCellS[0].getGlobalBounds().width * 3, activeCellS[0].getGlobalBounds().height);
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    return zone.contains(mp) ? (mp.x - zone.left) / activeCellS[0].getGlobalBounds().width : -1;
}

bool Inventory::isOnScreen() const {
    return true;
}