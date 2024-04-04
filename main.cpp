#include <iostream>
#include <unistd.h> // Для функции sleep

const int ROWS = 20; // Количество строк в игровом поле
const int COLS = 50; // Количество столбцов в игровом поле

// Функция для отображения текущего состояния игрового поля
void display(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
}

// Функция для обновления состояния игрового поля на следующий шаг
void update(char board[ROWS][COLS]) {
    char newBoard[ROWS][COLS];
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int neighbors = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLS && board[ni][nj] == '*') {
                        ++neighbors;
                    }
                }
            }
            if (board[i][j] == '*') {
                newBoard[i][j] = (neighbors == 2 || neighbors == 3) ? '*' : ' ';
            } else {
                newBoard[i][j] = (neighbors == 3) ? '*' : ' ';
            }
        }
    }
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            board[i][j] = newBoard[i][j];
        }
    }
}

int main() {
    char board[ROWS][COLS] = { ' ' }; // Создаем игровое поле, заполненное пробелами
    // Начальная конфигурация игрового поля
    board[5][5] = '*';
    board[6][6] = '*';
    board[6][7] = '*';
    board[7][5] = '*';
    board[7][6] = '*';

    while (true) {
        display(board); // Отображаем текущее состояние игрового поля
        update(board); // Обновляем состояние игрового поля
        sleep(1); // Задержка в секундах
        std::cout << "\033[2J\033[1;1H"; // Очистка терминала для отображения следующего состояния игрового поля
    }

    return 0;
}