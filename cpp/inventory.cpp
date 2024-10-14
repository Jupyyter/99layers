#include "../hpp/libs.hpp"

Inventory::Inventory()
    : pgcount(1), selectedItem(-1), shouldDraw(false), fc(true), movingItem(false), active{nullptr, nullptr, nullptr},
      activeSlots(3, -1)
{
    priorityLayer = 777;
    loadPanel();
    loadItems();
    loadTexts();
    borderHighlight.setSize(sf::Vector2f(cellTex.getSize()));
    borderHighlight.setFillColor(sf::Color::Transparent);
    borderHighlight.setOutlineColor(sf::Color::Red);
    borderHighlight.setOutlineThickness(-2.0f);
}

void Inventory::loadPanel()
{
    font.loadFromFile("../fonts/font.ttf");
    bpTex.loadFromFile("../imgs/BigPanel.jpg");
    cellTex.loadFromFile("../imgs/Cell.png");

    bpSprite.setTexture(bpTex);
    bpSprite.setPosition(world->getPartBounds().width / 2.0f - bpSprite.getGlobalBounds().width / 2.0f,
                         world->getPartBounds().height / 2.0f - bpSprite.getGlobalBounds().height / 2.0f);

    for (int i = 0; i < 40; i++)
    {
        cellSprite[i].setTexture(cellTex);
        cellSprite[i].setPosition(bpSprite.getPosition().x + 40 + (i % 5) * cellTex.getSize().x,
                                  bpSprite.getPosition().y + 40 + (i / 5) * cellTex.getSize().y);
    }

    // Adjust the position of the selected item info
    float infoSectionX = cellSprite[4].getPosition().x + cellSprite[4].getGlobalBounds().width + 40; // 40 pixels padding
    selectedSquare.setTexture(cellTex);
    selectedSquare.setPosition(infoSectionX, bpSprite.getPosition().y + 80);
    selectedItemS.setPosition(selectedSquare.getPosition());

    text.setFont(font);
    text.setCharacterSize(12);
    text.setPosition(infoSectionX, selectedSquare.getPosition().y + selectedSquare.getGlobalBounds().height + 20);

    for (int i = 0; i < 3; i++)
    {
        activeCellS[i].setTexture(cellTex);
        activeCellS[i].setPosition(cellSprite[35].getPosition().x + i * activeCellS->getGlobalBounds().width,
                                   cellSprite[39].getPosition().y + cellSprite[39].getGlobalBounds().height + 40);
    }
}

void Inventory::loadTexts()
{
    float infoSectionX = cellSprite[4].getPosition().x + cellSprite[4].getGlobalBounds().width + 40;

    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setString("Info");
    infoText.setPosition(infoSectionX, selectedSquare.getPosition().y - 30);

    itemsText.setFont(font);
    itemsText.setCharacterSize(20);
    itemsText.setString("Items");
    itemsText.setPosition(cellSprite[0].getPosition().x, cellSprite[0].getPosition().y - 30);

    activeItemsText.setFont(font);
    activeItemsText.setCharacterSize(20);
    activeItemsText.setString("Active Items");
    activeItemsText.setPosition(activeCellS[0].getPosition().x, activeCellS[0].getPosition().y - 30);

    zText.setFont(font);
    zText.setCharacterSize(16);
    zText.setString("Z/1");
    zText.setPosition(activeCellS[0].getPosition().x + activeCellS[0].getGlobalBounds().width / 2 - zText.getGlobalBounds().width / 2,
                      activeCellS[0].getPosition().y + activeCellS[0].getGlobalBounds().height + 5);

    xText.setFont(font);
    xText.setCharacterSize(16);
    xText.setString("X/2");
    xText.setPosition(activeCellS[1].getPosition().x + activeCellS[1].getGlobalBounds().width / 2 - xText.getGlobalBounds().width / 2,
                      activeCellS[1].getPosition().y + activeCellS[1].getGlobalBounds().height + 5);

    cText.setFont(font);
    cText.setCharacterSize(16);
    cText.setString("C/3");
    cText.setPosition(activeCellS[2].getPosition().x + activeCellS[2].getGlobalBounds().width / 2 - cText.getGlobalBounds().width / 2,
                      activeCellS[2].getPosition().y + activeCellS[2].getGlobalBounds().height + 5);
}
void Inventory::loadItems()
{
    unownedItems.resize((allItems).size());
    std::iota(unownedItems.begin(), unownedItems.end(), 0);
}

