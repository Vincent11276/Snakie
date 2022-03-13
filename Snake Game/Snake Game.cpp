// Snake Game C:\Users\VitalityEdge42\Documents\GitHub\Snake-Game\src\main.rs
//

#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

#define KEY_MOVE_UP         sf::Keyboard::W
#define KEY_MOVE_DOWN       sf::Keyboard::S
#define KEY_MOVE_LEFT       sf::Keyboard::A
#define KEY_MOVE_RIGHT      sf::Keyboard::D

#define KEY_START_GAME      sf::Keyboard::Num0
#define KEY_PAUSE_GAME      sf::Keyboard::Num1
#define KEY_RESTART_GAME    sf::Keyboard::Num2
#define KEY_QUIT_GAME       sf::Keyboard::Num3


class Helpers
{
public:
    static void setQuadVerticesTexCoords(sf::Vertex* quadPtr, const sf::FloatRect& rect)
    {
        quadPtr[0].texCoords = { rect.left, rect.top };
        quadPtr[1].texCoords = { rect.left + rect.width, rect.top };
        quadPtr[2].texCoords = { rect.left + rect.width, rect.top + rect.height};
        quadPtr[3].texCoords = { rect.left, rect.top + rect.height };
    }

    static void setQuadVerticesCorners(sf::Vertex* quadPtr, const sf::FloatRect& rect)
    {
        quadPtr[0].position = { rect.left, rect.top };
        quadPtr[1].position = { rect.left + rect.width, rect.top };
        quadPtr[2].position = { rect.left + rect.width, rect.top + rect.height };
        quadPtr[3].position = { rect.left, rect.top + rect.height };
    }

    static void setQuadVerticesColors(sf::Vertex* quadPtr, const sf::Color& color)
    {
        quadPtr[0].color = color;
        quadPtr[1].color = color;
        quadPtr[2].color = color;
        quadPtr[3].color = color;
    }
};


class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap()
    {
        this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    }

    TileMap(const sf::Vector2i& worldSize, const sf::Vector2f& cellSize)
        : m_cellSize(cellSize)
    {
        this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);

        this->setWorldSize(worldSize);
    }

    bool loadFromFile(const std::string& filePath)
    {
        return this->m_tilemap_tex.loadFromFile(filePath);
    }

    bool trySetBlock(const sf::Vector2i& coords, int value)
    {
        if (this->inBounds(coords))
        {
            this->setBlock(coords, value);

            return true;
        }
        return false;
    }


    void setBlock(const sf::Vector2i& coords, int value)
    {
        this->m_blocksData[coords.y][coords.x] = value;

        sf::Vertex* quadPtr = &this->m_vertices[(coords.x + coords.y * this->getWorldSize().x) * 4];

        sf::Vector2i texCoords = this->tileNumberToCoords(value);
        
        Helpers::setQuadVerticesTexCoords(quadPtr, sf::FloatRect(texCoords.x * this->m_cellSize.x,
            texCoords.y * this->m_cellSize.y, this->m_cellSize.x, this->m_cellSize.y));
    }

    void tryRemoveBlock(const sf::Vector2i& coords)
    {
        this->trySetBlock(coords, -1);
    }
    
    void removeBlock(const sf::Vector2i& coords)
    {
        this->setBlock(coords, -1);
    }

    void setWorldSize(const sf::Vector2i& worldSize)
    {
        sf::Vector2i currentWorldSize = this->getWorldSize();

        this->m_blocksData.resize(worldSize.y, std::vector<int>(worldSize.x, -1));
        this->m_vertices.resize((worldSize.x * worldSize.y) * 4);

        for (int x = currentWorldSize.x; x < worldSize.x; x++)
        {
            for (int y = currentWorldSize.y; y < worldSize.y; y++)
            {
                sf::Vertex* quadPtr = &this->m_vertices[(x + y * this->getWorldSize().x) * 4];

                Helpers::setQuadVerticesCorners(quadPtr, sf::FloatRect(x * this->m_cellSize.x,
                    y * this->m_cellSize.y, m_cellSize.x, m_cellSize.y));
            }
        }
    }

    sf::Vector2i getWorldSize()
    {
        if (this->m_blocksData.size())
        {
            return { (int)this->m_blocksData[0].size(), (int)this->m_blocksData.size() };
        }
        return { 0, 0 };
    }

    sf::Vector2i getTilemapSize()
    {
        sf::Vector2i texSize = (sf::Vector2i)this->m_tilemap_tex.getSize();

        return { texSize.x / (int)this->m_cellSize.x, texSize.y / (int)this->m_cellSize.y };
    }


