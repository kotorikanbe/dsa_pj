#pragma once
#include "map.hpp"
#include "shade.hpp"
#include "soldier.hpp"
#include "ui.hpp"
#include <queue>
enum State {
    Regular,
    Ui_Selecting,
    Soldier_Selecting,
    AI
};
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
        state = Regular;
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
            }
        }
    }
};