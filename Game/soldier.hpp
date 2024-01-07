#pragma once
#include "soldier_base.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
class Soldier_entity : public sf::Drawable, public sf::Transformable {
private:
    Factory* factory;
    std::vector<Soldier*> soldiers;
    sf::Texture T_soldiers;
    sf::VertexArray m_vertices;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &T_soldiers;

        // draw the vertex array
        target.draw(m_vertices, states);
    }
    bool changeSoldier(sf::Vector2u position, Direction direction, int phase)
    {
        auto found = std::find_if(soldiers.begin(), soldiers.end(), [=](Soldier* i) { return i->getPosition() == position; });
        if (found == soldiers.end()) {
            return false;
        } else {
            (*found)->setDirection(direction);
            (*found)->setPhase(phase);
            return true;
        }
    }

public:
    void update()
    {
        m_vertices.clear();
        m_vertices.resize(500 * 6);
        for (auto i : soldiers) {
            if (!i->getIsDead()) {
                sf::Vector2u position = i->getPosition();
                sf::Vertex triangles[6];
                triangles[0].position = sf::Vector2f(position.x * 32, position.y * 32);
                triangles[1].position = sf::Vector2f(position.x * 32 + 32, position.y * 32);
                triangles[2].position = sf::Vector2f(position.x * 32, position.y * 32 + 32);
                triangles[3].position = sf::Vector2f(position.x * 32, position.y * 32 + 32);
                triangles[4].position = sf::Vector2f(position.x * 32 + 32, position.y * 32);
                triangles[5].position = sf::Vector2f(position.x * 32 + 32, position.y * 32 + 32);
                int label = static_cast<int>(i->getLabel());
                int phase = i->getPhase();
                int direction = static_cast<int>(i->getDirection());
                triangles[0].texCoords = sf::Vector2f(label * 256 + phase * 64, direction * 64);
                triangles[1].texCoords = sf::Vector2f(label * 256 + phase * 64 + 64, direction * 64);
                triangles[2].texCoords = sf::Vector2f(label * 256 + phase * 64, direction * 64 + 64);
                triangles[3].texCoords = sf::Vector2f(label * 256 + phase * 64, direction * 64 + 64);
                triangles[4].texCoords = sf::Vector2f(label * 256 + phase * 64 + 64, direction * 64);
                triangles[5].texCoords = sf::Vector2f(label * 256 + phase * 64 + 64, direction * 64 + 64);
                m_vertices.append(triangles[0]);
                m_vertices.append(triangles[1]);
                m_vertices.append(triangles[2]);
                m_vertices.append(triangles[3]);
                m_vertices.append(triangles[4]);
                m_vertices.append(triangles[5]);
            }
        }
    }
    Soldier_entity()
    {
        factory = Factory::getInstance();
        T_soldiers.loadFromFile("soldiers.png");
        m_vertices.setPrimitiveType(sf::Triangles);
    }
    ~Soldier_entity()
    {
        for (auto i : soldiers) {
            delete i;
        }
    }

    void addSoldier(Order label, sf::Vector2u position, Direction direction)
    {
        soldiers.push_back(factory->createSoldier(label, position, direction));
    }
};