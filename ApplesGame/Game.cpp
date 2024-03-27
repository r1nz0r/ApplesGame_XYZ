#include <cassert>
#include <random>
#include "Game.h"
#include "Rock.h"
#include "Player.h"

namespace ApplesGame
{
	void LoadResources(Game& game)
	{
		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "Player.png"));
		assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
		assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "Rock.png"));
		assert(game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav"));
		assert(game.deathSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav"));
	}

	void InitializeScores(Game& game)
	{
		game.scores =
		{
			{"Sobaken", game.applesAmount},
			{"Kraken", game.applesAmount / 2},
			{"Obema", game.applesAmount / 3},
			{"Kandibober", game.applesAmount / 4}
		};
	}

	void InitializeGame(Game& game)
	{
		game.applesAmount = GetRandomInt(APPLES_AMOUNT_MIN, APPLES_AMOUNT_MAX);
		game.rocksAmount = GetRandomInt(ROCKS_AMOUNT_MIN, ROCKS_AMOUNT_MAX);
		game.gameState = EGameState::MainMenu;

		InitializePlayer(game.player, game);
		InitializeApples(game.apples, game);
		InitializeRocks(game.rocks, game);
		if (game.scores.empty())
			InitializeScores(game);

		game.eatenApplesCount = 0;
		game.scoreLabel.position = { 10, 10 };
		game.scoreLabel.message = "Score: " + std::to_string(game.eatenApplesCount);
		InitializeLabel(game.scoreLabel);

		game.hintLabel.position = { 10, 30 };
		game.hintLabel.message = "Use arrows to move pacman.\nEat apples, dont touch borders and rocks."
			"\nFor toggle sound press \"M\" key"
			"\nFor toggle hint message visibility press \"H\" key";
		InitializeLabel(game.hintLabel);
	}

	void Restart(Game& game)
	{
		InitializeGame(game);

		game.pauseTimeLeft = RESTART_DELAY;
		game.isScoreUpdated = false;
		game.gameState = EGameState::MainMenu;
	}

	void DrawGame(sf::RenderWindow& window, Game& game)
	{
		window.clear();
		DrawPlayer(game.player, window);

		for (Apple& apple : game.apples)
		{
			if (apple.isEaten == false)
				DrawApple(apple, window);
		}

		for (Rock& rock : game.rocks)
		{
			DrawRock(rock, window);
		}

		window.draw(game.scoreLabel.text);

		if (game.hintLabel.isVisible)
			window.draw(game.hintLabel.text);

		window.display();
	}

	void InitializeShape(const Vector2D& object, const float size, const sf::Color& color, sf::Shape& shape)
	{
		shape.setFillColor(color);
		shape.setOrigin(size / 2.f, size / 2.f);
		shape.setPosition(object.x, object.y);
	}

	void PlaySound(Game& game, const sf::SoundBuffer& soundToPlay)
	{
		game.sound.setBuffer(soundToPlay);
		game.sound.play();
	}

	void OnAppleCollisionEnter(Game& game, Apple& apple)
	{
		if (game.mode & ENDLESS_MODE)
		{
			SetRandomColliderPosition(apple.position, SCREEN_WIDTH, SCREEN_HEIGHT);
			apple.sprite.setPosition(apple.position.x, apple.position.y);
		}
		else
		{
			apple.isEaten = true;
		}

		++game.eatenApplesCount;
		game.scoreLabel.text.setString("Score: " + std::to_string(game.eatenApplesCount));

		if (game.mode & ACCELERATION_MODE)
			game.player.speed += game.player.ACCELERATION;

		if (!game.isMuted)
			PlaySound(game, game.eatSoundBuffer);
	}

	bool CheckPlayerCollisions(sf::RenderWindow& window, Game& game)
	{
		// Check bounds
		if (CheckCircleBoundsCollision(game.player))
		{
			if (!game.isMuted)
				PlaySound(game, game.deathSoundBuffer);

			return true;
		}

		for (int i = 0; i < game.applesAmount; ++i)
		{
			if (game.apples[i].isEaten)
				continue;

			if (CheckCircleCollision(game.player, game.apples[i]))
			{
				OnAppleCollisionEnter(game, game.apples[i]);

				if (game.eatenApplesCount == game.applesAmount && (game.mode & FINITE_MODE))
					return true;
			}
		}

		for (int i = 0; i < game.rocksAmount; ++i)
		{
			if (CheckCircleAndRectangleCollision(game.player, game.rocks[i]))
			{
				if (!game.isMuted)
					PlaySound(game, game.deathSoundBuffer);

				return true;
			}
		}

		return false;
	}

	void HandleMainMenuInput(const sf::Event& event, sf::RenderWindow& window, Game& game)
	{
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num1))
		{
			game.menuLabels[static_cast<int>(EMenuOptions::FiniteMode)].text.setFillColor(sf::Color::Green);
			game.menuLabels[static_cast<int>(EMenuOptions::EndlessMode)].text.setFillColor(sf::Color::Yellow);
			game.mode |= FINITE_MODE;
			game.mode &= ~ENDLESS_MODE;
		}
		
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num2))
		{
			game.menuLabels[static_cast<int>(EMenuOptions::FiniteMode)].text.setFillColor(sf::Color::Yellow);
			game.menuLabels[static_cast<int>(EMenuOptions::EndlessMode)].text.setFillColor(sf::Color::Green);
			game.mode |= ENDLESS_MODE;
			game.mode &= ~FINITE_MODE;
		}
		
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num3))
		{
			game.menuLabels[static_cast<int>(EMenuOptions::AccelerationMode)].text.setFillColor(sf::Color::Green);
			game.menuLabels[static_cast<int>(EMenuOptions::StandardSpeedMode)].text.setFillColor(sf::Color::Yellow);
			game.mode |= ACCELERATION_MODE;
			game.mode &= ~NO_ACCELERATION_MODE;
		}
		
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num4))
		{
			game.menuLabels[static_cast<int>(EMenuOptions::AccelerationMode)].text.setFillColor(sf::Color::Yellow);
			game.menuLabels[static_cast<int>(EMenuOptions::StandardSpeedMode)].text.setFillColor(sf::Color::Green);
			game.mode |= NO_ACCELERATION_MODE;
			game.mode &= ~ACCELERATION_MODE;
		}
		
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num5))
		{
			game.gameState = EGameState::Scoreboard;
		}

		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
		{
			InitializeGame(game);
			game.gameState = EGameState::Playing;
		}
	}

	void HandleScoreboardInput(const sf::Event& event, sf::RenderWindow& window, Game& game)
	{
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
		{
			game.gameState = EGameState::MainMenu;
		}
	}

	void UpdateEndGameState(sf::RenderWindow& window, Game& game, const float deltaTime)
	{
		game.pauseTimeLeft -= deltaTime;
		std::string endMessage;

		if (game.applesAmount != game.eatenApplesCount)
		{
			endMessage = "You loose! The game will restart in " + std::to_string(RESTART_DELAY) + " seconds";
		}
		else
		{
			endMessage = "You Win! The game will restart in " + std::to_string(RESTART_DELAY) + " seconds";
		}

		if (!game.isScoreUpdated)
		{
			game.scores["Player"] = std::max(game.scores["Player"], game.eatenApplesCount);
			game.isScoreUpdated = true;
		}
		
		DisplayMessage(game, endMessage, window);

		if (game.pauseTimeLeft <= 0.0f)
			Restart(game);
	}

	void UpdateMainMenuGameState(sf::RenderWindow& window, Game& game)
	{
		ShowMenu(game, window);
		window.display();
	}

	std::string GetScoresString(std::unordered_map<std::string, int>& scores)
	{
		std::string scoresString;
		const std::string separator = " - ";
		
		std::vector<std::pair<std::string, int>> scoreVector(scores.begin(), scores.end());

		auto compareByValue = [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b)
			{
				return a.second > b.second; // Sort in reverse order
			};

		std::sort(scoreVector.begin(), scoreVector.end(), compareByValue);	

		for (const auto& score : scoreVector)
		{
			scoresString += score.first + separator + std::to_string(score.second) + "\n";
		}

		return scoresString;
	}


	void UpdateScoreboardState(sf::RenderWindow& window, Game& game)
	{		
		std::string scoresString = GetScoresString(game.scores);
		scoresString += "\n\nPress ESC to return to main menu.";
		DisplayMessage(game, scoresString, window);
	}

	void UpdatePlayingGameState(sf::RenderWindow& window, Game& game, const float deltaTime)
	{
		CalculatePlayerMovement(game.player, deltaTime);
		RotatePlayer(game.player);
		
		if (CheckPlayerCollisions(window, game))
		{
			game.gameState = EGameState::EndGame;
		}

		DrawGame(window, game);
	}

	void HandlePlayingEvents(const sf::Event& event, sf::RenderWindow& window, Game& game)
	{
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			window.close();

		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::H))
			game.hintLabel.isVisible = !game.hintLabel.isVisible;

		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::M))
			game.isMuted = !game.isMuted;	
	}
}
