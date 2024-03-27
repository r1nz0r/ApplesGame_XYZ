#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <vector>
#include <unordered_map>
#include "Player.h"
#include "Apple.h"
#include "Rock.h"
#include "Constants.h"
#include "UI.h"

namespace ApplesGame
{
	//GameMode
	constexpr uint8_t FINITE_MODE = 0x01;
	constexpr uint8_t ENDLESS_MODE = 0x02;
	constexpr uint8_t ACCELERATION_MODE = 0x04;
	constexpr uint8_t NO_ACCELERATION_MODE = 0x08;
	constexpr uint8_t DEFAULT_MODE = FINITE_MODE | ACCELERATION_MODE;

	enum class EMenuOptions
	{
		FiniteMode,
		EndlessMode,
		AccelerationMode,
		StandardSpeedMode,
		Scoreboard,
		StartGame,
		None
	};

	enum class EGameState
	{
		MainMenu,
		Playing,
		Scoreboard,
		EndGame,
		None
	};

	struct Game
	{
		Player player;

		int applesAmount;
		std::vector<Apple> apples;

		int rocksAmount;
		std::vector<Rock> rocks;

		//std::vector<Score> scores;
		std::unordered_map <std::string, int> scores;
		bool isMuted = false;

		float pauseTimeLeft = RESTART_DELAY;

		int eatenApplesCount = 0;

		sf::Clock clock;
		Label menuLabels[static_cast<int>(EMenuOptions::None)];

		Label scoreLabel;
		Label hintLabel;

		//Textures
		sf::Texture playerTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;

		//Audio
		sf::SoundBuffer eatSoundBuffer;
		sf::SoundBuffer deathSoundBuffer;
		sf::Sound sound;

		EGameState gameState;
		bool isScoreUpdated = false;
		uint8_t mode = DEFAULT_MODE;
	};

	void InitializeGame(Game& game);
	void Restart(Game& game);
	void DrawGame(sf::RenderWindow& window, Game& game);
	bool CheckPlayerCollisions(sf::RenderWindow& window, Game& game);
	void InitializeShape(const Vector2D& object, const float size, const sf::Color& color, sf::Shape& shape);
	void PlaySound(Game& game, const sf::SoundBuffer& soundToPlay);
	void HandleMainMenuInput(const sf::Event& event, sf::RenderWindow& window, Game& game);
	void HandleScoreboardInput(const sf::Event& event, sf::RenderWindow& window, Game& game);
	void UpdateEndGameState(sf::RenderWindow& window, Game& game, const float deltaTime);
	void UpdateMainMenuGameState(sf::RenderWindow& window, Game& game);
	void UpdatePlayingGameState(sf::RenderWindow& window, Game& game, const float deltaTime);
	void HandlePlayingEvents(const sf::Event& event, sf::RenderWindow& window, Game& game);
	void LoadResources(Game& game);
	void InitializeScores(Game& game);
	void UpdateScoreboardState(sf::RenderWindow& window,Game& game);
	std::string GetScoresString(std::unordered_map<std::string, int>& scores);
}
