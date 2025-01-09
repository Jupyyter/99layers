#include "../hpp/libs.hpp"

Menu::Menu(sf::RenderWindow& window) : windowr(window) 
{
    if (!font.loadFromFile("../fonts/ARIAL.TTF"))
    {
        // Handle font loading error
    }
    
    initializeText();
    initializeButton();
}

void Menu::initializeText()
{
    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(24);
    playText.setFillColor(sf::Color::Black);
}

void Menu::initializeButton()
{
    playButton.setSize(sf::Vector2f(200, 50));
    playButton.setFillColor(sf::Color::Yellow);
}

void Menu::draw()
{
    // Save the current view
    sf::View originalView = windowr.getView();
    // Reset the view to the default (window coordinates)
    windowr.setView(windowr.getDefaultView());
    
    // Center the button
    playButton.setPosition(
        windowr.getSize().x / 2 - playButton.getSize().x / 2,
        windowr.getSize().y / 2 - playButton.getSize().y / 2
    );
    
    // Center the text within the button
    playText.setPosition(
        playButton.getPosition().x + (playButton.getSize().x - playText.getGlobalBounds().width) / 2,
        playButton.getPosition().y + (playButton.getSize().y - playText.getGlobalBounds().height) / 2 - 5
    );
    
    windowr.draw(playButton);
    windowr.draw(playText);
    
    // Restore the original view
    windowr.setView(originalView);
}

bool Menu::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(windowr);
        
        if (playButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            return true;  // Button was clicked
        }
    }
    return false;
}

bool Menu::isPlayButtonClicked()
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(windowr);
    return playButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y);
}

void Menu::updateButtonColor()
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(windowr);
    if (playButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
    {
        playButton.setFillColor(sf::Color::Red);
    }
    else
    {
        playButton.setFillColor(sf::Color::Yellow);
    }
}