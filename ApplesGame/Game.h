#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <vector>
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
	constexpr size_t MENU_OPTIONS = 5;
	constexpr uint8_t DEFAULT_MODE = FINITE_MODE | ACCELERATION_MODE;

	struct Score
	{
		std::string playerName;
		int value;
		Score() : playerName("Player"), value(0) {};
		Score(std::string name, int value) : playerName(name), value(value) {};
		bool operator< (const Score& other) const { return value < other.value; }
	};

	struct Game
	{
		Player player;

		int applesAmount;
		std::vector<Apple> apples;

		int rocksAmount;
		std::vector<Rock> rocks;

		std::vector<Score> scores;

		bool isStarted = false;
		bool isFinished = false;
		bool isPaused = false;
		bool isMuted = false;

		float pauseTimeLeft = RESTART_DELAY;

		int eatenApplesCount = 0;

		sf::Clock clock;
		Label menuLabels[MENU_OPTIONS];

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

		uint8_t mode = DEFAULT_MODE;
	};

	void InitializeGame(Game& game);
	void Restart(Game& game);
	void DrawGame(sf::RenderWindow& window, Game& game);
	bool CheckPlayerCollisions(sf::RenderWindow& window, Game& game);
	void InitializeShape(const Vector2D& object, const float size, const sf::Color& color, sf::Shape& shape);
	void PlaySound(Game& game, const sf::SoundBuffer& soundToPlay);
	void ProcessMenuInput(Game& game);
	void StartEndGame(sf::RenderWindow& window, Game& game);
	void UpdateEndGame(Game& game, const float deltaTime);
	void UpdateGameState(sf::RenderWindow& window, Game& game, const float deltaTime);
	void LoadResources(Game& game);
	void InitializeScores(Game& game);
	void ClearScores(std::vector<Score>& scores);
	void UpdateScores(std::vector<Score>& scores);
	std::string GetScoresString(std::vector<Score>& scores);
}
