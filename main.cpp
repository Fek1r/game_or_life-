#include <iostream>
#include <unistd.h> 

const int ROWS = 20; 
const int COLS = 40; 

void display(char board[ROWS][COLS]) {
    system("clear"); // Очистка экрана (работает на Unix-подобных системах)
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << (board[i][j] == '*' ? "\u25A0" : " "); // Используем юникодные символы для отображения клеток
        }
        std::cout << std::endl;
    }
}

void update(char board[ROWS][COLS]) {
    char newBoard[ROWS][COLS];
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int neighbors = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    int ni = (i + di + ROWS) % ROWS; // Обработка границы по вертикали
                    int nj = (j + dj + COLS) % COLS; // Обработка границы по горизонтали
                    if (board[ni][nj] == '*') {
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
    char board[ROWS][COLS] = { ' ' }; 

    board[5][10] = '*';
    board[6][11] = '*';
    board[6][12] = '*';
    board[7][10] = '*';
    board[7][11] = '*';

    while (true) {
        display(board); 
        update(board);
        usleep(50000); // Задержка в микросекундах
    }

    return 0;
}