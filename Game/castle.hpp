#pragma once
#include "soldier_base.hpp"
#include <SFML/Graphics.hpp>
class Castle : public Soldier {
private:
    int team;

public:
    Castle(int health = 10000, int damage = 0, int range = 0, int view = 0, int defense_p = 0, int defense_m = 0, int cost = 0, Order label = INFANTRY)
        : Soldier(sf::Vector2u(0, 0), down, health, damage, range, view, defense_p, defense_m, cost, label)
    {
        this->setIsCastle(true);
        team = 0;
    }
    int getTeam() { return team; }
    void setTeam(int team) { this->team = team; }
    void attack(Soldier& enemy) override
    {
        return;
    }
    static bool whereCastle(sf::Vector2u position, int label)
    {
        if (label == 0) {
            if (position.x >= 0 && position.x <= 2 && position.y >= 13 && position.y <= 15) {
                return true;
            } else {
                return false;
            }
        } else if (label == 1) {
            if (position.x >= 37 && position.x <= 39 && position.y >= 13 && position.y <= 15) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }
};
class Castle_blood : public sf::Drawable, public sf::Transformable {
private:
    Castle* castle;
    sf::RectangleShape blood;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(blood);
    }

public:
    Castle_blood(Castle* castle)
    {
        this->castle = castle;
        update();
    }
    void update()
    {
        int team = castle->getTeam();
        if (team == 0) {
            blood.setPosition(sf::Vector2f(0, 416));
        } else if (team == 1) {
            blood.setPosition(sf::Vector2f(1184, 416));
        }
        int health = castle->getHealth();
        double percent = (double)health / 10000;
        blood.setSize(sf::Vector2f(percent * 96, 4));
        blood.setFillColor(sf::Color(0, 255, 0, 150));
    }
};