void Inventory::selectItem(int i, bool isActiveSlot)
{
    selectedItem = isActiveSlot ? activeSlots[i] : ownedItems[i];
    if (selectedItem != -1)
    {
        Item *item = (allItems)[selectedItem];
        selectedItemS.setTexture(item->texture, true);
        selectedItemS.setScale(item->sprite.getScale());
        if (isActiveSlot)
        {
            borderHighlight.setPosition(activeCellS[i].getPosition());
        }
        else
        {
            borderHighlight.setPosition(cellSprite[i].getPosition());
        }

        std::string displaytext = item->name + "\n";
        int fds = std::round((item->speedb - 1.0f) * 100);
        int fdj = std::round((item->jumpb - 1.0f) * 100);
        int fdsi = std::round((item->sizet - 1.0f) * 100);

        if (fds != 0)
            displaytext += std::to_string(fds) + "% speed\n";
        if (fdj != 0)
            displaytext += std::to_string(fdj) + "% jump\n";
        if (fdsi != 0)
            displaytext += std::to_string(fdsi) + "% size\n";

        displaytext += item->description + "\n" + item->customText();
        text.setString(displaytext);
    }
}

inline int Inventory::getHoverCell(sf::RenderWindow &window)
{
    sf::IntRect zone(cellSprite[0].getPosition().x, cellSprite[0].getPosition().y,
                     cellSprite[0].getGlobalBounds().width * 5, cellSprite[0].getGlobalBounds().height * 8);
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    return zone.contains(mp) ? (mp.y - zone.top) / cellSprite[0].getGlobalBounds().height * 5 +
                                   (mp.x - zone.left) / cellSprite[0].getGlobalBounds().width
                             : -1;
}

inline int Inventory::getActiveHoverCell(sf::RenderWindow &window)
{
    sf::IntRect zone(activeCellS[0].getPosition().x, activeCellS[0].getPosition().y,
                     activeCellS[0].getGlobalBounds().width * 3, activeCellS[0].getGlobalBounds().height);
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    return zone.contains(mp) ? (mp.x - zone.left) / activeCellS[0].getGlobalBounds().width : -1;
}

void Inventory::moveItemToActiveSlot(int itemIndex, int slotIndex)
{
    if (itemIndex >= 0 && itemIndex < ownedItems.size() && slotIndex >= 0 && slotIndex < 3)
    {
        int itemId = ownedItems[itemIndex];
        for (int i = 0; i < 3; i++)
            if (activeSlots[i] == itemId)
            {
                active[i] = nullptr;
                activeSlots[i] = -1;
                break;
            }
        ownedItems.erase(ownedItems.begin() + itemIndex);
        active[slotIndex] = dynamic_cast<Item::Active *>((allItems)[itemId]);
        activeSlots[slotIndex] = itemId;
        updateItemPositions();
    }
}

