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

    void InitializeGame(Game& game)
    {
        game.applesAmount = GetRandomInt(APPLES_AMOUNT_MIN, APPLES_AMOUNT_MAX);
        game.rocksAmount = GetRandomInt(ROCKS_AMOUNT_MIN, ROCKS_AMOUNT_MAX);

        InitializePlayer(game.player, game);
        InitializeApples(game.apples, game);
        InitializeRocks(game.rocks, game);

        game.eatenApplesCount = 0;
        game.scoreLabel.position = {10, 10};
        game.scoreLabel.message = "Score: " + std::to_string(game.eatenApplesCount);
        InitializeLabel(game.scoreLabel);

        game.hintLabel.position = {10, 30};
        game.hintLabel.message = "Use arrows to move pacman.\nEat apples, dont touch borders and rocks."
            "\nFor toggle sound press \"M\" key"
            "\nFor toggle hint message visibility press \"H\" key";
        InitializeLabel(game.hintLabel);
    }
    
    void Restart(Game& game)
    {
        game.isStarted = false;
        InitializeGame(game);
        game.pauseTimeLeft = RESTART_DELAY;
        game.isPaused = false;
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

    void ProcessMenuInput(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            game.menuLabels[0].text.setFillColor(sf::Color::Green);
            game.menuLabels[1].text.setFillColor(sf::Color::Yellow);
            game.mode |= FINITE_MODE;
            game.mode &= ~ENDLESS_MODE;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            game.menuLabels[0].text.setFillColor(sf::Color::Yellow);
            game.menuLabels[1].text.setFillColor(sf::Color::Green);
            game.mode |= ENDLESS_MODE;
            game.mode &= ~FINITE_MODE;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            game.menuLabels[2].text.setFillColor(sf::Color::Green);
            game.menuLabels[3].text.setFillColor(sf::Color::Yellow);
            game.mode |= ACCELERATION_MODE;
            game.mode &= ~NO_ACCELERATION_MODE;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            game.menuLabels[2].text.setFillColor(sf::Color::Yellow);
            game.menuLabels[3].text.setFillColor(sf::Color::Green);
            game.mode |= NO_ACCELERATION_MODE;
            game.mode &= ~ACCELERATION_MODE;
        }
    }

    void StartEndGame(sf::RenderWindow& window, Game& game, const float deltaTime)
    {
        game.pauseTimeLeft -= deltaTime;
        std::string endMessage;

        if (game.applesAmount != game.eatenApplesCount)
        {
            endMessage = "You loose! The game will restart in " + std::to_string(RESTART_DELAY) + " seconds" +
                "\nYour score is: " + std::to_string(game.eatenApplesCount);
        }
        else
        {
            endMessage = "You Win! The game will restart in " + std::to_string(RESTART_DELAY) + " seconds" +
                "\nYour score is: " + std::to_string(game.eatenApplesCount);
        }

        DisplayEndMessage(game, endMessage, window);

        if (game.pauseTimeLeft <= 0.0f)
        {
            Restart(game);
        }
    }

    void UpdateGameState(sf::RenderWindow& window, Game& game, const float deltaTime)
    {
        CalculatePlayerMovement(game.player, deltaTime);
        RotatePlayer(game.player);
        game.isPaused = CheckPlayerCollisions(window, game);
        DrawGame(window, game);
    }
}
