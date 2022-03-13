#pragma once

#include <SFML/Graphics.hpp>

#include "direction.h"
#include "gamemode.h" 
#include "gamestate.h"
#include "tilemap.h"

namespace snek
{
    class SnakeGame : public sf::Drawable, public sf::Transformable
    {
    public:
        sf::Time m_snakeMoveTime;
        int m_snakeInitialLength;

        bool canCollideBody;
        bool canLeaveScreen;

        SnakeGame();

        void startGame();
            
        void pauseGame();
            
        void unpauseGame();
            
        void endGame();
            
        void setGameMode(const GameMode mode);
            
        void initTileMap(const sf::Vector2i& worldSize, const sf::Vector2f& cellSize, const std::string& path);
            
        void moveSnake();
            
        void trySetDirection(const Direction direction);
            
        void updateWorld();
            
        bool isInBounds(const sf::Vector2i& coords);
            
        int getSnakeLength();
            
        void processEvent(const sf::Event& event);
            
        void update(const sf::Time& delta);
            
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        std::vector<sf::Vector2i> m_snakeBodies;
        Direction m_snakeDirection = Direction::Right;
        sf::Vector2i m_worldSize;
        sf::Vector2i m_snakeHead;
        sf::Vector2i m_foodLocation;
        sf::Time m_elapsedTime;
        sf::Time m_lastFrameTime;
        int m_currentScore = 0;
        int m_bestScore = 0;
        GameState m_gameState;
        TileMap m_tilemap;
    };

}

