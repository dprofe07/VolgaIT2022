#include <iostream>
#include <vector>
#include "fairy_tail.hpp"

const int inf = 100;

/*
Система координат:
       |
       |
---------------> x
       |
       |
       v
        y
 */

void getRelValues(Direction dir, int& relXOut, int& relYOut) {
    //Функция для определения изменения координат персонажа
    relXOut = relYOut = 0;
    // Инициализация переменных
    switch (dir) {
        case Direction::Pass:
            // Переменные и так обнулены, поэтому просто выходим
            return;
        case Direction::Up:
            relYOut = -1;
            // Изменяем переменную y. Система координат показана выше
            return;
        case Direction::Down:
            // Изменяем переменную y. Система координат показана выше
            relYOut = 1;
            return;
        case Direction::Left:
            relXOut = -1;
            // Изменяем переменную x. Система координат показана выше
            return;
        case Direction::Right:
            relXOut = 1;
            // Изменяем переменную x. Система координат показана выше
            return;
    }

}

class MyCharacter { // Класс для упрощения хранения данных о персонажах
private:
    Character ch; // Имя персонажа
    bool was[19][19]{}; // Массив посещённых клеток. Массив относительный, was[9][9] - начальное положение
public:

    int x = 0, y = 0; // Координаты, относительные

    explicit MyCharacter(Character ch) { // Конструктор
        // Помечен explicit, чтобы отменить неявные преобразования
        this->ch = ch; // Сохраняем имя персонажа
        for (x = -9; x < 10; ++x) {
            for (y = -9; y < 10; ++y) {
                wasAt(x, y) = false; // Отмечаем, что ещё нигде на были
            }
        }
        x = 0; // Обнуляем x и y
        y = 0;
        wasAt(0, 0) = true; // В начальной клетке персонаж был
    }

    bool canGo(Fairyland& world, Direction dir) {
        // Метод, упрощающий доступ к функции canGo
        return world.canGo(this->ch, dir);
    }

   bool go(Fairyland& world, Direction dir) {
        // Метод, упрощающий доступ к функции go
       int relX, relY;
       getRelValues(dir, relX, relY); // Определение перемещения
       x += relX;
       y += relY;

       this->wasAt(x, y) = true; // Пометка клетки как посещённой
       switch (this->ch) { // Выполняем перемещение
           case Character::Ivan:
               return world.go(dir, Direction::Pass);
           case Character::Elena:
               return world.go(Direction::Pass, dir);
       }
   }

   bool& wasAt(int x_, int y_) { // Метод, упрощающий доступ к массиву was. Благодаря этому методу точка (0; 0) является начальной
       return this->was[x_ + 9][y_ + 9];
   }

};



Direction getOtherDirection(Direction dir) {
    // Функция для нахождения противоположного направления
    switch (dir) {
        case Direction::Pass:
            return dir; // Противоположного движения не сущесвует
        case Direction::Up:
            return Direction::Down;
        case Direction::Down:
            return Direction::Up;
        case Direction::Left:
            return Direction::Right;
        case Direction::Right:
            return Direction::Left;
    }
}

const std::vector<Direction> dirs{Direction::Up, Direction::Left, Direction::Right, Direction::Down};
// Список всех возможных направлений. std::vector используется для перебора

bool goAllGoes(Fairyland& world, MyCharacter& ch) {
    // Основная функция программы. Определяет и перемещает персонажа во ВСЕ доступные клетки.
    // Реализована рекурсивно
    if (world.getTurnCount() >= 100) return false; // Если не удалось найти за 100 перемещений, то пути нет.
    for (Direction dir : dirs) { // Перебор всех направлений
        int relX, relY;
        getRelValues(dir, relX, relY); // Определение перемещения персонажа
        bool meet = false;
        if (ch.x + relX > 9 || ch.y + relY > 9 || ch.x + relX < -9 || ch.y + relY < -9) {
            continue; // Обработка выхода за границы
        }
        if (ch.canGo(world, dir) && !ch.wasAt(ch.x + relX, ch.y + relY)) {
            // Если клетка доступна и не была посещена, то переходим в неё
            meet = ch.go(world, dir);
            if (meet) return true; // Выполняется дополнительная проверка чтобы избежать лишнего запуска goAllGoes()

            meet = goAllGoes(world, ch); // И выполняем все возможные перехоы из этой клетки
            if (meet) return true;
            ch.go(world, getOtherDirection(dir)); // Возвращаемся обратно, чтобы перемещаться из той же клетки.
        }
    }
    return false;
}

int main() {
    Fairyland world; // Инициализация мира

    MyCharacter ivan(Character::Ivan), elena(Character::Elena);
    // Создание персонажей

    if (goAllGoes(world, ivan) || goAllGoes(world, elena)) { // Если они могут найти друг друга (хотя бы один из них)
        std::cout << "Встретятся!"; // То они встретятся
    } else {
        std::cout << "К сожалунию, встреча не состоится"; // Иначе не встретятся
    }

    std::cout << "\nХодов: " << world.getTurnCount() << "\n"; // Вывод количества ходов

    return 0;
}