void Inventory::updateItemPositions()
{
    for (size_t i = 0; i < ownedItems.size(); i++)
        (allItems)[ownedItems[i]]->setPosition(cellSprite[i].getPosition());
    for (int i = 0; i < 3; i++)
        if (activeSlots[i] != -1)
            (allItems)[activeSlots[i]]->setPosition(activeCellS[i].getPosition());
}
void Inventory::addItem(Item* item) {
    allItems.push_back(item);
    int newItemIndex = allItems.size() - 1;
    ownedItems.push_back(newItemIndex);
    
    item->setPosition(cellSprite[ownedItems.size() - 1].getPosition());
    item->sprite.setScale(cellSprite[0].getGlobalBounds().width / item->sprite.getGlobalBounds().width,
                          cellSprite[0].getGlobalBounds().height / item->sprite.getGlobalBounds().height);
    
    item->applyItemChanges();
    item->shouldApplyItemChangesToPlayer = false;
    
    updateItemPositions();
}
void Inventory::update(float deltaTime, const sf::Vector2u &screenres)
{

    for (int i : ownedItems)
        (allItems)[i]->updateOwned(world->playerRef);
    for (int i = 0; i < 3; i++)
        if (activeSlots[i] != -1)
            (allItems)[activeSlots[i]]->updateOwned(world->playerRef);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !movingItem)
    {
        if (fc)
        {
            shouldDraw = !shouldDraw;
            if (!shouldDraw)
                selectedItem = -1;
            for (auto &item : (allItems))
                item->invisible = !item->invisible;
            fc = false;
        }
    }
    else if (!fc)
    {
        fc = true;
    }

    if (shouldDraw)
    {
        for (int i : ownedItems)
            (allItems)[i]->invisible = false;

        sf::Vector2i mousePos = sf::Mouse::getPosition(world->wndref);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for (size_t i = 0; i < ownedItems.size(); ++i)
                if (cellSprite[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    selectItem(i, false);
                    return;
                }
            for (int i = 0; i < 3; ++i)
                if (activeCellS[i].getGlobalBounds().contains(mousePos.x, mousePos.y) && activeSlots[i] != -1)
                {
                    selectItem(i, true);
                    return;
                }
            selectedItem = -1;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            if (movingItem)
            {
                (allItems)[movedItem]->setPosition(sf::Vector2f(mousePos) - cellSprite[0].getGlobalBounds().getSize() / 2.0f);
            }
            else
            {
                for (int i = 0; i < 3; ++i)
                    if (activeCellS[i].getGlobalBounds().contains(mousePos.x, mousePos.y) && activeSlots[i] != -1)
                    {
                        movedItem = activeSlots[i];
                        movingItem = true;
                        active[i] = nullptr;
                        activeSlots[i] = -1;
                        return;
                    }
                for (size_t i = 0; i < ownedItems.size(); ++i)
                    if (cellSprite[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        movedItem = ownedItems[i];
                        movingItem = true;
                        ownedItems.erase(ownedItems.begin() + i);
                        return;
                    }
            }
        }
        else if (movingItem)
        {
            int hoverCell = getHoverCell(world->wndref), activeHoverCell = getActiveHoverCell(world->wndref);
            if (hoverCell != -1 && hoverCell <= ownedItems.size())
            {
                ownedItems.insert(ownedItems.begin() + hoverCell, movedItem);
            }
            else if (activeHoverCell != -1 && dynamic_cast<Item::Active *>((allItems)[movedItem]))
            {
                active[activeHoverCell] = dynamic_cast<Item::Active *>((allItems)[movedItem]);
                activeSlots[activeHoverCell] = movedItem;
            }
            else
            {
                ownedItems.push_back(movedItem);
            }
            updateItemPositions();
            movingItem = false;
        }
    }

    // Check for active item behaviour
    if (active[0] && sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        active[0]->activate();
    if (active[1] && sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        active[1]->activate();
    if (active[2] && sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        active[2]->activate();
}

void Inventory::draw(sf::RenderWindow &window) const
{
    if (shouldDraw)
    {
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());

        window.draw(bpSprite);
        for (int i = 0; i < 40; i++)
            window.draw(cellSprite[i]);
        window.draw(selectedSquare);
        for (int i = 0; i < 3; i++)
            window.draw(activeCellS[i]);

        window.draw(infoText);
        window.draw(itemsText);
        window.draw(activeItemsText);
        window.draw(zText);
        window.draw(xText);
        window.draw(cText);

        for (size_t i = 0; i < ownedItems.size(); i++)
        {
            (allItems)[ownedItems[i]]->draw(window);
            if (ownedItems[i] == selectedItem)
            {
                window.draw(borderHighlight);
            }
        }

        for (int i = 0; i < 3; i++)
        {
            if (activeSlots[i] != -1)
            {
                (allItems)[activeSlots[i]]->setPosition(activeCellS[i].getPosition());
                (allItems)[activeSlots[i]]->draw(window);
                if (activeSlots[i] == selectedItem)
                {
                    window.draw(borderHighlight);
                }
            }
        }

        if (selectedItem != -1)
        {
            window.draw(selectedItemS);
            window.draw(text);
        }
        if (movingItem)
            (allItems)[movedItem]->draw(window);

        window.setView(originalView);
    }
}

void Inventory::reset(Player *player)
{
    pgcount = 1;
    selectedItem = -1;
    shouldDraw = false;
    fc = true;
    ownedItems.clear();
    unownedItems.clear();
    unownedItems.resize((allItems).size());
    std::iota(unownedItems.begin(), unownedItems.end(), 0);
    std::fill(active, active + 3, nullptr);
    std::fill(activeSlots.begin(), activeSlots.end(), -1);
}

void Inventory::moveItemToInventory(int slotIndex)
{
    if (slotIndex >= 0 && slotIndex < 3 && activeSlots[slotIndex] != -1)
    {
        int itemId = activeSlots[slotIndex];
        active[slotIndex] = nullptr;
        activeSlots[slotIndex] = -1;
        ownedItems.push_back(itemId);

        updateItemPositions();
    }
}
bool Inventory::isOnScreen() const {
    return true;
}