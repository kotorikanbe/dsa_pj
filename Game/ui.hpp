#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
class Ui : public sf::Drawable, public sf::Transformable {
private:
    sf::Texture background, soldiers;
    sf::Sprite S_background, Infantry, Caster, Archer, Knight, Berserker;
    sf::Font font;
    sf::Text button, value;
    int value_i;
    sf::RectangleShape button_shape;
    sf::RectangleShape selected;
    double down_length;
    bool selected_state;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(S_background);
        if (selected_state)
            target.draw(selected);
        target.draw(Infantry);
        target.draw(Caster);
        target.draw(Archer);
        target.draw(Knight);
        target.draw(Berserker);
        target.draw(button_shape);
        target.draw(button);
        target.draw(value);
    }

public:
    Ui()
    {
        selected_state = false;
        value_i = 0;
        down_length = 100;
        background.loadFromFile("background.png"); // 480*960
        soldiers.loadFromFile("soldiers.png");
        S_background.setTexture(background);
        S_background.setPosition(sf::Vector2f(1280, 0));
        Infantry.setTexture(soldiers);
        Infantry.setTextureRect(sf::IntRect(0, 0, 127, 127));
        Infantry.setPosition(sf::Vector2f(1304, down_length));
        Caster.setTexture(soldiers);
        Caster.setTextureRect(sf::IntRect(256, 0, 126, 127));
        Caster.setPosition(sf::Vector2f(1456, down_length));
        Archer.setTexture(soldiers);
        Archer.setTextureRect(sf::IntRect(512, 0, 126, 127));
        Archer.setPosition(sf::Vector2f(1608, down_length));
        Knight.setTexture(soldiers);
        Knight.setTextureRect(sf::IntRect(768, 0, 127, 127));
        Knight.setPosition(sf::Vector2f(1354.66667, 150 + down_length));
        Berserker.setTexture(soldiers);
        Berserker.setTextureRect(sf::IntRect(1024, 0, 126, 127));
        Berserker.setPosition(sf::Vector2f(1557.33333, 150 + down_length));
        font.loadFromFile("yahei.ttf");
        button.setFont(font);
        button.setString(std::wstring(L"����"));
        button.setCharacterSize(70);
        button.setFillColor(sf::Color::Black);
        button.setOrigin(button.getLocalBounds().width / 2, button.getLocalBounds().height / 2);
        button.setStyle(sf::Text::Bold);
        button.setPosition(sf::Vector2f(1520, 500 + down_length));
        value.setFont(font);
        value.setString(std::to_wstring(value_i));
        value.setCharacterSize(70);
        value.setFillColor(sf::Color::Red);
        value.setOrigin(value.getLocalBounds().width / 2, value.getLocalBounds().height / 2);
        value.setStyle(sf::Text::Bold);
        value.setPosition(sf::Vector2f(1520, 700 + down_length));
        button_shape.setSize(sf::Vector2f(220, 120));
        button_shape.setFillColor(sf::Color(0xf4, 0xf1, 0xb9));
        button_shape.setOrigin(button_shape.getLocalBounds().width / 2, button_shape.getLocalBounds().height / 2);
        button_shape.setPosition(sf::Vector2f(1520, 500 + down_length));
    }
    void update(int value_i)
    {
        this->value_i = value_i;
        value.setFont(font);
        value.setString(std::to_wstring(value_i));
        value.setOrigin(value.getLocalBounds().width / 2, value.getLocalBounds().height / 2);
        value.setPosition(sf::Vector2f(1520, 700 + down_length));
        value.setStyle(sf::Text::Bold);
        value.setFillColor(sf::Color::Red);
    }
    void selected(int label)
    {
        selected_state = true;
        selected.setSize(sf::Vector2f(128, 128));
        selected.setFillColor(sf::Color(0xf4, 0xf1, 0xb9));
        switch (label) {
        case 1:
            selected.setPosition(sf::Vector2f(1304, down_length));
            break;
        case 2:
            selected.setPosition(sf::Vector2f(1456, down_length));
            break;
        case 3:
            selected.setPosition(sf::Vector2f(1608, down_length));
            break;
        case 4:
            selected.setPosition(sf::Vector2f(1354.66667, 150 + down_length));
            break;
        case 5:
            selected.setPosition(sf::Vector2f(1557.33333, 150 + down_length));
            break;
        }
    }
    void unselcted()
    {
        selected_state = false;
    }
};