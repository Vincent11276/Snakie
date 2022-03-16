#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "direction.h"
#include "tilemap.h"
#include "gamestate.h"
#include "keybinds.h"
#include "gamemode.h" 
#include "worldscale.h"
#include "snakespeed.h"


#define SNAKE_SPEED_SLOW        sf::milliseconds(1000)
#define SNAKE_SPEED_NORMAL      sf::milliseconds(500)
#define SNAKE_SPEED_FAST        sf::milliseconds(200)

#define WORLD_SIZE_SMALL        sf::Vector2i(8, 8)
#define WORLD_SIZE_NORMAL       sf::Vector2i(16, 16)
#define WORLD_SIZE_BIG          sf::Vector2i(32, 32)
#define WORLD_SIZE_EXTREME      sf::Vector2i(64, 64)


namespace snek
{
    class SnakeGame : public sf::Drawable, public sf::Transformable
    {
        
    public:
        GameMode    gameMode;
        SnakeSpeed  snakeSpeed;
        WorldScale  worldScale;

        sf::Time snakeMoveTime = sf::milliseconds(1000);

        Direction inititalDirection = Direction::Right;

        int snakeInitialLength = 3;
        bool canCollideBody = false;
        bool canLeaveScreen = true;

        WorldScale worldSize;

        SnakeGame();

        void startGame();
            
        void pauseGame();
            
        void unpauseGame();
            
        void endGame();
            
        int getSnakeLength();
            
        void processEvent(const sf::Event& event);
            
        void update(const sf::Time& delta);
            
    private:
        void resetVariables();

        void initGameMode();

        void initSnakeSpeed();

        void initWorldScale();

        void loadResources();

        void initSnake();

        void moveSnake();

        void displaySnake();

        bool isInBounds(const sf::Vector2i& coords);

        void trySetDirection(const Direction direction);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        std::vector<sf::Vector2i> m_snakeBodies;
        Direction m_snakeDirection;
        bool m_isAppleExists;
        sf::Vector2i m_worldSize;
        sf::Vector2i m_foodLocation;
        sf::Time m_elapsedTime;
        sf::Time m_lastFrameTime;
        int m_currentScore = 0;
        int m_bestScore = 0;
        GameState m_gameState;
        TileMap m_tilemap;

        
    };
}

