#include "snakegame.h"

#include <iostream>
#include "keybinds.h"

namespace snek
{
    sf::Time m_snakeMoveTime = sf::milliseconds(200);
    int m_snakeInitialLength = 3;

    bool canCollideBody = false;
    bool canLeaveScreen = true;

    SnakeGame::SnakeGame()
    {
        // default..
    }

    void SnakeGame::startGame()
    {
        this->m_gameState = GameState::StatePlaying;
    }

    void SnakeGame::pauseGame()
    {
        this->m_gameState = GameState::StatePaused;
    }

    void SnakeGame::unpauseGame()
    {
        this->m_gameState = GameState::StatePlaying;
    }

    void SnakeGame::endGame()
    {
        this->m_gameState = GameState::StateGameEnded;
    }

    void SnakeGame::setGameMode(const GameMode mode)
    {
        switch (mode)
        {
        case GameMode::Classic:
            this->canCollideBody = false;
            this->canLeaveScreen = false;
            break;

        case GameMode::Borderlesss:
            this->canCollideBody = false;
            this->canLeaveScreen = true;
            break;

        case GameMode::Peaceful:
            this->canCollideBody = true;
            this->canLeaveScreen = true;
            break;
        }
    }

    void SnakeGame::initTileMap(const sf::Vector2i& worldSize, const sf::Vector2f& cellSize, const std::string& path)
    {
        if (!this->m_tilemap.loadFromFile(path))
        {
            std::cout << "Unable to load tile map image!" << std::endl;
        }
        this->m_tilemap.setWorldSize(worldSize);
    }

    void SnakeGame::moveSnake()
    {
        // Predict the next loation of the head for further analysis if it's a viable move or not
        sf::Vector2i nextHeadLocation = this->m_snakeBodies.front() + Helpers::directionToVector(this->m_snakeDirection);

        // Capture the location of the tail before it moves so when snake grows, we simply append this back
        sf::Vector2i oldTailLocation = this->m_snakeBodies.back();

        // Check if head will NOT collide with any of its body
        if (canCollideBody ? std::count(this->m_snakeBodies.begin(), this->m_snakeBodies.end(), nextHeadLocation) == 0 : true)
        {
            // Check if head will NOT leave the screen
            if (canLeaveScreen ? this->isInBounds(nextHeadLocation) : true)
            {
                // Make the tail follow the head
                for (int i = this->getSnakeLength() - 1; i >= 1; i--)
                {
                    // Copy the succeeding body
                    this->m_snakeBodies[i] = this->m_snakeBodies[i - 1];
                }
                // Set the new head location
                this->m_snakeBodies[0] = nextHeadLocation;

                // Check if head will eat the apple
                if (nextHeadLocation == this->m_foodLocation)
                {
                    std::cout << "You ate an apple! Your score now is " << this->m_currentScore << "!" << std::endl;

                    // grow back the old tail before it wasn't moving
                    this->m_snakeBodies.push_back(oldTailLocation);
                }
            }
            else std::cout << "Game Over! You have hit your head to the wall!" << std::endl;;
        }
        else std::cout << "Game Over! Eating your own flesh is baaad!" << std::endl;
    }

    void SnakeGame::trySetDirection(const Direction direction)
    {
        // Prevent player from moving to the opposite direction as it will eat its own body
        if (direction == Helpers::reverseDirection(this->m_snakeDirection))
        {
            return;
        }
        this->m_snakeDirection = direction;
    }

    void SnakeGame::updateWorld()
    {
        for (const auto& body : this->m_snakeBodies)
        {
            this->m_tilemap.trySetBlock(body, 1);
        }
    }

    bool SnakeGame::isInBounds(const sf::Vector2i& coords)
    {
        return (coords.x >= 0 && coords.y >= 0 && coords.x < this->m_worldSize.x&& coords.y < this->m_worldSize.y);
    }

    int SnakeGame::getSnakeLength()
    {
        return this->m_snakeBodies.size();
    }

    void SnakeGame::processEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case KEY_MOVE_UP:
                this->trySetDirection(Direction::Up);
                break;

            case KEY_MOVE_DOWN:
                this->trySetDirection(Direction::Down);
                break;

            case KEY_MOVE_RIGHT:
                this->trySetDirection(Direction::Right);
                break;

            case KEY_MOVE_LEFT:
                this->trySetDirection(Direction::Left);
                break;
            }
        }

    }

    void SnakeGame::update(const sf::Time& delta)
    {
        switch (this->m_gameState)
        {
        case GameState::StatePlaying:
            this->m_elapsedTime += delta;

            if (this->m_elapsedTime >= this->m_lastFrameTime + this->m_snakeMoveTime)
            {
                std::cout << delta.asMicroseconds() << std::endl;
                this->m_lastFrameTime = this->m_elapsedTime;
                this->moveSnake();
                this->updateWorld();
            }
        }
    }

    void SnakeGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();

        target.draw(m_tilemap, states);
    }
}

