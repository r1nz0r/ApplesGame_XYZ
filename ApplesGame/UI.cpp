#include "UI.h"
#include "Constants.h"
#include "Game.h"
#include <cassert>

namespace ApplesGame
{
    void InitializeLabel(Label& label)
    {
        assert(label.textFont.loadFromFile(RESOURCES_PATH + FONT_NAME));

        label.text.setFont(label.textFont);
        label.text.setCharacterSize(label.textSize);
        label.text.setString(label.message);
        label.text.setPosition(label.position.x, label.position.y);
        label.text.setFillColor(label.color);
    }

    void ShowMenu(Game& game, sf::RenderWindow& window)
    {
        window.clear();

        for (auto& menuLabel : game.menuLabels)
            window.draw(menuLabel.text);
    }

    void InitializeMenu(Game& game)
    {
        game.menuLabels[0].message = "1. Finite game mode";
        game.menuLabels[0].color = sf::Color::Green;

        game.menuLabels[1].message = "2. Endless game mode";
        game.menuLabels[1].color = sf::Color::Yellow;

        game.menuLabels[2].message = "3. Acceleration game mode";
        game.menuLabels[2].color = sf::Color::Green;

        game.menuLabels[3].message = "4. Standard speed game mode";
        game.menuLabels[3].color = sf::Color::Yellow;

        game.menuLabels[4].message = "Enter - Start game";
        game.menuLabels[4].color = sf::Color::Cyan;

        for (size_t i = 0; i < MENU_OPTIONS; ++i)
        {
            game.menuLabels[i].position = {10.0f, (static_cast<float>(i) + 1) * 15.0f};
            InitializeLabel(game.menuLabels[i]);
        }
    }
    
    void DisplayEndMessage(Game& game, const std::string& message, sf::RenderWindow& window)
    {
        game.scoreLabel.text.setString(message);
        window.clear();
        window.draw(game.scoreLabel.text);
        window.display();
    }
}
