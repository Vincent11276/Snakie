// Snake Game C:\Users\VitalityEdge42\Documents\GitHub\Snake-Game\src\main.rs
//

#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <tchar.h>

#include "snakegame.h"
#include "keybinds.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game");
    window.setFramerateLimit(60);

    sf::Vector2i worldSize(16, 16); 
    sf::Vector2f cellSize(64, 64);

    snek::SnakeGame snakeGame;
    snakeGame.snakeSpeed = snek::SnakeSpeed::Normal;
    snakeGame.worldScale = snek::WorldScale::Normal;
    snakeGame.setScale(0.2f, 0.2f);

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time delta = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case KEY_START_GAME:
                    std::cout << "Game has started!" << std::endl;
                    snakeGame.startGame();
                    break;

                default:
                    break;
                }
            }
            snakeGame.processEvent(event);
        }
        snakeGame.update(delta);

        window.clear();
        window.draw(snakeGame);
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
