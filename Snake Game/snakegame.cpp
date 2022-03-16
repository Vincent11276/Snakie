#include "snakegame.h"


namespace snek
{
    SnakeGame::SnakeGame()
    {
        this->loadResources();
    }

    void SnakeGame::loadResources()
    {
        const std::string assetsFolder = "..\\Assets";

        std::cout << "Loading tile map texture ";

        if (this->m_tilemap.loadFromFile(assetsFolder + "\\snake-graphics.png"))
        {
            std::cout << "-- success!" << std::endl;
        }
        else std::cout << "-- failed!" << std::endl;

        this->m_tilemap.setCellSize({ 60.0f, 60.0f });
    }

    void SnakeGame::startGame()
    {
        this->initGameMode();
        this->initWorldScale();
        this->initSnakeSpeed();
        this->initSnake();
        this->resetVariables();

        this->m_gameState = GameState::StatePlaying;

        std::cout << "The game has started!" << std::endl;
    }

    void SnakeGame::resetVariables()
    {
        this->m_isAppleExists = false;
        this->m_lastFrameTime = sf::Time::Zero;
        this->m_elapsedTime = sf::Time::Zero;
        this->m_currentScore = 0;
        this->m_snakeDirection = this->inititalDirection;
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

    void SnakeGame::initGameMode()
    {
        switch (this->gameMode)
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

    void SnakeGame::initSnakeSpeed()
    {
        switch (this->snakeSpeed)
        {
        case SnakeSpeed::Slow:      this->snakeMoveTime = SNAKE_SPEED_SLOW;     break;

        case SnakeSpeed::Normal:    this->snakeMoveTime = SNAKE_SPEED_NORMAL;   break;

        case SnakeSpeed::Fast:      this->snakeMoveTime = SNAKE_SPEED_FAST;     break;
        }
    }

    void SnakeGame::initWorldScale()
    {
        switch (this->worldScale)
        {
        case WorldScale::Small:     this->m_worldSize = WORLD_SIZE_SMALL;       break;

        case WorldScale::Normal:    this->m_worldSize = WORLD_SIZE_NORMAL;      break;

        case WorldScale::Big:       this->m_worldSize = WORLD_SIZE_BIG;         break;

        case WorldScale::Extreme:   this->m_worldSize = WORLD_SIZE_EXTREME;     break;
        }
        this->m_tilemap.setWorldSize(this->m_worldSize);
    }

    void SnakeGame::initSnake()
    {
        sf::Vector2i worldSize = this->m_tilemap.getDimensions();
        sf::Vector2i worldCentre = { worldSize.x / 2, worldSize.y / 2 };

        std::cout << worldCentre.x << "  " << worldCentre.y << std::endl;

        sf::Vector2i tailGrowDir = Helpers::directionToVector(Helpers::reverseDirection(this->inititalDirection));

        this->m_snakeBodies.clear();

        for (int i = 0; i < this->snakeInitialLength; i++)
        {
            this->m_snakeBodies.push_back(worldCentre + sf::Vector2i(tailGrowDir.x * i, tailGrowDir.y * i));
        }
        this->displaySnake();
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
        std::cout << "new dir = " << (int)direction << std::endl;
        this->m_snakeDirection = direction;
    }

    void SnakeGame::displaySnake()
    {
        this->m_tilemap.clear();

        for (const auto& body : this->m_snakeBodies)
        {
            this->m_tilemap.trySetBlock(body, 1);

            std::cout << "{ " << body.x << ", " << body.y << " }, ";
        }
        std::cout << std::endl;
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

            if (this->m_elapsedTime >= (this->m_lastFrameTime + this->snakeMoveTime))
            {
                std::cout << this->snakeMoveTime.asMicroseconds() << std::endl;
                this->m_lastFrameTime = this->m_elapsedTime;
                this->moveSnake();
                this->displaySnake();
            }
        }
    }

    void SnakeGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();

        target.draw(m_tilemap, states);
    }
}

