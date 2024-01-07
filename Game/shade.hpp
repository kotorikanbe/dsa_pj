#pragma once
#include "soldier.hpp"
#include <array>
class Shade : public sf::Drawable, public sf::Transformable {
private:
    std::vector<Soldier*>& soldiers;
    std::array<std::array<int, 30>, 40> shade; // 0 is not shade, 1 is shade
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 30; ++j) {
                if (shade[i][j] == 1) {
                    sf::RectangleShape rectangle(sf::Vector2f(32, 32));
                    rectangle.setPosition(i * 32, j * 32);
                    rectangle.setFillColor(sf::Color(0, 0, 0, 100));
                    target.draw(rectangle);
                }
            }
        }
    }

public:
    Shade(Soldier_entity& lhs)
        : 
         soldiers(lhs.soldiers)
    {
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 30; j++) {
                if (i <= 8 || i >= 31) {
                    shade[i][j] = 0;
                } else if (i >= 18 && i <= 21 && j >= 0 && j <= 4) {
                    shade[i][j] = 0;
                } else if (i >= 18 && i <= 21 && j >= 8 && j <= 12) {
                    shade[i][j] = 0;
                } else if (i >= 18 && i <= 21 && j >= 17 && j <= 22) {
                    shade[i][j] = 0;
                } else if (i >= 18 && i <= 21 && j >= 26 && j <= 29) {
                    shade[i][j] = 0;
                } else {
                    shade[i][j] = 1;
                }
            }
        }
    }
    void update()
    {
        for (auto i : soldiers) {
            if (!i->getIsDead()) {
                sf::Vector2u position = i->getPosition();
                int x = position.x;
                int y = position.y;
                int view = i->getView();
                for (int j = x - view; j <= x + view; j++) {
                    for (int k = y - view; k <= y + view; k++) {
                        if (j >= 0 && j < 40 && k >= 0 && k < 30) {
                            shade[j][k] = 0;
                        }
                    }
                }
            }
        }
    }
    int getShade(int x, int y)
    {
        return shade[x][y];
    }
    int getShade(sf::Vector2u position)
    {
        return shade[position.x][position.y];
    }
};