#pragma once
#include "components.hpp"
#include "map.hpp"
#include "shade.hpp"
#include "soldier.hpp"
#include "state.hpp"
#include "ui.hpp"
#include <queue>
#include <random>
class Game {
private:
    Map* map;
    Soldier_entity *blue_soldier, *red_soldier;
    sf::RenderWindow window;
    sf::Thread render_thread, mouse_thread;
    Shade* shade;
    std::queue<sf::Vector2i> mouse_event;
    Ui* ui;
    int value;
    sf::Clock clock;
    State state;
    Under_component* under;
    Up_component* up;
    Castle *blue_castle, *red_castle;
    Castle_blood *blue_blood, *red_blood;
    sf::Font font;
    sf::Text text;
    bool GameOver;
    sf::Sprite S_victory, S_defeat;
    sf::Texture T_victory, T_defeat;

public:
    Game()
        : window(sf::RenderWindow(sf::VideoMode(1760, 960), "Tilemap"))
        , blue_soldier(new Soldier_entity(1))
        , red_soldier(new Soldier_entity(0))
        , render_thread(&Game::render, this)
        , mouse_thread(&Game::handle_mouse, this)
        //, shade(new Shade(*red_soldier))
        , ui(new Ui)
        , value(5)
        , blue_castle(new Castle())
        , red_castle(new Castle())

