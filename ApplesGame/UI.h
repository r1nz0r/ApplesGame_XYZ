#pragma once
#include "Math.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace ApplesGame
{
    struct Game;
    
    struct Label
    {
        bool isVisible = true;
        Position2D position;
        std::string message;
        sf::Text text;
        sf::Font textFont;
        sf::Color color = sf::Color::Cyan;
        unsigned textSize = 15;
    };

    void InitializeLabel(Label& label);
    void ShowMenu(Game& game, sf::RenderWindow& window);
    void InitializeMenu(Game& game);
    void DisplayEndMessage(Game& game, const std::string& message, sf::RenderWindow& window);

    
}
