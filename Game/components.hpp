#pragma once
#include "map.hpp"
#include "soldier.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
// this is something small element
class Under_component : public sf::Drawable, public sf::Transformable {
private:
    static Soldier* soldier;
    static Map* map;
    static std::vector<Soldier*>& soldiers;
    static std::vector<sf::RectangleShape> under_components_b;
    static std::vector<sf::RectangleShape> under_components_r;
    static int label; // 0 is selected the ui ,1 is the soldier
    Under_component()
    {
        soldier = nullptr;
        map = Map::getInstance();
    }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        ;
    }
    static void update()
    {
        under_components_r.clear();
        under_components_b.clear();
        if (!label) {
            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 10; j++) {
                    sf::RectangleShape rectangle(sf::Vector2f(32, 32));
                    rectangle.setPosition(sf::Vector2f(32 * i, 32 * j));
                    rectangle.setFillColor(sf::Color(0, 0, 255, 128));
                    under_components_b.push_back(rectangle);
                }
                for (int j = 19; j < 31; j++) {
                    sf::RectangleShape rectangle(sf::Vector2f(32, 32));
                    rectangle.setPosition(sf::Vector2f(32 * i, 32 * j));
                    rectangle.setFillColor(sf::Color(255, 0, 0, 128));
                    under_components_r.push_back(rectangle);
                }
            }
        } else {
            if (soldier != nullptr) {
            }
        }
    }

public:
    static Under_component* getInstance(Soldier_entity& lhs)
    {
        static Under_component* instance;
        if (instance == nullptr) {
            instance = new Under_component();
        }
        Under_component::setSoldiers(lhs.soldiers);
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
    static void setSoldiers(std::vector<Soldier*>& soldiers)
    {
        Under_component::soldiers = soldiers;
    }
}

;