#pragma once
#include "components.hpp"
#include "map.hpp"
#include "shade.hpp"
#include "soldier.hpp"
#include "state.hpp"
#include "ui.hpp"
#include <queue>

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

public:
    Game()
        : window(sf::RenderWindow(sf::VideoMode(1760, 960), "Tilemap"))
        , blue_soldier(new Soldier_entity(1))
        , red_soldier(new Soldier_entity(0))
        , render_thread(&Game::render, this)
        , mouse_thread(&Game::handle_mouse, this)
        , shade(new Shade(*red_soldier))
        , ui(new Ui)
        , value(5)

    {
        map = Map::getInstance();
        map->load();
        this->state = State::Regular;
        under = Under_component::getInstance(red_soldier, blue_soldier);
        up = Up_component::getInstance(under);
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
                window.draw(*up);
            }
            window.draw(*blue_soldier);
            window.draw(*red_soldier);
            window.draw(*shade);
            window.draw(*ui);
            window.display();
            sf::sleep(sf::milliseconds(50));
        }
    }
    void run()
    {
        window.setActive(false);
        render_thread.launch();
        mouse_thread.launch();
        sf::Event event;
        while (window.isOpen()) {
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
        while (1) {
            if (!mouse_event.empty()) {
                sf::Vector2i now = mouse_event.front();
                mouse_event.pop();
                if (now.x >= 0 && now.y >= 0 && now.x <= 1760 && now.y <= 960) {
                    switch (state) {
                    case Regular:
                        if (now.x > 1280) {
                            State tmp;
                            ui->handle(now, &tmp);
                            state = tmp;
                            if (state != Regular) {
                                under->setLabel(0);
                            }
                        } else {
                            Soldier* tmp;
                            tmp = red_soldier->handleEvent(now);
                            if (tmp) {
                                state = State::Soldier_Selecting;
                                under->setSoldier(tmp);
                                under->setLabel(1);
                                up->update();
                            }
                        }
                        break;
                    case Ui_Selecting:
                        if (under->handleEvent(now)) {
                            red_soldier->addSoldier(ui->getOrder(), sf::Vector2u(now.x / 32, now.y / 32), Direction::right);
                        } else {
                            state = State::Regular;
                            ui->unselected();
                        }
                        break;
                    case Soldier_Selecting:
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
};