    {
        map = Map::getInstance();
        map->load();
        this->state = State::Regular;
        under = Under_component::getInstance(red_soldier, blue_soldier);
        up = Up_component::getInstance(under);
        shade = Shade::getInstance(*red_soldier);
        blue_castle->setTeam(1);
        red_castle->setTeam(0);
        blue_blood = new Castle_blood(blue_castle);
        red_blood = new Castle_blood(red_castle);
        font.loadFromFile("yahei.ttf");
        text.setFont(font);
        text.setCharacterSize(45);
        text.setFillColor(sf::Color(148, 0, 211));
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setStyle(sf::Text::Bold);
        text.setPosition(sf::Vector2f(1520, 370 + 100));
        GameOver = false;
        T_defeat.loadFromFile("failed.png");
        T_victory.loadFromFile("victory.png");
        S_defeat.setTexture(T_defeat);
        S_victory.setTexture(T_victory);
        S_defeat.setOrigin(S_defeat.getLocalBounds().width / 2, S_defeat.getLocalBounds().height / 2);
        S_victory.setOrigin(S_victory.getLocalBounds().width / 2, S_victory.getLocalBounds().height / 2);
        S_defeat.setPosition(880, 480);
        S_victory.setPosition(880, 480);
    }
    ~Game()
    {
        delete blue_soldier;
        delete red_soldier;
        delete shade;
        delete ui;
    }
    void render()
    {
        window.setActive(true);
        while (1) {
            window.clear();
            blue_soldier->update();
            red_soldier->update();
            shade->update();
            ui->update(value);
            window.draw(*map);
            if (state == State::Ui_Selecting || state == State::Soldier_Selecting) {
                window.draw(*under);
            }
            window.draw(*blue_soldier);
            window.draw(*red_soldier);
            if (state == State::Soldier_Selecting) {
                window.draw(*up);
            }
            window.draw(*shade);
            ui->update(value);
            window.draw(*ui);
            blue_blood->update();
            red_blood->update();
            window.draw(*blue_blood);
            window.draw(*red_blood);
            if (state == State::AI) {
                text.setString("AI");
                std::cout << "AI" << std::endl;
            } else {
                text.setString("Player");
                std::cout << "Player" << std::endl;
            }
            window.draw(text);
            if (GameOver) {
                if (red_castle->getIsDead()) {
                    window.draw(S_defeat);
                } else {
                    window.draw(S_victory);
                }
            }
            window.display();
            sf::sleep(sf::milliseconds(50));
        }
    }
    void red_refresh()
    {
        value += 5;
        red_soldier->refresh();
        under->setSoldiers(red_soldier);
        under->setEnemy(blue_soldier);
    }
    void AI_execute()
    {
        static int blue_value = 0;
        blue_value += 5;
        blue_soldier->refresh();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> soldier_gen(0, 4);
        std::uniform_int_distribution<int> half_gen(0, 1);
        std::uniform_int_distribution<int> x_gen(33, 39);
        std::uniform_int_distribution<int> y_gen1(0, 9);
        std::uniform_int_distribution<int> y_gen2(19, 29);
        under->setSoldiers(blue_soldier);
        under->setEnemy(red_soldier);
        under->setLabel(0);
        while (blue_value) {
            under->update();
            int order = soldier_gen(gen);
            if (blue_soldier->soldiers.size() >= 2) {
                break;
            }
            if (blue_value >= Soldier::getValue(static_cast<Order>(order))) {
                int x = x_gen(gen);
                int y;
                if (half_gen(gen)) {
                    y = y_gen1(gen);
                } else {
                    y = y_gen2(gen);
                }
                if (under->findComponent(sf::Vector2u(x, y))) {
                    blue_soldier->addSoldier(static_cast<Order>(order), sf::Vector2u(x, y), left);
                    blue_value -= Soldier::getValue(static_cast<Order>(order));
                }
            }
        }
        for (auto i : blue_soldier->soldiers) {
            under->setSoldier(i);
            under->setLabel(1);
            up->setSoldier(i);
            while (i->getMoveStep() > 0) {
                under->update();
                up->update();
                sf::Sprite* random_attack;
                bool isattacked = i->getisattacked();
                if ((!isattacked) && (random_attack = up->getRandom())) {
                    sf::Vector2u target = sf::Vector2u(static_cast<unsigned int>((*random_attack).getPosition().x / 32.0), static_cast<unsigned int>((*random_attack).getPosition().y / 32.0));
                    if (Castle::whereCastle(target, 0)) {
                        i->attack(*red_castle);
                    } else {
                        Soldier* attack_target = red_soldier->getSoldier(target);
                        i->attack(*attack_target);
                    }
                }
                under->update();
                up->update();
                Component* tmp = under->getRandom();
                int first, second;
                first = static_cast<int>(tmp->position.x) - static_cast<int>(i->getPosition().x);
                second = static_cast<int>(tmp->position.y) - static_cast<int>(i->getPosition().y);
                if (first > 0) {
                    blue_soldier->moveSoldier(i->getPosition(), right, first);

                } else if (first < 0) {
                    blue_soldier->moveSoldier(i->getPosition(), left, -first);

                } else if (second > 0) {
                    blue_soldier->moveSoldier(i->getPosition(), down, second);

                } else if (second < 0) {
                    blue_soldier->moveSoldier(i->getPosition(), Direction::up, -second);

                } else {
                    std::cout << "error";
                }
                while (1) {
                    if (!blue_soldier->getIsmoving()) {
                        break;
                    }
                    sf::sleep(sf::milliseconds(50));
                }
            }
        }
        this->red_refresh();
        state = State::Regular;
    }
    void run()
    {
        window.setActive(false);
        render_thread.launch();
        mouse_thread.launch();
        sf::Event event;
        while (window.isOpen()) {
            if (!GameOver && (blue_castle->getIsDead() || red_castle->getIsDead())) {
                GameOver = true;
                state = State::GameOver;
            }
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    mouse_event.push(localPosition);
                }
            }
        }
    }
    void handle_mouse()
    {
        // for test
        red_soldier->addSoldier(Order::BERSERKER, sf::Vector2u(38, 19), left);
        blue_soldier->addSoldier(Order::CASTER, sf::Vector2u(0, 16), right);
        red_soldier->addSoldier(Order::BERSERKER, sf::Vector2u(39, 19), left);
        red_soldier->addSoldier(Order::BERSERKER, sf::Vector2u(37, 19), left);
        blue_soldier->addSoldier(Order::CASTER, sf::Vector2u(1, 16), right);
        while (1) {
            if (GameOver) {
                break;
            }
            if (!mouse_event.empty()) {
                sf::Vector2i now = mouse_event.front();
                mouse_event.pop();
                if (!red_soldier->getIsmoving()) {
                    if (now.x >= 0 && now.y >= 0 && now.x <= 1760 && now.y <= 960) {
                        switch (state) {
                        case Regular:
                            if (now.x > 1280) {
                                State tmp;
                                ui->handle(now, &tmp);
                                state = tmp;
                                if (state == State::Ui_Selecting) {
                                    under->setLabel(0);
                                    under->update();
                                } else if (state == State::AI) {
                                    AI_execute();
                                }
                            } else {
                                Soldier* tmp;
                                tmp = red_soldier->handleEvent(now);
                                if (tmp) {
                                    state = State::Soldier_Selecting;
                                    under->setSoldier(tmp);
                                    under->setLabel(1);
                                    under->update();
                                    up->setSoldier(tmp);
                                    up->update();
                                }
                            }
                            break;
                        case Ui_Selecting:
                            if (under->handleEvent(now)) {
                                Order tmp = ui->getOrder();
                                int cost = Soldier::getValue(tmp);
                                if (value >= cost) {
                                    value -= cost;
                                    red_soldier->addSoldier(tmp, sf::Vector2u(now.x / 32, now.y / 32), Direction::right);
                                }
                                state = State::Regular;
                                ui->unselected();
                            } else {
                                state = State::Regular;
                                ui->unselected();
                            }
                            break;
                        case Soldier_Selecting:
                            if (under->handleEvent(now)) {
                                Soldier* tmp = under->getSoldier();
                                sf::Vector2u flag = under->getFlag();
                                int first, second;
                                first = static_cast<int>(flag.x) - static_cast<int>(tmp->getPosition().x);
                                second = static_cast<int>(flag.y) - static_cast<int>(tmp->getPosition().y);
                                if (first > 0) {
                                    red_soldier->moveSoldier(tmp->getPosition(), right, first);
                                    state = State::Regular;
                                } else if (first < 0) {
                                    red_soldier->moveSoldier(tmp->getPosition(), left, -first);
                                    state = State::Regular;
                                } else if (second > 0) {
                                    red_soldier->moveSoldier(tmp->getPosition(), down, second);
                                    state = State::Regular;
                                } else if (second < 0) {
                                    red_soldier->moveSoldier(tmp->getPosition(), Direction::up, -second);
                                    state = State::Regular;
                                } else {
                                    std::cout << "error";
                                }
                            } else if (up->handleEvent(now)) {
                                Soldier* tmp = under->getSoldier();
                                sf::Vector2u flag = up->getFlag();
                                if (Castle::whereCastle(flag, 1)) {
                                    tmp->attack(*blue_castle);
                                } else {
                                    Soldier* attack_target;
                                    if (attack_target = blue_soldier->getSoldier(flag)) {
                                        tmp->attack(*attack_target);
                                    }
                                }
                                state = State::Regular;
                            } else {
                                state = State::Regular;
                            }
                            break;
                        case AI:
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
};