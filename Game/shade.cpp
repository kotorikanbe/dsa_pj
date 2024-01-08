#include "shade.hpp"
#include "soldier.hpp"
#include <array>
class Shade : public sf::Drawable, public sf::Transformable {
private:
    static std::vector<Soldier*>* soldiers;
    static std::array<std::array<int, 30>, 40> shade; // 0 is not shade, 1 is shade
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
    Shade(Soldier_entity& lhs)
    {
        Shade::soldiers = &lhs.soldiers;
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 30; j++) {
                if (i <= 8) {
                    shade[i][j] = 0;
                } /*else if (i >= 18 && i <= 21 && j >= 0 && j <= 4) {
                    shade[i][j] = 0;
                } else if (i >= 18 && i <= 21 && j >= 8 && j <= 12) {
                    shade[i][j] = 0;
                } else if (i >= 18 && i <= 21 && j >= 17 && j <= 22) {
                    shade[i][j] = 0;
                } else if (i >= 18 && i <= 21 && j >= 26 && j <= 29) {
                    shade[i][j] = 0;
                }*/
                else {
                    shade[i][j] = 1;
                }
            }
        }
    }

public:
    static Shade* getInstance(Soldier_entity& lhs)
    {
        static Shade* instance;
        if (instance == nullptr) {
            instance = new Shade(lhs);
        }
        return instance;
    }
    static void update()
    {
        for (auto i : *soldiers) {
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
    static int getShade(int x, int y)
    {
        return shade[x][y];
    }
    static int getShade(sf::Vector2u position)
    {
        return shade[position.x][position.y];
    }
};
std::vector<Soldier*>* Shade::soldiers;
std::array<std::array<int, 30>, 40> Shade::shade;