private:
    sf::Vector2i tileNumberToCoords(int value)
    {
        auto worldSize = this->getTilemapSize();

        return { value % worldSize.x, value / worldSize.x };
    }

    bool inBounds(const sf::Vector2i& coords)
    {
        sf::Vector2i worldSize = this->getWorldSize();

        return (coords.x >= 0 && coords.y >= 0 && coords.x < worldSize.x && coords.y < worldSize.y);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
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


enum Direction
{
    Up, Down, Left, Right
};

sf::Vector2i directionToVector(Direction direction)
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

Direction reverseDirection(Direction direction)
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
    }
}

enum GameMode
{
    Classic,
    Borderlesss,
    Peaceful,
};

enum GameState
{
    StateMenu,
    StatePlaying,
    StatePaused,
    StateGameEnded,
    StateGameOver
};

class SnakeGame : public sf::Drawable, public sf::Transformable
{
public:
    bool canCollideBody;
    bool canLeaveScreen;

    SnakeGame()
    {

    }

    void setGameMode(const GameMode mode)
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

    void moveSnake()
    {
        // Predict the next loation of the head for further analysis if it's a viable move or not
        sf::Vector2i nextHeadLocation = this->m_snakeBodies.front() + directionToVector(this->m_snakeDirection);

        // Capture the location of the tail before it moves so when snake grows, we simply append this back
        sf::Vector2i oldTailLocation = this->m_snakeBodies.back();
       
        // Check if head will NOT collide with any of its body
        if (canCollideBody ? std::count(this->m_snakeBodies.begin(), this->m_snakeBodies.end(), nextHeadLocation) == 0: true)
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

    void trySetDirection(const Direction direction)
    {
        if (!direction == reverseDirection(this->m_snakeDirection))
        {
            this->m_snakeDirection = direction;
        }
    }

    void updateWorld()
    {
        for (const auto& body : this->m_snakeBodies)
        {
            this->m_tilemap.trySetBlock(body, 1);
        }
    }

    bool isInBounds(const sf::Vector2i &coords)
    {
        return (coords.x >= 0 && coords.y >= 0 && coords.x < this->m_worldSize.x && coords.y < this->m_worldSize.y);
    }

    int getSnakeLength()
    {
        return this->m_snakeBodies.size();
    }

    void processEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch(event.key.code)
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

    void update(const sf::Time &delta)
    {
        switch (this->m_gameState)
        {
        case GameState::StatePlaying:
            this->m_elapsedTime += delta;

            if (this->m_elapsedTime >= this->m_lastFrameTime + this->m_snakeMoveTime)
            {
                this->m_lastFrameTime = this->m_elapsedTime;
                this->moveSnake();
                this->updateWorld();
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states)
    {
        states.transform *= this->getTransform();

        target.draw(m_tilemap, states);
    }

private:
    std::vector<sf::Vector2i> m_snakeBodies;
    
    Direction m_snakeDirection;
    
    sf::Vector2i m_worldSize;
    sf::Vector2i m_snakeHead;
    sf::Vector2i m_foodLocation;

    sf::Time m_elapsedTime;
    sf::Time m_lastFrameTime;
    sf::Time m_snakeMoveTime;

    int m_snakeInitialLength;
    int m_currentScore;
    int m_bestScore;

    GameState m_gameState;

    TileMap m_tilemap;
};

class SnakeBody
{
public:
    SnakeBody()
    {
        // default..
    }

    
    

private:
    bool alive;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "Snake Game");
    window.setFramerateLimit(60);

    sf::Vector2i worldSize(16, 16); 
    sf::Vector2f cellSize(64, 64);

    TileMap tilemap(worldSize, cellSize);
    tilemap.loadFromFile("C:\\Users\\VitalityEdge42\\Downloads\\snake-graphics.png");
    tilemap.trySetBlock(sf::Vector2i(0, 0), 0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(tilemap);
        window.display();
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
