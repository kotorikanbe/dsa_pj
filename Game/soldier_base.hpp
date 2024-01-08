#pragma once
#include <SFML/System/Vector2.hpp>
enum Direction {
    down,
    left,
    right,
    up
};
enum Order {
    INFANTRY,
    CASTER,
    BERSERKER,
    ARCHER,
    KNIGHT
};
class Soldier {
private
    :
    int health;
    int damage;
    int range;
    int view;
    int defense_p; // physical defense is percentage
    int defense_m; // magical defense is percentage
    sf::Vector2u position;
    bool ismoving;
    bool isattacking;
    bool isdead;
    bool isunderattack;
    int cost;
    Direction direction;
    int phase; // 0-3 valid
    Order label;
    int move_step;

public:
    Soldier(sf::Vector2u position, Direction direction, int health, int damage, int range, int view, int defense_p, int defense_m, int cost, Order label)
        : health(health)
        , damage(damage)
        , range(range)
        , view(view)
        , defense_p(defense_p)
        , defense_m(defense_m)
        , position(position)
        , cost(cost)
        , label(label)
        , phase(0)
        , direction(direction)

    {
        ismoving = false;
        isattacking = false;
        isdead = false;
        isunderattack = false;
        move_step = view;
    }
    ~Soldier() = default;
    int getHealth()
    {
        return health;
    }
    int getDamage()
    {
        return damage;
    }
    int getRange()
    {
        return range;
    }
    int getView()
    {
        return view;
    }
    int getDefense_p()
    {
        return defense_p;
    }
    int getDefense_m()
    {
        return defense_m;
    }
    sf::Vector2u getPosition()
    {
        return position;
    }
    bool getIsMoving()
    {
        return ismoving;
    }
    bool getIsAttacking()
    {
        return isattacking;
    }
    bool getIsDead()
    {
        return isdead;
    }
    bool getIsUnderAttack()
    {
        return isunderattack;
    }
    int getCost()
    {
        return cost;
    }
    Direction getDirection()
    {
        return direction;
    }
    int getPhase()
    {
        return phase;
    }
    int getMoveStep()
    {
        return move_step;
    }
    Order getLabel()
    {
        return label;
    }
    void setLabel(Order label)
    {
        this->label = label;
    }
    void setPhase(int phase)
    {
        if (phase >= 0 && phase <= 3)
            this->phase = phase;
    }
    void setDirection(Direction direction)
    {
        this->direction = direction;
    }
    void setHealth(int health)
    {
        this->health = health;
    }
    void setDamage(int damage)
    {
        this->damage = damage;
    }
    void setRange(int range)
    {
        this->range = range;
    }
    void setView(int view)
    {
        this->view = view;
    }
    void setDefense_p(int defense_p)
    {
        this->defense_p = defense_p;
    }
    void setDefense_m(int defense_m)
    {
        this->defense_m = defense_m;
    }
    void setPosition(sf::Vector2u position)
    {
        this->position = position;
    }
    void setIsMoving(bool ismoving)
    {
        this->ismoving = ismoving;
    }
    void setIsAttacking(bool isattacking)
    {
        this->isattacking = isattacking;
    }
    void setIsDead(bool isdead)
    {
        this->isdead = isdead;
    }
    void setIsUnderAttack(bool isunderattack)
    {
        this->isunderattack = isunderattack;
    }
    void refresh()
    {
        move_step = view;
    }
    virtual void move(int steps, Direction direction)
    {
        move_step -= steps;
        switch (direction) {
        case up:
            this->position.y -= steps;
            break;
        case down:
            this->position.y += steps;
            break;
        case left:
            this->position.x -= steps;
            break;
        case right:
            this->position.x += steps;
            break;
        default:
            break;
        }
    }
    virtual void attack(Soldier& enemy) = 0;
};
class Infantry : public Soldier {
public:
    Infantry(sf::Vector2u position, Direction direction, int health = 1000, int damage = 325, int range = 1, int view = 5, int defense_p = 50, int defense_m = 30, int cost = 1, Order label = INFANTRY)
        : Soldier(position, direction, health, damage, range, view, defense_p, defense_m, cost, label)
    {
    }
    ~Infantry() = default;
    void attack(Soldier& enemy) override
    {
        this->setIsAttacking(true);
        enemy.setIsUnderAttack(true);
        enemy.setHealth(enemy.getHealth() - static_cast<int>((1.0 - static_cast<double>(enemy.getDefense_p())) / 100.0 * this->getDamage()));
        if (enemy.getHealth() <= 0) {
            enemy.setIsDead(true);
        }
    }
};
class Caster : public Soldier {
public:
    Caster(sf::Vector2u position, Direction direction, int health = 700, int damage = 425, int range = 5, int view = 5, int defense_p = 20, int defense_m = 60, int cost = 2, Order label = CASTER)
        : Soldier(position, direction, health, damage, range, view, defense_p, defense_m, cost, label)
    {
    }
    ~Caster() = default;
    void attack(Soldier& enemy) override
    {
        this->setIsAttacking(true);
        enemy.setIsUnderAttack(true);
        enemy.setHealth(enemy.getHealth() - static_cast<int>((1.0 - static_cast<double>(enemy.getDefense_m())) / 100.0 * this->getDamage()));
        if (enemy.getHealth() <= 0) {
            enemy.setIsDead(true);
        }
    }
};
class Berserker : public Soldier {
public:
    Berserker(sf::Vector2u position, Direction direction, int health = 1500, int damage = 500, int range = 1, int view = 4, int defense_p = 30, int defense_m = 10, int cost = 3, Order label = BERSERKER)
        : Soldier(position, direction, health, damage, range, view, defense_p, defense_m, cost, label)
    {
    }
    ~Berserker() = default;
    void attack(Soldier& enemy) override
    {
        this->setIsAttacking(true);
        enemy.setIsUnderAttack(true);
        enemy.setHealth(enemy.getHealth() - static_cast<int>((1.0 - static_cast<double>(enemy.getDefense_p())) / 100.0 * this->getDamage()));
        if (enemy.getHealth() <= 0) {
            enemy.setIsDead(true);
        }
    }
};
class Archer : public Soldier {
public:
    Archer(sf::Vector2u position, Direction direction, int health = 850, int damage = 300, int range = 6, int view = 8, int defense_p = 40, int defense_m = 30, int cost = 2, Order label = ARCHER)
        : Soldier(position, direction, health, damage, range, view, defense_p, defense_m, cost, label)
    {
    }
    ~Archer() = default;
    void attack(Soldier& enemy) override
    {
        this->setIsAttacking(true);
        enemy.setIsUnderAttack(true);
        enemy.setHealth(enemy.getHealth() - static_cast<int>((1.0 - static_cast<double>(enemy.getDefense_p())) / 100.0 * this->getDamage()));
        if (enemy.getHealth() <= 0) {
            enemy.setIsDead(true);
        }
    }
};
class Knight : public Soldier {
public:
    Knight(sf::Vector2u position, Direction direction, int health = 1600, int damage = 400, int range = 2, int view = 6, int defense_p = 60, int defense_m = 60, int cost = 6, Order label = KNIGHT)
        : Soldier(position, direction, health, damage, range, view, defense_p, defense_m, cost, label)
    {
    }
    ~Knight() = default;
    void attack(Soldier& enemy) override
    {
        this->setIsAttacking(true);
        enemy.setIsUnderAttack(true);
        enemy.setHealth(enemy.getHealth() - static_cast<int>((1.0 - static_cast<double>(enemy.getDefense_p())) / 100.0 * this->getDamage()));
        if (enemy.getHealth() <= 0) {
            enemy.setIsDead(true);
        }
    }
};
// SINGLETON
class Factory {
public:
    static Factory* getInstance()
    {
        static Factory* instance;
        if (instance == nullptr) {
            instance = new Factory();
        }
        return instance;
    }
    Soldier* createSoldier(Order label, sf::Vector2u position, Direction direction)
    {
        switch (label) {
        case INFANTRY:
            return new Infantry(position, direction);
        case CASTER:
            return new Caster(position, direction);
        case BERSERKER:
            return new Berserker(position, direction);
        case ARCHER:
            return new Archer(position, direction);
        case KNIGHT:
            return new Knight(position, direction);
        default:
            return nullptr;
        }
    }

private:
    Factory() = default;
};