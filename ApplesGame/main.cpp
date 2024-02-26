// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"

using namespace ApplesGame;

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_TITLE);
    std::srand(static_cast<unsigned>(time(nullptr)));

    Game game;
    LoadResources(game);
    InitializeMenu(game);
    InitializeGame(game);

    float lastTime = game.clock.getElapsedTime().asSeconds();

    while (window.isOpen())
    {
        //Read events
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::H))
                game.hintLabel.isVisible = !game.hintLabel.isVisible;

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
                game.isStarted = true;

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::M))
                game.isMuted = !game.isMuted;
        }

        if (game.isStarted)
        {
            //Calculate delta time
            float currentTime = game.clock.getElapsedTime().asSeconds();
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            if (game.isPaused == false)
            {
                UpdateGameState(window, game, deltaTime);
            }
            else
            {
                StartEndGame(window, game, deltaTime);
            }
        }
        else
        {
            ShowMenu(game, window);
            window.display();

            ProcessMenuInput(game);
        }
    }

    return 0;
}
