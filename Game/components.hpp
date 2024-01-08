#pragma once
#include "game.hpp"
#include "map.hpp"
#include "soldier.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
// this is something small element

class Component : public sf::Drawable, public sf::Transformable {
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(rectangle);
    }

public:
    sf::RectangleShape rectangle;
    sf::Color color;
    sf::Vector2u position;
    Component(sf::Vector2u position, sf::Color color)
    {
        this->position = position;
        this->color = color;
        rectangle.setSize(sf::Vector2f(32, 32));
        rectangle.setPosition(sf::Vector2f(32 * position.x, 32 * position.y));
        rectangle.setFillColor(color);
    }
};
class Under_component : public sf::Drawable, public sf::Transformable {
private:
    static Soldier* soldier;
    static Map* map;
    static Soldier_entity& soldiers;
    static Soldier_entity& enemy;
    static int label; // 0 is selected the ui ,1 is the soldier
    Under_component()
    {
        soldier = nullptr;
        map = Map::getInstance();
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        for (const Component& i : under_components) {
            target.draw(i);
        };
    }
    static void update()
    {
        under_components.clear();
        if (!label) {
            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 10; j++) {
                    sf::Vector2u position = sf::Vector2u(i, j);
                    if (!soldiers.findSoldier(position)) {
                        Component component(position, sf::Color::Blue);
                        under_components.push_back(component);
                    }
                }
                for (int j = 19; j < 31; j++) {
                    sf::Vector2u position = sf::Vector2u(i, j);
                    if (!soldiers.findSoldier(position)) {
                        Component component(position, sf::Color::Blue);
                        under_components.push_back(component);
                    }
                }
            }
        } else {
            if (soldier != nullptr) {
                sf::Vector2u position;
                position = soldier->getPosition();
                int view = soldier->getView();
                int x = position.x;
                int y = position.y;
                for (int i = x - 1; i >= x - view; i--) {
                    if (i != x) {
                        if (i >= 0 && i <= 39) {
                            if (!soldiers.findSoldier(sf::Vector2u(i, y))) {
                                if (!enemy.findSoldier(sf::Vector2u(i, y))) {
                                    if (!map->isBarrier(sf::Vector2u(i, y))) {
                                        Component component(sf::Vector2u(i, y), sf::Color::Blue);
                                        under_components.push_back(component);
                                    } else {
                                        break;
                                    }
                                } else {
                                    Component component(sf::Vector2u(i, y), sf::Color::Red);
                                    under_components.push_back(component);
                                }
                            }
                        }
                    }
                }
                for (int i = x + 1; i <= x + view; i++) {
                    if (i != x) {
                        if (i >= 0 && i <= 39) {
                            if (!soldiers.findSoldier(sf::Vector2u(i, y))) {
                                if (!enemy.findSoldier(sf::Vector2u(i, y))) {
                                    if (!map->isBarrier(sf::Vector2u(i, y))) {
                                        Component component(sf::Vector2u(i, y), sf::Color::Blue);
                                        under_components.push_back(component);
                                    } else {
                                        break;
                                    }
                                } else {
                                    Component component(sf::Vector2u(i, y), sf::Color::Red);
                                    under_components.push_back(component);
                                }
                            }
                        }
                    }
                }
                for (int j = y - 1; j >= y - view; j--) {
                    if (j != y) {
                        if (j >= 0 && j <= 29) {
                            if (!soldiers.findSoldier(sf::Vector2u(x, j))) {
                                if (!enemy.findSoldier(sf::Vector2u(x, j))) {
                                    if (!map->isBarrier(sf::Vector2u(x, j))) {
                                        Component component(sf::Vector2u(x, j), sf::Color::Blue);
                                        under_components.push_back(component);
                                    } else {
                                        break;
                                    }
                                } else {
                                    Component component(sf::Vector2u(x, j), sf::Color::Red);
                                    under_components.push_back(component);
                                }
                            }
                        }
                    }
                }
                for (int j = y + 1; j <= y + view; j++) {
                    if (j != y) {
                        if (j >= 0 && j <= 29) {
                            if (!soldiers.findSoldier(sf::Vector2u(x, j))) {
                                if (!enemy.findSoldier(sf::Vector2u(x, j))) {
                                    if (!map->isBarrier(sf::Vector2u(x, j))) {
                                        Component component(sf::Vector2u(x, j), sf::Color::Blue);
                                        under_components.push_back(component);
                                    } else {
                                        break;
                                    }
                                } else {
                                    Component component(sf::Vector2u(x, j), sf::Color::Red);
                                    under_components.push_back(component);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

public:
    static Under_component* getInstance(Soldier_entity& lhs, Soldier_entity& enemy)
    {
        static Under_component* instance;
        if (instance == nullptr) {
            instance = new Under_component();
        }
        Under_component::setSoldiers(lhs);
        Under_component::setEnemy(enemy);
        return instance;
    }
    static void setSoldier(Soldier* soldier)
    {
        Under_component::soldier = soldier;
        Under_component::update();
    }
    static void setLabel(int label)
    {
        if (label == 0 || label == 1) {
            Under_component::label = label;
            Under_component::update();
        }
    }
    static void setSoldiers(Soldier_entity soldiers)
    {
        Under_component::soldiers = soldiers;
    }
    static void setEnemy(Soldier_entity enemy)
    {
        Under_component::enemy = enemy;
    }
    static bool findComponent(sf::Vector2u position)
    {
        auto found = std::find_if(under_components.begin(), under_components.end(), [=](Component tmp) { return tmp.position == position; });
        return found != under_components.end();
    }
    static std::vector<Component> under_components;
};
std::vector<Component> Under_component::under_components;
Soldier* Under_component::soldier;
Map* Under_component::map;
Soldier_entity& Under_component::soldiers = *(Game::red_soldier);
Soldier_entity& Under_component::enemy = *(Game::blue_soldier);
int Under_component::label;
class Up_component : public sf::Drawable, public sf::Transformable {
private:
    static Under_component* ref;
    static sf::Texture texture;
    static std::vector<sf::Sprite> up_components;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (const sf::Sprite& i : up_components) {
            target.draw(i);
        }
    }
    Up_component(Under_component* ref)
    {
        texture.loadFromFile("conflict.png");
        Up_component::ref = ref;
    }
    static void setRef(Under_component* ref)
    {
        Up_component::ref = ref;
    }

public:
    static Up_component* getInstance(Under_component* ref)
    {
        static Up_component* instance;
        if (instance == nullptr) {
            instance = new Up_component(ref);
        } else if (ref) {
            Up_component::setRef(ref);
        }
        return instance;
    }
    static void update()
    {
        up_components.clear();
        for (const Component& i : ref->under_components) {
            if (i.rectangle.getFillColor() == sf::Color::Red) {
                sf::Sprite sprite;
                sprite.setTexture(texture);
                sprite.setPosition(i.rectangle.getPosition());
                up_components.push_back(sprite);
            }
        }
    }
};
Under_component* Up_component::ref;
sf::Texture Up_component::texture;
std::vector<sf::Sprite> Up_component::up_components;
