#pragma once

#include <SFML/Graphics.hpp>

#include "direction.h"


class Helpers
{
public:
    static void setQuadVerticesTexCoords(sf::Vertex* quadPtr, const sf::FloatRect& rect);

    static void setQuadVerticesCorners(sf::Vertex* quadPtr, const sf::FloatRect& rect);

    static void setQuadVerticesColors(sf::Vertex* quadPtr, const sf::Color& color);

    static sf::Vector2i directionToVector(Direction direction);

    static Direction reverseDirection(Direction direction);

    template <typename T>
    static sf::Vector2f getScaleRatio(sf::Vector2<T> currentSize, sf::Vector2<T> targetSize);
};