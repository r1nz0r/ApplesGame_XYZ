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
        game.menuLabels[static_cast<int>(EMenuOptions::FiniteMode)].message = "1. Finite game mode";
        game.menuLabels[static_cast<int>(EMenuOptions::FiniteMode)].color = sf::Color::Green;

        game.menuLabels[static_cast<int>(EMenuOptions::EndlessMode)].message = "2. Endless game mode";
        game.menuLabels[static_cast<int>(EMenuOptions::EndlessMode)].color = sf::Color::Yellow;

        game.menuLabels[static_cast<int>(EMenuOptions::AccelerationMode)].message = "3. Acceleration game mode";
        game.menuLabels[static_cast<int>(EMenuOptions::AccelerationMode)].color = sf::Color::Green;

        game.menuLabels[static_cast<int>(EMenuOptions::StandardSpeedMode)].message = "4. Standard speed game mode";
        game.menuLabels[static_cast<int>(EMenuOptions::StandardSpeedMode)].color = sf::Color::Yellow;

        game.menuLabels[static_cast<int>(EMenuOptions::Scoreboard)].message = "5. Show scoreboard";
        game.menuLabels[static_cast<int>(EMenuOptions::Scoreboard)].color = sf::Color::Yellow;

        game.menuLabels[static_cast<int>(EMenuOptions::StartGame)].message = "Enter - Start game";
        game.menuLabels[static_cast<int>(EMenuOptions::StartGame)].color = sf::Color::Cyan;

        for (size_t i = 0; i < static_cast<int>(EMenuOptions::None); ++i)
        {
            game.menuLabels[i].position = {10.0f, (static_cast<float>(i) + 1) * 15.0f};
            InitializeLabel(game.menuLabels[i]);
        }
    }
    
    void DisplayMessage(Game& game, const std::string& message, sf::RenderWindow& window)
    {
        game.scoreLabel.text.setString(message);
        window.clear();
        window.draw(game.scoreLabel.text);
        window.display();
    }
}
