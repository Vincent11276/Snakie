#pragma once

#include <SFML/Graphics.hpp>
#include "helpers.h"


void Helpers::setQuadVerticesTexCoords(sf::Vertex* quadPtr, const sf::FloatRect& rect)
{
    quadPtr[0].texCoords = { rect.left, rect.top };
    quadPtr[1].texCoords = { rect.left + rect.width, rect.top };
    quadPtr[2].texCoords = { rect.left + rect.width, rect.top + rect.height };
    quadPtr[3].texCoords = { rect.left, rect.top + rect.height };
}

void Helpers::setQuadVerticesCorners(sf::Vertex* quadPtr, const sf::FloatRect& rect)
{
    quadPtr[0].position = { rect.left, rect.top };
    quadPtr[1].position = { rect.left + rect.width, rect.top };
    quadPtr[2].position = { rect.left + rect.width, rect.top + rect.height };
    quadPtr[3].position = { rect.left, rect.top + rect.height };
}

void Helpers::setQuadVerticesColors(sf::Vertex* quadPtr, const sf::Color& color)
{
    quadPtr[0].color = color;
    quadPtr[1].color = color;
    quadPtr[2].color = color;
    quadPtr[3].color = color;
}

sf::Vector2i Helpers::directionToVector(Direction direction)
{
    switch (direction)
    {
    case Direction::Up:
        return { 0, -1 };

    case Direction::Down:
        return { 0, 1 };

    case Direction::Left:
        return { -1, 0 };

    case Direction::Right:
        return { 1, 0 };
    }
}

Direction Helpers::reverseDirection(Direction direction)
{
    switch (direction)
    {
    case Direction::Up:
        return Direction::Down;

    case Direction::Down:
        return Direction::Up;

    case Direction::Left:
        return Direction::Right;

    case Direction::Right:
        return Direction::Left;

    case Direction::None:
        return Direction::None;
    }
}

template <typename T>
static sf::Vector2f getScaleRatio(sf::Vector2<T> currentSize, sf::Vector2<T> targetSize)
{
    return { targetSize.x / currentSize.x, targetSize.y / currentSize.y };
}
