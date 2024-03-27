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

			if (game.gameState == EGameState::Playing)
				HandlePlayingEvents(event, window, game);
			else if (game.gameState == EGameState::MainMenu)
				HandleMainMenuInput(event, window, game);
			else if (game.gameState == EGameState::Scoreboard)
				HandleScoreboardInput(event, window, game);
		}

		//Calculate delta time
		float currentTime = game.clock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		
		switch (game.gameState)
		{
			case EGameState::MainMenu:
				UpdateMainMenuGameState(window, game);
				break;
			case EGameState::Playing:
				UpdatePlayingGameState(window, game, deltaTime);
				break;
			case EGameState::EndGame:
				UpdateEndGameState(window, game, deltaTime);
				break;
			case EGameState::Scoreboard:
				UpdateScoreboardState(window, game);
				break;
			default:
				break;
		}
	}

	return 0;
}


