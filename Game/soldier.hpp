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
    std::vector<Soldier*>::iterator soldier_m;
    Direction direction_m;
    int steps_m;
    bool isMoving;
    int count_m;
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
                if (isMoving && *soldier_m == i) {
                    int tmp_x = 0, tmp_y = 0;
                    switch (direction_m) {
                    case up:
                        tmp_y = -1;
                        break;
                    case down:
                        tmp_y = 1;
                        break;
                    case left:
                        tmp_x = -1;
                        break;
                    case right:
                        tmp_x = 1;
                        break;
                    default:
                        break;
                    }
                    double tmp = static_cast<double>(count_m) / 100.0 * static_cast<double>(steps_m) * 32.0;
                    triangles[0].position = sf::Vector2f(static_cast<double>(position.x * 32) + static_cast<double>(tmp_x) * tmp, static_cast<double>(position.y * 32) + static_cast<double>(tmp_y) * tmp);
                    triangles[1].position = sf::Vector2f(static_cast<double>(position.x * 32) + static_cast<double>(tmp_x) * tmp + 32, static_cast<double>(position.y * 32) + static_cast<double>(tmp_y) * tmp);
                    triangles[2].position = sf::Vector2f(static_cast<double>(position.x * 32) + static_cast<double>(tmp_x) * tmp, static_cast<double>(position.y * 32) + static_cast<double>(tmp_y) * tmp + 32);
                    triangles[3].position = sf::Vector2f(static_cast<double>(position.x * 32) + static_cast<double>(tmp_x) * tmp, static_cast<double>(position.y * 32) + static_cast<double>(tmp_y) * tmp + 32);
                    triangles[4].position = sf::Vector2f(static_cast<double>(position.x * 32) + static_cast<double>(tmp_x) * tmp + 32, static_cast<double>(position.y * 32) + static_cast<double>(tmp_y) * tmp);
                    triangles[5].position = sf::Vector2f(static_cast<double>(position.x * 32) + static_cast<double>(tmp_x) * tmp + 32, static_cast<double>(position.y * 32) + static_cast<double>(tmp_y) * tmp + 32);
                    count_m++;
                    int label = static_cast<int>(i->getLabel());
                    int phase = (i->getPhase() + 1) % 4;
                    int direction = static_cast<int>(i->getDirection());
                    changeSoldier(position, direction_m, phase);
                    triangles[0].texCoords = sf::Vector2f(label * 256 + phase * 64, direction * 64);
                    triangles[1].texCoords = sf::Vector2f(label * 256 + phase * 64 + 64, direction * 64);
                    triangles[2].texCoords = sf::Vector2f(label * 256 + phase * 64, direction * 64 + 64);
                    triangles[3].texCoords = sf::Vector2f(label * 256 + phase * 64, direction * 64 + 64);
                    triangles[4].texCoords = sf::Vector2f(label * 256 + phase * 64 + 64, direction * 64);
                    triangles[5].texCoords = sf::Vector2f(label * 256 + phase * 64 + 64, direction * 64 + 64);
                    if (count_m == 101) {
                        isMoving = false;
                        i->move(steps_m, direction_m);
                        count_m = 0;
                    }
                } else {
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
                }
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
        isMoving = false;
        direction_m = right;
        steps_m = 0;
        count_m = 0;
    }
    ~Soldier_entity()
    {
        for (auto i : soldiers) {
            delete i;
        }
    }
    void moveSoldier(sf::Vector2u position, Direction direction, int steps)
    {
        auto found = std::find_if(soldiers.begin(), soldiers.end(), [=](Soldier* i) { return i->getPosition() == position; });
        if (found != soldiers.end()) {
            isMoving = true;
            soldier_m = found;
            (*found)->setDirection(direction);
            direction_m = direction;
            steps_m = steps;
            count_m = 0;
        }
    }
    void addSoldier(Order label, sf::Vector2u position, Direction direction)
    {
        soldiers.push_back(factory->createSoldier(label, position, direction));
    }
};