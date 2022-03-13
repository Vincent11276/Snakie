#pragma once

#include <SFML/Graphics.hpp>
#include "helpers.h"

namespace snek
{
    class TileMap : public sf::Drawable, public sf::Transformable
    {
    public:
        TileMap();

        TileMap(const sf::Vector2i& worldSize, const sf::Vector2f& cellSize);

        bool loadFromFile(const std::string& filePath);

        bool trySetBlock(const sf::Vector2i& coords, int value);

        void setBlock(const sf::Vector2i& coords, int value);

        void tryRemoveBlock(const sf::Vector2i& coords);

        void removeBlock(const sf::Vector2i& coords);

        void setCellSize(const sf::Vector2f& cellSize);

        void setWorldSize(const sf::Vector2i& worldSize);

        sf::Vector2i getDimensions();

        sf::Vector2f getSize();

        sf::Vector2i getTilemapSize();

        const sf::Texture& getTexture();

    private:
        void reinitializeVertices();

        sf::Vector2i tileNumberToCoords(int value);

        bool inBounds(const sf::Vector2i& coords);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        std::vector<std::vector<int>> m_blocksData;
        sf::Vector2f m_cellSize;
        sf::Texture m_tilemap_tex;
        sf::VertexArray m_vertices;
    };
}