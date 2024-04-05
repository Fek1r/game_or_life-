#include <unistd.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring> // Для memcpy

const int ROWS = 20;
const int COLS = 40;

struct BMPHeader {
    uint32_t fileSize;
    uint32_t dataOffset;
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t width, height;
    uint16_t planes;
    uint16_t bitCount;
};

void readBMP(const std::string& filename, char board[ROWS][COLS]) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Невозможно открыть файл" << std::endl;
        return;
    }

    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;

    // Чтение заголовков
    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
    file.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    if (bmpInfoHeader.bitCount != 24) {
        std::cerr << "Неподдерживаемый формат BMP, должно быть 24 бита на пиксель" << std::endl;
        return;
    }

    // Переход к данным изображения
    file.seekg(bmpHeader.dataOffset, std::ios::beg);

    auto isWhite = [](uint8_t red, uint8_t green, uint8_t blue) {
        return red > 128 && green > 128 && blue > 128;
    };

    int32_t width = bmpInfoHeader.width;
    int32_t height = bmpInfoHeader.height;
    int32_t rowPadded = (width*3 + 3) & (~3);
    std::vector<uint8_t> rowData(rowPadded);

    for (int i = 0; i < height; ++i) {
        file.read(reinterpret_cast<char*>(rowData.data()), rowPadded);
        for (int j = 0; j < width; ++j) {
            uint8_t blue = rowData[j * 3];
            uint8_t green = rowData[j * 3 + 1];
            uint8_t red = rowData[j * 3 + 2];
            if (isWhite(red, green, blue)) {
                board[i % ROWS][j % COLS] = '*';
            } else {
                board[i % ROWS][j % COLS] = ' ';
            }
        }
    }
}

void display(char board[ROWS][COLS]) {
    system("clear"); // Или system("cls") для Windows
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            std::cout << (board[i][j] == '*' ? "\u25A0" : " ");
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
                    int ni = (i + di + ROWS) % ROWS;
                    int nj = (j + dj + COLS) % COLS;
                    if (board[ni][nj] == '*') {
                        ++neighbors;
                    }
                }
            }
            newBoard[i][j] = (board[i][j] == '*' && (neighbors == 2 || neighbors == 3)) || (board[i][j] == ' ' && neighbors == 3) ? '*' : ' ';
        }
    }
    memcpy(board, newBoard, sizeof(char) * ROWS * COLS); // Используйте memcpy для копирования данных
}

int main() {
    char board[ROWS][COLS] = {};

    // Загрузка начального состояния из BMP файла (пример)
    readBMP("путь_к_вашему_файлу.bmp", board);

    while (true) {
        display(board); 
        update(board);
        usleep(100000); // Задержка для наблюдения изменений
    }

    return 0;
}