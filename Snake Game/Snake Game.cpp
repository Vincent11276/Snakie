// Snake Game C:\Users\VitalityEdge42\Documents\GitHub\Snake-Game\src\main.rs
//

#include <iostream>
#include <SFML/Graphics.hpp>

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
        // default..
    }

    TileMap(const sf::Vector2i worldSize, const sf::Vector2f cellSize)
        : m_cellSize(cellSize)
    {
        this->m_blocksData.resize(worldSize.y, std::vector<int>(worldSize.x, -1));
        this->m_vertices.resize((worldSize.x * worldSize.y) * 4);
        this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);

        for (unsigned int x = 0; x < worldSize.x; x++) 
        {
            for (unsigned int y = 0; y < worldSize.y; y++)
            {
                sf::Vertex* quadPtr = &this->m_vertices[(x + y * this->getWorldSize().x) * 4];

                Helpers::setQuadVerticesCorners(quadPtr, sf::FloatRect(x * this->m_cellSize.x,
                    y * this->m_cellSize.y, m_cellSize.x, m_cellSize.y));
            }
        }
    }

    bool loadFromFile(const std::string& filePath)
    {
        return this->m_tilemap_tex.loadFromFile(filePath);
    }

    bool tryPlace(const sf::Vector2i& coords, int value)
    {
        if (this->inBounds(coords))
        {
            this->place(coords, value);

            return true;
        }
        return false;
    }

    void place(const sf::Vector2i& coords, int value)
    {
        this->m_blocksData[coords.y][coords.x] = value;

        sf::Vertex* quadPtr = &this->m_vertices[(coords.x + coords.y * this->getWorldSize().x) * 4];

        sf::Vector2i texCoords = this->tileNumberToCoords(value);
        
        std::cout << value << "   " << texCoords.x << "   " << texCoords.y << std::endl;

        Helpers::setQuadVerticesTexCoords(quadPtr, sf::FloatRect(texCoords.x * this->m_cellSize.x,
            texCoords.y * this->m_cellSize.y, this->m_cellSize.x, this->m_cellSize.y));
    }

    sf::Vector2i getWorldSize()
    {
        return { (int)this->m_blocksData[0].size(), (int)this->m_blocksData.size() };
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
        auto worldSize = this->getWorldSize();

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


namespace GameEngine
{
    class Node : public sf::Drawable, public sf::Transformable
    {
    public:
        Node()
        {
            // defaut..
        }

        void addChild(Node& node)
        {
            node.parent = this;
            node.

            this->children.push_back(&node);
        }

        void update(float delta)
        {
            for (auto node : children)
            {
                node->update(delta);
            }
            this->process();
        }

    private:
        

        virtual void process()
        {
            std::cout << "inside node\n";
        }

    protected:
        Node* parent;
        std::vector<Node*> children;
    };

    class Node2D : public Node
    {
    public:
        Node2D()
        {
            // default..
        }

    private:
        void process() override
        {
            std::cout << "inside node2D\n";
        }
    };
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "Snake Game");

    //sf::Vector2i worldSize(16, 16); 
    //sf::Vector2f cellSize(64, 64);
    //TileMap tilemap(worldSize, cellSize);
    //tilemap.loadFromFile("C:\\Users\\VitalityEdge42\\Downloads\\snake-graphics.png");
    //tilemap.tryPlace(sf::Vector2i(0, 0), 0);

    Node node1;
    Node node2;

    node1.addChild(node2);

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
