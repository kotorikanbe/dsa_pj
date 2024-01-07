#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Map_node {
private:
    bool isBarrier;
    sf::Vector2u position;

public:
    Map_node()
        : position(sf::Vector2u(0, 0))
        , isBarrier(false)
    {
    }
    Map_node(sf::Vector2u pos, bool isbarrier = false)
        : position(pos)
        , isBarrier(isbarrier)
    {
    }
    bool getBarrier() { return isBarrier; }
    sf::Vector2u getPosition() { return position; }
};
// singleton
class Map : public sf::Drawable, public sf::Transformable {
public:
    static Map* getInstance()
    {
        static Map* instance;
        if (instance == nullptr) {
            instance = new Map();
        }
        return instance;
    }
    static bool load(sf::Vector2u tileSize = sf::Vector2u(32, 32), const unsigned int width = 40, const unsigned int height = 30)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile("map.png"))
            return false;
        getMap(tiles);
        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Triangles);
        m_vertices.resize(width * height * 6);
        const int prim = m_tileset.getSize().x / tileSize.x;
        // populate the vertex array, with two triangles per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j) {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % prim;
                int tv = tileNumber / prim;

                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

                // define the 6 matching texture coordinates
                triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            }
        if (!base_blue.loadFromFile("base_blue.png"))
            return false;
        if (!base_red.loadFromFile("base_red.png"))
            return false;
        m_sprite_blue.setTexture(base_blue);
        m_sprite_red.setTexture(base_red);
        m_sprite_red.setPosition(sf::Vector2f(0.0, 416.0));
        m_sprite_blue.setPosition(sf::Vector2f(1184.0, 416.0));
    }
    static std::array<std::array<Map_node, 30>, 40>& getData()
    {
        return data;
    }

private:
    Map()
    {
        this->load();
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
        target.draw(m_sprite_blue);
        target.draw(m_sprite_red);
    }
    static void getMap(std::vector<int>& number)
    {
        std::ifstream file("map.txt"); // 替换为你的文件名
        int x = 0;
        int y = 0;
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string num;
                while (std::getline(ss, num, ',')) {
                    number.push_back(std::stoi(num));
                    data[x][y] = Map_node(sf::Vector2u(x, y), std::stoi(num));
                    x++;
                }
                x = 0;
                y++;
            }
            file.close();
        } else {
            std::cout << "无法打开文件" << std::endl;
        }
    }
    static std::vector<int> tiles;
    static std::array<std::array<Map_node, 30>, 40> data;
    static sf::VertexArray m_vertices;
    static sf::Sprite m_sprite_blue, m_sprite_red;
    static sf::Texture m_tileset, base_blue, base_red;
};
std::vector<int> Map::tiles;
std::array<std::array<Map_node, 30>, 40> Map::data;
sf::VertexArray Map::m_vertices;
sf::Sprite Map::m_sprite_blue;
sf::Sprite Map::m_sprite_red;
sf::Texture Map::m_tileset;
sf::Texture Map::base_blue;
sf::Texture Map::base_red;