#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Math.h"

namespace ApplesGame
{
    struct Game;

    enum class MoveDirection
    {
        Right,
        Up,
        Left,
        Down,
        None
    };
    
    struct Player : Circle
    {
        const float ACCELERATION = 20.0f;
        const float INITIAL_SPEED = 200.0f;
        float speed = INITIAL_SPEED;
        sf::Sprite sprite;
        MoveDirection direction = MoveDirection::None;
        std::string name;
    };

    void InitializePlayer(Player& player, const Game& game);
    void CalculatePlayerMovement(Player& player, const float deltaTime);
    void DrawPlayer(Player& player, sf::RenderWindow& window);
    void RotatePlayer(Player& player);
}