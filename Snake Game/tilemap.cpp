#include "tilemap.h"



namespace snek
{
    TileMap::TileMap()
    {
        this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    }

    TileMap::TileMap(const sf::Vector2i& worldSize, const sf::Vector2f& cellSize)
        : m_cellSize(cellSize)
    {
        this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);

        this->setWorldSize(worldSize);
    }

    bool TileMap::loadFromFile(const std::string& filePath)
    {
        return this->m_tilemap_tex.loadFromFile(filePath);
    }

    bool TileMap::trySetBlock(const sf::Vector2i& coords, int value)
    {
        if (this->inBounds(coords))
        {
            this->setBlock(coords, value);

            return true;
        }
        return false;
    }


    void TileMap::setBlock(const sf::Vector2i& coords, int value)
    {
        this->m_blocksData[coords.y][coords.x] = value;

        sf::Vertex* quadPtr = &this->m_vertices[(coords.x + coords.y * this->getDimensions().x) * 4];

        sf::Vector2i texCoords = this->tileNumberToCoords(value);

        Helpers::setQuadVerticesTexCoords(quadPtr, sf::FloatRect(texCoords.x * this->m_cellSize.x,
            texCoords.y * this->m_cellSize.y, this->m_cellSize.x, this->m_cellSize.y));
    }

    void TileMap::tryRemoveBlock(const sf::Vector2i& coords)
    {
        this->trySetBlock(coords, -1);
    }

    void TileMap::removeBlock(const sf::Vector2i& coords)
    {
        this->setBlock(coords, -1);
    }

    void TileMap::setCellSize(const sf::Vector2f& cellSize)
    {
        this->m_cellSize = cellSize;

        this->reinitializeVertices();
    }

    void TileMap::setWorldSize(const sf::Vector2i& worldSize)
    {
        sf::Vector2i currentWorldSize = this->getDimensions();

        this->m_blocksData.resize(worldSize.y, std::vector<int>(worldSize.x, -1));
        this->m_vertices.resize((worldSize.x * worldSize.y) * 4);

        for (int x = currentWorldSize.x; x < worldSize.x; x++)
        {
            for (int y = currentWorldSize.y; y < worldSize.y; y++)
            {
                sf::Vertex* quadPtr = &this->m_vertices[(x + y * this->getDimensions().x) * 4];

                Helpers::setQuadVerticesCorners(quadPtr, sf::FloatRect(x * this->m_cellSize.x,
                    y * this->m_cellSize.y, m_cellSize.x, m_cellSize.y));
            }
        }
    }

    sf::Vector2i TileMap::getDimensions()
    {
        if (this->m_blocksData.size())
        {
            return { (int)this->m_blocksData[0].size(), (int)this->m_blocksData.size() };
        }
        return { 0, 0 };
    }

    sf::Vector2f TileMap::getSize()
    {
        sf::Vector2i dimensions = this->getDimensions();

        return { dimensions.x * this->m_cellSize.x, dimensions.y * this->m_cellSize.y };
    }

    sf::Vector2i TileMap::getTilemapSize()
    {
        sf::Vector2i texSize = (sf::Vector2i)this->m_tilemap_tex.getSize();

        return { texSize.x / (int)this->m_cellSize.x, texSize.y / (int)this->m_cellSize.y };
    }

    const sf::Texture& TileMap::getTexture()
    {
        return this->m_tilemap_tex;
    }

    void TileMap::reinitializeVertices()
    {
        sf::Vector2i worldSize = this->getDimensions();

        for (int x = 0; x < worldSize.x; x++)
        {
            for (int y = 0; y < worldSize.y; y++)
            {
                sf::Vertex* quadPtr = &this->m_vertices[(x + y * this->getDimensions().x) * 4];

                Helpers::setQuadVerticesCorners(quadPtr, sf::FloatRect(x * this->m_cellSize.x,
                    y * this->m_cellSize.y, m_cellSize.x, m_cellSize.y));
            }
        }
    }

    sf::Vector2i TileMap::tileNumberToCoords(int value)
    {
        auto worldSize = this->getTilemapSize();

        return { value % worldSize.x, value / worldSize.x };
    }

    bool TileMap::inBounds(const sf::Vector2i& coords)
    {
        sf::Vector2i worldSize = this->getDimensions();

        return (coords.x >= 0 && coords.y >= 0 && coords.x < worldSize.x&& coords.y < worldSize.y);
    }

    void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();

        states.texture = &this->m_tilemap_tex;

        target.draw(this->m_vertices, states);
    }

    std::vector<std::vector<int>> m_blocksData;
    sf::Vector2f m_cellSize;
    sf::Texture m_tilemap_tex;
    sf::VertexArray m_vertices;
};