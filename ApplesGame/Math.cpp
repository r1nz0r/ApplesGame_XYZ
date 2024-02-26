#include <SFMl/Graphics.hpp>
#include <cstdlib>
#include "Math.h"
#include "Constants.h"

namespace ApplesGame
{
    bool CheckCircleCollision(const Circle& object, const Circle& other)
    {
        const float squareDistance = (object.position.x - other.position.x) * (object.position.x - other.position.x) +
            (object.position.y - other.position.y) * (object.position.y - other.position.y);
        const float squareRadiusSum = (object.radius + other.radius) * (object.radius + other.radius);

        if (squareDistance <= squareRadiusSum)
        {
            return true;
        }

        return false;
    }

    bool CheckRectangleCollision(const Rectangle& object, const Rectangle& other)
    {
        if (fabs(object.position.x - other.position.x) <= (object.size.x + other.size.x) / 2.0f &&
            fabs(object.position.y - other.position.y) <= (object.size.y + other.size.y) / 2.0f)
        {
            return true;
        }

        return false;
    }

    bool CheckCircleAndRectangleCollision(const Circle& circle, const Rectangle& rectangle)
    {
        const float dx = circle.position.x - std::max(rectangle.position.x,
                                                std::min(circle.position.x, rectangle.position.x + rectangle.size.x));
        const float dy = circle.position.y - std::max(rectangle.position.y,
                                                std::min(circle.position.y, rectangle.position.y + rectangle.size.y));

        return (dx * dx + dy * dy) < (circle.radius * circle.radius);
    }

    bool CheckCircleBoundsCollision(const Circle& object)
    {
        if (object.position.x - object.radius <= 0 ||
            object.position.x + object.radius >= SCREEN_WIDTH ||
            object.position.y - object.radius <= 0 ||
            object.position.y + object.radius >= SCREEN_HEIGHT)
        {
            return true;
        }

        return false;
    }

    int GetRandomInt(const int minValue, const int maxValue)
    {
        return minValue + std::rand() % (maxValue - minValue + 1);
    }

    float GetRandomFloat(float minValue, float maxValue)
    {
        return minValue +  static_cast<float>(std::rand()) / static_cast<float>((RAND_MAX)) * (maxValue - minValue);
    }

    void SetRandomColliderPosition(Vector2D& position, const float screenWidth, const float screenHeight)
    {
        position.x = GetRandomFloat(0.f, screenWidth);
        position.y = GetRandomFloat(0.f, screenHeight);
    }

    auto SetSpriteSize(sf::Sprite& sprite, const float desiredWidth, float desiredHeight) -> void
    {
        const sf::FloatRect spriteRect = sprite.getLocalBounds();
        const sf::Vector2f scale = {desiredWidth / spriteRect.width, desiredHeight / spriteRect.height};
        sprite.setScale(scale);
    }

    void SetSpriteRelativeOrigin(sf::Sprite& sprite, const float originX, const float originY)
    {        
        const sf::FloatRect spriteRect = sprite.getLocalBounds();
        sprite.setOrigin(originX * spriteRect.width, originY * spriteRect.height);
    }
}
